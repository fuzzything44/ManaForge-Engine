package main;

import java.util.HashMap;
import java.util.Map;

public class World {
	
	public Coordinate ChunkRes = new Coordinate(50, 50);
	
//	public Map<Coordinate, WorldChunk> chunks = new HashMap<Coordinate, WorldChunk>();

	public WorldChunk[][] chunks = new WorldChunk[10][10];
	public WorldChunk persistentChunk = new WorldChunk(new Coordinate(0, 0), this);

	public void load(String file) {
		// Method to load world from a file.
		for (int X = 0; X < 10; X++) {
			for (int Y = 0; Y < 10; Y++) {
				new WorldChunk(new Coordinate(X, Y), this);
			}	// End Y for
		}		// End X for
		// Temporary chunk generation.
		for (int chunkX = 0; chunkX < Game.world.chunks.length; chunkX++) {
			for (int chunkY = 0; chunkY < Game.world.chunks[0].length; chunkY++) {
				for (int actorX = (int) -Game.world.ChunkRes.X; actorX < (int) Game.world.ChunkRes.X; actorX++) {
					for (int actorY = (int) -Game.world.ChunkRes.X; actorY < (int) Game.world.ChunkRes.Y; actorY++) {
						Game.world.chunks[chunkX][chunkY];	// TODO fix WorldChunk actors, fix basic generation.
					}
				}
			}
		}
	}			// End load()
}
