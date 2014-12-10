package items;

import main.PlayerCharacter;
import main.PlayerCharacter.equipmentTypes;
import buffs.Buff.BuffValues;

public class Weapon extends Equipment {
	int hands;
	public Weapon(int price, String name, String description, int amount,
			PlayerCharacter looter, BuffValues buffFromItem, int handsUsed) {
		super(price, name, description, amount, PlayerCharacter.equipmentTypes.weapon, looter, buffFromItem);
		hands = handsUsed;
		
	}

}
