package items;

public class ManaPotion extends ConsumableItem {
	public ManaPotion(String itemName, int itemAmount) {
		super(itemName, itemAmount);
	}
	
	public void use() {
		super.use();
		System.out.println("YOU DIE!!!!!");
	}

}
