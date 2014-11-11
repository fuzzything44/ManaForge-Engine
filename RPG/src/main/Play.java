package main;

import org.lwjgl.input.Keyboard;
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
		
		Input i = gc.getInput();
		
		gr.drawString(Float.toString(Game.GameTotalTime), 100, 100);
		
		
		if(i.isKeyPressed(Keyboard.KEY_E)){
			
			Buff.BuffValues vals = null;
			
			vals.buffTimeLeft = 2000;
			
			buff = new Buff("Buff1", vals, character);
		} 
		if(buff != null){
			
			gr.drawString(Float.toString(buff.getTimeLeft()), 300, 300);
			
		}
		
	}

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {
		
		Game.GameTotalTime += delta;
	
		
		for(int i = 0; i < Game.TickingObjects.size(); i++){
			Game.TickingObjects.get(i).tick(delta);
		}
		
		Input i = gc.getInput();
		
		if(i.isKeyPressed(Keyboard.KEY_ESCAPE)){
			sbg.enterState(Game.pause);
		}
		

		
	}

	@Override
	public int getID() {
		
		return state;
	}
	


}
