/*
** client.c --
** Aniket Handa
** GNU GPL v3
** Ref: W. Richard Stevens' UNIX Network Programming 2nd edition.
**      Beej's guide to network programming.
*/

#include "shared.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
    PACKET tempPacket;

	if (argc != 3) {
	    fprintf(stderr,"usage: client [hostname] [mnemonic name]\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

    
    //send reg
    generatePacket(&tempPacket, MSG_HEAD_REG, getpid(), argv[2]);
    
    if (send(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0) == -1)
        perror("send");
    
    
    //recv reg_ack
	if ((numbytes = recv(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0)) == -1) {
	    perror("recv");
	    exit(1);
	}


    if (tempPacket.packet_head.type==MSG_HEAD_REG_ACK) {
        
    
	printf("client(%d): received ACK for reg from server(%d)\n",getpid(),tempPacket.packet_head.sender_pid);
    }
    printf("Press any key to send msg: 'Hello Server!'");
    getchar();
    //Send msg
    generatePacket(&tempPacket, MSG_HEAD_NORMAL, getpid(), argv[2]);
    addMsg(&tempPacket,"Hello Server!");
    if (send(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0) == -1)
        perror("send");

    //recv msg_ack
	if ((numbytes = recv(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
    
    
    if (tempPacket.packet_head.type==MSG_HEAD_NORMAL_ACK) {
        
        
        printf("client(%d): received ACK for msg from server(%d)\n",getpid(),tempPacket.packet_head.sender_pid);
    }
    printf("Press any key to deregister.");
    getchar();
    
    //send dereg
    generatePacket(&tempPacket, MSG_HEAD_DEREG, getpid(), argv[2]);
    
    if (send(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0) == -1)
        perror("send");
    
    
    //recv dereg_ack
	if ((numbytes = recv(sockfd, (void*)&tempPacket, sizeof(tempPacket), 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
    
    
    if (tempPacket.packet_head.type==MSG_HEAD_DEREG_ACK) {
        
        
        printf("client(%d): received ACK for dereg  from server(%d)\n",getpid(),tempPacket.packet_head.sender_pid);
    }


	close(sockfd);

	return 0;
}

