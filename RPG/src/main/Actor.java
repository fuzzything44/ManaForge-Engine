package main;

/* The base class for anything placeable in the world
 * 
 */



public class Actor extends Object {
	
	public  Coordinate getLocation(){
		return Location;
	}
	
	public void Teleport(Coordinate newLocation){
		Location = newLocation;
		
		// TODO: Add teleport handling stuff
	}
	
	 
	
	
	private Coordinate Location;
}
