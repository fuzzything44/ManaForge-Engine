package buffs;

public class Buff {
	
	//	Percent Buffs. Put in decimal form.
	private float strBuffPercent, 
		dexBuffPercent,
		wisBuffPercent,
		healthBuffPercent,
		manaBuffPercent, 
		attackBuffPercent, 
		defenceBuffPercent;
	
	//	Hard value buffs. Calculated after percent.
	private int strBuffNumber,
		dexBuffNumber,
		wisBuffNumber,
		healthBuffNumber,
		manaBuffNumber,
		attackBuffNumber,
		defenceBuffNumber;
	
	//	How much time left until the buff runs out
	int buffTimeLeft;
	
	//	Name of the buff
	final String buffName;
	
	public Buff(String name) {
		buffName = name;
	}
	
	

}

