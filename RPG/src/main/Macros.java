package main;

import buffs.Buff.BuffValues;
import items.ConsumableItem;
import items.Equipment;

/*
 * Class to automatically generate Actors, items and more.
 * Before I go farther, I want to make sure that we have all constructors how we want them.
 */
public class Macros {

	public static Actor actor (String actorType) {
		
		// Well, actors currently automatically add themselves to their chunk
		// Should that happen with this class?
		Actor generatedActor = new Actor();
		if (actorType.equalsIgnoreCase("grass")) {
			// Make grass actor.
		}
		
		return generatedActor;
	}
	
	public static ConsumableItem consumable (String itemName) {
		ConsumableItem generatedItem;
		
		
		return generatedItem;
	}
	
	public static Equipment equipment (String equipName) {
		Equipment generatedEquipment;
		if (equipName.equalsIgnoreCase("noEquip_hat")) {
			generatedEquipment = new Equipment();
		} else if (equipName.equalsIgnoreCase("noEquip_shirt")) {
			
		} else {
			// Throw an error.
		}
		
		return generatedEquipment;
	}	// End equipment
	
	public static BuffValues buff (String buffName) {
		BuffValues generatedBuff = new BuffValues();
		if (buffName.equalsIgnoreCase("attack_small")) {
			generatedBuff.attackBuffNumber = 5;
		} else if (buffName.equalsIgnoreCase("megaBuff")) {
			
		}
		
		
		
		
		return generatedBuff;
	}
}
