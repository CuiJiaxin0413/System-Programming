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

    int to_master_client_socket_fd = connect_with_server(hostname, hostport);

    int id = player_set_up(to_master_client_socket_fd);

    // player server socket port number should be assigned by os???
    int server_socket_fd = init_server_socket("");
    int player_server_port_num = get_port_num_by_socket(server_socket_fd);
    // send its server port to the master
    send_int(to_master_client_socket_fd, player_server_port_num);


    // receive info of left neigbour from the master
    int left_port_num = receive_int(to_master_client_socket_fd);
    cout << "left port:" << left_port_num << endl;
    char left_port_num_char[10];
    sprintf(left_port_num_char, "%d", left_port_num);
    
    char ip[INET_ADDRSTRLEN];
    if (recv(to_master_client_socket_fd, ip, INET_ADDRSTRLEN, MSG_WAITALL) == -1) {
        perror("receive failed line 46");
        exit(1);
    }

    cout << "left ip: " << ip << endl;

    // connect with left neighbour first
    // client will return immeditealy
    int left_client_fd = connect_with_server(ip, left_port_num_char);
    // accept neighbour connect
    server_connect_with_client(server_socket_fd);

    char buffer[sizeof(potato)];

    // recv(to_master_client_socket_fd, buffer, sizeof(buffer), MSG_WAITALL);

    // potato * p = (potato *) buffer;
    // p->trace[p->counter] = id;

    // cout << p->hops << endl;
    // cout << p->trace[0] << endl;

    // p->hops -= 1;
    // p->counter += 1;

    int socket_set[3];
    socket_set[0] = to_master_client_socket_fd;
    socket_set[1] = server_socket_fd;
    socket_set[2] = left_client_fd;
    fd_set readfds;
    int max_fd = socket_set[0];
    int bytes_received = 0;
    // to receive the potato
    while (1) {
        FD_ZERO(&readfds);
        // add 3 socket to the set
        for (int i = 0; i < 3; i++) {
            int fd = socket_set[i];
            if (fd > 0) {
                FD_SET(fd, &readfds);
            }
            if (fd > max_fd) {
                max_fd = fd;
            }
        }

        int num_ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);  
        if (num_ready < 0) {
            cerr << "error" << endl;
            return -1;
        } else if (num_ready == 0) {
            cerr << "time out" << endl;
            return -1;
        }

        for (int i = 0; i < 3; i++) {
            if (FD_ISSET(socket_set[i], &readfds)) {
                bytes_received = recv(socket_set[i], &buffer, sizeof(potato), MSG_WAITALL);
                break;
            }
        }
        // 没收到
        if (bytes_received == 0) {
            break;
        }
        // 收到了
        potato * p = (potato *) buffer;
        p->trace[p->counter] = id;

        cout << p->hops << endl;
        cout << p->trace[0] << endl;

        p->hops -= 1;
        p->counter += 1;
        break;

    }








    

}