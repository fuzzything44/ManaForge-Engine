/*	Basic class for consumable items. Extends class item for code reuse.
 * 	Methods:
 * 		Constructor.	Takes Item name and starting amount.
 * 		gainItems(int)	Lets you change amount of items.
 * 		getItemAmount() Returns how many you have.
 * 		getItemName()	Returns name of the item.
 * 		use()			Uses the item as detailed in the specific class.
 */

package items;

import main.PlayerCharacter;

public class ConsumableItem extends Item {
	
	//	Constructor for the class. Takes name and starting amount.
	public ConsumableItem(int price, String name, String description, int amount) {
		super(price, name, description, amount);
		
		int indexOfConsumable = 0;
		for (int x = 1; x <= owner.consumables.size(); x++) {	// Loops through to see if you have this item
			if (owner.consumables.get(x).getItemName().equals(name) ) {
				indexOfConsumable = x;
			}
		}	// End loop
		
		if (indexOfConsumable == 0) {
			owner.consumables.add(this);
		} else {
			owner.consumables.get(indexOfConsumable).gainItems(amount);
		}
	}
	// Uses the item. Goes to specific use method.
	public void use() {}
	
	public void addToInventory(PlayerCharacter looter) {
		owner = looter;
		
		int indexOfItem = -1;
		hasItem : for (int i = 0; i < owner.consumables.size(); i++) {
			if (owner.consumables.get(i).equals(this) ) {
				indexOfItem = i;
				break hasItem;
			}
		}
		// Checking to see if you have this item already.

		if (indexOfItem != -1) {
			// If you don't have it, give it to you.
			owner.consumables.addElement(this);
		} else {
			// If you have it, give more of it.
			owner.consumables.get(indexOfItem).gainItems(itemAmount);
		}
	
	}	// End addToInventory()
}

