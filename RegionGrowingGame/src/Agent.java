import java.awt.*;

/**
 * Animated agent, defined by a position and size, 
 * and the ability to move, draw itself, and see if a point is inside.
 * 
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Fall 2012
 */

public class Agent {
	protected double x, y;	// position
	protected int r=5;		// radius; defaults to 5

	/**
	 * @param x		initial x coordinate
	 * @param y		initial y coordinate
	 */
	public Agent(double x, double y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * Following are auto-generated getters & setters for position, for demonstration
	 */

	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
	}

	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
	}

	/**
	 * Updates the position.
	 * Default: stay still.
	 */
	public void move() {
	}

	/**
	 * Draws the agent on the graphics.
	 * @param g
	 */
	public void draw(Graphics g) {
		g.fillOval((int) x - r, (int) y - r, 2 * r, 2 * r);
	}

	/**
	 * Tests whether the point is inside the agent.
	 * @param x2
	 * @param y2
	 * @return		is (x2,y2) in the agent?
	 */
	public boolean contains(double x2, double y2) {
		return (x-x2)*(x-x2) + (y-y2)*(y-y2) <= r*r;
	}
}
