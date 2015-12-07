import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.TreeMap;

import javax.swing.JFileChooser;

/**
 * Lab 4 Huffman Coding
 * Takes a file, reads in the characters, and puts the characters into a map based on their frequency
 * Puts the map into a Binary Tree based on the frequency of the characters
 * @uthor Nicky Golini 
 */

public class HuffmanCoding {
	private static Map<Character, Integer> charFreq; // map of character frequencies
	private static HuffmanBT<Character, Integer> codedTree; // final binary tree ordered in highest frequency characters to lowest frequency characters
	private static Map<Character, String> codedWordMap; // map of code words in binary
	private static File file; // file to be read 
	static String file_comp, file_decomp; // output files with compressed and decompressed text

	/**
	 * Creating the character Frequency Map
	 * @param file
	 */
	
	public static void charFreqMapCreation(File file){
		try {
			BufferedReader in = new BufferedReader(new FileReader(file));
			Map<Character, Integer> charFreq = new TreeMap<Character, Integer>();

			char c; // character placeholder while reading in file
			boolean flag = false;
			int i = in.read(); // int placeholder while reading in file
			while(!flag){
				if(i != -1) break;
				if(i == -1){
					//System.out.println("File is Empty. Exitting program now."); 
					decompress();
				}
			}
			while(i != -1){  // while there are still characters in the file
				c = (char) i;
				if(charFreq.containsKey(c)){ // if the character is already in the map, add 1 to its frequency
					charFreq.put(c, charFreq.get(c)+1); // 
				} else{
					charFreq.put(c, 1); // else put it in the map
				}
				i = in.read();
			}
			in.close();
			createTree(charFreq); // send map to be ordered in a binary tree
			System.out.println("Character Frequency Map\n" + charFreq);
		} catch (IOException e) {
			e.printStackTrace();
			System.err.println("The file path name is unavailable");
		}
	}
	
	/**
	 * Creating the ordered Binary Tree
	 */
	public static HuffmanBT<Character, Integer> createTree(Map<Character, Integer> charFreq){
		int queueSize = charFreq.size();
		HuffmanBT<Character, Integer> lowestFreq = null;
		HuffmanBT<Character, Integer> nextFreq = null;
		PriorityQueue<HuffmanBT<Character, Integer>> treesQueue = 
				new PriorityQueue<HuffmanBT<Character, Integer>>(queueSize, new TreeComparator()); // create new priority queue from Java implementation
		
		for(Character chars : charFreq.keySet()){ // loop through map and make singleton binary trees for every character 
			HuffmanBT<Character, Integer> single = new HuffmanBT<Character, Integer>(chars, charFreq.get(chars));
			treesQueue.add(single); // add all these singleton trees to a priority queue based on their frequencies
		}	
		while(!treesQueue.isEmpty()){ // while the priority queue still has character trees in it
			lowestFreq = treesQueue.remove(); // take out the character with the minimum frequency
			if(!treesQueue.isEmpty()){ // if there is a next tree in the queue to be extracted
				nextFreq = treesQueue.remove(); // take out character with second lowest frequency
				int nodeSum = (Integer)lowestFreq.getFreq() + (Integer)nextFreq.getFreq();
				HuffmanBT<Character, Integer> newNode = new HuffmanBT<Character, Integer>(nodeSum, lowestFreq, nextFreq); //create a new node
				newNode.dataF = nodeSum; newNode.left = lowestFreq; newNode.right = nextFreq; //put the summed frequency as the root and the characters with their frequencies as left and right
				treesQueue.add(newNode); // add the new node back to the queue
			} else break;
		}
		codedTree = lowestFreq;
		return codedTree;
	}
		/**
		 * Calls the recursion through the Binary Tree to create the map of characters and their bit code words
		 * @param codedTree
		 * @return
		 */
	public static Map<Character, String> wordCodeHelper(HuffmanBT<Character, Integer> codedTree){
		String code = "";
		Map<Character, String> codedWordMap = new TreeMap<Character, String>();
		
		wordCodeRecurser(codedTree, code, codedWordMap);
		System.out.println("Binary Word Code Map\n" + codedWordMap);
		return codedWordMap;
	}
	/**
	 * Recurses through the coded map 
	 * When it reaches a leaf, add the character and code word to the code word map
	 * If you go left, add a 0 to the code, if you go right, add a 1 to the code
	 */
	public static void wordCodeRecurser(HuffmanBT<Character, Integer> codedTree, String code, Map<Character, String> codedWordMap){
		if(codedTree.isLeaf()){
			codedWordMap.put(codedTree.dataC, code); // if there is a leaf, put in the character and its code word into the code word map 
		} else{
			if(codedTree.hasLeft()){
				wordCodeRecurser(codedTree.getLeft(), code + "0", codedWordMap); // if the char takes a left path, add a 0 to the string and recurse left
			}if (codedTree.hasRight()){
				wordCodeRecurser(codedTree.getRight(), code + "1", codedWordMap); // if the char takes a right path, add a 1 to the string and recurse right
			}
		}
	}
	
