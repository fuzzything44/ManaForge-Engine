package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;

import org.newdawn.slick.Image;

public class World {
	
	public Coordinate ChunkRes;
	
	public WorldChunk[][] chunks;
	public WorldChunk persistentChunk = new WorldChunk(new Coordinate(0, 0), this);

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
			
			
			
			
			
			
			WorldChunk workingChunk;
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
					// Anything else we need?
				}	// End chunk line
				else if (splitLine[0].equals("A") ) {
					// Type is Actor
					float actorX = Float.parseFloat(splitLine[1]);
					float actorY = Float.parseFloat(splitLine[2]);
					
					Macros.actor(splitLine[3]);
					
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
