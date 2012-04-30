/*
** shared.c
*/

#include "shared.h"

void generatePacket(PACKET *gPacket, int gType, pid_t sender){
    
    gPacket->packet_head.type=gType;
    gPacket->packet_head.sender_pid=sender;
    
}

int addMsg(PACKET *gPacket,char *gMsg){

    if (gPacket->packet_head.type==MSG_HEAD_NORMAL && strlen(gMsg)<=MAX_MSG_SIZE) {
    
        strcpy(gPacket->packet_body.charbody,gMsg);
        return 0;
    }

    return 1;
}
