package main;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Map<Coordinate, Actor> actors = new HashMap<Coordinate, Actor>();
		
	public WorldChunk(Coordinate locationIn, World world) {
		world.chunks[(int) locationIn.X][(int) locationIn.Y] = this;
	}
	
}
