public enum enemyData {
	enemy1 (
		1,			// enemy HP
		1,			// enemy ATK
		1,			// enemy money
		"enemy1", 	// enemy name
		String array[] = {
			"head",
			"body",
			"arm",
			"leg"
		},			// end body part array
		Item array[] = {
			item drop1 = new item("Item1", 1, item.HPrestore1)
		}			// end item drop array
		int array[] = {
			25
		}
	),
	
	public final int enemyHP;
	public final int enemyATK;
	public final int enemyMoney;
	public final String enemyName;
	public final String array[] bodyParts;
	public final String Item[] drops;
	public final int array[] dropChance;
}
