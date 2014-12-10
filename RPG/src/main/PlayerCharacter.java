package main;

import buffs.*;
import items.*;

import java.util.Vector;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	public final float moveSpeed = 5f;

// Equipment takes:
//		int price, String name, String description, int amount, PlayerCharacter looter, Buff.BuffValues buffFromEquipment
	public Equipment hat = Macros.equipment("noEquip_hat");
	public Equipment shirt = Macros.equipment("noEquip_shirt");
	public Equipment pants = Macros.equipment("noEquip_pants");
//	public Shoes shoes = null;
//	public Necklace necklace = null;
//	public Earrings earrings = null;
//	public Ring ring = null;
//	public Gloves gloves = null;
	
	
	public int baseHealth = 100;
//	int maxHealth;		Already in Character
//	int currentHealth;	Already in Character
	
	public int currentMana = 0,
			maxMana = 100,
			baseMana = 100;
	
	public int baseStr = 10,	// Strength
		baseDex = 10,	// Dexterity
		baseWis = 10;	// Wisdom
	
	public int buffedStr = 10,	// Buffed versions. Used for most calculations
		buffedDex = 10,
		buffedWis = 10;
	
	public int level = 1;		// Character Level
//	int experience;

	public int buffedDefence = 1,
		buffedAttack = 1;
	
//	int defence;
//	int attack;
		
	public Vector<Buff> buffs = new Vector<Buff>();
	public Vector<Equipment> equips = new Vector<Equipment>();
	public Vector<ConsumableItem> consumables = new Vector<ConsumableItem>();
	
	public PlayerCharacter(int renderOrder) {
		super(renderOrder, Game.world.persistentChunk);
	}
	public PlayerCharacter(Coordinate place, int renderOrder) {
		super(place, renderOrder, Game.world.persistentChunk);
	}
	public PlayerCharacter(String image, int renderOrder) {
		super(image, renderOrder, Game.world.persistentChunk);
	}
	public PlayerCharacter(String image, Coordinate place, int renderOrder) {
		super(image, place, renderOrder, Game.world.persistentChunk);
	}
	
	public enum equipmentTypes {
		hat,
		shirt,
		pants,
		shoes,
		necklace,
		earring,
		ring,
		weapon
	}
	
	public enum statType {
		health,
		mana,
		str,
		dex,
		wis,
		exp,
		level,
		attack,
		defence
	}
	
	public void removeBuff(Buff buffRemoved) {
		buffs.remove(buffRemoved);
		refreshStats();
	}	// remove expired buffs and refreshes stats
	
	public void refreshStats() {
		// Method to recalculate all stats.
		
		// How much each stat is being buffed.
		int healthBuff = 0,
				manaBuff = 0,
				strBuff = 0,
				dexBuff = 0,
				wisBuff = 0,
				attackBuff = 0,
				defenceBuff = 0;
		// Setting each stat to right buff amount
		for (int x = 0; x < buffs.size(); x++) {	// buff loop. Gets the total stat buff for everything.
			healthBuff += buffs.get(x).getBuffAmount(baseHealth, statType.health);
			manaBuff += buffs.get(x).getBuffAmount(baseMana, statType.mana);
			strBuff += buffs.get(x).getBuffAmount(baseStr, statType.str);
			dexBuff += buffs.get(x).getBuffAmount(baseWis, statType.wis);
			wisBuff += buffs.get(x).getBuffAmount(baseDex, statType.dex);
			attackBuff += buffs.get(x).getBuffAmount(attack, statType.attack);
			defenceBuff += buffs.get(x).getBuffAmount(defenceBuff, statType.defence);
		}	// end buff loop.
		maxHealth = healthBuff + baseHealth;	// setting buffed values to correct amounts
		maxMana = manaBuff + baseMana;
		buffedStr = strBuff + baseStr;			// Would it be better to just use Math.max(statBuff + baseStat, 1); here?
		buffedDex = dexBuff + baseDex;
		buffedWis = wisBuff + baseWis;
		buffedAttack = attackBuff + attack;
		buffedDefence = defenceBuff + defence;
		if (maxHealth <= 0) {					// Using the math method gets rid of code from here...
			maxHealth = 1;
		}
		if (maxMana <= 0) {
			maxMana = 1;
		}
		if (buffedStr <= 0) {
			buffedStr = 1;
		}
		if (buffedDex <= 0) {
			buffedDex = 1;
		}
		if (buffedWis <= 0) {
			buffedWis = 1;
		}
		if (buffedAttack <= 0) {
			buffedAttack = 1;					// ...to here.
		}	// making sure stats stay above 0. Defence can be negative.
		
	}		// end refreshStats
	
	public void addBuff(Buff buffAdded) {
		buffs.addElement(buffAdded);
		refreshStats();
	}	// Not needed to run. Just run constructor for a buff.
	
	public int amountBuffs() {
		return buffs.size();
	}	// returns how many buffs are active
	
	public void equip(Equipment equippedItem) {
		unEquip(equippedItem.equip);
		
		switch (equippedItem.equip) {
		case hat:
			hat = equippedItem;
			break;
		case shirt:
			shirt = equippedItem;	// TODO before I go farther, is this the correct thing to do?
			break;
		default:
			break;
		}
	}
	
	public void unEquip(equipmentTypes equipSlot) {
		switch(equipSlot) {
		case hat:
			removeBuff(hat.equipmentBuff);
			hat = Macros.equipment("noEquip_hat");
			break;
		case shirt:
			removeBuff(shirt.equipmentBuff);
			shirt = Macros.equipment("noEquip_shirt");
			break;
		default:
			break;
		}
	}
	
	public void tick(float deltaTime) {
//		if (location.X >= maxX && location.X <= minX) {
		location = location.add(velocity.multiply(deltaTime/1000) );
	}

}
