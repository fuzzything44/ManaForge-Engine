package items;

import main.*;
import main.Object;

public class Item extends Object {
	final int sellPrice;
	final String itemName;
	private int itemAmount;
	
	public Item(String name, int amount, int sellsFor) {
		itemAmount = amount;
		itemName = name;
		sellPrice = sellsFor;
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
}
