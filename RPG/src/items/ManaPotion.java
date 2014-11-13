package items;

public class ManaPotion extends ConsumableItem {
	// Why is this? I want to override the variables set out in Item.
	// Seems to be giving me a bug.
	
	public ManaPotion(int itemAmount) {
		super(itemVars.ManaPotionSell, itemVars.ManaPotionName, "WILL NOT KILL YOU AT ALL", itemAmount);
	}
	
	public void use() {
		super.use();	// Is this correct? We want to override use().
		System.out.println("It has a delivious, fruity taste to it. Additionally, you die.");
		gainItems(-1); 
	}	// method to use item.
}