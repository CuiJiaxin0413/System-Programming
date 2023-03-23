package entity;

import jakarta.persistence.*;

@Entity
public class State {
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    @Column(name = "STATE_ID")
    private int stateId;
    @Basic
    @Column(name = "NAME")
    private String name;

    public int getStateId() {
        return stateId;
    }

    public void setStateId(int stateId) {
        this.stateId = stateId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        State state = (State) o;

        if (stateId != state.stateId) return false;
        if (name != null ? !name.equals(state.name) : state.name != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = stateId;
        result = 31 * result + (name != null ? name.hashCode() : 0);
        return result;
    }
}
