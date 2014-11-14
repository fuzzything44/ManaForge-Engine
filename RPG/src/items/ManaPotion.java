package items;

import main.PlayerCharacter;

public class ManaPotion extends ConsumableItem {
	// Why is this? I want to override the variables set out in Item.
	// Seems to be giving me a bug.
	
	public ManaPotion(int itemAmount, PlayerCharacter looter) {
		super(5, "Blue Potion", "WILL NOT KILL YOU AT ALL", itemAmount, looter);
	}
	
	public void use() {
		super.use();	// Is this correct? We want to override use().
		System.out.println("It has a delicious, fruity taste to it. Additionally, you die.");
		gainItems(-1); 
	}	// method to use item.
}