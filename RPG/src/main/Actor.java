package main;

import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;


/* The base class for anything placeable in the world
 * 
 */


public class Actor extends Object {
	public boolean doesCollide = false;
	
	public Actor() {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.addElement(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
	}

	public Actor(Coordinate place) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.addElement(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		location = place;
		
	}	
	
	public Actor(String image) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.addElement(this);
		}
		displayImage = image;
	
		renderOrder = 1;
	}
	public Actor(String image, Coordinate place) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.addElement(this);
		}
		displayImage = image;
	
		renderOrder = 1;
		
		location = place;
	}
	 
	public String displayImage;
	
	public Coordinate location = new Coordinate(0);
	
	public boolean isRendered = true;
	
	// 0 - 9
	public int renderOrder;
}
