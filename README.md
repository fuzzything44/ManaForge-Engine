RPG-Simulator
=============
Overview:

What do we want to do?
-General Game Design
	-Theme: Do we want it to be medieval, oriental, futuristic, dystopian, or something else? 
		-I'm thinking medieval fantasy would work best,  but I don't know for sure
	-Arc: Will there be a storyline, or will it just be pure fighting? If there is a story, should it be happy, sad, or something else?
	-Progression: What keeps the player hooked?
-Character stuff:
	Stats-what ones will we use? How are they gained?
	Classes-what classes are there? What stats do they use?
		-Possible Classes (Archetypes) : Warrior, Magic User, Rogue, Monk, Druid?, Barbarian, Paladin, Healer, Archer, etc.
	Skills-what skills are there? How are they gained? What do they do?
-Enemies:
	What enemies are there? For that matter, what are we going to do for encounters?
		Will encounters be from a "explore X" button or can you explore a map?
		If there is a map, what will it be? How will you move? Are encounters random or visible?
	What stats do enemies have? Just damage and HP or is there other stuff?
		(I am thinking we can have HP, attack, defense, experience, drops (items and money), maybe a few special resistances (immune to spells, ex.), and body parts (you stab it in the X) -Tanner
	Anything else?
-Items:
	How will the inventory work? Are items stacked? (This is fundamentals, not just design).
		(Okay, have a list of objects that are your items. Ex. item redPotion = new item(String itemName, int itemNumber, method onUse() (what it does when you use redPotion.use)).
		In your item.java you have methods: increment(int itemChange) which changes the amount of the item you have, getAmount() which returns how many of the item you have, use() which uses the item by running the specified method, then private int itemAmount, private String itemName, methods for use) -Tanner
	What items are there? How are they acquired?
-Other stuff:
	How will the GUI look?
	Anything else?
