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
	
	public void addActor(Coordinate location, int renderOrder, String image) {
		int ChunkX = (int) Math.floor(location.X / ChunkRes.X);
		int ChunkY = (int) Math.floor(location.Y / ChunkRes.Y);
		if (chunks[ChunkX][ChunkY] != null) {
			
			new Actor(renderOrder, chunks[ChunkX][ChunkY], image, new Coordinate(location.X - (ChunkRes.X * ChunkX), location.Y - (ChunkRes.Y * ChunkY) ) );
			
		} else {
			
			chunks[ChunkX][ChunkY] = new WorldChunk(new Coordinate(ChunkX, ChunkY), this);
			
		}
	}
	
	public WorldChunk getActorChunk(Coordinate location) {
		
	}
	
	public void load(String file, Image background) {
		// load loads the chunk array from a file.
		// It does not load the persistent chunk!!!!!
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file) );
			
			String line = reader.readLine();
			String[] splitLine = line.split("_");
			ChunkRes.X = Float.parseFloat(splitLine[0]);
			ChunkRes.Y = Float.parseFloat(splitLine[1]);
			chunks = new WorldChunk[Integer.parseInt(splitLine[2])][Integer.parseInt(splitLine[3])];
			// First line gives world constants.
			// Format: ChunkResX_ChunkResY_NumChunksX_NumChunksY
			
			// TODO load background here
			
			Map<Color, String> imageInfo = new HashMap<Color, String>();
			imageInfo.put(Color.red, "res/grass.png");
			imageInfo.put(Color.blue, "res/Default.png");
			imageInfo.put(Color.green, "res/Tree.png");
			imageInfo.put(Color.black, "res/dirt.png");
			Landscape.loadIntoChunks(Game.world, background, imageInfo, new Coordinate(0, 0));
			// Loads landscape from image.
			
			WorldChunk workingChunk = null;
			while (reader.ready()) {	// As long as there's another line...
				line = reader.readLine();
				
				// Most lines will be in the form of TYPE_CoordX_CoordY_(Other data)
				
				splitLine = line.split("_");
				if (splitLine[0].equals("C") ) {
					// Type is WorldChunk
					int chunkX = Integer.parseInt(splitLine[1]);
					int chunkY = Integer.parseInt(splitLine[2]);
					
					workingChunk = new WorldChunk(new Coordinate(chunkX, chunkY), this);
					chunks[chunkX][chunkY] = workingChunk;
					
					// Chunks should be in format of C_coordX_coordY
				}	// End chunk line
				else if (splitLine[0].equals("A") ) {
					// Type is Actor
					float actorX = Float.parseFloat(splitLine[1]);
					float actorY = Float.parseFloat(splitLine[2]);
					
					workingChunk.actors.addElement( Macros.actor(splitLine[3], new Coordinate(actorX, actorY) ) );
					
					// Actor lines should be in format of A_coordX_coordY_name
				}	// End Actor line
			}		// End while

			reader.close();
			// Closing reader to stop memory leaks.
		} catch (FileNotFoundException e) {
			e.printStackTrace();	// TODO get some exception handling here.

		} catch (Exception e) {
			e.printStackTrace();

		}
		
		
	}			// End load()
}
