package main;

// base character class. Can be used by either enemy or friendly
public class Character extends DynamicActor {
	
	public Character(int renderOrder) {
		super(renderOrder);
	}

	public Character(int renderOrder, Coordinate place) {
		super(renderOrder, place);
	}

	public Character(int renderOrder, String image) {
		super(renderOrder, image);
	}

	public Character(int renderOrder, String image, Coordinate place) {
		super(renderOrder, image, place);
	}
	
	public int currentHealth = 100,
		maxHealth = 100,
		attack = 1,
		experience = 0,
		defence = 1,
		money = 0;	
	
}
