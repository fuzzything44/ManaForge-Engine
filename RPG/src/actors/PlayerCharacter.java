package actors;

import buffs.*;
import items.*;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Vector;

import main.Coordinate;
import main.Macros;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	public final float moveSpeed = 5f;

// Equipment takes:
//		int price, String name, String description, int amount, PlayerCharacter looter, Buff.BuffValues buffFromEquipment
//	public Equipment hat = Macros.equipment("noEquip_hat");TODO: uncomment and fix bugs in Macros so it doesn't crash!
//	public Equipment shirt = Macros.equipment("noEquip_shirt");
//	public Equipment pants = Macros.equipment("noEquip_pants");
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
		super(renderOrder);
		isPersistent = true;
	}
	public PlayerCharacter(int renderOrder, Coordinate place) {
		super(renderOrder, place);
		isPersistent = true;
	}
	public PlayerCharacter(int renderOrder, String image) {
		super(renderOrder, image);
		isPersistent = true;
	}
	public PlayerCharacter(int renderOrder, String image, Coordinate place) {
		super(renderOrder, image, place);
		isPersistent = true;
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
			healthBuff += buffs.get(x).getBuffAmount(statType.health);
			manaBuff += buffs.get(x).getBuffAmount(statType.mana);
			strBuff += buffs.get(x).getBuffAmount(statType.str);
			dexBuff += buffs.get(x).getBuffAmount(statType.wis);
			wisBuff += buffs.get(x).getBuffAmount(statType.dex);
			attackBuff += buffs.get(x).getBuffAmount(statType.attack);
			defenceBuff += buffs.get(x).getBuffAmount(statType.defence);
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
	//	unEquip(equippedItem.equip);
		// TODO finish item equipping
		switch (equippedItem.equip) {
		case hat:
	//		hat = equippedItem;
			break;
		case shirt:
	//		shirt = equippedItem;	
			break;
		case pants:
			
			break;
		case shoes:
			
			break;
		case necklace:
			
			break;
		case earring:
			
			break;
		case ring:
			
			break;
		case weapon:
			
			break;
		default:
			break;
		}
	}
	/*
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
	*/
	public void tick(float deltaTime) {
//		if (location.X >= maxX && location.X <= minX) {
		location = location.add(velocity.multiply(deltaTime/1000) );
	}
	
	public void loadSave(String saveFile) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(saveFile) );
			String[] line = reader.readLine().split("_");
			// Save file first line is in the form of :
			// Health_Mana_STR_DEX_WIS_ATK_DEF_Level_EXP_Money
			// Adding new base variables go on this line.
			baseHealth = Integer.parseInt(line[0]);
			baseMana = Integer.parseInt(line[1]);
			baseStr = Integer.parseInt(line[2]);
			baseDex = Integer.parseInt(line[3]);
			baseWis = Integer.parseInt(line[4]);
			attack = Integer.parseInt(line[5]);
			defence = Integer.parseInt(line[6]);
			level = Integer.parseInt(line[7]);
			experience = Integer.parseInt(line[8]);
			money = Integer.parseInt(line[9]);
			
			line = reader.readLine().split("_");
			// Second line is consumables held
			for (int i = 0; i < line.length; i++) {
				Macros.consumable(line[i]).addToInventory(this);
			}
			// Should be in form of:
			// Name.amount repeated with _ in between
			
			line = reader.readLine().split("_");
			// Third line is equipment
			for (int i = 0; i < line.length; i++) {
				Macros.equipment(line[i]).addToInventory(this);
			}
			// Same format as consumables
			
			line = reader.readLine().split("_");
			// Fourth line is buffs
			for (int i = 0; i < line.length; i++) {
				Macros.buff(line[i]).giveBuff(this);
			}
			// Should be in format:
			// Name.time repeated with _ in between
			// When we save, we don't want to save equipment buffs
			
			line = reader.readLine().split("_");
			// Fifth line is equipment
			// Should be in format:
			// Hat_Shirt_Pants_Shoes_Necklace_Earrings_Ring_Gloves_OffHand_Weapon
			// All are integer indices. (Maybe. We should figure out how we will handle equips.
			
			
			
			
			
			
			refreshStats();
			reader.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}	// End file loading
}
