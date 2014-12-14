package main;


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
		if (!isPersistent) {
			chunk = world.getActorChunk(location);
		} else {
			chunk = world.persistentChunk;
		}
	}
}
