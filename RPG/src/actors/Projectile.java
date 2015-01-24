package actors;

import main.Coordinate;

public class Projectile extends DynamicActor {
	
	int timeLeft;
	final Actor parent;
	
	public Projectile(Coordinate location, Coordinate direction, int lifetime, String display, Actor generator) {
		super(9, display, location);
		velocity = direction;
		timeLeft = lifetime;
		collides = true;
		parent = generator;
	}
	
	@Override
	public void tick(float deltaTime) {
		location = location.add(velocity.multiply(deltaTime/1000) );
		// Move where it should
		
		if (timeLeft <= 0) {
			chunk.actors.remove(this);
		}
		// If it should despawn, do so.
		
		// Decrement time until it despawns.
		timeLeft -= deltaTime;
	}
}
