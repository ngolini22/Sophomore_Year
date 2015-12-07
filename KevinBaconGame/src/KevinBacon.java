import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Queue;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import javax.swing.JFileChooser;

import net.datastructures.Edge;
import net.datastructures.Vertex;
/**
 * 
 * @author Nicky Golini
 * Finds the Bacon number of any actor in the Luther College Bacon database
 * Sorry this is not gender normative. Read "actor(s)" as "actor(s)/actress(es)"
 * @param <V>
 * @param <E>
 */


public class KevinBacon<V, E> {
    private DirectedAdjListMap<String, String> baconGraph; // directed graph produced by bfs
    private AdjacencyListGraphMap<String, String> movieData; // undirected adjacency map 
    private Map<String, String> actorMap; // map with actors and their code name from the actors text file
    private Map<String, String> movieMap; // map with actors and their code name from the actors text file
	
	public KevinBacon(){
		baconGraph = new DirectedAdjListMap<String, String>();
		movieData = new AdjacencyListGraphMap<String, String>();
		actorMap = new TreeMap<String, String>();
		movieMap = new TreeMap<String, String>();
	}
	
	/**
	 * Creates a map for the actors and movies with their respective code names to be easily translated when reading from the movie-actors file
	 * 
	 * @throws IOException
	 */
	
	public void loadActorsAndMovies() throws IOException{
		BufferedReader inActors = new BufferedReader(new FileReader("TextDocs/actors.txt"));
		BufferedReader inMovies = new BufferedReader(new FileReader("TextDocs/Movies.txt"));

		String lineActors = inActors.readLine();
		while(lineActors != null){
			String[] tokens = lineActors.split("\\|");
			String actorVal = tokens[0];
			String actor = tokens[1];
			actorMap.put(actorVal, actor);
			if(movieData.vertexInGraph(actor)) break; // if the movie is already there, keep going
			movieData.insertVertex(actor); // creates a vertex for each actor in the undirected graph
			lineActors = inActors.readLine();
		}
		inActors.close();
		String lineMovies = inMovies.readLine();
		while(lineMovies != null){
			String[] tokens = lineMovies.split("\\|");
			String movieVal = tokens[0];
			String movie = tokens[1];
			movieMap.put(movieVal, movie);
			lineMovies = inMovies.readLine();
		}
		inMovies.close();
	}
	
	/**
	 * Creates a map of all of the movies, each having a set of actors to be used in a recursive call to create the non-directed map.
	 * This setup is useful because then every actor who played in the same movie will be together, methodically making sure every actor and 
	 * movie gets into the non-directed map.
	 * Recursively creates the non-directed map of every actor connected to each other with the edge being the movie they were in together using the 
	 * other map made in this method.
	 * 
	 * @param Bacon
	 * @throws IOException
	 */
	
	public static void AdjGraphMaker(KevinBacon<String, String> Bacon) throws IOException{
		BufferedReader in = new BufferedReader(new FileReader("TextDocs/movie-actors.txt"));
		Map<String, Set<String>> movieActorFreq = new TreeMap<String, Set<String>>(); // map of movies with sets of 
		
		Bacon.loadActorsAndMovies(); // load the actor and movie translation maps

		String line = in.readLine();
		while(line != null){ // parses the movie-actors file and translates the codes to the actual actor and movie names
			String[] tokens = line.split("\\|");
			String movie = tokens[0];
			String actor = tokens[1];
			if(!movieActorFreq.containsKey(movie)){ // creating the set of actors for each movie
				Set<String> actors = new TreeSet<String>();
				movieActorFreq.put(movie, actors);
			}
			movieActorFreq.get(movie).add(actor);
			line = in.readLine();
		}
		in.close();

		for(String movies : movieActorFreq.keySet()){ // recursively create the adjacency map
			Bacon.recursive1(movieActorFreq, movies, 1);
		}
		Bacon.bfs("Kevin Bacon");
	}
	
	/**
	 * Creates an edge between all of the actors in each movie set in the non-directed map of every actor. 
	 * In each given movie: Connects 1 with 2, 1 to 3, 1 to 4... etc. Then connects 2 to 3, 2 to 4, etc... Then connects 3 to 4, etc. 
	 * 
	 * @param movieActorFreq - map with every movie with set of actors
	 * @param movie - movie name 
	 * @param count - keeps track of which actor to start with when traversing the set of actors for each movie
	 */
	
	public void recursive1(Map<String, Set<String>> movieActorFreq, String movie, int count){
		Iterator<String> itr = movieActorFreq.get(movie).iterator(); // iterates across every movie
		String actor1 = null;
		int i = 1;
		while(i <= count && itr.hasNext()){
			actor1 = itr.next();
			i++;
		}
		while(itr.hasNext()){
			String actor2 = itr.next();
			movieData.insertEdge(actorMap.get(actor1), actorMap.get(actor2), movieMap.get(movie));
		}
		if(count < movieActorFreq.get(movie).size())
			recursive1(movieActorFreq, movie, i);
	}
	
	/**
	 * Creates the directed edge map tree with each actor's vertex being in the layer that corresponds to their bacon number by queueing
	 * all of the data from the non-directed map in order to create a directed path between actors in the same movie.
	 * The edges are the names of the movies the two vertices starred in together, pointing to the vertex above it, making every
	 * vertex having a least-step route pointing to Kevin Bacon. 
	 * 
	 * @param start
	 */
	
