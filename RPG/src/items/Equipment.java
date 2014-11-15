package items;

import buffs.Buff;
import main.*;

public class Equipment extends Item {
	public final PlayerCharacter.equipmentTypes equip;
	
	Buff equipmentBuff = null;
	Buff.BuffValues vals;
	
	public Equipment(int price, String name, String description, int amount, PlayerCharacter.equipmentTypes equippedItem, PlayerCharacter looter, Buff.BuffValues buffFromItem) {
		super(price, name, description, amount, looter);
		equip = equippedItem;
		vals = buffFromItem;
		vals.isBuffPermanent = true;	// Just to make sure buff is permanent. It should be, but it may not.
		
		int indexOfConsumable = 0;
		for (int x = 1; x <= owner.equips.size(); x++) {	// Loops through to see if you have this item
			if (owner.equips.get(x).getItemName().equals(name) ) {
				indexOfConsumable = x;
			}
		}	// End loop
		
		if (indexOfConsumable == 0) {
			owner.equips.add(this);
		} else {
			owner.equips.get(indexOfConsumable).gainItems(amount);
		}
	}
	
	public void equip() {
		owner.equip(this);
	}
	
	public void unEquip() {
		owner.removeBuff(equipmentBuff);
	}
}
