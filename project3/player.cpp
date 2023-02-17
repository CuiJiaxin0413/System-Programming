#include "potato.h"

using namespace std;

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

    // int id = player_set_up(to_master_client_socket_fd);

    int id = receive_int(to_master_client_socket_fd);
    int total_num_player = receive_int(to_master_client_socket_fd);
    cout << "Connected as player " << id << " out of " << total_num_player << " total players" << endl;

    // player server socket port number should be assigned by os???
    int server_socket_fd = init_server_socket("");
    int player_server_port_num = get_port_num_by_socket(server_socket_fd);
    // send its server port to the master
    send_int(to_master_client_socket_fd, player_server_port_num);

    // receive info of left neigbour from the master
    int left_port_num = receive_int(to_master_client_socket_fd);
    //cout << "left port:" << left_port_num << endl;
    char left_port_num_char[10];
    sprintf(left_port_num_char, "%d", left_port_num);
    
    char ip[INET_ADDRSTRLEN];
    if (recv(to_master_client_socket_fd, ip, INET_ADDRSTRLEN, MSG_WAITALL) == -1) {
        perror("receive failed line 46");
        exit(1);
    }

    //cout << "left ip: " << ip << endl;

    // connect with left neighbour first
    // client will return immeditealy
    int left_client_fd = connect_with_server(ip, left_port_num_char);
    // accept neighbour connect
    int right_fd = server_connect_with_client(server_socket_fd);


    char buffer[sizeof(potato)];

    int socket_set[3];
    socket_set[0] = to_master_client_socket_fd;
    socket_set[1] = right_fd;
    socket_set[2] = left_client_fd;
    fd_set readfds;
    int max_fd = socket_set[0];
    int bytes_received = -2;
    // to receive the potato
    while (1) {
        FD_ZERO(&readfds);
        // add 3 socket to the set
        for (int i = 0; i < 3; i++) {
            int fd = socket_set[i];
            FD_SET(fd, &readfds);
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

        potato p;

        for (int i = 0; i < 3; i++) {
            if (FD_ISSET(socket_set[i], &readfds)) {
                bytes_received = recv(socket_set[i], &p, sizeof(potato), MSG_WAITALL);
                break;
            }
        }

        // game end, close from server
        if (bytes_received == 0) {
            break;
        }

        if (bytes_received > 0) {
            // 收到potato
            //potato * p = (potato *) buffer;
            p.trace[p.counter] = id;
            p.hops -= 1;
            p.counter += 1;


            // the remaining number of hops is greater than 0, 
            // ramdomly choose a neigbhor and send the potato
            if (p.hops > 0) {
                int rand_num = rand() % 2;
                if (rand_num == 0) {  // send potato to right
                    send(right_fd, &p, sizeof(potato), MSG_WAITALL);
                    int right_id = (id + 1) % total_num_player;
                    cout << "Sending potato to " << right_id << endl;
                } else {
                    send(left_client_fd, &p, sizeof(potato), MSG_WAITALL);
                    int left_id = (id - 1 + total_num_player) % total_num_player;
                    cout << "Sending potato to " << left_id << endl;
                }
            } else { // hops == 0, send to master
                send(to_master_client_socket_fd, &p, sizeof(potato), MSG_WAITALL);
                cout << "I'm it" << endl;
                //break;
            }

        }

        
        //break;

    }


    // close(to_master_client_socket_fd);
    // close(server_socket_fd);
    // close(left_client_fd);

    return 0;
}