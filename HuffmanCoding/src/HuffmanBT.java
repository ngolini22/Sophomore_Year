import java.util.*;
import java.io.*;

/**
 * Generic binary tree, storing data of a parametric data in each node
 *
 * @author Chris Bailey-Kellogg, Dartmouth CS 10, Fall 2012
 */

public class HuffmanBT<Character, Integer> {
	HuffmanBT<Character, Integer> left, right;	// children; can be null
	Character dataC;
	Integer dataF;

	/**
	 * Constructs leaf node -- left and right are null
	 */
	public HuffmanBT(Character dataC, Integer dataF) {
		this.dataC = dataC;
		this.dataF = dataF;
		this.left = null; 
		this.right = null;
	}
	/**
	 * Constructs a node with just the frequency, left, and right
	 * @param dataF
	 * @param left
	 * @param right
	 */
	
	public HuffmanBT(Integer dataF, HuffmanBT<Character, Integer> left, HuffmanBT<Character, Integer> right){
		this.dataF = null;
		this.left = null;
		this.right = null;
	}

	/**
	 * Constructs inner node
	 */
	public HuffmanBT(Character dataC, Integer dataF, HuffmanBT<Character, Integer> left, HuffmanBT<Character, Integer> right) {
		this.dataC = dataC;
		this.dataF = dataF;
		this.left = left; 
		this.right = right;
	}

	public HuffmanBT() {
//		this.dataC = null;
		this.dataF = null;
	}

	/**
	 * Is it an inner node?
	 */
	public boolean isInner() {
		return left != null || right != null;
	}

	/**
	 * Is it a leaf node?
	 */
	public boolean isLeaf() {
		return left == null && right == null;
	}

	/**
	 * Does it have a left child?
	 */
	public boolean hasLeft() {
		return left != null;
	}

	/**
	 * Does it have a right child?
	 */
	public boolean hasRight() {
		return right != null;
	}
	
	public HuffmanBT<Character, Integer> getLeft() {
		return left;
	}

	public HuffmanBT<Character, Integer> getRight() {
		return right;
	}

	public Character getChar() {
		return dataC;
	}
	
	public Integer getFreq(){
		return dataF;
	}

	/**
	 * Number of nodes (inner and leaf) in tree
	 */
	public int size() {
		int num = 1;
		if (hasLeft()) num += left.size();
		if (hasRight()) num += right.size();
		return num;
	}

	/**
	 * Longest length to a leaf node from here
	 */
	public int height() {
		if (isLeaf()) return 0;
		int h = 0;
		if (hasLeft()) h = Math.max(h, left.height());
		if (hasRight()) h = Math.max(h, right.height());
		return h+1;						// inner: one higher than highest child
	}	

	/**
	 * Same structure and data?
	 */
	public boolean equalsTree(HuffmanBT<Character, Integer> t2) {
		if (hasLeft() != t2.hasLeft() || hasRight() != t2.hasRight()) return false;
		if (!dataC.equals(t2.dataC) && !dataF.equals(t2.dataF)) return false;
		if (hasLeft() && !left.equalsTree(t2.left)) return false;
		if (hasRight() && !right.equalsTree(t2.right)) return false;
		return true;
	}

	/**
	 * Leaves, in order from left to right
	 */
	public ArrayList<Character> fringe() {
		ArrayList<Character> f = new ArrayList<Character>();
		addToFringe(f);
		return f;
	}

	/**
	 * Helper for fringe, adding fringe data to the list
	 */
	public void addToFringe(ArrayList<Character> fringe) {
		if (isLeaf()) {
			fringe.add(dataC);
		}
		else {
			if (hasLeft()) left.addToFringe(fringe);
			if (hasRight()) right.addToFringe(fringe);
		}
	}

	/**
	 * Returns a string representation of the tree
	 */
	public String toString() {
		return toStringHelper("");
	}

	/**
	 * Recursively constructs a String representation of the tree from this node, 
	 * starting with the given indentation and indenting further going down the tree
	 */
	public String toStringHelper(String indent) {
		String res = indent + dataC + " : " + dataF + "\n";
		if (hasLeft()) res += left.toStringHelper(indent+"  ");
		if (hasRight()) res += right.toStringHelper(indent+"  ");
		return res;
	}

	/**
	 * Very simplistic binary tree parser based on Newick representation
	 * Assumes that each node is given a label; that becomes the data
	 * Any distance information (following the colon) is stripped
	 * <tree> = "(" <tree> "," <tree> ")" <label> [":"<dist>]
	 *        | <label> [":"<dist>]
	 * No effort at all to handle malformed trees or those not following these strict requirements
	 */
/*
	public static HuffmanBT<Character, Integer> parseNewick(String s) {
		HuffmanBT<String> t = parseNewick(new StringTokenizer(s, "(,)", true));
		// Get rid of the semicolon
		t.dataC = t.dataC.substring(0,t.dataC.length()-1);
		t.dataF = t.dataF.substring(0,t.dataF.length()-1);

		return t;
	}

	**
	 * Does the real work of parsing, now given a tokenizer for the string
	 *
	public static HuffmanBT<String> parseNewick(StringTokenizer st) {
		String token = st.nextToken();
		if (token.equals("(")) {
			// Inner node
			HuffmanBT<String> left = parseNewick(st);
			String comma = st.nextToken();
			HuffmanBT<String> right = parseNewick(st);	
			String close = st.nextToken();
			String label = st.nextToken();
			String[] pieces = label.split(":");
			return new HuffmanBT<String>(pieces[0], pieces[0], left, right);
		}
		else {
			// Leaf
			String[] pieces = token.split(":");
			return new HuffmanBT<String>(pieces[0], pieces[0]);
		}
	}
*/
	/**
	 * Slurps the entire file into a single String, and returns it
	 */
	public static String readIntoString(String filename) throws IOException {
		StringBuffer buff = new StringBuffer();
		BufferedReader in = new BufferedReader(new FileReader(filename));
		String line;
		while ((line = in.readLine()) != null) buff.append(line);
		in.close();
		return buff.toString();
	}

	/**
	 * Some tree testing
	 */
	public static void main(String[] args) throws IOException {
		// Tree of life
//		String s = readIntoString("inputs/itol.txt");
//		HuffmanBT<String> t = parseNewick(s);
//		System.out.println(t);
//		System.out.println("height:" + t.height());
//		System.out.println("size:" + t.size());
//
		// Smaller trees
//  		HuffmanBT<String> t1 = parseNewick("((a,b)c,(d,e)f)g;");		
//		System.out.println("fringe:" + t1.fringe());
//
//  		HuffmanBT<String> t2 = parseNewick("((a,b)c,(d,e)f)g;");
//   		HuffmanBT<String> t3 = parseNewick("((a,b)z,(d,e)f)g;");
//		System.out.println("== " + t1.equalsTree(t2) + " " + t1.equalsTree(t3));
	}
}