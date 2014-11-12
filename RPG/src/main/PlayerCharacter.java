package main;

import buffs.*;
import items.*;

import java.util.Vector;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	
	Equipment hat;
	Equipment shirt;
	Equipment pants;
	Equipment shoes;
	Equipment accessory1;
	
	int baseHealth = 100;
//	int maxHealth;		Already in Character
//	int currentHealth;	Already in Character
	
	int currentMana = 0;
	int maxMana = 100;
	int baseMana = 100;
	
	int baseStr = 10;	// Strength
	int baseDex = 10;	// Dexterity
	int baseWis = 10;	// Wisdom
	
	int buffedStr = 10;	// Buffed versions. Used for most calculations
	int buffedDex = 10;
	int buffedWis = 10;
	
	int level = 1;		// Character Level
	
	int buffedDefence = 1;
	int buffedAttack = 1;
	
//	int defence;
//	int attack;
	
//	int experience;
	
	Vector<Buff> buffs = new Vector<Buff>();
	
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
		buffedStr = strBuff + baseStr;
		buffedDex = dexBuff + baseDex;
		buffedWis = wisBuff + baseWis;
		buffedAttack = attackBuff + attack;
		buffedDefence = defenceBuff + defence;
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
		}
		if (buffedAttack <= 0) {
			buffedAttack = 1;
		}	// making sure stats stay above 0. Defence can be negative.
		
	}		// end refreshStats
	
	public void addBuff(Buff buffAdded) {
		buffs.addElement(buffAdded);
		refreshStats();
	}
	public int amountBuffs(){
		return buffs.size();
	}
}
