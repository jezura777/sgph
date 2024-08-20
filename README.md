sgph - Simple GoPHer server
===========================

Is a gopher server implementation that aims to be simple.
Everything is up to the server admin, the server is only 
sending files that the client requests.
The smaple/ directory contains sample gopher page.

It follows the RFC 1436 standard. 
If you want to know more iabout the standard goto: https://datatracker.ietf.org/doc/html/rfc1436

Requirements
------------
A C compiler and POSIX make.

Installation
------------
Enter the following command to build and install sgph
(if necessary as root):
    
        `make install`

Usage
-----
To run ./sample/ page enter the following command
(if necessary as root):
    
        `sgph -d ./sample/ -e /error.gph`

This tells the server to have main -d(ir) at ./sample/ 
and that the -e(rror) file (which is by default /index.gph) is /error.gph. 
(For more arguments see `sgph -h`)

Note
----
The server uses chdir(2) and open(2) to determine if files/directories exist or not 
so it is better and safer to use absolute paths from server root dir. 
The implementation uses chroot(2) to make server root dir its root dir.
        - I know that the manual page explicitely sais that it shouldn't be used in this way,
          but after close revision I (the author) came to the conclusion that the causes specified 
          in the manual aren't able to occur in this usecase, however if you find better and simpler way
          than using chroot(2), contact the author.

Author
------
Lukas Fiala <jezura777@duck.com>

