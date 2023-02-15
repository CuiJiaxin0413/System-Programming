#include "potato.h"

using namespace std;

// player set up
void player_set_up(int socket_fd) {
    int id = receive_int(socket_fd);
    int total_num_player = receive_int(socket_fd);

    cout << "Connected as player " << id << " out of " << total_num_player << " total players" << endl;
}

// connect with neigbhours


// receive the potato

// pass to a random neigbour

/*
player <machine_name> <port_num>
(example: ./player vcm-xxxx.vm.duke.edu 1234)
*/

int main(int argc, char * argv[]) {
    if (argc != 3) {
        cerr << "Error: input format. Usage: ./player <machine_name> <port_num>" << endl;
        return -1;
    }
    const char *hostname = argv[1];
    const char *hostport = argv[2];

    int client_socket_fd = connect_with_server(hostname, hostport);

    player_set_up(client_socket_fd);
}