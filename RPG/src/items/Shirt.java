package items;

import buffs.Buff;
import main.*;
import main.PlayerCharacter.equipmentTypes;

public class Shirt extends Equipment {
	// Takes sell price (0 is unsellable), name, description, how many you have, 
	// owner, and buff it gives.
	// Total requirements: int, string, string, int, PlayerCharacter, Buff.BuffVals
	public Shirt(int price, String name, String description, int amount, PlayerCharacter looter, Buff.BuffValues buffFromEquipment) {
		super(price, name, description, amount, equipmentTypes.shirt, looter, buffFromEquipment);
	}
}
