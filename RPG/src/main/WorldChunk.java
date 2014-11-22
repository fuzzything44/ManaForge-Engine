package main;

import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Vector<Vector<Actor> > actors = new Vector<Vector<Actor> >();
	
	public Coordinate location;
	
	public WorldChunk(Coordinate location, World world){
		world.chunks.addElement(this);
		
		for (int i = 0; i < 10; i++){
			actors.add(new Vector<Actor>());
		}
		
	}
}
