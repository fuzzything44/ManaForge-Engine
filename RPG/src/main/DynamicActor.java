package main;

public class DynamicActor extends Actor {
	protected Coordinate velocity = new Coordinate(0);
	
	public Coordinate getVelocity() {
		return velocity;
	}
	
	public void setVelocity(Coordinate c) {
		velocity = c;
	}

	public DynamicActor(int renderOrder) {
		super(renderOrder);
		doesTick = true;
	}

	public DynamicActor(int renderOrder, Coordinate place) {
		super(renderOrder, place);
		doesTick = true;
	}

	public DynamicActor(int renderOrder, String image) {
		super(renderOrder, image);
		doesTick = true;
	}

	public DynamicActor(int renderOrder, String image, Coordinate place) {
		super(renderOrder, image, place);
		doesTick = true;
	}

}
