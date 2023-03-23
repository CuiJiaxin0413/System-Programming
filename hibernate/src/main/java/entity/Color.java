package entity;

import jakarta.persistence.*;

@Entity
public class Color {
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    @Column(name = "COLOR_ID")
    private int colorId;
    @Basic
    @Column(name = "NAME")
    private String name;

    public int getColorId() {
        return colorId;
    }

    public void setColorId(int colorId) {
        this.colorId = colorId;
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

        Color color = (Color) o;

        if (colorId != color.colorId) return false;
        if (name != null ? !name.equals(color.name) : color.name != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = colorId;
        result = 31 * result + (name != null ? name.hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return "Color{" + "colorId=" + colorId + ", name='" + name + '\'' + '}';
    }
}
