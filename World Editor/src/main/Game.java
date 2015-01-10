package main;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;

public class Game extends StateBasedGame {
	
	static final int menu = 0;
	static final int place = 1;
	static final int save = 2;
	
	public static Coordinate topLeft = new Coordinate(0,0);
	// Coordinate to determine the top left hand corner of the screen.
	
	public static World workingWorld;
	// The world you are working on. Set in menu.
	
	public Game(String name) {
		super(name);
		addState(new Menu(menu) );
		addState(new Place(place) );
		addState(new Save(save) );
		
	}

	public static void main(String args[]) {
		AppGameContainer agc;
		
		try {
			
			agc = new AppGameContainer(new Game("RPG Game v.-1.0: Hope you like crashes.") );
			agc.setDisplayMode(800, 600, false);
			agc.start();
			
			
		} catch (SlickException e) {
			e.printStackTrace();
			System.exit(-1);
		}
		
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {

		getState(menu).init(gc, this);
		getState(place).init(gc, this);
		getState(save).init(gc, this);
		
		enterState(menu);

	}
} 
