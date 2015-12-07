import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import javax.swing.AbstractAction;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

/**
 * Webcam-based catch game
 * Sample solution to Lab 1, Dartmouth CS 10, Winter 2015
 * 
 * @author Chris Bailey-Kellogg, Winter 2014 (based on a very different structure from Fall 2012)
 * @author Travis W. Peters, Dartmouth CS 10, Updated Winter 2015
 * @author Zach Tannenbaum and Nicky Golini
 */
public class Catch extends Webcam {
    private Universe universe;			       // handles finding regions
    private static final int numFliers = 5;  // the # of fliers to create 

    /**
     * Overrides Webcam method to do the region finding the flier stuff.
     */
    public void processImage() {
    	if (universe != null) {	
        	// Set the image in universe
            // TODO: YOUR CODE HERE
        	universe.setImage(image);
   	
            // Detect regions and recolor
            // TODO: YOUR CODE HERE
        	universe.findRegions();
        	universe.recolorRegions();

            // Move the fliers and detect catches and misses
            // TODO: YOUR CODE HERe
        	Timer timer = new Timer(1, new AbstractAction("update") {
                public void actionPerformed(ActionEvent e) {
                	universe.moveFliers();
                    universe.setImage(image);
                }
            });
            timer.start();
        }
    }

    /**
     * Overrides the Webcam method to also draw the flier.
     */
    public void draw(Graphics g) {
        super.draw(g);
        // Draw fliers
       if(universe != null) {
    	   universe.drawFliers(g);
       }
    }

    /**
     * Overrides the Webcam method to create the universe, set the track 
     * color and start the flier.
     */
    public void handleMousePress(MouseEvent event) {
        // Create universe
        // TODO: YOUR CODE HERE       
    	universe = new Universe(image); 	

        // Set tracking color
        // TODO: YOUR CODE HERE       
        int x = event.getX();
        int y = event.getY();
        Color color = new Color(image.getRGB(x, y));
        universe.setTrackingColor(color);

        // Create fliers and start them flying
        // TODO: YOUR CODE HERE               
        for (int i = 0; i < numFliers; i++) {
        	Flier flier = new Flier(universe);
        	universe.addFlier(flier);
        	flier.toss();
        } 
    }

    /**
     * Main method for the application
     * @param args      command-line arguments (ignored)
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new Catch();
            }
        });
    }
}
