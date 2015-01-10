package main;

import java.util.Vector;

import org.newdawn.slick.Image;

public class WorldChunk {
	public Coordinate location;
	public World world;
	public Vector<Actor> actors;
	public Image[][] background;
	
	public WorldChunk(Coordinate location, World inWorld) {
		this.location = location;
		world = inWorld;
		background = new Image[world.chunkResX][world.chunkResY];
		actors = new Vector<Actor>();
	}
	
	public void addActor(Actor a) {
		actors.addElement(a);
	}
	
}
