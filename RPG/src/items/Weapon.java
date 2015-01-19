package items;

import buffs.Buff;

public class Weapon extends Equipment {
	boolean twoHanded;
	int specialCooldown;
	int timeSinceSpecial;
	int timeSinceAttack;
	int attackCooldown;
	public Weapon(int price, String name, String description, int amount,
			Buff buffFromItem, boolean isTwoHanded, int specialCooldown, int attackCooldown) {

		super(price, name, description, amount, equipmentTypes.weapon, buffFromItem);
		twoHanded = isTwoHanded;
		this.specialCooldown = specialCooldown;
		this.attackCooldown = attackCooldown;
		timeSinceSpecial = specialCooldown;
		timeSinceAttack = attackCooldown;
	}
	public void attack() {
		// When you attack with your weapon.
		
	}	// End attack
	
	public void specialAttack() {
		if (timeSinceSpecial >= specialCooldown) {
			// Use special attack
		} else {
			// Can't do special attack
		}
	}		// End special attack
	
	public void tick(int deltaTime) {
		timeSinceSpecial += deltaTime;
		timeSinceAttack += deltaTime;
	}
}
