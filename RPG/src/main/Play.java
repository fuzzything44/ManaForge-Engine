package main;
/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */
import java.util.Vector;

import org.lwjgl.input.Keyboard;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import buffs.Buff;

public class Play extends BasicGameState {


	Buff buff = null;
	
	public final int state;
	public PlayerCharacter character = null;
	public enum rightPaneStates {
		equipment,
		consumables,
		buffs,
		equipmentSpecific,
		consumableSpecific,
		buffSpecific
	}	// What the right pane can display. Specific is the exact buff/consumable/equipment info.
	public int infoPaneSpecific = 1;	// What vector index the specific thing is at.
	public rightPaneStates rightPaneState = rightPaneStates.buffs;
	
	public Play(int statein) {
		state = statein;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {
		
		if(character == null)
			character = new PlayerCharacter();
		
		Actor build;
		

		Image i = new Image("res/grass.png");
		
		for(int x = -10; x <= 10; x++){
			for(int y = -10; y <= 10; y++){
				build = new Actor();
				build.location = new Coordinate(x, y);
				
				build.displayImage = i;
					
				
			}
		}
		
		Actor a = new Actor();
		a.location = new Coordinate(3, 4);
		Actor a1 = new Actor();
		a1.location = new Coordinate(-5, -7);
		Actor a2 = new Actor();
		a2.location = new Coordinate(-2, 1);
		Actor a3 = new Actor();
		a3.location = new Coordinate(0, -8);
		
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		
		/*a
		
		Input i = gc.getInput();
		gr.setColor(Color.white);
		gr.drawString(Float.toString(Game.GameTotalTime/1000), 100, 100);
		
		gr.fillRect(0, 0, 640, 50);				// Fills the top pane.
		gr.fillRect(440, 50, 200, 310);			// Fills the right pane.

		gr.setColor(Color.black);
		gr.drawLine(0, 50, 640, 50);			// Separates top and right panes.
		gr.drawString("Buff Time:", 10, 20);	// Yes, code here does need to be cleaned up. I have the rectangle size correct.
		
		
		if(i.isKeyPressed(Keyboard.KEY_E) ) {
			if (character.buffs.size() == 0) {	// If you have no buffs...
				Buff.BuffValues vals = new Buff.BuffValues();
				vals.buffTimeLeft = 2000;
				buff = new Buff("Buff1", vals, character);
			} else {	// If you do have a buff...
				buff.increaseBuffLength(2000);
			}
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
			gr.drawString(character.buffs.elementAt(infoPaneSpecific).getBuffName(), 450, 70);
			gr.drawString(character.buffs.elementAt(infoPaneSpecific).getBuffDescription(), 0, 0);
			// Draws the description if not default. Fix placement.
			
		} else if (rightPaneState == rightPaneStates.consumableSpecific) {
			
		} else if (rightPaneState == rightPaneStates.equipmentSpecific) {
			
		}
		*/


		for(int order = 0; order < 10; order++){
			for(int i1 = 0; i1 < Game.allActors.size(); i1++){
				if( Game.allActors.get(i1).isRendered && Game.allActors.get(i1).renderOrder == order){
					Actor a =  Game.allActors.get(i1);
					int x, y;
					x = (int) (((a.location.X - character.location.X) * Game.zoom) + gc.getWidth()/2);
					y = (int) (((a.location.Y - character.location.Y) * Game.zoom) + gc.getHeight()/2);
					
					
					gr.drawImage(a.displayImage, x, y,x + Game.zoom, y + Game.zoom,
							0, 0, a.displayImage.getWidth(), a.displayImage.getHeight());
					
					
					
				}
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

		character.setVelocity(new Coordinate(0));
		
		if(i.isKeyDown(Keyboard.KEY_W)){
			character.setVelocity(new Coordinate(character.getVelocity().X, -character.moveSpeed));
		}
		if(i.isKeyDown(Keyboard.KEY_A)){			
			character.setVelocity(new Coordinate(-character.moveSpeed, character.getVelocity().Y));
		}
		if(i.isKeyDown(Keyboard.KEY_S)){			
			character.setVelocity(new Coordinate(character.getVelocity().X, character.moveSpeed));
		}
		if(i.isKeyDown(Keyboard.KEY_D)){			
			character.setVelocity(new Coordinate(character.moveSpeed, character.getVelocity().Y));
		}
		if(i.isKeyPressed(Keyboard.KEY_Q)){
			Game.zoom +=5;
		}		
		if(i.isKeyPressed(Keyboard.KEY_R)){
			Game.zoom -=5;
		}
	}	// End update method

	@Override
	public int getID() {
		return state;
	}
}
