#ifndef __MODBUS_TCP_H__
#define __MODBUS_TCP_H__


int modbus_tcp_server_init(int iPort, struct sockaddr_in* serverAddr);
int modbus_tcp_server_uninit(int modbus_fd);
int modbus_tcp_server_set_socket_timeout(int sockfd, int timeout_seconds);


#endif

