package main;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Vector<Actor> actors = new Vector<Actor>();
	public Coordinate location;
		
	public WorldChunk(Coordinate locationIn, World world) {
		location = locationIn;
	}
	
}
