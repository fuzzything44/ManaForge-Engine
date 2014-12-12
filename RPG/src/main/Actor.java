package main;


/* The base class for anything placeable in the world
 * 
 * Constructor:
 * 	int renderOrder, WorldChunk chunkIn [, String image, Coordinate place]
 *  
 * 	changeRenderOrder(int newOrder)
 */


public class Actor extends Object {
	public boolean doesCollide = false;
	
	public WorldChunk chunk;
	
	public String displayImage;
	
	public Coordinate location = new Coordinate(0, 0);
	
	public boolean isRendered = true;
	
	// 0 - 9, 9 is last, 0 is first
	public int renderOrder;
	
	public Actor(int renderOrder, WorldChunk chunkIn) {
				
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		chunk = chunkIn;
	}

	public Actor(int renderOrder, WorldChunk chunkIn, Coordinate place) {
				
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		location = place;
		chunk = chunkIn;
		
	}	
	
	public Actor( int renderOrder, WorldChunk chunkIn, String image) {
		
		displayImage = image;
	
		renderOrder = 1;
		chunk = chunkIn;
	}
	public Actor( int renderOrder, WorldChunk chunkIn, String image, Coordinate place) {
				
		displayImage = image;
	
		this.renderOrder = renderOrder;
		
		location = place;
		chunk = chunkIn;
	}
	
	public void changeRenderOrder(int newRenderOrder) {
		// Now we need to actually get the actor location.
		chunk.actors.addElement(this);
	}

}
