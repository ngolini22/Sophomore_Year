import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.image.*;
import java.util.*;

import javax.swing.AbstractAction;
import javax.swing.Timer;

/**
 * Universe for PS-1 Catch game.
 * Holds the fliers and the background image.
 * Also finds and holds the regions in the background image.
 * Each region is a list of contiguous points with colors similar to a target color.
 * Sample solution to Lab 1, Dartmouth CS 10, Winter 2015
 * 
 * @author Chris Bailey-Kellogg, Winter 2014 (based on a very different structure from Fall 2012)
 * @author Travis W. Peters, Dartmouth CS 10, Updated Winter 2015
 * @author Zach Tannenbaum and Nicky Golini
 */
public class Universe {
    private static final int maxColorDiff = 20;				// how similar a pixel color must be to the target color, to belong to a region
    private static final int minRegion = 50; 				// how many points in a region to be worth considering

    private BufferedImage image;                            // a reference to the background image for the universe
    private Color trackColor;                               // color of regions of interest (set by mouse press)

    private ArrayList<ArrayList<Point>> regions;			// a region is a list of points
    // so the identified regions are in a list of lists of points

    private ArrayList<Flier> fliers;                        // all of the fliers
    
    private ArrayList<Point> newRegion;
    private ArrayList<Point> toVisit;
    
    /**
     * New universe with a background image and an empty list of fliers
     * @param image
     */
    public Universe(BufferedImage image) {
        this.image = image;		
        fliers = new ArrayList<Flier>();       
    }

    /**
     * Set the image (from the webcam) that makes up the universe's background
     * @param image
     */
    public void setImage(BufferedImage image) {
        this.image = image;
    }

    /**
     * Allow others to ask about the state of the trackColor in the universe
     * @return
     */
    public Color getTrackingColor() {
        return trackColor;
    }

    /**
     * Setting the color from an explicitly defined Color object 
     * as opposed to getting input from the player.
     * @param color
     */
    public void setTrackingColor(Color color) {
        trackColor = color;
    }

    /**
     * Allow others to ask about the size of the universe (width)
     * @return
     */
    public int getWidth() {
        return image.getWidth();
    }

    /**
     * Allow others to ask about the size of the universe (height)
     * @return
     */
    public int getHeight() {
        return image.getHeight();
    }

    /**
     * Accesses the currently-identified regions.
     * @return
     */
    public ArrayList<ArrayList<Point>> getRegions() {
        return regions;
    }

    /**
     * Set the universe's regions.
     * @return
     */
    public void setRegions(ArrayList<ArrayList<Point>> regions) {
        this.regions = regions;
    }

    /**
     * Adds the flier to the universe
     * @param f
     */
    public void addFlier(Flier f) {
        fliers.add(f);
    }

    /**
     *  Move the flier and detect catches and misses
     */
    public void moveFliers() { // moves all of the fliers in the array list holding all of the fliers
        // TODO: YOUR CODE HERe	
    			for (Flier flier : fliers) {	
    				flier.move();
    				flier.checkLose();
    				flier.checkWin();
    			}
    }

    /**
     * Draw the fliers
     */
    public void drawFliers(Graphics g) {   // draws all of the fliers available in the flier array list
        // TODO: YOUR CODE HERE
    	for (Flier flier : fliers) { 
    	   flier.draw(g);
    	}
    }

    /**
     * Sets regions to the flood-fill regions in the image, similar enough to the trackColor.
     */
    public void findRegions() { // flood fill - checks the neighbors of neighbors and stores all of the pixels of appropriate color into a nested array list of regions and points
        // TODO: YOUR CODE HERE
    	regions = new ArrayList<ArrayList<Point>>();
        Boolean[][] flags = new Boolean[image.getWidth()][image.getHeight()]; // keeps track of all of the pixels that did not initiate a region
    	for(int k = 0; k < image.getWidth(); k++){
    		for(int l = 0; l < image.getHeight(); l++){
    			flags[k][l] = false;
    		}
    	}
    	for (int x = 0; x < image.getWidth(); x++) {
    		for(int y = 0; y < image.getHeight(); y++) {
    			Color c = new Color(image.getRGB(x,y));
    			if (flags[x][y] == false && colorMatch(c, trackColor) == true) {
    				flags[x][y] = true;
    				Point p = new Point(x,y);
    				newRegion = new ArrayList<Point>();
    				toVisit = new ArrayList<Point>(); 			// create new region for list of points to be added to
    				toVisit.add(p);
    				while (!toVisit.isEmpty()) {
    					p = toVisit.get(0);
    					toVisit.remove(0);
    					newRegion.add(p);
    				
    					for(int i = Math.max(0, p.x - 1); i < Math.min(p.x + 2, image.getWidth()); i++) {
    						for (int j = Math.max(0, p.y - 1); j < Math.min(p.y +2, image.getHeight()); j++) {
    							Color c1 = new Color(image.getRGB(i, j));
    							if (flags[i][j] == false && colorMatch(c1, trackColor)== true) {
    								Point p1 = new Point(i,j);
    								toVisit.add(p1);
    								flags[i][j] = true;
    							}
    						}
    					}
    				}
        			if (newRegion.size() >= minRegion) {
        				regions.add(newRegion);
        			}
    			}
    		}
    	}
    }

    /**
     * Tests whether the two colors are "similar enough" (your definition, subject to the static threshold).
     * @param c1
     * @param c2
     * @return
     */
    private static boolean colorMatch(Color c1, Color c2) { // checks to see if the color initially set by the mouse click is close enough to each pixel in the flood fill regions
        // TODO: YOUR CODE HERE
    	if ((Math.abs(c1.getRed() - c2.getRed()) < maxColorDiff) &&
    		(Math.abs(c1.getGreen() - c2.getGreen()) < maxColorDiff) &&
    		(Math.abs(c1.getBlue() - c2.getBlue()) < maxColorDiff)) {
    		return true;
    	}
    	else {
    		return false;
    	}
    }

    /**
     * Recolors image so that each region is a random uniform color, so we can see where they are
     */
    public void recolorRegions() { // recolors the regions to a random color
        // TODO: YOUR CODE HERE	
    	for(ArrayList<Point> list : regions){
    		int Red = (int)(Math.random()*255);
    		int Green = (int)(Math.random()*255);
    		int Blue = (int)(Math.random()*255);
    		Color Random = new Color(Red, Green, Blue);
    		for(Point point : list) {
    			image.setRGB(point.x, point.y, Random.getRGB());
    		}
    	}
    }
}