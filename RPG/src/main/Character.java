package main;

import java.util.Vector;

// base character class. Can be used by either enemy or friendly
public class Character extends DynamicActor {
	
	public Character(int renderOrder) {
		super(renderOrder);
		// TODO Auto-generated constructor stub
	}
	public Character(Coordinate place, int renderOrder) {
		super(place, renderOrder);
		// TODO Auto-generated constructor stub
	}
	public Character(String image, int renderOrder) {
		super(image, renderOrder);
		// TODO Auto-generated constructor stub
	}
	public Character(String image, Coordinate place, int renderOrder) {
		super(image, place, renderOrder);
		// TODO Auto-generated constructor stub
	}

	//	Enum of all POSSIBLE body parts. Then use an array for body parts it has.	
	enum PossibleBodyParts{
		torso,
		leg,
		arm,
		head
	 }
	

	
	int currentHealth;
	int maxHealth;
	int attack;
	int experience;
	int defence;
	Vector<PossibleBodyParts> bodyParts;
	
	
}
