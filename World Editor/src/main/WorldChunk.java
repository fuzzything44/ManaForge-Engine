package main;

import java.util.Vector;

import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;

public class WorldChunk {
	public World world;
	public Vector<Actor> actors;
	public Image[][] background;
	
	public WorldChunk(World inWorld) {
		world = inWorld;
		background = new Image[world.chunkResX][world.chunkResY];
		for (int x = 0; x < background.length; x++) {
			for (int y = 0; y < background[x].length; y++) {
				try {
					background[x][y] = new Image("res/default.png");
				} catch (SlickException e) { e.printStackTrace(); }
			}
		}
		
		actors = new Vector<Actor>();
	}
	
	public void addActor(Actor a) {
		actors.addElement(a);
	}
	
}
