import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import net.datastructures.Edge;
import net.datastructures.Vertex;



public class BaconGame<V, E> {

	private AdjacencyListGraphMap<String,String> baconGraph;
	private DirectedAdjListMap<String, String> baconBFS;
    private Map<String, String> actorMap;
    private Map<String, String> movieMap;
    private static String actorFile = "TextDocs/actors.txt";
    private static String movieFile = "TextDocs/movies.txt";
    private static String movieActorFile = "TextDocs/movie-actors.txt";
    
	
	public BaconGame() {
		baconGraph = new AdjacencyListGraphMap<String,String>();
		baconBFS = new DirectedAdjListMap<String, String>();
		actorMap = new TreeMap<String, String>();
		movieMap = new TreeMap<String, String>();
	}
	
	
	public static void makeKevinBaconBFSTree(BaconGame<String, String> bacon) throws IOException {
		Map<String, Set<String>> movieActorMap = new TreeMap<String, Set<String>>();
		BufferedReader in = new BufferedReader(new FileReader(movieActorFile));
		
		bacon.readFilesToMap();
		
		String line = in.readLine();
		while(line != null) {
			String[] token = line.split("\\|");
			if (!movieActorMap.containsKey(token[0])) {
				Set<String> actors = new TreeSet<String>();
				movieActorMap.put(token[0], actors);
			}
			movieActorMap.get(token[0]).add(token[1]);
			line = in.readLine();
		}
		in.close();
		
		for (String movie : movieActorMap.keySet()) {
			bacon.makeGraphEdges(movieActorMap, movie, 1);
		}
		bacon.bfs_tree("Kevin Bacon");
	}
	
	public void readFilesToMap() throws IOException {
		BufferedReader inActors = new BufferedReader(new FileReader(actorFile));
		BufferedReader inMovies = new BufferedReader(new FileReader(movieFile));
		
		String line1 = inActors.readLine();
		while (line1 != null) {
			String[] actor = line1.split("\\|");
			actorMap.put(actor[0], actor[1]);
			baconGraph.insertVertex(actor[1]);
			line1 = inActors.readLine();
		}
		inActors.close();
		
		String line2 = inMovies.readLine();
		while (line2 != null) {
			String[] movie = line2.split("\\|");
			movieMap.put(movie[0], movie[1]);
			line2 = inMovies.readLine();
		}
		inMovies.close();
	}
	
	
	public void makeGraphEdges(Map<String, Set<String>> movieActorMap, String movie, int i) {
		Iterator<String> itr = movieActorMap.get(movie).iterator();
		int j = 1;
		String actor1 = null;
		String actor2 = null;
		while (j <= i && itr.hasNext()) {
			actor1 = itr.next();
			j++;
		}
		
		while (itr.hasNext()) {
			actor2 = itr.next();
			baconGraph.insertEdge(actorMap.get(actor1), actorMap.get(actor2), movieMap.get(movie));
		}
		if (j < movieActorMap.get(movie).size()) {
			makeGraphEdges(movieActorMap, movie, j);
		}
	}
	 
	
	public void bfs_tree(String start) {
		LinkedList<Vertex<String>> discovered = new LinkedList<Vertex<String>>();
		
		discovered.add(baconGraph.getVertex(start));
		baconBFS.insertVertex(start);
		
		while (!discovered.isEmpty()) {
			Vertex<String> v = discovered.removeFirst();
			for(Edge<String> e : baconGraph.incidentEdges(v)) {
				Vertex<String> dest = baconGraph.opposite(v, e);
				if (!baconBFS.vertexInGraph(dest.toString())) {
					baconBFS.insertVertex(dest.toString());
					baconBFS.insertDirectedEdge(dest, v, e.toString());
					discovered.add(dest);
				}
			}
		}
//		System.out.println(baconGraph);
	}
	
	
	public static void main(String[] args) throws IOException {
		BaconGame<String,String> bacon = new BaconGame<String,String>();
		String name;
		String name2;
		char command = ' ';
		int i = 0;
		Scanner input = new Scanner(System.in);
		
		makeKevinBaconBFSTree(bacon);
		
		System.out.println("To quit the program, type return in answer to a question.");
		while (command != '\n') {
			System.out.print("Enter the name of an actor: ");
			name = input.nextLine();
			name2 = name;
			command = name.charAt(0);
			System.out.println(name);
			System.out.println(command);
			if (bacon.baconBFS.vertexInGraph(name)) {
				while (!name2.equals("Kevin Bacon")) {
					for (Edge<String> e : bacon.baconBFS.incidentEdgesOut(name)) {
						name2 = bacon.baconBFS.opposite(name, e).toString();
						i++;
					}
				}
				System.out.println(name +"'s Bacon Number is " + i);
			}
			else {
				System.out.println(name + "has an infinite Bacon Number");
			}
			
		}
		input.close();
	}
}
