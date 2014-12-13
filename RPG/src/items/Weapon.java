package items;

import main.PlayerCharacter;
import buffs.Buff.BuffValues;

public class Weapon extends Equipment {
	boolean twoHanded;
	int specialCooldown;
	int timeSinceSpecial;
	public Weapon(int price, String name, String description, int amount,
			PlayerCharacter looter, BuffValues buffFromItem, boolean isTwoHanded, int cooldown) {

		super(price, name, description, amount, equipmentTypes.weapon, looter, buffFromItem);
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
		}
	}	// End special attack
}
