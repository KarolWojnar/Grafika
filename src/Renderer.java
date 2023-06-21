//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Renderer {
    private BufferedImage render;
    public final int h;
    public final int w;
    private String filename;
    private LineAlgo lineAlgo;

    public Renderer(String filename, int width, int height, String algo) {
        this.w = width;
        this.h = height;
        this.render = new BufferedImage(this.w, this.h, 2);
        this.filename = filename;
        if (algo.equals("LINE_NAIVE")) {
            this.lineAlgo = Renderer.LineAlgo.NAIVE;
        }

        if (algo.equals("BRESENHAM")) {
            this.lineAlgo = Renderer.LineAlgo.BRESENHAM;
        }

        if (algo.equals("BRESENHAM_INT")) {
            this.lineAlgo = Renderer.LineAlgo.BRESENHAM_INT;
        }

    }

    public LineAlgo getLineAlgo() {
        return this.lineAlgo;
    }

    public void drawPoint(int x, int y) {
        int white = -1;
        this.render.setRGB(x, y, white);
    }

    public void drawLine(int x0, int y0, int x1, int y1, LineAlgo lineAlgo) {
        if (lineAlgo == Renderer.LineAlgo.NAIVE) {
            this.drawLineNaive(x0, y0, x1, y1);
        }

        if (lineAlgo == Renderer.LineAlgo.DDA) {
            this.drawLineDDA(x0, y0, x1, y1);
        }

        if (lineAlgo == Renderer.LineAlgo.BRESENHAM) {
            this.drawLineBresenham(x0, y0, x1, y1);
        }

        if (lineAlgo == Renderer.LineAlgo.BRESENHAM_INT) {
            this.drawLineBresenhamInt(x0, y0, x1, y1);
        }

    }

    public void drawLineNaive(int x0, int y0, int x1, int y1) {
        double a = ((double)y1 * 1.0 - (double)y0 * 1.0) / ((double)x1 * 1.0 - (double)x0 * 1.0);
        double b = (double)y1 * 1.0 - a * (double)x1;
        int white = -1;
        if (x0 - x1 != 0 && a != 0.0) {
            System.out.println("f(x) = " + a + "x + " + b);

            for(double i = (double)Math.min(x0, x1); i < (double)Math.max(x0, x1); i += 1.0 / a) {
                int y = (int)Math.round(a * i + b);
                if (y >= Math.min(y0, y1) && y <= Math.max(y0, y1)) {
                    this.render.setRGB((int)Math.round(i), y, white);
                }
            }
        } else {
            int i;
            if (x0 - x1 == 0) {
                for(i = Math.min(y0, y1); i < Math.max(y0, y1); ++i) {
                    this.render.setRGB(x0, i, white);
                }
            } else {
                for(i = Math.min(x0, x1); i < Math.max(x0, x1); ++i) {
                    this.render.setRGB(i, y0, white);
                }
            }
        }

    }

    public void drawLineDDA(int x0, int y0, int x1, int y1) {
    }

    public void drawLineBresenham(int x0, int y0, int x1, int y1) {
        int white = -1;
        int dx = x1 - x0;
        int dy = y1 - y0;
        float derr = Math.abs((float)dy / (float)dx);
        float err = 0.0F;
        int y = y0;

        for(int x = x0; x <= x1; ++x) {
            this.render.setRGB(x, y, white);
            err += derr;
            if ((double)err > 0.5) {
                y += y1 > y0 ? 1 : -1;
                err = (float)((double)err - 1.0);
            }
        }

    }

    public void drawLineBresenhamInt(int x0, int y0, int x1, int y1) {
        int white = -1;
        int dx = Math.abs(x1 - x0);
        int dy = Math.abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;

        while(x0 != x1 || y0 != y1) {
            this.render.setRGB(x0, y0, white);
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }

    }

    public void save() throws IOException {
        File outputfile = new File(this.filename);
        this.render = verticalFlip(this.render);
        ImageIO.write(this.render, "png", outputfile);
    }

    public void clear() {
        for(int x = 0; x < this.w; ++x) {
            for(int y = 0; y < this.h; ++y) {
                int black = -16777216;
                this.render.setRGB(x, y, black);
            }
        }

    }

    public static BufferedImage verticalFlip(BufferedImage img) {
        int w = img.getWidth();
        int h = img.getHeight();
        BufferedImage flippedImage = new BufferedImage(w, h, img.getColorModel().getTransparency());
        Graphics2D g = flippedImage.createGraphics();
        g.drawImage(img, 0, 0, w, h, 0, h, w, 0, (ImageObserver)null);
        g.dispose();
        return flippedImage;
    }

    public static enum LineAlgo {
        NAIVE,
        DDA,
        BRESENHAM,
        BRESENHAM_INT;

        private LineAlgo() {
        }
    }
}
