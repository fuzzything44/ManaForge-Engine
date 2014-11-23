package main;

/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.lwjgl.input.Keyboard;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import buffs.Buff;

public class Play extends BasicGameState {

	int minZoom = 12;
	int maxZoom = 96;
	Vector<WorldChunk> relevantChunks = new Vector<WorldChunk>();
	Buff buff = null;

	public final int state;
	public PlayerCharacter character = null;

	public enum rightPaneStates {
		equipment, consumables, buffs, equipmentSpecific, consumableSpecific, buffSpecific
	} // What the right pane can display. Specific is the exact
		// buff/consumable/equipment info.

	public int infoPaneSpecific = 1; // What vector index the specific thing is
										// at.
	public rightPaneStates rightPaneState = rightPaneStates.buffs;

	public Play(int statein) {
		state = statein;
	}

	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {

		Game.textures = new HashMap<String, Image>();

		Game.textures.put("res/Default.png", new Image("res/Default.png"));
		Game.textures.put("res/Knight.png", new Image("res/Knight.png"));
		Game.textures.put("res/grass.png", new Image("res/grass.png"));

		character = new PlayerCharacter("res/Knight.png", new Coordinate(0, 0),
				9);

		for (int chunkX = -5; chunkX < 5; chunkX++) {
			for (int chunkY = -5; chunkY < 5; chunkY++) {
				WorldChunk buildChunk = new WorldChunk(new Coordinate(chunkX,
						chunkY), Game.world);
				for (int actorX = 0; actorX < Game.world.ChunkRes.X; actorX++) {
					for (int actorY = 0; actorY < Game.world.ChunkRes.Y; actorY++) {
						if ( (actorX == Game.world.ChunkRes.Y - 1) || (actorX == 0) || (actorY == 0) || (actorY == Game.world.ChunkRes.Y - 1)) {
							// if at the edge (edge of a chunk is x or y being 0 or the final part of the for loop, which is chunkres - 1
							new Actor ("res/Default.png", new Coordinate (actorX, actorY), 0, buildChunk);
						} else {
						new Actor("res/grass.png", new Coordinate(actorX,
								actorY), 0, buildChunk);
						}
					}
				}
			}
		}

	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		
		Map<String, Image> texturesScaled = new HashMap<String, Image>();
				
		Vector<Vector<Actor> > relevantActors = new Vector<Vector<Actor> >();
		for(int i = 0; i < 10; i++){
			relevantActors.add(new Vector<Actor>());
		}
		
		for(int i = 0; i < relevantChunks.size(); i++) {
			for (int i1 = 0; i1 < relevantChunks.get(i).actors.size(); i1++) {
				for (int i2 = 0; i2 < relevantChunks.get(i).actors.get(i1).size(); i2++) {
					relevantActors.get(i1).add(relevantChunks.get(i).actors.get(i1).get(i2) );
				}
			}
		}
		
		int RenderedObjects = 0;
			for(int i = 0; i < relevantActors.size(); i++){
				for(int i1 = 0; i1 < relevantActors.get(i).size(); i1++){
					Actor a = relevantActors.get(i).get(i1);
					if(a.isRendered){
						int x, y;
						x = (int) (((a.location.X - character.location.X + (a.chunk.location.X * Game.world.ChunkRes.X)) * Game.zoom) + gc.getWidth()/2);
						y = (int) (((a.location.Y - character.location.Y + (a.chunk.location.Y * Game.world.ChunkRes.Y)) * Game.zoom) + gc.getHeight()/2);
						
						if (x > -Game.zoom && y > -Game.zoom && x < gc.getWidth() && y < gc.getHeight() && a.displayImage != null) {
							// if this is on the screen... 
							
							if(!texturesScaled.containsKey(a.displayImage)){
								texturesScaled.put(a.displayImage, Game.textures.get(a.displayImage).getScaledCopy(Game.zoom, Game.zoom) );
							}
							RenderedObjects ++;
							texturesScaled.get(a.displayImage).draw(x, y);
						}
					}
				}
			}
		gr.setColor(Color.black);
		gr.fillRect(0, 0, 1000, 60);
		gr.setColor(Color.white);
		gr.drawString("Rendered objects: " + RenderedObjects, 0, 20);
		gr.drawString("Character location: " + character.location.X + ", " + character.location.Y, 0, 0);
		gr.drawString("Chunks loaded: " + relevantChunks.size() , 0, 35);
		gr.drawString("Zoom level: " + Game.zoom + ", Zoom multiplier: " + Game.zoomMult, 400, 0);
		
		// drawing all the random info we might want on the screen.
		
	}	// End render method.

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {

		Game.zoom = Game.zoomMult * gc.getHeight()/1000;
		
		Game.GameTotalTime += delta;
		relevantChunks.removeAllElements();
		relevantChunks.add(Game.world.persistentChunk);

		
		for (int i = 0; i < Game.world.chunks.size(); i++) {
			WorldChunk testingChunk = Game.world.chunks.get(i);
			int screenWidth = gc.getWidth();
			int screenHeight = gc.getHeight();
			if (	testingChunk.location.X * Game.world.ChunkRes.X - character.location.X - .2 * Game.world.ChunkRes.X < screenWidth / (2 * Game.zoom) &&
					testingChunk.location.Y * Game.world.ChunkRes.Y - character.location.Y - .2 * Game.world.ChunkRes.Y < screenHeight / (2 * Game.zoom) &&
					testingChunk.location.X * Game.world.ChunkRes.X - character.location.X + 1.2 * Game.world.ChunkRes.X > -screenWidth / (2 * Game.zoom) &&
					testingChunk.location.Y * Game.world.ChunkRes.Y - character.location.Y + 1.2 * Game.world.ChunkRes.Y > -screenHeight / (2 * Game.zoom) 
					) {
				/*	.	+-----------------------------------------+
				 * 	.	|                   |                     |
				 * 	.	|                   |                     |
				 * 	.	|                   Y                     |
				 * 	.	|                   |                     |
				 * 	L	|                   |                     |
				 * 	.	|.........X.........C..........X..........|
				 * 	.	|                   |                     |
				 * 	.	|                   |                     |
				 * 	.	|                   Y                     |
				 * 	.	|                   |                     |
				 *  .	|                   |                     |
				 * 	.	+-----------------------------------------+
				 * 
				 *  C is player character. In the center of the world.
				 *  Y is the distance from the character to the top or bottom of the screen.
				 *  X is the distance from the character to the side of the world.
				 *  
				 *  testingChunk.location gives the coordinate for the top left hand corner of the chunk.
				 *  
				 *  The chunk coordinate (in actors, not chunks) - the character coordinate gives us how many actors the chunk is away from the character.
				 *  	screenSize/(2 * Game.zoom) gives us the amount of actors between the character and the side of the screen
				 *  	(screenSize/2 for pixels to side, / by Game.zoom to get that in actors to the side (Game.zoom is pixels/actor)
				 *  This gets us the basic equations:
				 *  	testingChunk.location * Game.world.ChunkRes - character.location < screenSize / (2 * Game.zoom)
				 *  	testingChunk.location * Game.world.ChunkRes - character.location > -screenSize / (2 * Game.zoom)
				 *  
				 *  The multiple of Game.world.ChunkRes is because we want the chunk to be visible if any part of it is on the screen.
				 *  	(Only relevant for lower bounds)
				 *  The .2 is because for some reason, chunks weren't loading instantly, so we had a slight amount of blackness around the edges.
				 *  
				 *  Basically, we need to check if the upper left hand corner of the chunk is more than one chunk away from the screen. (Or just off the screen for lower and right hand bounds)
				 */
				relevantChunks.addElement(testingChunk);
			} 
					
		}
		for (int chunk = 0; chunk < relevantChunks.size(); chunk++) {
			for (int i1 = 0; i1 < relevantChunks.get(chunk).tickingObjects.size(); i1++) {
				relevantChunks.get(chunk).tickingObjects.get(i1).tick(delta);
			}
		}
		
		Input i1 = gc.getInput();

		if (i1.isKeyPressed(Keyboard.KEY_ESCAPE)) {
			sbg.enterState(Game.pause);
		}

		character.setVelocity(new Coordinate(0));

		if (i1.isKeyDown(Keyboard.KEY_W)) {
			character.setVelocity(new Coordinate(character.getVelocity().X,
					-character.moveSpeed));
		}
		if (i1.isKeyDown(Keyboard.KEY_A)) {
			character.setVelocity(new Coordinate(-character.moveSpeed,
					character.getVelocity().Y));
		}
		if (i1.isKeyDown(Keyboard.KEY_S)) {
			character.setVelocity(new Coordinate(character.getVelocity().X,
					character.moveSpeed));
			if (i1.isKeyDown(Keyboard.KEY_W) ) {
				character.setVelocity(new Coordinate(character.getVelocity().X, 0) );
			}
		}
		if (i1.isKeyDown(Keyboard.KEY_D)) {
			character.setVelocity(new Coordinate(character.moveSpeed, character
					.getVelocity().Y));
			if (i1.isKeyDown(Keyboard.KEY_A) ) {
				character.setVelocity(new Coordinate(0,character.getVelocity().Y));
			}
		}
		if (i1.isKeyPressed(Keyboard.KEY_Q)) {
			if (Game.zoom < maxZoom) {
				Game.zoomMult *= 2;
			}
		}
		if (i1.isKeyPressed(Keyboard.KEY_R)) {
			if (Game.zoom > minZoom) {
				Game.zoomMult *= .5;
			}
		}
		if (i1.isKeyPressed(Keyboard.KEY_GRAVE)) {
			gc.exit();
		}
	} // End update method

	@Override
	public int getID() {
		return state;
	}
}
