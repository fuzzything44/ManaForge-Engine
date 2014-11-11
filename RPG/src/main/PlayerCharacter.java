package main;

import buffs.*;
import java.util.Vector;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	
	int baseHealth;
//	int maxHealth;		Already in Character
//	int currentHealth;	Already in Character
	
	int currentMana;
	int maxMana;
	int baseMana;
	
	int baseStr;	// Strength
	int baseDex;	// Dexterity
	int baseWis;	// Wisdom
	
	int buffedStr;	// Buffed versions. Used for most calculations
	int buffedDex;
	int buffedWis;
	
	int level;		// Character Level
	
//	int defence;
//	int attack;
	
//	int experience;
	
	Vector<Buff> buffs;
	
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
				wisBuff = 0;
		for (int x = 0; x < buffs.size(); x++) {	// buff loop. Gets the total stat buff for everything.
			healthBuff += buffs.get(x).getBuffAmount(baseHealth, statType.health);
			manaBuff += buffs.get(x).getBuffAmount(baseMana, statType.mana);
			strBuff += buffs.get(x).getBuffAmount(baseStr, statType.str);
			dexBuff += buffs.get(x).getBuffAmount(baseWis, statType.wis);
			wisBuff += buffs.get(x).getBuffAmount(baseDex, statType.dex);
		}	// end buff loop.
		maxHealth = healthBuff + baseHealth;	// setting buffed values to correct amounts
		maxMana = manaBuff + baseMana;
		buffedStr = strBuff + baseStr;
		buffedDex = dexBuff + baseDex;
		buffedWis = wisBuff + baseWis;
		if (maxHealth <= 0) {
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
		}	// making sure stats stay above 0
	}		// end refreshStats
	
	public void addBuff(Buff buffAdded) {
		buffs.addElement(buffAdded);
		refreshStats();
	}
}
