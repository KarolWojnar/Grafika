import java.util.Objects;

public class Vec3i {
    public int x;
    public int y;
    public int z;

    public Vec3i(int x, int y, int z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public String toString() {
        return x + " " + y + " " + z;
    }

    public boolean equals(Vec3i vec3i) {
        return this.x == vec3i.x && this.y == vec3i.y && this.z == vec3i.z;
    }
}