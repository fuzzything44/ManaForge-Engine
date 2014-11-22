package main;

import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;


/* The base class for anything placeable in the world
 * 
 */


public class Actor extends Object {
	public boolean doesCollide = false;
	
	public Actor(int renderOrder) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.get(renderOrder).add(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
	}

	public Actor(Coordinate place, int renderOrder) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.get(renderOrder).add(this);
		}
		displayImage = "res/Default.png";
	
		renderOrder = 1;
		
		location = place;
		
	}	
	
	public Actor(String image, int renderOrder) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.get(renderOrder);/*.add(this);*/
		}
		displayImage = image;
	
		renderOrder = 1;
	}
	public Actor(String image, Coordinate place, int renderOrder) {
		
		if(!Game.allActors.contains(this)){
		
			Game.allActors.get(renderOrder).add(this);
		}
		displayImage = image;
	
		this.renderOrder = renderOrder;
		
		location = place;
	}
	
	public void changeRenderOrder(int newRenderOrder) {
		Game.allActors.get(renderOrder).remove(this);
		Game.allActors.get(newRenderOrder).add(this);
	}
	 
	public String displayImage;
	
	public Coordinate location = new Coordinate(0);
	
	public boolean isRendered = true;
	
	// 0 - 9
	public int renderOrder;
}
