package main;

import java.util.Vector;

public class WorldChunk {

	public Vector<Object> tickingObjects = new Vector<Object>();
	public Vector<Vector<Actor> > actors = new Vector<Vector<Actor> >();
	
	public Coordinate location;
	
	public WorldChunk(Coordinate locationIn, World world){
		world.chunks.addElement(this);
		
		location = locationIn;
		
		for (int i = 0; i < 10; i++){
			actors.add(new Vector<Actor>());
		}
		
	}
}
