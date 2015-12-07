import java.util.*;
import java.awt.*;

/**
 * An animated object flying across the scene in a fixed direction
 * Sample solution to Lab 1, Dartmouth CS 10, Winter 2015
 * 
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Fall 2012; revised Winter 2014
 * @author Travis W. Peters, Dartmouth CS 10, Updated Winter 2015
 * @author Zach Tannenbaum and Nicky Golini
 */
public class Flier extends Agent {

    /* 
     * TODO: YOUR CODE HERE
     * 
     * The Flier class should have AT LEAST the following attributes 
     * in addition to what we've defined for you:
     *  - speed
     *  - direction of flight
     */

    private Universe universe;         // the universe that a flier exists within
 
    private int xmax, ymax;	// size of bouncing area
	private double dx, dy;	// step size in x and y
	
    public Flier(Universe universe) { // initializes the universe
        super(0,0);
    	
    	this.universe = universe;
        universe.addFlier(this);
        
        dx =  Math.random()/500;
		dy =  Math.random()/500;
    }

    /**
     * Overrides Agent.move() to step by dx, dy
     */
    public void move() { // moves the fliers at a step size of dx and dy
        // TODO: YOUR CODE HERE
    		x += dx;
        	y += dy;
    }

    /**
     * Detect hitting the region (and restart)
     */
    public void checkWin() { // checks over all points in the region to see if the mitt catches the flier, then prints that the player wins and toss another flier
        // TODO: YOUR CODE HERE  
    	ArrayList<ArrayList<Point>> regions = new ArrayList<ArrayList<Point>>(universe.getRegions());
    	for (ArrayList<Point> list : regions) {
    		for (Point point : list) {
    			if ((int) x == point.x && (int) y == point.y) {
    				System.out.println("You win!");
    				toss();
    			}
    		}
    	}
    }

    /**
     * Detect exiting the window (and restart)
     */
    public void checkLose() { // if the flier leaves the screen fully, tells player they lose and toss another flier
        // TODO: YOUR CODE HERE
    	if(x < (r) || x > universe.getWidth()+(r) || y < (r) || y > universe.getHeight()+(r)) {
    		System.out.println("You lose! Try Again!");
    		toss();
    	}
    }

    /**
     * Puts the object at a random point on one of the four borders, 
     * flying in to the window at a random speed.
     */
    public void toss() { // tosses the fliers from a random spot from a random border
        // TODO: YOUR CODE HERE	    
    	int choice =  (int) (Math.random() * 4);
    	
    	switch (choice) {
    		case 0:
    			x = 0;
    			y = (int) (Math.random()*(universe.getHeight())); 
    			break;
    		case 1:
    			x = universe.getWidth();
    			y = (int) (Math.random()*(universe.getHeight()));
    			break;
    		case 2:
    			x = (int) (Math.random()*(universe.getWidth()));
    			y = 0;
    			break;
    		case 3:
    			x = (int) (Math.random()*(universe.getWidth()));
    			y = universe.getHeight();	
    			break;
    	}

    	if (x == universe.getWidth()) {
			dx = -dx;
		}
		if (y == universe.getHeight()) {
			dy = -dy;
		}
    }
}
