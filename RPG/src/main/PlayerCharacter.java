package main;

public class PlayerCharacter extends Character {
	
/*	Max is the max buffed HP/MP/Stat
 *	Current is the amount of HP/MP you have left
 *	Base is the unbuffed MaxHP/MaxMP/Stat
 */
	
	int baseHP;
//	int maxHP;		Already in Character
//	int currentHP;	Already in Character
	
	int currentMP;
	int maxMP;
	int baseMP;
	
	int baseStr;	// Strength
	int baseDex;	// Dexterity
	int baseWis;	// Wisdom
	
	int buffedStr;	// Buffed versions. Used for most calculations
	int buffedDex;
	int buffedWis;
	
	int level;
	
//	int defence;
//	int attack;
	
//	int experience;
	
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

}
