package main;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;

import java.util.Map;
import java.util.Vector;

public class Game extends StateBasedGame {

	static final int play = 0;
	static final int pause = 1;
	public static float GameTotalTime = 0;
//	public static Vector<Object> TickingObjects = new Vector<Object>();
	
	
	public static World world = new World();
	
	public static WorldChunk persistantChunk = new WorldChunk(new Coordinate(0, 0), world);
	
	public static int zoom = 20;
	public static int maxZoom = 55;
	public static int minZoom = 15;
//	public static Vector<Vector<Actor> > allActors = new Vector<Vector<Actor> >();

	static Map<String, Image> textures;
	
	public Game(String name) {
		super(name);
		
		addState(new Play(play));
		addState(new Pause(pause));
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {
		for (int x = 0; x < 9; x++) {
			allActors.add(new Vector<Actor>() );
		}
		System.out.println(allActors.size());
		getState(pause).init(gc, this);
		enterState(play);
		
	}
	
	public static void main(String args[]) {
		AppGameContainer agc;
		
		try {
			
			agc = new AppGameContainer(new Game("YAY") );
			agc.setDisplayMode(800, 600, false);
			agc.start();
			
			
		} catch(SlickException e) {
			e.printStackTrace();
		}
		
	}
	
	

}
