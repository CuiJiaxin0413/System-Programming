#include <vector>

#include "potato.h"

using namespace std;

// 1. The ringmaster sets up a server socket and waits for players to connect.
// 2. When all players have connected, the ringmaster creates a potato with a certain number of hops and sends it to a random player.


void send_neighbor_info() {

}

// init a potato

// start the game

// send potato to a random player

// check left hops

// end the game

// print trace


int main(int argc, char * argv[]) {
    if (argc != 4) {
        cerr << "Error: input format. Usage: ./ringmaster <port_num> <num_players> <num_hops>" << endl;
        return -1;
    }

    const char *server_port_num = argv[1];
    int num_players = stoi(argv[2]);
    int num_hops = stoi(argv[3]);

    int ringmaster_server_socket = init_server_socket(server_port_num);

    cout << "Potato Ringmaster" << endl;
    cout << "Players = " << num_players << endl;;
    cout<< "Hops = " << num_hops << endl;

    vector<int> players_socket_fd;
    vector<int> players_porm_num;
    vector<string> players_ip;

    for (int i = 0; i < num_players; i++) {
        int player_connection_fd = server_connect_with_client(ringmaster_server_socket);
        // send id and total number of players to each player
        send_int(player_connection_fd, i);
        send_int(player_connection_fd, num_players);

        players_socket_fd.push_back(player_connection_fd);
        
    }

}