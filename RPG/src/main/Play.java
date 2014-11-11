package main;

import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;
import java.util.Vector;

public class Play extends BasicGameState {

	public final int state;
	
	public Play(int statein){
		state=statein;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {


	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {


		gr.drawString(Float.toString(Game.GameTotalTime), 50, 50);
		
	}

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {
		
		Game.GameTotalTime += delta;
		
		for(int i = 0; i < Game.TickingObjects.size(); i++){
			Game.TickingObjects
		}
		
	}

	@Override
	public int getID() {
		
		return state;
	}
	


}
