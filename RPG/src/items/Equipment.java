package items;

import actors.PlayerCharacter;
import buffs.Buff;

public class Equipment extends Item {

	public enum equipmentTypes {
		hat,
		shirt,
		pants,
		shoes,
		necklace,
		earring,
		ring,
		weapon
	}
		
	public final equipmentTypes equip;
	
	public Buff equipmentBuff = null;
	Buff vals;


	
	public Equipment(int price, String name, String description, int amount, equipmentTypes equippedItem, Buff buffFromItem) {
		super(price, name, description, amount);
		equip = equippedItem;
		vals = buffFromItem;
		vals.buffVals.isBuffPermanent = true;	// Just to make sure buff is permanent. It should be, but it may not.
		
		
		if (!description.equals("Empty Slot") ) {	// Don't want template equips to go into inventory.
			int indexOfEquipment = 0;
			for (int x = 1; x <= owner.equips.size(); x++) {	// Loops through to see if you have this item
				if (owner.equips.get(x).equals(this) ) {	// If the names are equal.
					indexOfEquipment = x;
				}
			}	// End loop
			
			if (indexOfEquipment == 0) {
				owner.equips.add(this);
			} else {
				owner.equips.get(indexOfEquipment).gainItems(amount);
			}
		}	// End if.
	}		// End constructor
	
	public boolean equals(Equipment e) {
		return (this.equip == e.equip && this.itemName.equals(e.itemName) && this.equipmentBuff.equals(e.equipmentBuff) );
		// For two equipments to be equal, they need the same equip type, name, and buff.
	}	// End equals method
	
	public void addToInventory(PlayerCharacter looter) {
		owner = looter;
		
		int indexOfItem = -1;
		hasItem : for (int i = 0; i < owner.equips.size(); i++) {
			if (owner.equips.get(i).equals(this) ) {
				indexOfItem = i;
				break hasItem;
			}
		}
		// Checking to see if you have this item already.

		if (indexOfItem != -1) {
			// If you don't have it, give it to you.
			owner.equips.addElement(this);
		} else {
			// If you have it, give more of it.
			owner.equips.get(indexOfItem).gainItems(itemAmount);
		}
			
	}	// End addToInventory()
}
