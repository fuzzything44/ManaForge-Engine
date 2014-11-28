/* Overview:
 * 	
 * 
 * 
 * 
 * 
 */
package buffs;

import main.Object;
import main.PlayerCharacter;
import main.Game;

public class Buff extends Object {
	
	static public class BuffValues {
		
		public BuffValues() {}
		
		//	Percent Buffs. Put in decimal form.
		public float strBuffPercent = 0, 
			dexBuffPercent = 0,
			wisBuffPercent = 0,
			healthBuffPercent = 0,
			manaBuffPercent = 0, 
			attackBuffPercent = 0, 
			defenceBuffPercent = 0;
		
		//	Hard value buffs. Calculated after percent.
		public int strBuffNumber = 0,
			dexBuffNumber = 0,
			wisBuffNumber = 0,
			healthBuffNumber = 0,
			manaBuffNumber = 0,
			attackBuffNumber = 0,
			defenceBuffNumber = 0;

		//	How much time left until the buff runs out
		public float buffTimeLeft = 1;
		
		// Description of the buff. If default, will display basic nonzero stat modifiers (dexBuffNumber, dexBuffPercent ex.)
		public String buffDescription = "default";
		
		// Determines if the buff is permanent or not.
		public boolean isBuffPermanent = false;
		
		public boolean equals(BuffValues b) {
			// For two buff values to be equal, they need the exact same buff amounts,
			// description, and state of permanency.
			return (this.strBuffPercent 	== b.strBuffPercent 	&&
					this.dexBuffPercent 	== b.dexBuffPercent 	&&
					this.wisBuffPercent 	== b.wisBuffPercent 	&&
					this.attackBuffPercent	== b.attackBuffPercent 	&&
					this.defenceBuffPercent == b.defenceBuffPercent &&
					this.healthBuffPercent 	== b.healthBuffPercent 	&&
					this.manaBuffPercent 	== b.manaBuffPercent 	&&
					this.strBuffNumber		== b.strBuffNumber		&&
					this.dexBuffNumber		== b.dexBuffNumber		&&
					this.wisBuffNumber		== b.wisBuffNumber		&&
					this.attackBuffNumber	== b.attackBuffNumber	&&
					this.defenceBuffNumber	== b.defenceBuffNumber	&&
					this.healthBuffNumber	== b.healthBuffNumber	&&
					this.manaBuffNumber		== b.manaBuffNumber		&&
				
					this.buffDescription.equals(b.buffDescription)	&&
					
					this.isBuffPermanent	== b.isBuffPermanent
					
					);
		}	// End buffValues equals method.
		
	}	// End buffValues
	
	public class buffAmountsClass {
			public float buffPercent;
			public int buffAmount;
			public String buffType;
	}	// End buffAmountsClass
	
	private BuffValues buffVals;
	// own instance of buffValues. Used for ... things
	
	private PlayerCharacter owner;
	// The player. Used to remove this buff.
	
	//	Name of the buff
	final String buffName;
	
	private buffAmountsClass[] buffAmounts = new buffAmountsClass[7];
	// Array of values, useful for getting the description
	
	// Constructor for Buff
	public Buff(String name, Buff.BuffValues buffValsIn, PlayerCharacter player) {
		buffName = name;
		buffVals = buffValsIn;
		
		for (int numberOfBuffTypes = 0; numberOfBuffTypes < buffAmounts.length; numberOfBuffTypes++) {
			buffAmounts[numberOfBuffTypes] = new buffAmountsClass();
		}
		
		buffAmounts[0].buffPercent = buffVals.strBuffPercent;
		buffAmounts[0].buffAmount = buffVals.strBuffNumber;
		buffAmounts[0].buffType = "STR";
		
		buffAmounts[1].buffPercent = buffVals.dexBuffPercent;
		buffAmounts[1].buffAmount = buffVals.dexBuffNumber;
		buffAmounts[1].buffType = "DEX";
		
		buffAmounts[2].buffPercent = buffVals.wisBuffPercent;
		buffAmounts[2].buffAmount = buffVals.wisBuffNumber;
		buffAmounts[2].buffType = "WIS";
		
		buffAmounts[3].buffPercent = buffVals.attackBuffPercent;
		buffAmounts[3].buffAmount = buffVals.attackBuffNumber;
		buffAmounts[3].buffType = "Attack";
		
		buffAmounts[4].buffPercent = buffVals.defenceBuffPercent;
		buffAmounts[4].buffAmount = buffVals.defenceBuffNumber;
		buffAmounts[4].buffType = "Defence";
		
		buffAmounts[5].buffPercent = buffVals.healthBuffPercent;
		buffAmounts[5].buffAmount = buffVals.healthBuffNumber;
		buffAmounts[5].buffType = "Health";
		
		buffAmounts[6].buffPercent = buffVals.manaBuffPercent;
		buffAmounts[6].buffAmount = buffVals.manaBuffNumber;
		buffAmounts[6].buffType = "Mana";
		
		owner = player;
		
		assert owner == null : "Owner is set to null. How did this happen?";
		assert buffVals == null : "The given buff values (Buff.BuffValues) was null. Did you initialize it?";
		
		int hasBuff = -1;	// the index of the buff. If -1, you do not have the buff.
		for (int x = 0; x < owner.amountBuffs(); x++) {	// loops through to make sure you don't have the buff.
			if (owner.buffs.get(x).equals(this)) {
				hasBuff = x;
			}
		}
		if (hasBuff == -1) {
			owner.addBuff(this);
		} else {
			owner.buffs.get(hasBuff).increaseBuffLength(buffVals.buffTimeLeft);
		}
		if (!buffVals.isBuffPermanent) {
			Game.world.persistentChunk.tickingObjects.addElement(this);
		}
	}
	
