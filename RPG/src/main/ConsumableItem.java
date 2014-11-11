package main;


public class ConsumableItem /*extends Item */{
	private int itemAmount;
	private final String itemName;
	
	public ConsumableItem(String nameIn, int amountIn) {
		itemAmount = amountIn;
		itemName = nameIn;
	}
	
	public void gainItems(int itemChange) {
		itemAmount += itemChange;
	}
	
	public int getItemAmount() {
		return itemAmount;
	}
	
	public String getItemName() {
		return itemName;
	}
	
	public void use() {}
}
