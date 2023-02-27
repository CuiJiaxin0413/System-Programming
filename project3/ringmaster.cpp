#include <vector>

#include "potato.h"

using namespace std;

// 1. The ringmaster sets up a server socket and waits for players to connect.
// 2. The ringmaster send neighbour info to the players, and the players should connect to its neigbour
// 3. When all players have connected, the ringmaster creates a potato with a certain number of hops and sends it to a random player.

// ./ringmaster 1234 2 1
int main(int argc, char * argv[]) {
    if (argc != 4) {
        cerr << "Error: input format. Usage: ./ringmaster <port_num> <num_players> <num_hops>" << endl;
        return -1;
    }

    const char *server_port_num = argv[1];
    int num_players = stoi(argv[2]);
    int num_hops = stoi(argv[3]);

    if (num_players < 2) {
        cerr << "Error: num_players must be greater than 1" << endl;
        return -1;
    }

    if (num_hops < 0 || num_hops > 512) {
        cerr << "Error: num_hops must be greater than or equal to zero and less than or equal to 512" << endl;
        return -1;
    }

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

        cout << "Player " << i << " is ready to play" << endl;

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

    // ringmaster init a potato
    potato p;
    memset(&p, 0, sizeof(potato));
    p.hops = num_hops;

    // hop == 0, just end the game
    if (p.hops == 0) {
        close(ringmaster_server_socket);
        for (int i = 0; i < num_players; i++) {
            close(players_socket_fd[i]);
        }
        return 0;
    }


    // send the potato to a random player
    srand((unsigned int)time(NULL));
    int random_player_id = random() % num_players;
    send(players_socket_fd[random_player_id], &p, sizeof(potato), MSG_WAITALL);
    cout << "Ready to start the game, sending potato to player " << random_player_id << endl;

    // listen to all the n connections with the player to receive the potato and end the game
    // this does not need a loop, because ringmaster just receive one time for the potato
    fd_set readfds;
    FD_ZERO(&readfds);
    int max_fd = players_socket_fd[0];
    int bytes_received = 0;
    for (int i = 0; i < num_players; i++) {
        if (players_socket_fd[i] > max_fd) {
            max_fd = players_socket_fd[i];
            
        } 
        FD_SET(players_socket_fd[i], &readfds);
    }

    select(max_fd+1, &readfds, NULL, NULL, NULL);

    for (int i = 0; i < num_players; i++) {
        if (FD_ISSET(players_socket_fd[i], &readfds)) {
            //cout << "here" << endl;
            int bytes = recv(players_socket_fd[i], &p, sizeof(potato), MSG_WAITALL);
            if (bytes != 0) {
                break;
            }
            
        }
    }

    // print trace of the potato
    cout << "Trace of the potato:" << endl;
    for (int i = 0; i < p.counter; i++) {
        if (i != p.counter - 1) {
            cout << p.trace[i] << ",";
        } else {
            cout << p.trace[i];
        }
        
    }
    cout << endl;

    // close the socket
    close(ringmaster_server_socket);
    for (int i = 0; i < num_players; i++) {
        close(players_socket_fd[i]);
    }
}