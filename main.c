#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <string.h>

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
//		printf("cykl2\n");
		if((numrecv = recv(fd, buff, MAX_PATH_SIZE, 0)) < 0) {
			die("recv failed");
		}
// printf("'0x%x 0x%x %d'\n", buff[numrecv - 2], buff[numrecv - 1], numrecv);
	} while(buff[numrecv - 1] == '\r' && buff[numrecv] == '\n');

	buff[numrecv - 2] = '\0';

	return;
}

static void
send_file(int fd, char *path) {

	FILE *inputfd = NULL;
	size_t bytes_read = 0;
	
	char edited_path[MAX_PATH_SIZE+sizeof(INDEX_FILE)];

//	printf("trying 1st time to open %s", path);
	if ((inputfd = fopen(path, "r")) == NULL) {

		sprintf(edited_path, "%s%s", path, INDEX_FILE);
//		printf("path is %s\n", edited_path);
		
		if((inputfd = fopen(edited_path, "r")) == NULL) {

//			printf("Nothing worked using path /index.gph\n");

			if((inputfd = fopen(INDEX_FILE, "r")) == NULL) { // Make index.gph somehow a const
//				perror("3st attempt");
				die("Couldn't open requested path nor /index.gph.\n");
			}
		}
	}

//	printf("path is %s and the input FILE * is %p\n", path, inputfd);

	while ((bytes_read = read(fileno(inputfd), path, MAX_PATH_SIZE)) > 0) {
        	write(fd, path, bytes_read);
//		printf("cykl3\n");

    	}

	close(fileno(inputfd));
	return;
}

static void
usage(void)
{
	die("usage: %s [-p port] [-d dir] [-v]\n", argv0);
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
			die("sgph %s © 2024-2029 sgph engeneers, see LICENSE for details.\n", VERSION);
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
//		printf("cykl1\n");
		if ((listen(sockfd, 5)) < 0) { 
			die("Couldn't listen!\n"); 
        	}
	
		len = sizeof(client_addr);

		if ((connfd = accept(sockfd, (struct sockaddr*)&client_addr, &len)) < 0) {
//			perror("accept");
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

//		printf("before wait\n");
		wait_for_cr_lf(connfd, request);
//		printf("before send\n");

		send_file(connfd, request);
//		printf("before close\n");
		
		close(connfd);
	}

	return 0;
}