	public void bfs(String start){
		LinkedList<Vertex<String>> queue = new LinkedList<Vertex<String>>();
		queue.add(movieData.getVertex(start));
		baconGraph.insertVertex(start);
		while(!queue.isEmpty()){
			Vertex<String> v = queue.removeFirst();
			for(Edge<String> e : movieData.incidentEdges(v)){
				Vertex<String> dest = movieData.opposite(v, e);
				if(!baconGraph.vertexInGraph(dest.toString())){
					queue.add(dest);
					baconGraph.insertVertex(dest.toString());
					baconGraph.insertDirectedEdge(dest.toString(), v.toString(), e.toString());
				}
			}
		}
	}
	
	/**
	 * Puts up a fileChooser and gets path name for file to be opened.
	 * 
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
	 * Main method with a way to print the movie path between an actor and Kevin Bacon, with the actor being asked as user input. 
	 * See side comments for a more detailed explanation. 
	 * 
	 * @param args
	 * @throws IOException
	 */
	
	public static void main(String[] args) throws IOException{
		Scanner input = new Scanner(System.in);
		KevinBacon<String, String> Bacon = new KevinBacon<String, String>();
		Queue<String> Actors = new LinkedList<String>(); // creates queue used in printing 
		Queue<String> Movies = new LinkedList<String>(); // ^
		
		AdjGraphMaker(Bacon); // create directed map tree of actors
		
		boolean flag = false; // used in determining whether or not to quit the program
//		System.out.println(Bacon.baconGraph);
		System.out.println("To quit the program, type return in answer to a question.");
		System.out.print("Enter the name of an actor: ");
		String baconFinder = input.nextLine();	
		Vertex<String> v = Bacon.baconGraph.getVertex(baconFinder); // set user input actor at the corresponding actor vertex in the directed map
		int baconCount = 0; // bacon number counter
		while(!flag){ // keeps program running until user asks to quit
			if(baconFinder.equals("")) { // if the user asks to quit the program, exit while loop
				System.out.println("You are now leaving the program."); 
				flag = true;
				break;
			}
			if(Bacon.actorMap.containsValue(baconFinder) && !Bacon.baconGraph.vertexInGraph(baconFinder)){ // if the actor does not have a link to KB
				System.out.println("The Bacon number is infinite because they do not have a Bacon number.");
			}
			if((Bacon.actorMap.containsValue(baconFinder)) && Bacon.baconGraph.vertexInGraph(baconFinder)){ // if the actor does have a link to KB 
				while(!v.toString().equals("Kevin Bacon")){ // while the traversal is still not at Kevin Bacon
					for(Edge<String> e : Bacon.baconGraph.incidentEdgesOut(v)){ // Find the movie pointing to the next actor in the map
						String[] tokens = e.toString().split("\\)"); // gets the actor and movie names to be printed after the bacon number is found
						String actor1 = v.toString();
						String movie = tokens[0] + ")";
						v = Bacon.baconGraph.opposite(v, e); // move up the edge to the next actor in the map
						String actor2 = v.toString();
						Actors.add(actor1);
						Movies.add(movie);
						Actors.add(actor2);
						baconCount++; // keep track of bacon number
					}
				}
				System.out.println(baconFinder + "'s bacon number is: " + baconCount);	
				while(!Movies.isEmpty()){ // methodically prints the actor's path to KB
					System.out.println(Actors.remove() + " appeared in " + Movies.remove() + " with " + Actors.remove());
				} 
			} else if(!Bacon.actorMap.containsValue(baconFinder)){ // if the actor is not in the database
				System.out.println("The actor is not in the database.");
			} else{
				System.out.println("Re-enter an actor.");
			}
			System.out.print("Enter the name of an actor: "); // after bacon number is found, ask user if they want to find another bacon number
			baconFinder = input.nextLine();
			v = Bacon.baconGraph.getVertex(baconFinder); // reset vertex
			baconCount = 0; // reset bacon number counter
		}

//		********** TEST MAP **********
//		
//		KevinBacon<String, String> Bacon = new KevinBacon<String, String>();
//		
//		Bacon.movieData.insertVertex("Kevin Bacon");
//		Bacon.movieData.insertVertex("Actor 1");
//		Bacon.movieData.insertVertex("Actor 2");
//		Bacon.movieData.insertVertex("Actor 3");
//		Bacon.movieData.insertVertex("Actor 4");
//		Bacon.movieData.insertVertex("Actor 5");
//		Bacon.movieData.insertVertex("Actor 6");
//		Bacon.movieData.insertEdge("Kevin Bacon", "Actor 1", "Movie 1");
//		Bacon.movieData.insertEdge("Kevin Bacon", "Actor 2", "Movie 1");
//		Bacon.movieData.insertEdge("Actor 1", "Actor 2", "Movie 1");
//		Bacon.movieData.insertEdge("Actor 1", "Actor 3", "Movie 2");
//		Bacon.movieData.insertEdge("Actor 3", "Actor 2", "Movie 3");
//		Bacon.movieData.insertEdge("Actor 3", "Actor 4", "Movie 4");
//		Bacon.movieData.insertEdge("Actor 5", "Actor 6", "Movie 5");
//		Bacon.bfs("Actor 2");
//		System.out.println(Bacon.baconGraph);
	}
}
