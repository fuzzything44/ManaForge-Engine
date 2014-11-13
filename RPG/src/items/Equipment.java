package items;

import buffs.Buff;
import buffs.Buff.BuffValues;
import main.*;

public class Equipment extends Item {
	public final PlayerCharacter.equipmentTypes equip;
		
	public Equipment(int price, String name, String description, int amount, PlayerCharacter.equipmentTypes equippedItem) {
		super(price, name, description, amount);
		equip = equippedItem;
	}
	
	public void equip() {}
	public void unEquip() {}
}
