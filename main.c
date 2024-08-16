#include <arpa/inet.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"
#include "arg.h"

/* function declarations */

static void wait_for_cr_lf(int fd, char *buff);
static void send_file(int fd, char *path);
static void usage(void);

/* functions implementations */

static void 
wait_for_cr_lf(int fd, char *buff) {
	int numrecv = 0;
	do {
		memset(buff, 0, MAX_PATH_SIZE);
		if((numrecv = recv(fd, buff, MAX_PATH_SIZE, 0)) < 0) {
			die("recv failed");
		}
	} while(buff[numrecv - 1] == '\r' && buff[numrecv] == '\n');

	buff[numrecv - 2] = '\0';

	return;
}

static void
send_file(int fd, char *path) {

	int ifd = -1;
	size_t bytes_read = 0;

	/* Please help this monster has been bothering me, for quite a while now. I tried many ways of getting rid of it, but thay just made it worse/ */
	if(chdir((strcmp(path, "") == 0)? ".":path) != -1) {
		if((ifd = open(INDEX_FILE, O_RDONLY)) == -1) {
			warn("There isn't index file in the directory %s opening /index.gph.\n", path);
			goto error;
		}
		chdir("/");
	} else if((ifd = open(path, O_RDONLY)) != -1) {
	} else {
error:
		chdir("/");
		if((ifd = open(INDEX_FILE, O_RDONLY)) == -1) {
			die("Couldn't open requested path %s nor /index.gph.\n", path);
		}
	}

	while ((bytes_read = read(ifd, path, MAX_PATH_SIZE)) > 0) {
        	write(fd, path, bytes_read);

    	}

	close(ifd);
	return;
}

static void
usage(void)
{
	die("usage: %s [-p port] [-d base dir for the server] [-v]\n", argv0);
}

int
main(int argc, char *argv[])
{
	int sockfd, connfd; 
	unsigned int len; 
	struct sockaddr_in serv_addr, client_addr; 
	const char *err;
	char request[MAX_PATH_SIZE];

        /* defaults */
        int port = 70;
        char *servedir = ".";

	ARGBEGIN {
		case 'p':
			err = NULL;
			port = strtonum(EARGF(usage()), 0, INT_MAX, &err);
			if (err) {
				die("strtonum '%s': %s", EARGF(usage()), err);
			}
			break;
		case 'd':
			servedir = EARGF(usage());
			break;
		case 'v':
			die("sgph %s © 2024-2029 Jezura777, see LICENSE for details.\n", VERSION);
			break;
		default:
			usage();
	} ARGEND;
	
	chdir(servedir);
	chroot(servedir);

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	memset(request, 0, MAX_PATH_SIZE);

	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	serv_addr.sin_port = htons(port);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		die("Couldn't create socket!\n");
	}
	
	if ((bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) { 
		die("Couldn't bind socket!\n"); 
	}
	
	for (;;) {
		if ((listen(sockfd, 5)) < 0) { 
			die("Couldn't listen!\n"); 
        	}
	
		len = sizeof(client_addr);

		if ((connfd = accept(sockfd, (struct sockaddr*)&client_addr, &len)) < 0) {
			die("Couldn't create connection file descriptor!\n");
		}

	/*
	 * Client: Connects
	 * Server: Accepsts
	 * Client: <cr><rf><nul> terminated string either nothing that means send /index.gph or "/path/to/smthin" that means hey send "/path/to/smthin" 
	 * Server: Sends the file
	 * Server: closes the connection
	 *
	 */

		wait_for_cr_lf(connfd, request);
		send_file(connfd, request);
		
		close(connfd);
	}

	return 0;
}
