package main;


/* The base class for anything placeable in the world
 * 
 */


public class Actor extends Object {
	public boolean doesCollide = false;
	
	public WorldChunk chunk;
	
	public Actor(int renderOrder, WorldChunk chunkIn) {
		
		if(!chunkIn.actors.contains(this)){
		
			chunkIn.actors.get(renderOrder).add(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		chunk = chunkIn;
	}

	public Actor(Coordinate place, int renderOrder, WorldChunk chunkIn) {
		
		if(!chunkIn.actors.contains(this)){
		
			chunkIn.actors.get(renderOrder).add(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		location = place;
		chunk = chunkIn;
		
	}	
	
	public Actor(String image, int renderOrder, WorldChunk chunkIn) {
		
		if(!chunkIn.actors.contains(this)){
		
			chunkIn.actors.get(renderOrder).add(this);
		}
		displayImage = image;
	
		renderOrder = 1;
		chunk = chunkIn;
	}
	public Actor(String image, Coordinate place, int renderOrder, WorldChunk chunkIn) {
		
		if(!chunkIn.actors.contains(this)){
		
			chunkIn.actors.get(renderOrder).add(this);
		}
		displayImage = image;
	
		this.renderOrder = renderOrder;
		
		location = place;
		chunk = chunkIn;
	}
	
	public void changeRenderOrder(int newRenderOrder, WorldChunk chunkIn) {
		chunkIn.actors.get(renderOrder).remove(this);
		chunkIn.actors.get(newRenderOrder).add(this);
	}
	 
	public String displayImage;
	
	public Coordinate location = new Coordinate(0);
	
	public boolean isRendered = true;
	
	// 0 - 9
	public int renderOrder;
}
