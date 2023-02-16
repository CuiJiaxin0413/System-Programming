#include "potato.h"

using namespace std;

// player set up
int player_set_up(int socket_fd) {
    int id = receive_int(socket_fd);
    int total_num_player = receive_int(socket_fd);

    cout << "Connected as player " << id << " out of " << total_num_player << " total players" << endl;

    return id;
}

// connect with neigbhours


// receive the potato

// pass to a random neigbour

/*
player <machine_name> <port_num>
(e.g. ./player vcm-30716.vm.duke.edu 1234)
*/

int main(int argc, char * argv[]) {
    if (argc != 3) {
        cerr << "Error: input format. Usage: ./player <machine_name> <port_num>" << endl;
        return -1;
    }
    const char *hostname = argv[1];
    const char *hostport = argv[2];

    int client_socket_fd = connect_with_server(hostname, hostport);

    int id = player_set_up(client_socket_fd);

    // player server socket port number should be assigned by os???
    int server_socket_fd = init_server_socket("");
    int player_server_port_num = get_port_num_by_socket(server_socket_fd);
    // send its server port to the master
    send_int(client_socket_fd, player_server_port_num);


    // receive info of left neigbour from the master
    int left_port_num = receive_int(client_socket_fd);
    cout << "left port:" << left_port_num << endl;
    char left_port_num_char[10];
    sprintf(left_port_num_char, "%d", left_port_num);
    
    char ip[INET_ADDRSTRLEN];
    if (recv(client_socket_fd, ip, INET_ADDRSTRLEN, MSG_WAITALL) == -1) {
        perror("receive failed line 46");
        exit(1);
    }

    cout << "left ip: " << ip << endl;

    // connect with left neighbour first
    // client will return immeditealy
    connect_with_server(ip, left_port_num_char);
    // accept neighbour connect
    server_connect_with_client(server_socket_fd);

}