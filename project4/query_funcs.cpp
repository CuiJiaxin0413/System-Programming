#include "query_funcs.h"

// https://libpqxx.readthedocs.io/en/6.1.1/a01408.html
void add_player(connection * C,
                int team_id,
                int jersey_num,
                string first_name,
                string last_name,
                int mpg,
                int ppg,
                int rpg,
                int apg,
                double spg,
                double bpg) {
  work w(*C);
  C->prepare("add_player",
             "INSERT INTO player(team_id, uniform_num, first_name, last_name, mpg, ppg, "
             "rpg, apg, spg, bpg)"
             "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)");
  w.exec_prepared("add_player",
                  team_id,
                  jersey_num,
                  first_name,
                  last_name,
                  mpg,
                  ppg,
                  rpg,
                  apg,
                  spg,
                  bpg);
  w.commit();
}

void add_team(connection * C,
              string name,
              int state_id,
              int color_id,
              int wins,
              int losses) {
  work w(*C);
  C->prepare("add_team",
             "INSERT INTO TEAM(name, state_id, color_id, wins, losses) VALUES($1, $2, "
             "$3, $4, $5)");
  w.exec_prepared("add_team", name, state_id, color_id, wins, losses);
  w.commit();
}

void add_state(connection * C, string name) {
  work w(*C);
  C->prepare("add_state", "INSERT INTO STATE(name) VALUES($1)");
  w.exec_prepared("add_state", name);
  w.commit();
}

void add_color(connection * C, string name) {
  work w(*C);
  C->prepare("add_color", "INSERT INTO COLOR(name) VALUES($1)");
  w.exec_prepared("add_color", name);
  w.commit();
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */
// query1(): show all attributes of each player with average statistics that fall between the
// min and max (inclusive) for each enabled statistic
void query1(connection * C,
            int use_mpg,
            int min_mpg,
            int max_mpg,
            int use_ppg,
            int min_ppg,
            int max_ppg,
            int use_rpg,
            int min_rpg,
            int max_rpg,
            int use_apg,
            int min_apg,
            int max_apg,
            int use_spg,
            double min_spg,
            double max_spg,
            int use_bpg,
            double min_bpg,
            double max_bpg) {
  stringstream select;
  select << "SELECT * FROM PLAYER ";
  bool firstUseFlag = true;
  //string cond = firstUseFlag ? "WHERE " : "AND ";
  if (use_mpg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "(mpg BETWEEN " << min_mpg << " AND " << max_mpg << ") ";
    firstUseFlag = false;
  }
  if (use_ppg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "(ppg BETWEEN " << min_ppg << " AND " << max_ppg << ") ";
    firstUseFlag = false;
  }
  if (use_rpg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "(rpg BETWEEN " << min_rpg << " AND " << max_rpg << ") ";
    firstUseFlag = false;
  }
  if (use_apg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "(apg BETWEEN " << min_apg << " AND " << max_apg << ") ";
    firstUseFlag = false;
  }
  if (use_spg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "spg BETWEEN " << min_spg << " AND " << max_spg << ") ";
    firstUseFlag = false;
  }
  if (use_bpg) {
    string cond = firstUseFlag ? "WHERE " : "AND ";
    select << cond;
    select << "bpg BETWEEN " << min_bpg << " AND " << max_bpg << ") ";
    firstUseFlag = false;
  }

  select << ";";
  //cout << select.str() << endl;
  //cout << select.str();
  work w(*C);
  C->prepare("query1",
             select.str());
  result r = w.exec_prepared("query1");
  cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;
  for (const auto & row : r) {
    cout << row["player_id"].as<int>() << " "
         << row["team_id"].as<int>() << " "
         << row["uniform_num"].as<int>() << " "
         << row["first_name"].as<string>() << " "
         << row["last_name"].as<string>() << " "
         << row["mpg"].as<int>() << " "
         << row["ppg"].as<int>() << " "
         << row["rpg"].as<int>() << " "
         << row["apg"].as<int>() << " "
         << row["spg"].as<double>() << " "
         << row["bpg"].as<double>() << " " << endl;
  }
  w.commit();
}

// query2(): show the name of each team with the indicated uniform color
void query2(connection * C, string team_color) {
  work w(*C);
  C->prepare("query2",
             "SELECT TEAM.NAME FROM TEAM, COLOR "
             "WHERE TEAM.COLOR_ID = COLOR.COLOR_ID "
             "AND COLOR.NAME = $1");
  result r = w.exec_prepared("query2", team_color);
  cout << "NAME" << endl;
  for (const auto & row : r) {
    cout << row["name"].as<string>() << endl;
  }
  w.commit();
}

// query3(): show the first and last name of each player that plays for the indicated team,
// ordered from highest to lowest ppg (points per game)
void query3(connection * C, string team_name) {
  work w(*C);
  C->prepare("query3",
             "SELECT player.first_name, player.last_name FROM player, team "
             "WHERE player.team_id = team.team_id "
             "AND team.name = $1 "
             "ORDER BY ppg DESC;");
  result r = w.exec_prepared("query3", team_name);
  cout << "FIRST_NAME LAST_NAME" << endl;
  for (const auto & row : r) {
    cout << row["first_name"].as<string>() << " "
         << row["last_name"].as<string>() << endl;
  }
  w.commit();
}

// query4(): show uniform number, first name and last name of each player that plays in
// the indicated state and wears the indicated uniform color
void query4(connection * C, string team_state, string team_color) {
  work w(*C);
  C->prepare("query4",
             "SELECT p.uniform_num, p.first_name, p.last_name "
             "FROM player p, team t, color c, state s "
             "WHERE p.team_id = t.team_id "
             "AND t.color_id = c.color_id "
             "AND t.state_id = s.state_id "
             "AND s.name = $1 "
             "AND c.name = $2;");
  result r = w.exec_prepared("query4", team_state, team_color);
  cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;
  for (const auto & row : r) {
    cout << row["uniform_num"].as<std::string>() << " "
         << row["first_name"].as<std::string>() << " "
         << row["last_name"].as<std::string>() << endl;
  }
  w.commit();
}

//○ query5(): show first name and last name of each player, and team name and number of
// wins for each team that has won more than the indicated number of games

void query5(connection * C, int num_wins) {
  work w(*C);
  C->prepare("query5",
             "SELECT player.first_name, player.last_name, team.name, team.wins "
             "FROM player "
             "INNER JOIN team "
             "ON player.team_id = team.team_id "
             "WHERE team.wins > $1");
  result r = w.exec_prepared("query5", num_wins);
  cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
  for (const auto & row : r) {
    cout << row["first_name"].as<string>() << " " << row["last_name"].as<string>() << " "
         << row["name"].as<string>() << " " << row["wins"].as<string>() << endl;
  }
  w.commit();
}
