import java.util.ArrayList;

public class Neighbours {
    public ArrayList<Vec3i> list;

    public Neighbours () {
        list = new ArrayList<>();
    }

    public ArrayList<Vec3i> getNeighbours(Model model, Vec3f[] coords) {

        for (Vec3i face : model.getFaceList()) {
            if (face.x == coords[0].x || face.x == coords[1].x || face.x == coords[2].x || face.y == coords[0].y || face.y == coords[1].y || face.y == coords[2].y || face.z == coords[0].z || face.z == coords[1].z || face.z == coords[2].z) {
                list.add(face);
            }
        }

        return list;
    }
}