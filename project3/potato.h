#ifndef __POTATO_H
#define __POTATO_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

typedef struct potato {
    int hops;
    int trace[512];
} potato;

// init a sever socket
int init_server_socket(const char * port) {
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = "0.0.0.0";

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family, 
                host_info_list->ai_socktype, 
                host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot bind socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    status = listen(socket_fd, 100);
    if (status == -1) {
        cerr << "Error: cannot listen on socket" << endl; 
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    cout <<"Server is listening on port " << port << endl;

    return socket_fd;
}

int server_connect_with_client(int socket_fd) {
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
        cerr << "Error: cannot accept connection on socket" << endl;
        return -1;
    } //if
    
    return client_connection_fd;
}

// estiblish connections
int connect_with_server(const char *hostname, const char *port) {
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family, 
                host_info_list->ai_socktype, 
                host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if
    
    cout << "Connecting to " << hostname << " on port " << port << "..." << endl;
    
    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot connect to socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    return socket_fd;
}

int send_int(int socket_fd, int to_send) {
    int n = htonl(to_send);
    send(socket_fd, &n, sizeof(n), 0);
    // check
    return 0;
}

int receive_int(int socket_fd) {
    int integer_received;
    int bytes_received = recv(socket_fd, &integer_received, sizeof(integer_received), 0);
    if (bytes_received == -1) {
        perror("recv failed");
        exit(1);
    }
    integer_received = ntohl(integer_received);

  return integer_received;
}

int get_port_num_by_socket(int socket_fd) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if(getsockname(socket_fd, (struct sockaddr *) &addr, &len) == -1) {
        perror("getsockname");
        return 1;
    }

    int port_num = ntohs(addr.sin_port);

    printf("Local port number: %d\n", ntohs(addr.sin_port));

    return port_num;
}



#endif