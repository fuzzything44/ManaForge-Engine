package main;

import java.util.Vector;

// base character class. Can be used by either enemy or friendly
public class Character {
	
	enum BodyParts{
		torso,
		leg,
		arm,
		head
	 }
	
	int currentHP;
	int maxHP;
	int attackStrength;
	Vector<BodyParts> bodyParts;
	
	
}
/*
Item creation:
ManaPotion item = new ManaPotion("NOT DEADLY!", 1);

public void dispInventory() {
	for(int x = 0, x < size of item array/whatever , x++) {
		if (itemArrayObject.getItemAmount() ) > 0 {
			dispOnCorrectPlace(itemArrayObject.getItemName);
			Increment dispOnCorrectPlace
		}
	}
}

What I want to do with items.

*/