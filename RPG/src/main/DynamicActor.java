package main;

public class DynamicActor extends Actor {
	protected Coordinate velocity = new Coordinate(0);
	
	public Coordinate getVelocity() {
		return velocity;
	}
	
	public void setVelocity(Coordinate c) {
		velocity = c;
	}

	public DynamicActor(int renderOrder, WorldChunk chunkIn) {
		super(renderOrder, chunkIn);
		
		chunkIn.tickingObjects.add(this);
		
	}

	public DynamicActor(Coordinate place, int renderOrder, WorldChunk chunkIn) {
		super(place, renderOrder, chunkIn);
		
		chunkIn.tickingObjects.add(this);
	}

	public DynamicActor(String image, int renderOrder, WorldChunk chunkIn) {
		super(image, renderOrder, chunkIn);
		
		chunkIn.tickingObjects.add(this);
	}

	public DynamicActor(String image, Coordinate place, int renderOrder, WorldChunk chunkIn) {
		super(image, place, renderOrder, chunkIn);
		
		chunkIn.tickingObjects.add(this);
	}

}
