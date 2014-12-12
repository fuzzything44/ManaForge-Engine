package main;

import buffs.Buff.BuffValues;
import items.ConsumableItem;
import items.Equipment;

/*
 * Class to automatically generate Actors, items and more.
 * Before I go farther, I want to make sure that we have all constructors how we want them.
 */
public class Macros {

	public static Actor actor (String actorType, Coordinate location) {
		Actor generatedActor = new Actor(0, new WorldChunk(new Coordinate(0, 0), Game.world)); //TODO: actually parse it !!
		if (actorType.equalsIgnoreCase("grass")) {
			//generatedActor = new Actor(0, );
		} else {
			
		}
		
		return generatedActor;
	}
	
	public static ConsumableItem consumable (String itemName) {
		ConsumableItem generatedItem = null;
		
		
		return generatedItem;
	}
	
	public static Equipment equipment (String equipName) {
		Equipment generatedEquipment = null;
		if (equipName.equalsIgnoreCase("noEquip_hat")) {
			generatedEquipment = new Equipment(0, equipName, equipName, 0, null, null, null); //TODO: actually parse it!!
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
