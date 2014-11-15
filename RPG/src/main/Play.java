package main;

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
	public PlayerCharacter character = new PlayerCharacter();
	
	
	public Play(int statein) {
		state = statein;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {
		// What is the purpose of this method? 

	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		
		Input i = gc.getInput();
		gr.setColor(Color.white);
		gr.drawString(Float.toString(Game.GameTotalTime/1000), 100, 100);
		
		gr.fillRect(0, 0, 640, 50);				// Fills the top pane.
		gr.fillRect(440, 50, 200, 310);			// Fills the side pane.

		gr.setColor(Color.black);
		gr.drawLine(0, 50, 640, 50);			// Separates top and side panes.
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
		for (int x = 0; x < character.buffs.size(); x++) {	// Automatically displays all buffs you have. At the same place. Whatever.
			gr.drawString(Float.toString(character.buffs.get(x).getTimeLeft() /1000), 100, 20);	// Buff time left in seconds.
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
			
	}	// End update method

	@Override
	public int getID() {
		return state;
	}
}
