package entity;

import jakarta.persistence.*;

import java.util.Collection;

@Entity
public class Team {
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    @Column(name = "TEAM_ID")
    private int teamId;
    @Basic
    @Column(name = "NAME")
    private String name;

    @Basic
    @Column(name = "WINS")
    private Integer wins;
    @Basic
    @Column(name = "LOSSES")
    private Integer losses;
    @OneToMany(mappedBy = "teamByTeamId")
    private Collection<Player> playersByTeamId;
    @ManyToOne
    @JoinColumn(name = "STATE_ID", referencedColumnName = "STATE_ID")
    private State stateByStateId;
    @ManyToOne
    @JoinColumn(name = "COLOR_ID", referencedColumnName = "COLOR_ID")
    private Color colorByColorId;


    public int getTeamId() {
        return teamId;
    }

    public void setTeamId(int teamId) {
        this.teamId = teamId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    // public Integer getStateId() {
    //     return stateId;
    // }
    //
    // public void setStateId(Integer stateId) {
    //     this.stateId = stateId;
    // }

    // public Integer getColorId() {
    //     return colorId;
    // }

    // public void setColorId(Integer colorId) {
    //     this.colorId = colorId;
    // }

    public Integer getWins() {
        return wins;
    }

    public void setWins(Integer wins) {
        this.wins = wins;
    }

    public Integer getLosses() {
        return losses;
    }

    public void setLosses(Integer losses) {
        this.losses = losses;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Team team = (Team) o;

        if (teamId != team.teamId) return false;
        if (name != null ? !name.equals(team.name) : team.name != null) return false;
        //if (stateId != null ? !stateId.equals(team.stateId) : team.stateId != null) return false;
        //if (colorId != null ? !colorId.equals(team.colorId) : team.colorId != null) return false;
        if (wins != null ? !wins.equals(team.wins) : team.wins != null) return false;
        if (losses != null ? !losses.equals(team.losses) : team.losses != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = teamId;
        result = 31 * result + (name != null ? name.hashCode() : 0);
        //result = 31 * result + (stateId != null ? stateId.hashCode() : 0);
        // result = 31 * result + (colorId != null ? colorId.hashCode() : 0);
        result = 31 * result + (wins != null ? wins.hashCode() : 0);
        result = 31 * result + (losses != null ? losses.hashCode() : 0);
        return result;
    }

    public Collection<Player> getPlayersByTeamId() {
        return playersByTeamId;
    }

    public void setPlayersByTeamId(Collection<Player> playersByTeamId) {
        this.playersByTeamId = playersByTeamId;
    }

    public State getStateByStateId() {
        return stateByStateId;
    }

    public void setStateByStateId(State stateByStateId) {
        this.stateByStateId = stateByStateId;
    }

    public Color getColorByColorId() {
        return colorByColorId;
    }

    public void setColorByColorId(Color colorByColorId) {
        this.colorByColorId = colorByColorId;
    }

    @Override
    public String toString() {
        return "" + teamId;
    }
}
