import entity.Color;
import entity.Player;
import entity.State;
import entity.Team;
import jakarta.persistence.EntityManager;
import jakarta.persistence.EntityManagerFactory;
import jakarta.persistence.EntityTransaction;
import jakarta.persistence.Persistence;
import jakarta.persistence.criteria.*;

import java.util.List;

public class Main {
    public static void query1(EntityManager entityManager,
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
        CriteriaBuilder criteriaBuilder = entityManager.getCriteriaBuilder();
        CriteriaQuery<Player> query = criteriaBuilder.createQuery(Player.class);
        Root<Player> root = query.from(Player.class);
        query.select(root);

        if (use_mpg == 1) {
            query.where(criteriaBuilder.between(root.get("mpg"), min_mpg, max_mpg));
        }
        if (use_ppg == 1) {
            query.where(criteriaBuilder.between(root.get("ppg"), min_ppg, max_ppg));
        }
        if (use_rpg == 1) {
            query.where(criteriaBuilder.between(root.get("rpg"), min_rpg, max_rpg));
        }
        if (use_apg == 1) {
            query.where(criteriaBuilder.between(root.get("apg"), min_apg, max_apg));
        }
        if (use_spg == 1) {
            query.where(criteriaBuilder.between(root.get("spg"), min_spg, max_spg));
        }
        if (use_bpg == 1) {
            query.where(criteriaBuilder.between(root.get("spg"), min_bpg, max_bpg));
        }

        List<Player> results = entityManager.createQuery(query).getResultList();

        System.out.println("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG");

        for (Player p : results) {
            System.out.println(p);
        }

    }

    public static void query2(EntityManager entityManager, String teamColor) {
        CriteriaBuilder criteriaBuilder = entityManager.getCriteriaBuilder();
        CriteriaQuery<Object[]> query = criteriaBuilder.createQuery(Object[].class);

        Root<Team> team = query.from(Team.class);
        Join<Team, Color> teamColorJoin = team.join("colorByColorId", JoinType.INNER);

        query.multiselect(team.get("name"), teamColorJoin.get("name"));
        query.where(criteriaBuilder.equal(teamColorJoin.get("name"), teamColor));

        List<Object[]> results = entityManager.createQuery(query).getResultList();

        System.out.println("NAME");
        for (Object[] result : results) {
            System.out.println(result[0]);
        }
    }

    // query3(): show the first and last name of each player that plays for the indicated team,
    // ordered from highest to lowest ppg (points per game)
    public static void query3(EntityManager entityManager, String teamName) {
        /*
             "SELECT player.first_name, player.last_name FROM player, team "
             "WHERE player.team_id = team.team_id "
             "AND team.name = $1 "
             "ORDER BY ppg DESC;"
        * */

        CriteriaBuilder criteriaBuilder = entityManager.getCriteriaBuilder();
        CriteriaQuery<Object[]> query = criteriaBuilder.createQuery(Object[].class);

        Root<Player> player = query.from(Player.class);
        Join<Player, Team> teamJoin = player.join("teamByTeamId", JoinType.INNER);

        query.multiselect(player.get("firstName"), player.get("lastName"))
                .where(criteriaBuilder.equal(teamJoin.get("name"), teamName))
                .orderBy(criteriaBuilder.desc(player.get("ppg")));

        List<Object[]> results = entityManager.createQuery(query).getResultList();

        System.out.println("FIRST_NAME LAST_NAME");

        for (Object[] result : results) {
            System.out.println(result[0]
                              + " " + result[1]);
        }


    }

    public static void query4(EntityManager entityManager, String stateName, String colorName) {
            /*
             "SELECT p.uniform_num, p.first_name, p.last_name "
             "FROM player p, team t, color c, state s "
             "WHERE p.team_id = t.team_id "
             "AND t.color_id = c.color_id "
             "AND t.state_id = s.state_id "
             "AND s.name = $1 "
             "AND c.name = $2;");
    * */
        CriteriaBuilder criteriaBuilder = entityManager.getCriteriaBuilder();
        CriteriaQuery<Object[]> query = criteriaBuilder.createQuery(Object[].class);

        Root<Player> player = query.from(Player.class);
        Join<Player, Team> playerTeamJoin = player.join("teamByTeamId", JoinType.INNER);
        Join<Team, Color> teamColorJoin = playerTeamJoin.join("colorByColorId", JoinType.INNER);
        Join<Team, State> teamStateJoin = playerTeamJoin.join("stateByStateId", JoinType.INNER);

        query.multiselect(player.get("uniformNum"), player.get("firstName"), player.get("lastName"))
                .where(criteriaBuilder.and(
                        criteriaBuilder.equal(teamStateJoin.get("name"), stateName),
                        criteriaBuilder.equal(teamColorJoin.get("name"), colorName)
                ));

        List<Object[]> results = entityManager.createQuery(query).getResultList();

        System.out.println("UNIFORM_NUM FIRST_NAME LAST_NAME");

        for (Object[] result : results) {
            System.out.println(result[0]
                    + " " + result[1]
                    + " " + result[2]);
        }



    }

    // query5(): show first name and last name of each player, and team name and number of
    // wins for each team that has won more than the indicated number of games
    public static void query5(EntityManager entityManager, int wins) {
        /*
             "SELECT player.first_name, player.last_name, team.name, team.wins "
             "FROM player, team "
             "WHERE player.team_id = team.team_id "
             "AND team.wins > $1"
        */
        CriteriaBuilder criteriaBuilder = entityManager.getCriteriaBuilder();
        CriteriaQuery<Object[]> query = criteriaBuilder.createQuery(Object[].class);

        Root<Player> player = query.from(Player.class);
        Join<Player, Team> playerTeamJoin = player.join("teamByTeamId", JoinType.INNER);

        query.multiselect(player.get("firstName"), player.get("lastName"), playerTeamJoin.get("name"), playerTeamJoin.get("wins"))
                .where(criteriaBuilder.greaterThan(playerTeamJoin.get("wins"), wins));

        List<Object[]> results = entityManager.createQuery(query).getResultList();

        System.out.println("FIRST_NAME LAST_NAME NAME WINS");

        for (Object[] result : results) {
            System.out.println(
                    result[0]
                  + " " + result[1]
                  + " " + result[2]
                + " " + result[3]);
        }


    }



    public static void main(String[] args) {
        EntityManagerFactory entityManagerFactory = Persistence.createEntityManagerFactory("default");
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        EntityTransaction transaction = entityManager.getTransaction();

        try {
            transaction.begin();

            query1(entityManager, 0, 0, 0, 1, 15, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            query2(entityManager, "Orange");
            query3(entityManager, "UNC");
            query4(entityManager, "NC", "DarkBlue");
            query5(entityManager, 13);

            transaction.commit();
        } finally {
            if (transaction.isActive()) {
                transaction.rollback();
            }
            entityManager.close();
            entityManagerFactory.close();
        }
    }
}
