package actors;

import main.Coordinate;
import main.Game;
import main.Object;
import main.World;
import main.WorldChunk;


/* The base class for anything placeable in the world
 * 
 * Constructor:
 * 	int renderOrder [, String image, Coordinate place]
 *  
 * 	changeRenderOrder(int newOrder)
 */


public class Actor extends Object {
	public boolean doesCollide = false;
	
	public WorldChunk chunk;

	private World world;
	
	public String displayImage;
	
	public Coordinate location = new Coordinate(0, 0);
	
	public boolean isRendered = true;
	
	public boolean isPersistent = false;
	public boolean collides = false;
	
	// 0 - 9, 9 is last, 0 is first
	public int renderOrder;
	
	public Actor(int renderOrder) {
		displayImage = "res/Default.png";
		this.renderOrder = renderOrder;
		world = Game.world;
	}

	public Actor(int renderOrder, Coordinate place) {			
		displayImage = "res/Default.png";	
		this.renderOrder = renderOrder;		
		location = place;
		world = Game.world;
	}	
	
	public Actor( int renderOrder, String image) {
		
		displayImage = image;	
		this.renderOrder = renderOrder;
		world = Game.world;
	}
	public Actor( int renderOrder, String image, Coordinate place) {
				
		displayImage = image;	
		this.renderOrder = renderOrder;		
		location = place;
		world = Game.world;
	}
	
	public void changeRenderOrder(int newRenderOrder) {
		renderOrder = newRenderOrder;
		// TODO change render order in chunk.
	}
	
	public void refreshChunk() {
		// refreshes what chunk it is in. If it is in the wrong chunk, it adds it to the right chunk.
		
		WorldChunk chunkIn = world.getActorChunk(location);
		if (isPersistent) {
			chunkIn = world.persistentChunk;
		}	// If it is persistent, the chunk it should be in is the persistent chunk

		if (chunk != chunkIn && chunk != null) {
			// If the chunk it is in and the chunk it should be in are different...
			// We want to remove it from the old chunk and add it to the new chunk
			
			chunk.actors.remove(this);
			if (doesTick) {
				chunk.tickingObjects.remove(this);
			}
			// Removing from old chunk.
			
			chunk = chunkIn;
			chunk.actors.addElement(this);
			if (doesTick) {
				chunk.tickingObjects.addElement(this);
			}
			// Adding it to the new chunk.
			
		}	// End if chunks are different
		else if (chunk == null) {
			// If there is no chunk set yet.
			chunk = chunkIn;
			chunk.actors.addElement(this);
			if (doesTick) {
				chunk.tickingObjects.addElement(this);
			}
		}
	}	// End refresh chunk
}
