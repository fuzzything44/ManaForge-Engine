/*	Basic class for consumable items. Extends class item for code reuse.
 * 	Methods:
 * 		Constructor.	Takes Item name and starting amount.
 * 		gainItems(int)	Lets you change amount of items.
 * 		getItemAmount() Returns how many you have.
 * 		getItemName()	Returns name of the item.
 * 		use()			Uses the item as detailed in the specific class.
 */

package items;

public class ConsumableItem /*extends Item */{
	private int itemAmount;
	private final String itemName;
	
	//	Constructor for the class. Takes name and starting amount.
	public ConsumableItem(String nameIn, int amountIn) {
		itemAmount = amountIn;
		itemName = nameIn;
	}
	
	//	Lets you gain items
	public void gainItems(int itemChange) {
		itemAmount += itemChange;
	}
	
	//	Returns amount of items you have
	public int getItemAmount() {
		return itemAmount;
	}
	
	//	Returns the name of the item
	public String getItemName() {
		return itemName;
	}
	
	// Uses the item. Goes to specific use method.
	public void use() {}
}

