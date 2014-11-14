package items;

import main.*;
import main.Object;

public abstract class Item extends Object {
	protected final int sellPrice;
	protected final String itemName;
	protected final String itemDescription;
	protected int itemAmount;
	protected PlayerCharacter owner;
	
	protected Item(int price, String name, String description, int amount, PlayerCharacter looter) {
		sellPrice       = price;
		itemName        = name;
		itemDescription = description;
		itemAmount      = amount;
		owner			= looter;
	}
	
	//	Lets you gain items
	public void gainItems(int itemChange) {
		itemAmount += itemChange;
		if (itemAmount <= 0) {						// If you have no items of this kind left, it removes it.
			if (this instanceof Equipment) {		// I think this detects if it is of class Equipment.
				owner.equips.remove(this);
			} else if (this instanceof ConsumableItem) {
				owner.consumables.remove(this);
			}
		}											// End having no items
	}
	
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
			if (itemAmount >= sellQuantity) {
				itemAmount -= sellQuantity;
				// TODO GIVE CASH!. Also other sell stuff here.
			} else {
				// WHAT IF NOT ENOUGH TO SELL?
			}
		} else {
			// Can't sell it. What do we do?
		}
	}	// method to sell items with optional parameter.
	
	public void sellItem() {
		sellItem(1);
	}
}
