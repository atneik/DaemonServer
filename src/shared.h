#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define MSG_HEAD_REG 1
#define MSG_HEAD_DEREG 2
#define MSG_HEAD_NORMAL 3

#define MSG_HEAD_REG_ACK 4
#define MSG_HEAD_DEREG_ACK 5
#define MSG_HEAD_NORMAL_ACK 6

#define MAX_MSG_SIZE 50
#define PORT "4461" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once - client
#define BACKLOG 10	 // how many pending connections queue will hold - server


//A structs to store Head and body of the message
typedef struct
{
    int type;
    pid_t sender_pid;
    pid_t receiver_pid;
    
} MSG_HEAD;

typedef struct
{
    char charbody[MAX_MSG_SIZE];
    
} MSG_BODY;


typedef struct
{
    MSG_HEAD packet_head;
    MSG_BODY packet_body;
    
} PACKET;

void generatePacket(PACKET *, int, pid_t);
int addMsg(PACKET *,char *);