	public int getBuffAmount(int baseValue, PlayerCharacter.statType statType) {	// gives how much a stat is being buffed.
		switch (statType) {	// lets you find out what variable is buffed and returns it.
		case health:
			return Math.round(baseValue*(buffVals.healthBuffPercent)) + buffVals.healthBuffNumber;
		case mana:
			return Math.round(baseValue*(buffVals.manaBuffPercent)) + buffVals.manaBuffNumber;
		case str:
			return Math.round(baseValue*(buffVals.strBuffPercent)) + buffVals.strBuffNumber;
		case dex:
			return Math.round(baseValue*(buffVals.dexBuffPercent)) + buffVals.dexBuffNumber;
		case wis:
			return Math.round(baseValue*(buffVals.wisBuffPercent)) + buffVals.wisBuffNumber;
		case attack:
			return Math.round(baseValue*(buffVals.attackBuffPercent)) + buffVals.attackBuffNumber;
		case defence:
			return Math.round(baseValue*(buffVals.defenceBuffPercent)) + buffVals.defenceBuffNumber;
//		case newStat:
//			return Math.round(baseValue*(buffVals.statBuffPercent)) + buffVals.statBuffNumber;
		default:
			return baseValue;
		}	// end switch
	}

	public void increaseBuffLength(float timeIncrease) {
		buffVals.buffTimeLeft += timeIncrease;
	}
	// Increases the time left on a buff.
	
	public float getTimeLeft() {
		return buffVals.buffTimeLeft;
	}
	
	// What the buff does every tick: decrease time left and remove itself if no time is left
	public void tick(float deltaTime) {
		super.tick(deltaTime);
		buffVals.buffTimeLeft -=deltaTime;
		if (buffVals.buffTimeLeft <= 0 && owner != null) {
			owner.removeBuff(this);
			Game.world.persistentChunk.tickingObjects.remove(this);
		}
	}	// End tick
	
	public String getBuffName () {
		return buffName;
	}	// End get name
	
	public String getBuffDescription() {
		if (buffVals.buffDescription.equals("default") ) {
			String desc = new String();
			for (int x =0; x < buffAmounts.length; x++) {
				/* This loops through to get you the entire values of what a buff does.
				 * If it gives you +5 STR, then there will be a line displaying "5 STR"
				 * +5% STR gives you a line displaying "5% STR"
				 */
				if (buffAmounts[x].buffPercent != 0) {
					if (buffAmounts[x].buffPercent > 0) {
						desc += "+";
					}
					desc += String.format("%d%% %s \n", Math.round(buffAmounts[x].buffPercent*100), buffAmounts[x].buffType);
					// Displays : "X% STAT" where X is the percent stat buff. If X is positive, put a plus sign in front.
				}
				if (buffAmounts[x].buffAmount != 0) {
					if (buffAmounts[x].buffAmount > 0) {
						desc += "+";
					}
					desc += String.format("%d %s \n", buffAmounts[x].buffAmount, buffAmounts[x].buffType);
				}
			}	// End for
			return desc;
		} else { 
			if (buffVals.buffDescription.length() < 20) {
				return buffVals.buffDescription;
			} else {
				// add line breaks every 20 characters.
				return "Description too long";
			}
		}		// End if/else
	}	// End getBuffDescription()
	
	public boolean equals(Buff equalTo) {
		return (this.buffVals.equals(equalTo.buffVals)	&&
				this.buffName.equals(equalTo.buffName)
				);
	}	// Overriding .equals method.
		// For two buffs to be equal, they need the same buffVals and name.

}		// End Buff class.

