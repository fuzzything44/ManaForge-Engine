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
	
	 
	public Image displayImage;
	
	public Coordinate location = new Coordinate(0);
	
	public boolean isRendered = true;
}
