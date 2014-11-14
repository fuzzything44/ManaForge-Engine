package items;

import buffs.Buff;
import main.*;

public class Equipment extends Item {
	public final PlayerCharacter.equipmentTypes equip;
	
	Buff equipmentBuff = null;
	Buff.BuffValues vals = new Buff.BuffValues();
	
	public Equipment(int price, String name, String description, int amount, PlayerCharacter.equipmentTypes equippedItem, PlayerCharacter looter) {
		super(price, name, description, amount, looter);
		equip = equippedItem;
		vals.isBuffPermanent = true;
	}
	
	public void equip() {
		owner.equip(this);
	}
	
	public void unEquip() {
		owner.removeBuff(equipmentBuff);
	}
}
