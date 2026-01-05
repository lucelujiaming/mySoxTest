#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>
#define MAX_PAYLOAD 1024 // maximum payload size
#define NETLINK_TEST 25
int main(int argc, char* argv[])
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    int sock_fd, retval;
    int state,state_smg = 0;
    // Create a socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
    if(sock_fd == -1){
        printf("error getting socket: %s", strerror(errno));
        return -1;
    }
    // To prepare binding
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = 100;
    src_addr.nl_groups = 0;
    //Bind
    retval = bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    if(retval < 0){
        printf("bind failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }
    // To orepare create mssage head
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if(!nlh){
        printf("malloc nlmsghdr error!\n");
        close(sock_fd);
        return -1;
    }
    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;
    dest_addr.nl_groups = 0;
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = 100;
    nlh->nlmsg_flags = 0;
    strcpy(NLMSG_DATA(nlh),"Hello you!");
    //send message
    printf("state_smg\n");
    sendto(sock_fd,nlh,NLMSG_LENGTH(MAX_PAYLOAD),0,(struct sockaddr*)(&dest_addr),sizeof(dest_addr));
    if(state_smg == -1)
    {
        printf("get error sendmsg = %s\n",strerror(errno));
    }
    memset(nlh,0,NLMSG_SPACE(MAX_PAYLOAD));
    //receive message
    printf("waiting received!\n");
while(1){
        printf("In while recvmsg\n");
state=recvfrom(sock_fd,nlh,NLMSG_LENGTH(MAX_PAYLOAD),0,NULL,NULL);
        if(state<0)
        {
            printf("state<1");
        }
        printf("Received message: %s\n",(char *) NLMSG_DATA(nlh));
        memset(nlh,0,NLMSG_SPACE(MAX_PAYLOAD));
    }
    close(sock_fd);
    return 0;
}
