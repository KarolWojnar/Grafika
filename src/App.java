import javax.swing.*;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class App {
    String version = "0.02";

    public App() {
    }

    public static void main(String[] args) {
        Renderer mainRenderer = new Renderer(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]), args[3]);
        mainRenderer.clear();
        mainRenderer.drawLine(300, 350, 50, 50, mainRenderer.getLineAlgo());

        try {
            mainRenderer.save();
        } catch (IOException var3) {
            Logger.getLogger(App.class.getName()).log(Level.SEVERE, (String)null, var3);
        }

    }

    public String getVersion() {
        return this.version;
    }
}