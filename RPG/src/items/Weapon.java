package items;

import main.Game;
import buffs.Buff;

public class Weapon extends Equipment {
	boolean twoHanded;
	int specialCooldown;
	int timeSinceSpecial;
	public Weapon(int price, String name, String description, int amount,
			Buff buffFromItem, boolean isTwoHanded, int cooldown) {

		super(price, name, description, amount, equipmentTypes.weapon, buffFromItem);
		twoHanded = isTwoHanded;
		specialCooldown = cooldown;
		timeSinceSpecial = cooldown;
	}
	public void attack() {
		// When you attack with your weapon.
		
	}	// End attack
	
	public void specialAttack() {
		if (timeSinceSpecial >= specialCooldown) {
			// Use special attack
		} else {
			// do special attack
			// Also add this to ticking objects
			Game.world.persistentChunk.tickingObjects.addElement(this);
		}
	}		// End special attack
	
	public void tick(int deltaTime) {
		timeSinceSpecial += deltaTime;
		if (timeSinceSpecial >= specialCooldown) {
			Game.world.persistentChunk.tickingObjects.remove(this);
		}
	}
}
