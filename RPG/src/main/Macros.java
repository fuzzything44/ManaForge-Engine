package main;

import buffs.Buff;
import buffs.Buff.BuffValues;
import items.ConsumableItem;
import items.Equipment;

/*
 * Class to automatically generate Actors, items and more.
 * Before I go farther, I want to make sure that we have all constructors how we want them.
 */
public class Macros {

	public static Actor actor (String actorType, Coordinate location) {
		WorldChunk actorChunk = Game.world.getActorChunk(location);
		String actorImage = "res/Default.png";
		int actorRender = 0;
		// Variables needed to create an actor.
		Actor generatedActor = null;
		if (actorType.equalsIgnoreCase("grass")) {
			actorImage = "res/grass.png";
			// Grass is a background actor, and therefore is rendered first.
		} else if (actorType.equalsIgnoreCase("tree")) {
			generatedActor = new Actor(1, actorChunk, "res/tree.png", location);
			// Trees have a render order of 1. Currently.
			// Trees also need to be solid.
			
		} else {
			
		}

		if (generatedActor == null) {
			generatedActor = new Actor(actorRender, actorChunk, actorImage, location);
		}
		
		return generatedActor;
	}
	
	public static ConsumableItem consumable (String itemName) {
		ConsumableItem generatedItem = null;
		
		
		
		return generatedItem;
	}	// End consumable generation.
	
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
	 
	public static Buff buff(String buffName) {
		BuffValues generatedBuff = new BuffValues();
		String generatedName = "Buff";
		// Basic buff set to do nothing and be named "Buff"
		
		if (buffName.equalsIgnoreCase("attack_small")) {
			generatedBuff.attackBuffNumber = 5;
		} else if (buffName.equalsIgnoreCase("megaBuff")) {
			
		}
		
		return new Buff(generatedName, generatedBuff);
	}	// End buff generation
	
}
