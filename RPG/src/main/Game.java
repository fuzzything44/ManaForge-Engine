package main;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;
import java.util.Vector;

public class Game extends StateBasedGame {

	static final int play = 0;
	static final int pause = 1;
	public static float GameTotalTime = 0;
	public static Vector<Object> TickingObjects = new Vector<Object>();
	public static float zoom;
	public static Vector<Actor> allActors = new Vector<Actor>();
	
	
	public Game(String name) {
		super(name);
		
		addState(new Play(play));
		addState(new Pause(pause));
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {
		getState(play).init(gc, this);
		getState(pause).init(gc, this);
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
