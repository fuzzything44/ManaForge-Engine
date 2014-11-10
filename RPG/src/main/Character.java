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
