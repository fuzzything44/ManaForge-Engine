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

	public DynamicActor(int renderOrder, WorldChunk chunkIn, Coordinate place) {
		super(renderOrder, chunkIn, place);
		
		chunkIn.tickingObjects.add(this);
	}

	public DynamicActor(int renderOrder, WorldChunk chunkIn, String image) {
		super(renderOrder, chunkIn, image);
		
		chunkIn.tickingObjects.add(this);
	}

	public DynamicActor(int renderOrder, WorldChunk chunkIn, String image, Coordinate place) {
		super(renderOrder, chunkIn, image, place);
		
		chunkIn.tickingObjects.add(this);
	}

}
