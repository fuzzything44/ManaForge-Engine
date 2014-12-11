package main;

import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Vector<Actor> actors = new Vector<Actor>();
	public Coordinate location = new Coordinate(0, 0);
		
	public WorldChunk(Coordinate locationIn, World world) {
		location = locationIn;
	}
	
}
