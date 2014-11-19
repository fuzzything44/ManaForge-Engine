package main;
/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */
import org.lwjgl.input.Keyboard;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import buffs.Buff;

public class Play extends BasicGameState {


	Buff buff = null;
	
	public final int state;
	public PlayerCharacter character;
	public enum rightPaneStates {
		equipment,
		consumables,
		buffs,
		equipmentSpecific,
		consumableSpecific,
		buffSpecific
	}	// What the right pane can display. Specific is the exact buff/consumable/equipment info.
	public int infoPaneSpecific = 0;	// What vector index the specific thing is at.
	public rightPaneStates rightPaneState = rightPaneStates.buffs;
	
	public Play(int statein) {
		state = statein;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {
		// What is the purpose of this method? 

		character = new PlayerCharacter();
		
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		
		
		
		Input i = gc.getInput();
		gr.setColor(Color.white);
		gr.drawString(Float.toString(Game.GameTotalTime/1000), 100, 100);
		
		gr.fillRect(0, 0, 640, 50);				// Fills the top pane.
		gr.fillRect(440, 50, 200, 310);			// Fills the right pane.

		gr.setColor(Color.black);
		gr.drawLine(0, 50, 640, 50);			// Separates top and right panes.
		gr.drawString("Buff Time:", 10, 20);	// Yes, code here does need to be cleaned up. I have the rectangle size correct.
		
		
		if(i.isKeyPressed(Keyboard.KEY_F) ) {
			if (character.buffs.size() == 0) {	// If you have no buffs...
				Buff.BuffValues vals = new Buff.BuffValues();
				vals.attackBuffNumber = 5;
				vals.attackBuffPercent = 0.3f;	// f says it is a float constant, not a double.
				vals.buffTimeLeft = 2000;
				buff = new Buff("Buff1", vals, character);
				rightPaneState = rightPaneStates.buffSpecific;
			} else {	// If you do have a buff...
				buff.increaseBuffLength(2000);	
			}	// Should this be done in buff constructor? I think maybe. Also identify buffs by name. 
		}	// What this is doing is giving you a buff when you press E, unless you already have one. 
			// If you have one, it increases the time left.
		if (rightPaneState == rightPaneStates.buffs) {
			for (int x = 0; x < character.buffs.size(); x++) {	// Automatically displays all buffs you have. At the same place. Whatever.
				gr.drawString(Float.toString(character.buffs.get(x).getTimeLeft() /1000), 100, 20);	// Buff time left in seconds.
			}	// Needs to be changed to be like consumables without "[Use]" option. Finish consumables first and copy.
		} else if (rightPaneState == rightPaneStates.consumables) {
			for (int x = 0; x < character.buffs.size(); x++) {
				gr.drawString(character.consumables.get(x).getItemName(), 450, 50 + 20*x);
				gr.drawString("[Use]", 450, 50 + 20*x);
				gr.drawString("[Info]", 480, 50 + 20*x);
				// Spacing here needs to change. Also "[Use]" and "[Info]" need to be clickable
				// TODO Also we should probably have a scroll bar here. Hmm.
			}
		} else if (rightPaneState == rightPaneStates.equipment) {
			// Display Equipment
		} else if (rightPaneState == rightPaneStates.buffSpecific) {
			if (character.buffs.size() > infoPaneSpecific) {
				gr.drawString(character.buffs.elementAt(infoPaneSpecific).getBuffName(), 450, 70);
				gr.drawString(character.buffs.elementAt(infoPaneSpecific).getBuffDescription(), 450, 90);
				// Draws the description of what the buff does.
				gr.drawString(Float.toString(character.buffs.get(infoPaneSpecific).getTimeLeft() /1000), 100, 20);
				// gives time left of this specific buff.
				// TODO fix placement of stuff.
			} else {
				rightPaneState = rightPaneStates.buffs;
			}	// If/else makes sure that there is a buff at that index. If there isn't, 
				// it returns the right pane to buffs.
		} else if (rightPaneState == rightPaneStates.consumableSpecific) {
			// Need to do the same as buffs with equipment.
		} else if (rightPaneState == rightPaneStates.equipmentSpecific) {
			// Need to do the same with equipment.
		}
		
		
		for(int i1 = 0; i1 < Game.allActors.size(); i1++){
			if(Game.allActors.get(i1).isRendered){
				Actor a = Game.allActors.get(i1);
				
				int x, y;
				x = (int) ((a.getLocation().X * Game.zoom) + gc.getWidth()/2);
				y = (int) ((a.getLocation().Y * Game.zoom) + gc.getHeight()/2);
				
				gr.drawImage(a.displayImage, x, y,x + Game.zoom, y + Game.zoom,
						0, 0, a.displayImage.getWidth(), a.displayImage.getHeight());
				

			}
		}
		
		
	}	// End render method.

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {
		
		Game.GameTotalTime += delta;
	
		
		for (int i = 0; i < Game.TickingObjects.size(); i++) {
			Game.TickingObjects.get(i).tick(delta);
		}
		
		Input i = gc.getInput();
		
		if (i.isKeyPressed(Keyboard.KEY_ESCAPE) ) {
			sbg.enterState(Game.pause);
		}

		if(i.isKeyPressed(Keyboard.KEY_W) ) {
			character.Teleport(new Coordinate(character.getLocation().X, character.getLocation().Y - 1));
		}
		if(i.isKeyPressed(Keyboard.KEY_A) ) {
			character.Teleport(new Coordinate(character.getLocation().X - 1, character.getLocation().Y));
		}
		if(i.isKeyPressed(Keyboard.KEY_S) ) {
			character.Teleport(new Coordinate(character.getLocation().X, character.getLocation().Y + 1));
		}
		if(i.isKeyPressed(Keyboard.KEY_D) ) {
			character.Teleport(new Coordinate(character.getLocation().X + 1, character.getLocation().Y));
		}	// For movement, we want to continually move in a direction while the key is pressed. This means 
			// that coordinates will increase by fractional components every tick.
			// meaning we want i.isKeyDown(Keyboard.KEY_X); in the tick method.
			// and then do stuff for speed. (maybe .01 units per millisecond?)
		if(i.isKeyPressed(Keyboard.KEY_Q) ) {
			Game.zoom +=5;
			if (Game.zoom > 50) {
				Game.zoom = 50;	// Max zoom level. Needs adjusting.
			}
		}		
		if(i.isKeyPressed(Keyboard.KEY_E)){
			Game.zoom -=5;
			if (Game.zoom < 5) {
				Game.zoom = 5;	// min zoom level. Needs adjusting
			}
		}
	}	// End update method

	@Override
	public int getID() {
		return state;
	}
}
