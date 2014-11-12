package items;

public class ManaPotion extends ConsumableItem {
	public ManaPotion(String itemName, int itemAmount, int sellPrice) {
		super(itemName, itemAmount, sellPrice);
	}
	
	public void use() {
		super.use();
		System.out.println("YOU DIE!!!!!");
	}

}
