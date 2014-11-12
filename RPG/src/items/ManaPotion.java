package items;

public class ManaPotion extends ConsumableItem {
	public ManaPotion(int itemAmount) {
		super(itemAmount);
	}
	
	public void use() {
		super.use();	// Is this correct? We want to override use().
		System.out.println("YOU DIE!!!!!");
	}

}
