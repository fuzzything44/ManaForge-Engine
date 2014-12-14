package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.HashMap;
import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class World {
	
	public Coordinate ChunkRes = new Coordinate(50, 50);
	
	public WorldChunk[][] chunks;
	public WorldChunk persistentChunk;

	public World()
	{
		persistentChunk = new WorldChunk(new Coordinate(0, 0), this);
		chunks = new WorldChunk[1][1];
	}
	
	public void addActor(Actor a) {
		a.refreshChunk();
//		if (a.doesTick) {
//			a.chunk.tickingObjects.addElement(a);
//		}
		a.chunk.actors.addElement(a);
		
	}
	public WorldChunk getActorChunk(Coordinate location) {
		return chunks[(int) Math.floor(location.X / ChunkRes.X)][(int) (location.Y / ChunkRes.Y)];
	}
	
	public void load(String file, Image background) {
		// load loads the chunk array from a file.
		// It does not load the persistent chunk!!!!!
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file) );
			
			String[] line = reader.readLine().split("_");
			ChunkRes.X = Float.parseFloat(line[0]);
			ChunkRes.Y = Float.parseFloat(line[1]);
			chunks = new WorldChunk[Integer.parseInt(line[2])][Integer.parseInt(line[3])];
			// First line gives world constants.
			// Format: ChunkResX_ChunkResY_NumChunksX_NumChunksY
			
			for (int x = 0; x < chunks.length; x++) {
				for (int y = 0; y < chunks[x].length; y++) {
					chunks[x][y] = new WorldChunk(new Coordinate(x, y), this);
				}
			}
			// Initializing all chunks in the array.
			
			
			Map<Color, String> imageInfo = new HashMap<Color, String>();
			imageInfo.put(Color.red, "grass");
			imageInfo.put(Color.blue, "default");
			imageInfo.put(Color.green, "tree");
			imageInfo.put(Color.black, "dirt");
			Landscape.loadIntoChunks(Game.world, background, imageInfo, new Coordinate(0, 0));
			// Loads landscape from image.
			
			while (reader.ready() ) {	// As long as there's another line...
				line = reader.readLine().split("_");
				float actorX = Float.parseFloat(line[0]);
				float actorY = Float.parseFloat(line[1]);
				addActor(Macros.actor(line[2], new Coordinate(actorX, actorY) ) );
				// All lines but first one should be in form X_Y_name
				// where name is the actor name keyed to macros.
				
			}	// End while

			reader.close();
			// Closing reader to stop memory leaks.

		} catch (FileNotFoundException e) {
			e.printStackTrace();	// TODO get some exception handling here.

		} catch (Exception e) {
			e.printStackTrace();

		}
		
		
	}			// End load()
}
