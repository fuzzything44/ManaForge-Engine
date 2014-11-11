package main;

import org.lwjgl.input.Keyboard;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

public class Pause extends BasicGameState {

	boolean isESCStillDown = false;
	
	public final int state;
	
	
	public Pause(int stateID){
		state = stateID;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbf, Graphics gr)
			throws SlickException {

		gr.drawString("Paused", 200, 200);
		gr.drawString(Float.toString(Game.GameTotalTime), 100, 100);
	}

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int deltaTime)
			throws SlickException {

		if(gc.getInput().isKeyPressed(Keyboard.KEY_ESCAPE)){
			sbg.enterState(Game.play);
		}
		
	}

	@Override
	public int getID() {
		return state;
	}

}
