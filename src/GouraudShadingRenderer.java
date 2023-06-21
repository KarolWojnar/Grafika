import java.io.IOException;
import java.util.*;

public class GouraudShadingRenderer extends Renderer {

    public int maxDepth = 1000;
    public static Vec3f lightVector = new Vec3f(0, 0, 1);
    public static Vec3f cameraVector = new Vec3f(0, 0, 1);
    public static Map<Vec3i, Float> intensities = new HashMap<>();
    public static float[][] colors;

    public GouraudShadingRenderer(String filename, int w, int h) {
        super(filename, w, h);
        colors = new float[w][h];
    }

    public void render(Model model) {
        for (Vec3i face : model.getFaceList()) {

            Vec3i[] screen_coords = new Vec3i[3];
            Vec3f[] world_coords = new Vec3f[3];

            world_coords[0] = model.getVertex(face.x);
            world_coords[1] = model.getVertex(face.y);
            world_coords[2] = model.getVertex(face.z);

            for (int j = 0; j < 3; j++) {
                screen_coords[j] = new Vec3i((int) ((world_coords[j].x + 1.0) * render.getWidth() / 2.0),
                        (int) ((world_coords[j].y + 1.0) * render.getHeight() / 2.0) - render.getHeight() / 2,
                        (int) ((world_coords[j].z + 1.0) * 0.5 * maxDepth));
            }

            Vec3f normalVector = new Vec3f(0 ,0 ,0);
            var v1 = world_coords[0];
            var v2 = world_coords[1];
            var v3 = world_coords[2];
            normalVector.x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
            normalVector.y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
            normalVector.z = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
            float length = normalVector.length();
            normalVector.x /= length;
            normalVector.y /= length;
            normalVector.z /= length;

            float cameraValue = normalVector.x * cameraVector.x + normalVector.y * cameraVector.y + normalVector.z * cameraVector.z;
            if (cameraValue>=0) {

                for (Vec3i point : screen_coords) {
                    ArrayList<Vec3i> neighbourFaces = getNeighbourFaces(point, model);
                    ArrayList<Vec3f> normalVectors = new ArrayList<>();

                    for (Vec3i vector : neighbourFaces) {

                        Vec3f[] temp_world_coords = new Vec3f[3];

                        temp_world_coords[0] = model.getVertex(vector.x);
                        temp_world_coords[1] = model.getVertex(vector.y);
                        temp_world_coords[2] = model.getVertex(vector.z);

                        Vec3f tempNormalVector = new Vec3f(0, 0, 0);
                        var tempV1 = temp_world_coords[0];
                        var tempV2 = temp_world_coords[1];
                        var tempV3 = temp_world_coords[2];
                        tempNormalVector.x = (tempV2.y - tempV1.y) * (tempV3.z - tempV1.z) - (tempV2.z - tempV1.z) * (tempV3.y - tempV1.y);
                        tempNormalVector.y = (tempV2.z - tempV1.z) * (tempV3.x - tempV1.x) - (tempV2.x - tempV1.x) * (tempV3.z - tempV1.z);
                        tempNormalVector.z = (tempV2.x - tempV1.x) * (tempV3.y - tempV1.y) - (tempV2.y - tempV1.y) * (tempV3.x - tempV1.x);
                        float tempLength = tempNormalVector.length();
                        tempNormalVector.x /= tempLength;
                        tempNormalVector.y /= tempLength;
                        tempNormalVector.z /= tempLength;
                        normalVectors.add(tempNormalVector);
                    }

                    Vec3f tempToNormal = new Vec3f(0, 0, 0);
                    for (Vec3f norm : normalVectors) {
                        tempToNormal.x += norm.x;
                        tempToNormal.y += norm.y;
                        tempToNormal.z += norm.z;
                    }
                    float down = tempToNormal.length();

                    Vec3f averagedPointNormalVector = new Vec3f(0, 0, 0);
                    averagedPointNormalVector.x = tempToNormal.x / down;
                    averagedPointNormalVector.y = tempToNormal.y / down;
                    averagedPointNormalVector.z = tempToNormal.z / down;

                    float intensityAtPoint = averagedPointNormalVector.x * lightVector.x + averagedPointNormalVector.y * lightVector.y + averagedPointNormalVector.z * lightVector.z;

                    intensityAtPoint = Math.max(0, intensityAtPoint);
                    intensityAtPoint = Math.min(1, intensityAtPoint);

                    intensities.put(point, intensityAtPoint);
                }

                drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2]);
            }
        }
    }

    public ArrayList<Vec3i> getNeighbourFaces(Vec3i point, Model model) {
        ArrayList<Vec3i> neighbourFaces = new ArrayList<>();

        for (Vec3i face : model.getFaceList()) {

            Vec3f[] temp_world_coords = new Vec3f[3];
            Vec3i[] temp_screen_coords = new Vec3i[3];

            temp_world_coords[0] = model.getVertex(face.x);
            temp_world_coords[1] = model.getVertex(face.y);
            temp_world_coords[2] = model.getVertex(face.z);

            for (int j = 0; j < 3; j++) {
                temp_screen_coords[j] = new Vec3i((int) ((temp_world_coords[j].x + 1.0) * render.getWidth() / 2.0),
                        (int) ((temp_world_coords[j].y + 1.0) * render.getHeight() / 2.0) - render.getHeight() / 2,
                        (int) ((temp_world_coords[j].z + 1.0) * 0.5 * maxDepth));
            }

            Vec3f normalVector = new Vec3f(0 ,0 ,0);
            var v1 = temp_world_coords[0];
            var v2 = temp_world_coords[1];
            var v3 = temp_world_coords[2];
            normalVector.x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
            normalVector.y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
            normalVector.z = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
            float length = normalVector.length();
            normalVector.x /= length;
            normalVector.y /= length;
            normalVector.z /= length;

            float cameraValue = normalVector.x * cameraVector.x + normalVector.y * cameraVector.y + normalVector.z * cameraVector.z;
            if (cameraValue>=0) {
                for (Vec3i fPoint : temp_screen_coords) {
                    if (!neighbourFaces.contains(face)) {
                        if (fPoint.equals(point)) neighbourFaces.add(face);
                    }
                }
            }
        }

        return neighbourFaces;
    }

    public void drawTriangle(Vec3i A, Vec3i B, Vec3i C) {

        float minX = Integer.MAX_VALUE, maxX = Integer.MIN_VALUE, minY = Integer.MAX_VALUE, maxY = Integer.MIN_VALUE;
        maxX = Math.max(A.x, Math.max(B.x, C.x));
        minX = Math.min(A.x, Math.min(B.x, C.x));
        maxY = Math.max(A.y, Math.max(B.y, C.y));
        minY = Math.min(A.y, Math.min(B.y, C.y));

        minX = Math.max(0, minX);
        minY = Math.max(0, minY);
        maxX = Math.min(w, maxX);
        maxY = Math.min(h, maxY);

        for (int i=(int)minX; i<maxX; i++) {
            for (int j = (int) minY; j < maxY; j++) {
                Vec2f P = new Vec2f(i, j);

                Vec3f vector = barycentric(A, B, C, P);

                if (vector.x >= 0 && vector.y >= 0 && vector.z >= 0 && vector.x <= 1 && vector.y <= 1 && vector.z <= 1) {

                    double zbuffor = A.z * vector.x + B.z * vector.y + C.z * vector.z;
                    if (zbuffor < zbufforTable[i][j]) {
                        zbufforTable[i][j] = zbuffor;

                        float intA = intensities.get(A);
                        float intB = intensities.get(B);
                        float intC = intensities.get(C);

                        float intensity = vector.z*intA + vector.x*intB + vector.y*intC;

                        intensity = Math.min(1, intensity);
                        intensity = Math.max(0, intensity);

                        int color = 255 << 24 | ((int)(intensity*255)) << 16 | ((int)(intensity*255)) << 8 | ((int)(intensity*255));
                        colors[i][j] = intensity;
                        render.setRGB(i, j, color);
                    }
                }

            }
        }
    }
}