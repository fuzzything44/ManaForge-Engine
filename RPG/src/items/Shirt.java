package items;

import main.*;
import main.PlayerCharacter.equipmentTypes;

public class Shirt extends Equipment {

	public Shirt(int price, String name, String description, int amount, PlayerCharacter looter) {
		super(price, name, description, amount, equipmentTypes.shirt, looter);
	}
}
