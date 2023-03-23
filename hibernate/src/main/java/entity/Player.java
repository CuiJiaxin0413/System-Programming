package entity;

import jakarta.persistence.*;

@Entity
public class Player {
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    @Column(name = "PLAYER_ID")
    private int playerId;
    // @Basic
    // @Column(name = "TEAM_ID")
    // private Integer teamId;
    @Basic
    @Column(name = "UNIFORM_NUM")
    private Integer uniformNum;
    @Basic
    @Column(name = "FIRST_NAME")
    private String firstName;
    @Basic
    @Column(name = "LAST_NAME")
    private String lastName;
    @Basic
    @Column(name = "MPG")
    private Integer mpg;
    @Basic
    @Column(name = "PPG")
    private Integer ppg;
    @Basic
    @Column(name = "RPG")
    private Integer rpg;
    @Basic
    @Column(name = "APG")
    private Integer apg;
    @Basic
    @Column(name = "SPG")
    private Double spg;
    @Basic
    @Column(name = "BPG")
    private Double bpg;

    @ManyToOne
    @JoinColumn(name = "TEAM_ID", referencedColumnName = "TEAM_ID")
    private Team teamByTeamId;

    public int getPlayerId() {
        return playerId;
    }

    public void setPlayerId(int playerId) {
        this.playerId = playerId;
    }

    // public Integer getTeamId() {
    //     return teamId;
    // }
    //
    // public void setTeamId(Integer teamId) {
    //     this.teamId = teamId;
    // }

    public Integer getUniformNum() {
        return uniformNum;
    }

    public void setUniformNum(Integer uniformNum) {
        this.uniformNum = uniformNum;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public Integer getMpg() {
        return mpg;
    }

    public void setMpg(Integer mpg) {
        this.mpg = mpg;
    }

    public Integer getPpg() {
        return ppg;
    }

    public void setPpg(Integer ppg) {
        this.ppg = ppg;
    }

    public Integer getRpg() {
        return rpg;
    }

    public void setRpg(Integer rpg) {
        this.rpg = rpg;
    }

    public Integer getApg() {
        return apg;
    }

    public void setApg(Integer apg) {
        this.apg = apg;
    }

    public Double getSpg() {
        return spg;
    }

    public void setSpg(Double spg) {
        this.spg = spg;
    }

    public Double getBpg() {
        return bpg;
    }

    public void setBpg(Double bpg) {
        this.bpg = bpg;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Player player = (Player) o;

        if (playerId != player.playerId) return false;
        //if (teamId != null ? !teamId.equals(player.teamId) : player.teamId != null) return false;
        if (uniformNum != null ? !uniformNum.equals(player.uniformNum) : player.uniformNum != null) return false;
        if (firstName != null ? !firstName.equals(player.firstName) : player.firstName != null) return false;
        if (lastName != null ? !lastName.equals(player.lastName) : player.lastName != null) return false;
        if (mpg != null ? !mpg.equals(player.mpg) : player.mpg != null) return false;
        if (ppg != null ? !ppg.equals(player.ppg) : player.ppg != null) return false;
        if (rpg != null ? !rpg.equals(player.rpg) : player.rpg != null) return false;
        if (apg != null ? !apg.equals(player.apg) : player.apg != null) return false;
        if (spg != null ? !spg.equals(player.spg) : player.spg != null) return false;
        if (bpg != null ? !bpg.equals(player.bpg) : player.bpg != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = playerId;
        //result = 31 * result + (teamId != null ? teamId.hashCode() : 0);
        result = 31 * result + (uniformNum != null ? uniformNum.hashCode() : 0);
        result = 31 * result + (firstName != null ? firstName.hashCode() : 0);
        result = 31 * result + (lastName != null ? lastName.hashCode() : 0);
        result = 31 * result + (mpg != null ? mpg.hashCode() : 0);
        result = 31 * result + (ppg != null ? ppg.hashCode() : 0);
        result = 31 * result + (rpg != null ? rpg.hashCode() : 0);
        result = 31 * result + (apg != null ? apg.hashCode() : 0);
        result = 31 * result + (spg != null ? spg.hashCode() : 0);
        result = 31 * result + (bpg != null ? bpg.hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return  playerId + " " + teamByTeamId + " " + uniformNum + " " + firstName + " " + lastName  + " " + mpg + " " + ppg + " " + rpg + " " + apg + " " + spg + " " + bpg;
    }
}
