package main;

public class DynamicActor extends Actor {
	protected Coordinate velocity = new Coordinate(0);
	
	public Coordinate getVelocity() {
		return velocity;
	}
	
	public void setVelocity(Coordinate c) {
		velocity = c;
	}

	public DynamicActor() {
		// TODO Auto-generated constructor stub
	}

	public DynamicActor(Coordinate place) {
		super(place);
		// TODO Auto-generated constructor stub
	}

	public DynamicActor(String image) {
		super(image);
		// TODO Auto-generated constructor stub
	}

	public DynamicActor(String image, Coordinate place) {
		super(image, place);
		// TODO Auto-generated constructor stub
	}

}
