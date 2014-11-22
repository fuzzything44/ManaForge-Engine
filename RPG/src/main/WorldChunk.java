package main;

import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Vector<Vector<Actor> > actors = new Vector<Vector<Actor> >();
	
	public Coordinate location;
	
	public WorldChunk(Coordinate location, World world){
		world.chunks.addElement(this);
		
		
		
	}
}
