import java.util.Comparator;

public class TreeComparator implements Comparator<HuffmanBT<Character, Integer>> {

	public int compare(HuffmanBT<Character, Integer> freq1, HuffmanBT<Character, Integer> freq2) {
		if((Integer) freq1.getFreq() > (Integer) freq2.getFreq())
			return 1;
		else if((Integer) freq1.getFreq() < (Integer) freq2.getFreq())
			return -1;
		else
			return 0;	
	}	
}
