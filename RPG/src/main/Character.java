package main;

import java.util.Vector;

// base character class. Can be used by either enemy or friendly
public class Character extends DynamicActor {
	
	public Character(int renderOrder, WorldChunk chunkIn) {
		super(renderOrder, chunkIn);
		// TODO Auto-generated constructor stub
	}
	public Character(int renderOrder, WorldChunk chunkIn, Coordinate place) {
		super(renderOrder, chunkIn, place);
		// TODO Auto-generated constructor stub
	}
	public Character(int renderOrder, WorldChunk chunkIn, String image) {
		super(renderOrder, chunkIn, image);
		// TODO Auto-generated constructor stub
	}
	public Character(int renderOrder, WorldChunk chunkIn, String image, Coordinate place) {
		super(renderOrder, chunkIn, image, place);
		// TODO Auto-generated constructor stub
	}

	//	Enum of all POSSIBLE body parts. Then use an array for body parts it has.	
	enum PossibleBodyParts{
		torso,
		leg,
		arm,
		head
	 }
	

	
	public int currentHealth;
	public int maxHealth;
	public int attack;
	public int experience;
	public int defence;
	public Vector<PossibleBodyParts> bodyParts;
	
	
}
