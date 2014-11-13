/*	Basic class for consumable items. Extends class item for code reuse.
 * 	Methods:
 * 		Constructor.	Takes Item name and starting amount.
 * 		gainItems(int)	Lets you change amount of items.
 * 		getItemAmount() Returns how many you have.
 * 		getItemName()	Returns name of the item.
 * 		use()			Uses the item as detailed in the specific class.
 */

package items;

public class ConsumableItem extends Item {
	
	//	Constructor for the class. Takes name and starting amount.
	public ConsumableItem(int price, String name, String description, int amount) {
		super(price, name, description, amount);
	}
	// Uses the item. Goes to specific use method.
	public void use() {}
}

