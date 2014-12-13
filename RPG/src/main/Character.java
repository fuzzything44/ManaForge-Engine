package main;

// base character class. Can be used by either enemy or friendly
public class Character extends DynamicActor {
	
	public Character(int renderOrder, WorldChunk chunkIn) {
		super(renderOrder, chunkIn);
	}

	public Character(int renderOrder, WorldChunk chunkIn, Coordinate place) {
		super(renderOrder, chunkIn, place);
	}

	public Character(int renderOrder, WorldChunk chunkIn, String image) {
		super(renderOrder, chunkIn, image);
	}

	public Character(int renderOrder, WorldChunk chunkIn, String image, Coordinate place) {
		super(renderOrder, chunkIn, image, place);
	}
	
	public int currentHealth = 100,
		maxHealth = 100,
		attack = 1,
		experience = 0,
		defence = 1,
		money = 0;	
	
}
