package buffs;



public class Buff {
	
	public class BuffValues {
//		Percent Buffs. Put in decimal form.
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
	
	

}

