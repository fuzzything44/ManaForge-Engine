package main;

import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

import javax.imageio.ImageIO;

import org.newdawn.slick.Image;


public class World {

	public LinkedList<LinkedList<WorldChunk>> chunks = new LinkedList<LinkedList<WorldChunk>>();
	public int chunkResX = 50;
	public int chunkResY = 50;

	public World() {
		
	}
	
	public WorldChunk getActorChunk(Coordinate location) {
		return chunks.get( (int) Math.floor(location.x / chunkResX) ).get( (int) Math.floor(location.y / chunkResY) );
		// Gets you what chunk the absolute location maps to.
	}
	
	public WorldChunk getActorChunk(Actor a) {
		return getActorChunk(a.actorLocation);
	}
	
	public void save(String saveFolder) {
		BufferedImage backgroundSave = new BufferedImage(chunkResX * chunks.size(), chunkResY * chunks.getFirst().size(), BufferedImage.TYPE_INT_RGB);

		Map<Image, Integer> imageMap = new HashMap<Image, Integer>();
		// Looping through every chunk and then all background parts of the chunks.
		for (int x = 0; x < chunks.size(); x++) {
			for (int y = 0; y < chunks.get(x).size(); y++) {
				// These two looped through the chunks.
				
				for (int chunkX = 0; chunkX < chunkResX; chunkX++) {
					for (int chunkY = 0; chunkY < chunkResY; chunkY++) {
						// End of the mass of loops. These two looped through the background chunk arrays.
						
						if (imageMap.get(chunks.get(x).get(y).background[chunkX][chunkY]) != null) {
							// If the image is not new.
							backgroundSave.setRGB( (x * chunkResX) + chunkX,// x is what chunk so this gives the actor absolute position
									(y * chunkResY) + chunkY,	// gives actor absolute y value
									imageMap.get(
											chunks.get(x).get(y).background[chunkX][chunkY]	// Gives the background image at the coordinate.
											));
						} else {
							
							boolean pixelAdded = false;

							while (!pixelAdded) {
								int r = (int) Math.round(Math.random() * 255);
								int g = (int) Math.round(Math.random() * 255);
								int b = (int) Math.round(Math.random() * 255);
								int rgb = ((r&0x0ff)<<16)|((g&0x0ff)<<8)|(b&0x0ff);
								// Creates a random color as an int.
								
								if (!imageMap.containsValue(rgb) ) {
									// If the color hasn't been used yet.
									backgroundSave.setRGB( (x * chunkResX) + chunkX, (y * chunkResY) + chunkY, rgb);
									// Add the pixel to the image.
									
									imageMap.put(chunks.get(x).get(y).background[chunkX][chunkY], rgb);
									// This color is now keyed. 
								}
							}// End while
							
						}// End if/else
					}// End chunkY for
				}
			}// End y for
		}// End x for
		// Image has been created!
		try {
			ImageIO.write(backgroundSave, "PNG", new File(saveFolder + "background") );
			// Saving the image
		} catch (IOException e1) {
			
		}
		
		// TODO: Create text file here.
		try {
			BufferedWriter textFile = new BufferedWriter(new FileWriter(saveFolder + "main.world") );
			
			textFile.write(String.format("%d_%d_%d_%d", chunkResX, chunkResY, chunks.size(), chunks.getFirst().size() ) );
			textFile.newLine();
			Image[] images = (Image[]) imageMap.keySet().toArray();
			// Array of all the images used.
			
			for (int i = 0; i < images.length; i++) {
				// Adding the color mappings
				int color = imageMap.get(images[i]);
				String imageName = images[i].getName();
				
				textFile.write(color + "_" + imageName);
				// Writes the actual mapping.
				textFile.newLine();
			}
			textFile.write("end");
			// Ending color mapping
			
			// Now to write all actors to the file
			// First 2 loops go through the chunks. Inside goes through the actor vector
			for (int x = 0; x < chunks.size(); x++) {
				for (int y = 0; y < chunks.get(x).size(); y++) {
					for (int i = 0; i < chunks.get(x).get(y).actors.size(); i++) {
						Actor writtenActor = chunks.get(x).get(y).actors.get(i);
						textFile.write(writtenActor.actorLocation.x + "_" + writtenActor.actorLocation.y + "_" + writtenActor.actorName);
						// Writes the Macros name of the actor.
					}
				}
			}
			
			
			textFile.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}// End save
	
	public void addChunksLeft() {
		LinkedList<WorldChunk> chunkRow = new LinkedList<WorldChunk>();
		// Creating a row of chunks to add to the left
		
		for (int i = 0; i < chunks.getFirst().size(); i++) {
			chunkRow.add(new WorldChunk(this) );
			// Filling the row of chunks.
		}
		chunks.addFirst(chunkRow);
		// Adding the row.
	}
	
	public void addChunksRight() {
		LinkedList<WorldChunk> chunkRow = new LinkedList<WorldChunk>();
		// Creating a row of chunks to add to the right
		
		for (int i = 0; i < chunks.getFirst().size(); i++) {
			chunkRow.add(new WorldChunk(this) );
			// Filling the row of chunks.
		}
		chunks.addLast(chunkRow);
		// Adding the row.
	}
	
	public void addChunksTop() {
		// Loop through the list of chunks.
		for (int i = 0; i < chunks.size(); i++) {
			chunks.get(i).addFirst(new WorldChunk(this) );
			// Add a world chunk at this x and top y (addFirst)
		}
	}
	
	public void addChunksBottom() {
		// Loop through the list of chunks.
		for (int i = 0; i < chunks.size(); i++) {
			chunks.get(i).addLast(new WorldChunk(this) );
			// Add a world chunk at this x and bottom y (addLast)
		}

	}
}
