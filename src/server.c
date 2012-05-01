/*
** server.c --
** Aniket Handa
** GNU GPL v3
** Ref: W. Richard Stevens' UNIX Network Programming 2nd edition.
**      Beej's guide to network programming.
*/

#include "shared.h"

volatile sig_atomic_t keep_going = 1; /* controls program termination */
int daemonFlag =0;

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void termination_handler (int signum)
{
    keep_going = 0;
    signal (signum, termination_handler);
}

int main(int argc, char **argv)
{
	int sockfd, new_fd,numbytes;  // listen on sock_fd, new connection on new_fd
	char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv,tempType=0;
    pid_t tempPid;
    PACKET tempPacket;
    
    if (argc==2 && (strcmp("-d",argv[1])==0)) {
        printf("== Daemon mode selected.==\n");
        
        daemonFlag=1;
        
    }
    
    if (daemonFlag) {

    
     // Become a daemon:
     switch (fork ())
     {
     case -1:                    // can't fork 
             perror ("fork()");
             exit (0);
     case 0:                     // child, process becomes a daemon:
             close (STDIN_FILENO);
             close (STDOUT_FILENO);
             close (STDERR_FILENO);
             if (setsid () == -1)      // request a new session (job control)
             {
                 exit (0);
             }
             break;
     default:                    // parent returns to calling process: 
             return 0;
     }
     
     // Establish signal handler to clean up before termination:
     if (signal (SIGTERM, termination_handler) == SIG_IGN)
     signal (SIGTERM, SIG_IGN);
     signal (SIGINT, SIG_IGN);
     signal (SIGHUP, SIG_IGN);
     
    }
    
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
    


	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

    
    printf("server(%d): waiting for connections...\n",getpid());
    
	while(keep_going) 
    {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server(%d): got connection from %s\n", getpid(),s);

		if (!fork()) { // this is the child process
    
            close(sockfd); // child doesn't need this descriptor
            openlog("DaemonServer",LOG_PID,LOG_SYSLOG);
            
            while (tempType!=2) {//child stops when client deregisters
                
                
                //recv packet
                
                if (recv(new_fd, (void*)&tempPacket, 
                                     sizeof(tempPacket), 0) == -1) {
                    perror("recv");
                    break;
                }
            
                tempType=tempPacket.packet_head.type;
                tempPid=tempPacket.packet_head.sender_pid;
                
                if (tempType<=3) {
                    
                printf("server(%d): received msg type:'%d' from %s-client(%d) \n",getpid(),
                       tempType,tempPacket.packet_head.mid,tempPid);
                    
                    syslog(LOG_NOTICE, "server(%d): received msg type:'%d' from %s-client(%d) \n",getpid(),
                           tempType,tempPacket.packet_head.mid,tempPid);
                   
                if (tempType==MSG_HEAD_NORMAL) {
                    
                    printf("server(%d): received following msg string from %s-client(%d): %s \n",getpid(),tempPacket.packet_head.mid,tempPid,tempPacket.packet_body.charbody);
                    
                    syslog(LOG_NOTICE, "server(%d): received following msg string from %s-client(%d): %s \n",getpid(),tempPacket.packet_head.mid,tempPid,tempPacket.packet_body.charbody);
                }
                    
                //generate ACKs
                    printf("server(%d): sent ACK type:'%d' to %s-client(%d)\n",getpid(),
                           tempType+3,tempPacket.packet_head.mid,tempPid);
                    syslog(LOG_NOTICE, "server(%d): received following msg string from %s-client(%d): %s \n",getpid(),tempPacket.packet_head.mid,tempPid,tempPacket.packet_body.charbody);
                    generatePacket(&tempPacket, tempType+3, getpid(), "Server");
                
                //send acks
                if (send(new_fd, (void*)&tempPacket, sizeof(tempPacket), 0) == -1) {
                    
                        perror("send");
                        break;
                }
            
                }
            }
            
            closelog();
			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}


