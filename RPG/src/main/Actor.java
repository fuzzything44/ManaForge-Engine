package main;

import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;


/* The base class for anything placeable in the world
 * 
 */


public class Actor extends Object {
	
	
	public Actor() {
		Game.allActors.addElement(this);
	
		try{
			displayImage = new Image("res/Default.png");
	
		}catch (SlickException e){
			e.printStackTrace();
		}
	}
	
	public  Coordinate getLocation(){
		return Location;
	}
	
	public void Teleport(Coordinate newLocation){
		Location = newLocation;
		
		// TODO: Add teleport handling stuff
	}
	
	 
	public Image displayImage;
	
	private Coordinate Location = new Coordinate();
	
	public boolean isRendered = true;
}
