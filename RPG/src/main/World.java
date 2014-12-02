package main;

import java.util.HashMap;
import java.util.Map;

public class World {
	
	public Coordinate ChunkRes = new Coordinate(50, 50);
	
	public Map<Coordinate, WorldChunk> chunks = new HashMap<Coordinate, WorldChunk>();
	
	public WorldChunk persistentChunk = new WorldChunk(new Coordinate(0, 0), this);
	
}
