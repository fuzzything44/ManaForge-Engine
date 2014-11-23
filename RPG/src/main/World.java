package main;

import java.util.Vector;

public class World {
	
	public Coordinate ChunkRes = new Coordinate(100, 100);
	
	public Vector<WorldChunk> chunks = new Vector<WorldChunk>();
	
	public WorldChunk persistentChunk = new WorldChunk(new Coordinate(0, 0), this);
	
}
