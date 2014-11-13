package items;

import main.*;
import main.Object;

public abstract class Item extends Object {
	protected final int sellPrice;
	protected final String itemName;
	protected final String itemDescription;
	protected int itemAmount;
	
	protected Item(int price, String name, String description, int amount) {
		sellPrice       = price;
		itemName        = name;
		itemDescription = description;
		itemAmount      = amount;
	}
	
	//	Lets you gain items
	public void gainItems(int itemChange) {
		itemAmount += itemChange;
		if (itemAmount <= 0) {
			// get rid of item
		}
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
				// GIVE CASH!
			} else {
				// WHAT IF NOT ENOUGH TO SELL?
			}
		} else {
			// Can't sell it. What do we do?
		}
	}	// method to sell items optional paramater
	
	public void sellItem() {
		sellItem(1);
	}
}
