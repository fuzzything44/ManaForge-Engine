package items;

import main.PlayerCharacter;
import main.PlayerCharacter.equipmentTypes;

public class Hat extends Equipment {

	public Hat(int price, String name, String description, int amount) {
		super(price, name, description, amount, equipmentTypes.hat);
	}
	
	public void unEquip() {
		
	}
}
