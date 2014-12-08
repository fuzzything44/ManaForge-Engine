package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.HashMap;
import java.util.Map;

public class World {
	
	public Coordinate ChunkRes;
	
//	public Map<Coordinate, WorldChunk> chunks = new HashMap<Coordinate, WorldChunk>();

	public WorldChunk[][] chunks;
	public WorldChunk persistentChunk = new WorldChunk(new Coordinate(0, 0), this);

	public void load(String file) {
		// TODO Get the correct file. Needs to work on all builds.
		
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file) );
			while (reader.ready()) {	// As long as there's another line...
				String line = reader.readLine();
				
				// Most lines will be in the form of TYPE_CoordX_CoordY_(Other data)
				
				String[] splitLine = line.split("_");
				if (splitLine[0].equals("C") ) {
					// Type is chunk.
					
				} else if (splitLine[0].equals("A") ) {
					
				} else {
					ChunkRes.X = Float.parseFloat(splitLine[0]);
					ChunkRes.Y = Float.parseFloat(splitLine[1]);
					
					chunks = new WorldChunk[Integer.parseInt(splitLine[2])][Integer.parseInt(splitLine[3])];
					
					// Should be the first line of a world. Should be in the form of
					// ChunkResX_ChunkResY_NumChunksX_NumChunksY
				}
				
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
