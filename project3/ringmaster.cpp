#include <vector>

#include "potato.h"

using namespace std;

// 1. The ringmaster sets up a server socket and waits for players to connect.
// 2. The ringmaster send neighbour info to the players, and the players should connect to its neigbour
// 3. When all players have connected, the ringmaster creates a potato with a certain number of hops and sends it to a random player.

// init a potato

// start the game

// send potato to a random player

// check left hops

// end the game

// print trace

// ./ringmaster 1234 2 1
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
    vector<int> players_port_num;
    vector<char *> players_ip;

    // n connections with players
    for (int i = 0; i < num_players; i++) {
        int player_client_fd = server_connect_with_client(ringmaster_server_socket);
        // send id and total number of players to each player
        send_int(player_client_fd, i);
        send_int(player_client_fd, num_players);

        cout << "Player " << i << " is ready to play." << endl;

        players_socket_fd.push_back(player_client_fd);

        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        if (getsockname(player_client_fd, (struct sockaddr *) &client_addr, &len) == -1) {
            perror("getsockname");
            return 1;
        }

        char * ip = inet_ntoa(client_addr.sin_addr);
        //printf("IP address: %s\n", ip);
        //cout << sizeof(players_ip[i]) << endl;
        players_ip.push_back(ip);

        // receive from player
        int port_num = receive_int(player_client_fd);
        players_port_num.push_back(port_num);
    }

    // now the master has all the player's ip and port num
    // send neighbour info to players
    for (int i = 0; i < num_players; i++) {
        int right_player_id = (i + 1) % num_players;
        // send port and ip of this player to the its right
        send_int(players_socket_fd[right_player_id], players_port_num[i]);
        
        //cout << players_ip[i] << endl;

        send(players_socket_fd[right_player_id], players_ip[i], INET_ADDRSTRLEN, MSG_WAITALL);
    }

    // TODO: check if hops is 0

    // ringmaster init a potato
    potato p;
    memset(&p, 0, sizeof(potato));
    p.hops = num_hops;

    // send the potato to a random player
    srand((unsigned int)time(NULL));
    int random_player_id = random() % num_players;

    send(players_socket_fd[random_player_id], &p, sizeof(potato), MSG_WAITALL);

}