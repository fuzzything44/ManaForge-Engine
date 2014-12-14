package items;

import main.*;
import main.Object;

public abstract class Item extends Object {
	protected final int sellPrice;
	protected final String itemName;
	protected final String itemDescription;
	protected int itemAmount;
	protected PlayerCharacter owner;
	
	protected Item(int price, String name, String description, int amount) {
		sellPrice       = price;
		itemName        = name;
		itemDescription = description;
		itemAmount      = amount;
	}
	
	//	Lets you gain items
	public void gainItems(int itemChange) {
		itemAmount += itemChange;
		if (itemAmount <= 0) {						// If you have no items of this kind left, it removes it.
			if (this instanceof Equipment) {		// Detects if it is of class Equipment.
				owner.equips.remove(this);
			} else if (this instanceof ConsumableItem) {
				owner.consumables.remove(this);
			}
		}											// End having no items
	}	// End gainItems
	
	//	Returns amount of items you have
	public int getItemAmount() {
		return itemAmount;
	}
	
	//	Returns the name of the item
	public String getItemName() {
		return itemName;
	}
	
	public void sellItem(int sellQuantity) {
		if (sellPrice != 0) {
			if (itemAmount >= sellQuantity) {	// If you have enough to sell
				owner.money += sellQuantity*sellPrice;
				gainItems(-sellQuantity);
			} else {	// Not enough sells all you have
				owner.money += sellPrice*itemAmount;
				gainItems(-sellQuantity);
			}
		}	// End if
	}		// method to sell items with optional parameter.
	
	public void sellItem() {
		sellItem(1);
	}
	
}
