import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

import javax.swing.*;

/**
 * Testing code for Flier in Lab 1.
 * 
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Winter 2014
 * @author Travis W. Peters, Dartmouth CS 10, Updated Winter 2015
 */
public class FlierTest extends DrawingFrame {

    private Flier flier;								// one test flier
    private ArrayList<ArrayList<Point>> regions;		// one test region

    public FlierTest(String filename) {
        super("Flier Test", filename);

        // Create a test region that the flier might hit.
        ArrayList<Point> region = new ArrayList<Point>();
        for (int y=image.getHeight()/3; y<image.getWidth()*2/3; y++) {
            for (int x=image.getWidth()/3; x<image.getWidth()*2/3; x++) {
                region.add(new Point(x,y));
            }
        }
        regions = new ArrayList<ArrayList<Point>>();
        regions.add(region);

        // Create a test universe that the flier exists within.
        Universe universe = new Universe(image); // image loaded by super constructor
        universe.setRegions(regions);

        // Create and toss the flier.
        flier = new Flier(universe);
        flier.toss();

        // Move every 100 milliseconds.
        Timer timer = new Timer(100, new AbstractAction("update") {
            public void actionPerformed(ActionEvent e) {
                flier.move();
                System.out.println(flier.getX()+","+flier.getY());
                flier.checkLose();
                flier.checkWin();
                canvas.repaint();
            }
        });
        timer.start();
    }

    /**
     * Override method from DrawingFrame, in order to also draw the region and flier
     */
    public void draw(Graphics g) {
        super.draw(g);

        for (ArrayList<Point> region : regions) {
            for (Point point : region) {
                g.drawRect(point.x, point.y, 1, 1);
            }
        }

        flier.draw(g);
    }

    /**
     * Main method for the application
     * @param args      command-line arguments (ignored)
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new FlierTest("pictures/baker.jpg");
            }
        });
    }

}