	/**
	 * Writes all of the characters as their binary String code words into a file
	 */
	public static void compress(Map<Character, String> codedWordMap){
		file = new File(getFilePath());
		charFreqMapCreation(file);
		codedWordMap = wordCodeHelper(codedTree);
		
		try {
			BufferedReader in = new BufferedReader(new FileReader(file));
			file_comp = file + "_comp.txt";
			BufferedBitWriter out = new BufferedBitWriter(file_comp);

			char c; // character placeholder while reading in file
			int i; // int placeholder while reading in file
			while((i = in.read()) != -1){  // while there are still characters in the file
				c = (char) i; // read in the characters (as ints) and convert them to chars
				String code = codedWordMap.get(c); // Make the values for the characters Strings to be written to the compressed file
				for(int j = 0; j < code.length(); j++){
					int intCode = Integer.parseInt(code.substring(j, j+1)); 
					out.writeBit(intCode); // write the bit strings into the compressed file
				}
			}
			in.close();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
			System.err.println("The file path name is unavailable.");
		}	
	}	
	
	/**
	 * Reads the compressed file and translates it back to the original characters, and writes them back into a new file
	 */
	public static void decompress(){
		try {
			file_comp = file + "_comp.txt";
			file_decomp = file + "_decomp.txt";
			BufferedBitReader in = new BufferedBitReader(file_comp);
			BufferedWriter out = new BufferedWriter(new FileWriter(file_decomp));
			HuffmanBT<Character, Integer> newTree = new HuffmanBT<Character, Integer>(); // placeholder tree to iterate through the coded tree
			newTree = codedTree;
			int k = in.readBit();
			while(k != -1){ // while there are still bits in the compressed file
				if(k == 0){ // if the bit is a 0, recurse to the left
					newTree = newTree.getLeft();
				} else if(k == 1){ // if the bit is a 1, recurse to the right
					newTree = newTree.getRight();
				}
				if(newTree.isLeaf()){ // if it's at a leaf, write the character that is assigned to the bit code into the decompressed file
					out.write("" + newTree.getChar());
					newTree = codedTree; // reset the placeholder tree to to root
				}
				k = in.readBit(); // go to next bit
			}
			in.close();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
			System.err.println("The file path name is unavailable.");
			System.out.println("Choose a new file to use");
			file = new File(getFilePath());
			charFreqMapCreation(file);
		}
	}
	
	/**
	 * Puts up a fileChooser and gets path name for file to be opened.
	 * Returns an empty string if the user clicks "cancel".
	 * @return path name of the file chosen  
	 */
	public static String getFilePath() {
	    JFileChooser fc = new JFileChooser("."); // start at current directory

	    int returnVal = fc.showOpenDialog(null);
	    if(returnVal == JFileChooser.APPROVE_OPTION) {
	        File file = fc.getSelectedFile();
	        String pathName = file.getAbsolutePath();
	        return pathName;
	    }
	    else {
	        return "";  
	    }
	}
	
	/**
	 * Calls the compression and decompression methods to start the code
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args){
		compress(codedWordMap);
		decompress();
	}
}
