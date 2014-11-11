package buffs;

import main.Object;
import main.PlayerCharacter;
import main.Game;

public class Buff extends Object {

	public class BuffValues {
		
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
		float buffTimeLeft = 0;
		
		
	}
	
	private BuffValues buffVals;
	

	
	//	Name of the buff
	final String buffName;
	
	public Buff(String name, Buff.BuffValues buffValsIn) {
		buffName = name;
		buffVals = buffValsIn;
	}
	
	public int getBuffAmount(int baseValue, PlayerCharacter.statType statType) {
		switch (statType) {
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
		default:
			return baseValue;
		}
	}
	
	public void increaseBuffLength(float timeIncrease) {
		buffVals.buffTimeLeft += timeIncrease;
	}
	
	public float getTimeLeft() {
		return buffVals.buffTimeLeft;
	}

	public void tick(float deltaTime) {
		super.tick(deltaTime);
		
	}
}

