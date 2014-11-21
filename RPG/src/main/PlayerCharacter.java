package main;

import buffs.*;
import items.*;

import java.util.Vector;

import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	public final float moveSpeed = 15f;
	
	private Coordinate velocity = new Coordinate(0);
	
	public Hat hat = null;
	public Shirt shirt = null;
//	public Pants pants = null;
//	public Shoes shoes = null;
//	public Necklace necklace = null;
//	public Earrings earrings = null;
//	public Ring ring = null;
//	public Gloves gloves = null;
	
	public Coordinate getVelocity(){
		return velocity;
	}
	
	public void setVelocity(Coordinate c){
		velocity = c;
	}
	
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
	
	public enum equipmentTypes {
		hat,
		shirt,
		pants,
		shoes,
		necklace,
		earring,
		ring
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
	
	public PlayerCharacter() {
		super();
		
		Game.TickingObjects.addElement(this);
		
		displayImage = "res/Knight.png";
		
		isRendered = true;
		
		renderOrder = 9;
		
	}
	
	public void removeBuff(Buff buffRemoved) {
		buffs.remove(buffRemoved);
		refreshStats();
	}	// remove expired buffs and refreshes stats
	
	public void refreshStats() {
		int healthBuff = 0,
				manaBuff = 0,
				strBuff = 0,
				dexBuff = 0,
				wisBuff = 0,
				attackBuff = 0,
				defenceBuff = 0;
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
	
	public int amountBuffs(){
		return buffs.size();
	}	// returns how many buffs are active
	
	public void equip(Equipment equippedItem) {
		switch (equippedItem.equip) {
		case hat:
			hat.unEquip();
			hat = (Hat) equippedItem;
		case shirt:
			shirt.unEquip();
			shirt = (Shirt) equippedItem;	// TODO before I go farther, is this the correct thing to do?
		default:
			break;
		}
		
	}
	
	public void tick(float deltaTime){
		location = location.add(velocity.mulitply(deltaTime/1000));
	}

}
