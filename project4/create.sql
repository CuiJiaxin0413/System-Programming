DROP TABLE IF EXISTS PLAYER, TEAM, STATE, COLOR;

CREATE TABLE COLOR(
    COLOR_ID SERIAL PRIMARY KEY NOT NULL,
    NAME VARCHAR(100)
);

CREATE TABLE STATE(
  STATE_ID SERIAL PRIMARY KEY NOT NULL,
  NAME VARCHAR(100)
);

CREATE TABLE TEAM(
    TEAM_ID SERIAL PRIMARY KEY NOT NULL,
    NAME VARCHAR(100),
    STATE_ID INT NOT NULL, 
    COLOR_ID INT NOT NULL,
    WINS INT,
    LOSSES INT,
    FOREIGN KEY(STATE_ID) REFERENCES STATE(STATE_ID) ON DELETE SET NULL ON UPDATE CASCADE,
    FOREIGN KEY(COLOR_ID) REFERENCES COLOR(COLOR_ID) ON DELETE SET NULL ON UPDATE CASCADE
);

CREATE TABLE PLAYER(
    PLAYER_ID SERIAL PRIMARY KEY NOT NULL,
    TEAM_ID INT NOT NULL,
    UNIFORM_NUM INT,
    FIRST_NAME VARCHAR(100),
    LAST_NAME VARCHAR(100),
    MPG INT,
    PPG INT,
    RPG INT,
    APG INT,
    SPG DOUBLE PRECISION,
    BPG DOUBLE PRECISION,
    FOREIGN KEY(TEAM_ID) REFERENCES TEAM(TEAM_ID) ON DELETE SET NULL ON UPDATE CASCADE
);



