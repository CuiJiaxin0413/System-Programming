#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <sstream>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

string read_sql(const char * path) {
  ifstream file(path);
  stringstream sqlStream;
  sqlStream << file.rdbuf();
  string sql = sqlStream.str();

  return sql;
}

void create_tables(string sql, connection * C) {
  work w(*C);
  w.exec(sql);
  w.commit();
}

void insertState(const char * path, connection * C) {
  string name;
  int state_id;

  string line;

  ifstream file(path);
  if (!file.is_open()) {
    cout << "cannot open file" << endl;
    exit(1);
  }

  while (getline(file, line)) {
    stringstream ss;
    ss << line;
    ss >> state_id >> name;
    add_state(C, name);
  }
  file.close();
}

void insertColor(const char * path, connection * C) {
  string name;
  int color_id;

  string line;

  ifstream file(path);
  if (!file.is_open()) {
    cout << "cannot open file" << endl;
    exit(1);
  }

  while (getline(file, line)) {
    stringstream ss;
    ss << line;
    ss >> color_id >> name;
    add_color(C, name);
  }
  file.close();
}

void insertTeam(const char * path, connection * C) {
  string name;
  int team_id, state_id, color_id, wins, losses;

  string line;

  ifstream file(path);
  if (!file.is_open()) {
    cout << "cannot open file" << endl;
    exit(1);
  }

  while (getline(file, line)) {
    stringstream ss;
    ss << line;
    ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
    add_team(C, name, state_id, color_id, wins, losses);
  }
  file.close();
}

void insertPlayer(const char * path, connection * C) {
  string first_name, last_name;
  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  double spg, bpg;

  string line;

  ifstream file(path);
  if (!file.is_open()) {
    cout << "cannot open file" << endl;
    exit(1);
  }

  while (getline(file, line)) {
    stringstream ss;
    ss << line;
    ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> mpg >> ppg >>
        rpg >> apg >> spg >> bpg;
    add_player(
        C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
  }

  file.close();
}

int main(int argc, char * argv[]) {
  //Allocate & initialize a Postgres connection object
  connection * C;

  try {
    //Establish a connection to the database
    //Parameters: database name, user name, user password

    //!!!!!!!!!!!!!!!!!!!!!!db name!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    C = new connection("dbname=acc_bball user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    }
    else {
      cout << "Can't open database" << endl;
      return 1;
    }
  }
  catch (const std::exception & e) {
    cerr << e.what() << std::endl;
    return 1;
  }

  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  string sql = read_sql("create.sql");
  create_tables(sql, C);
  //cout << "created tables" << endl;

  insertState("state.txt", C);
  insertColor("color.txt", C);
  insertTeam("team.txt", C);
  insertPlayer("player.txt", C);

  exercise(C);

  //Close database connection
  C->disconnect();

  return 0;
}
