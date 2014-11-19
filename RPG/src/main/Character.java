package main;

import java.util.Vector;

// base character class. Can be used by either enemy or friendly
public class Character extends Actor {
	
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
