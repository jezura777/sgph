#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>


#include "arg.h"

char *argv0;

#include "config.h"

/* function declarations */

/* global variables */
int port = 70;

/* functions implementations */
void
die(const char *errstr, ...)
{
        va_list ap;

        va_start(ap, errstr);
        vfprintf(stderr, errstr, ap);
        va_end(ap);
        exit(1);
}

static void
usage(void)
{
	die("usage: %s [-p port] [-d dir] [-v]\n", argv0);
}

int
main(int argc, char *argv[])
{

	int sockfd, connfd, len; 
	struct sockaddr_in serv_addr, client_addr; 

        /* defaults */
        int port = 70;
        char *servedir = ".";
	

	ARBEGIN {
		case 'p':
			port = EARGF(usage());
			break;
		case 'd':
			serverdir = EARGF(usage());
			break;
		case 'v':
			die("sgph " VERSION " © 2024-2029 sgph engeneers, "
			    "see LICENSE for details\n");
		default:
			usage();
	} ARGEND;
	
	server_addr = {
		.sin_family = AF_INET, 
    		.sin_addr.s_addr = htonl(INADDR_ANY), 
    		.sin_port = htons(port),
	};

	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_GOPHER)) < 0) {
		die("Couldn't create socket!\n");
	}
	
	if ((bind(sockfd, &serv_addr, sizeof(servaddr))) != 0) { 
		die("Socket bind failed!\n"); 
	}

	

	return 0;
}
