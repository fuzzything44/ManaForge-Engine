package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.HashMap;
import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class World {
	
	public Coordinate chunkRes = new Coordinate(50, 50);
	
	public WorldChunk[][] chunks;
	public WorldChunk persistentChunk;
	public Image[][] backgroundImages;
	
	public World()
	{
		persistentChunk = new WorldChunk(new Coordinate(0, 0), this);
		chunks = new WorldChunk[1][1];
	}
	
	public WorldChunk getActorChunk(Coordinate location) {
		return chunks[(int) Math.floor(location.X / chunkRes.X)][(int) (location.Y / chunkRes.Y)];
	}
	
	public void load(String file, Image background) {
		// load loads the chunk array from a file.
		// It does not load the persistent chunk!!!!!
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file) );
			
			String[] line = reader.readLine().split("_");
			chunkRes.X = Float.parseFloat(line[0]);
			chunkRes.Y = Float.parseFloat(line[1]);
			chunks = new WorldChunk[Integer.parseInt(line[2])][Integer.parseInt(line[3])];
			// First line gives world constants.
			// Format: ChunkResX_ChunkResY_NumChunksX_NumChunksY
			
			for (int x = 0; x < chunks.length; x++) {
				for (int y = 0; y < chunks[x].length; y++) {
					chunks[x][y] = new WorldChunk(new Coordinate(x, y), this);
				}
			}
			// Initializing all chunks in the array.
			
			backgroundImages = new Image[(int) (chunkRes.X) * chunks.length][(int) (chunkRes.Y) * chunks[0].length];
			
			
			// TODO: Will need to add this to the .world file
			Map<Color, Image> imageInfo = new HashMap<Color, Image>();
			
			addColors : while (true) {
				String nextLine = reader.readLine();
				if (!nextLine.equalsIgnoreCase("end") ) {
					line = nextLine.split("_");
					
					Color color = Color.decode(line[0]);
					Image image = new Image(line[1]);
					
					imageInfo.put(color, image);
					
				} else {
					break addColors;
				}	// End if not end.
			}	// End while
				
			
			Landscape.loadIntoWorld(Game.world, background, imageInfo, new Coordinate(0, 0));
			// Loads landscape from image.
			
			while (reader.ready() ) {	// As long as there's another line...
				line = reader.readLine().split("_");
				float actorX = Float.parseFloat(line[0]);
				float actorY = Float.parseFloat(line[1]);
				Macros.actor(line[2], new Coordinate(actorX, actorY) ).refreshChunk();
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
