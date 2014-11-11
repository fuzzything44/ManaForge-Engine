package main;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;
import java.util.Vector;

public class Game extends StateBasedGame {

	static final int play = 0;
	public static float GameTotalTime = 0;
	
	
	
	public Game(String name) {
		super(name);
		
		addState(new Play(play));
		
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {
		getState(play).init(gc, this);
		enterState(play);
		
	}
	
	public static void main(String args[]) {
		AppGameContainer agc;
		
		try {
			
			agc = new AppGameContainer(new Game("YAY") );
			agc.setDisplayMode(640, 360, false);
			agc.start();
			
		} catch(SlickException e) {
			e.printStackTrace();
		}
		
	}
	
	

}
