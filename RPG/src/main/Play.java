package main;

/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.bridj.Pointer;
import org.lwjgl.input.Keyboard;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import com.nativelibs4java.opencl.CLBuffer;
import com.nativelibs4java.opencl.CLEvent;
import com.nativelibs4java.opencl.CLMem;

import actors.Actor;
import actors.DynamicActor;
import actors.PlayerCharacter;


public class Play extends BasicGameState {

	final int MIN_ZOOM = 1;
	final int MAX_ZOOM = 96;
	Vector<WorldChunk> relevantChunks = new Vector<WorldChunk>();
	Vector<Vector<Actor> > relevantActors = new Vector<Vector<Actor> >();

	public final int state;
	public PlayerCharacter character = null;

	public enum rightPaneStates {
		equipment, consumables, buffs, equipmentSpecific, consumableSpecific, buffSpecific
	} 	// What the right pane can display. Specific is the exact
		// buff/consumable/equipment info.

	public int infoPaneSpecific = 1; // What vector index the specific thing is
										// at.
	public rightPaneStates rightPaneState = rightPaneStates.buffs;

	public Vector<DynamicActor> dynamicActors;
	
	Pointer<Float> h_static, h_dynamic;
	Pointer<Short> h_out;
	CLBuffer<Float> d_static, d_dynamic;
	CLBuffer<Short> d_out;
	
	int staticSize, dynamicSize;
	
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
		Game.textures.put("res/dirt.png", new Image("res/dirt.png"));
		Game.textures.put("res/Tree.png", new Image("res/Tree.png"));

		Game.world.load("src/res/w1/main.world", new Image("/src/res/w1/TestLandscape.png") );
		
		character = new PlayerCharacter(9, "res/Knight.png", new Coordinate(0, 0));	
		character.isPersistent = true;
		character.refreshChunk();
		
		dynamicActors = new Vector<DynamicActor>();
		
		// Stored in AoS format : XYXY
		// Syntatx - h means host/CPU, d means device/gpu
		
		// allocate CPU memory
		h_out = Pointer.allocateShorts(100).order(Game.byteOrder);
		h_static = Pointer.allocateFloats(100).order(Game.byteOrder);
		h_dynamic = Pointer.allocateFloats(100).order(Game.byteOrder);
		
		   
		// allocate GPU memory
		d_out = Game.context.createShortBuffer(CLMem.Usage.Output, 100);
		d_static = Game.context.createFloatBuffer(CLMem.Usage.Input, 100);
		d_dynamic = Game.context.createFloatBuffer(CLMem.Usage.Input, 100);
		   
		
		DynamicActor build = new DynamicActor(2, "res/Knight.png", new Coordinate(5, 5));
		
		build.collides = true;
		build.isPersistent = true;
		build.refreshChunk();
		
		Actor build1= new Actor(2, "res/Knight.png", new Coordinate(7, 5));
		
		build1.collides = true;
		build1.isPersistent = true;
		build1.refreshChunk();
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		

		staticSize = 0;
		dynamicSize = 0;
		dynamicActors.clear();
		
		Map<String, Image> texturesScaled = new HashMap<String, Image>();
		
		
		for(int i = 0; i < relevantChunks.size(); i++) {
			for(int i1 = 0; i1 < relevantChunks.get(i).actors.size(); i1++) {
				parseChunk(relevantChunks.get(i).actors.get(i1) );
			}
		}
		
		
		int RenderedObjects = 0;
		for (int i = 0; i < relevantActors.size(); i++) {
			for (int i1 = 0; i1 < relevantActors.get(i).size(); i1++) {
				Actor a = relevantActors.get(i).get(i1);
				if (a.isRendered) {
					int x, y;
					x = (int) ( ( (a.location.X - character.location.X) * Game.zoom) + gc.getWidth() / 2);
					y = (int) ( ( (a.location.Y - character.location.Y) * Game.zoom) + gc.getHeight() / 2);

					if (x > -Game.zoom && y > -Game.zoom && x < gc.getWidth() && y < gc.getHeight() && a.displayImage != null) {
						// if this is on the screen... 
						
						if(!texturesScaled.containsKey(a.displayImage) ) {
							texturesScaled.put(a.displayImage, Game.textures.get(a.displayImage).getScaledCopy(Game.zoom, Game.zoom) );
							// If the image has no scaled copy yet.
						}
						RenderedObjects ++;
						texturesScaled.get(a.displayImage).draw(x, y);
						// Draw the scaled image at correct position.
						
					}	// End if on screen.
					
					// check for collision
					if(a.collides) {
						// if it collides, add to the check for collison Vector
						
						if(a instanceof DynamicActor) {
							// add to dynamic array
							h_dynamic.set(++dynamicSize, a.location.X);
							h_dynamic.set(dynamicSize, a.location.Y);
							dynamicActors.add((DynamicActor)(a));
						} else {
							h_static.set(++staticSize, a.location.X);
							h_static.set(staticSize, a.location.Y);
						}
					}
					
				}	// End render if.
			}	// End render order loop.
		}	// End relevantActors loop.
			
		/* Collision kernel: This is how it works
		 * 
		 * spawns a 2d array of all actors with collison on X and all dynamic actors with collison on 
		 * Y because all static actors don't need to collide with other static actors
		 * 
		 * 		D	Y	N	A	M	I	C		A	C	T	O	R	S			S	T	A	T	I	C		A	C	T	O	R	S
		 *   D	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   Y	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   N	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   A	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   M	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   I	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   C	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   A	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   C	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   T	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   O	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   R	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   S	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|	|
		 *   
		 *   Each square decides if it needs to collide and stores it to the out buffer
		 */
	
		
		// check if dynamic actors is Zero
		if(dynamicSize > 0){
			
			// copy memory over
			d_static.write(Game.queue, h_static, true);
			d_dynamic.write(Game.queue, h_static, true);
			
			// define event for completion
			CLEvent kernelCompletion;
			
			// set arguments for the collide kernel
			Game.collideKern.setArgs(d_static, d_dynamic, d_out, staticSize, dynamicSize);
			
			
			// call the kernel and store the event to kernelcompletion
			kernelCompletion = Game.collideKern.enqueueNDRange(Game.queue, new int[]{dynamicSize + staticSize, dynamicSize});
			
			// wait for kernel completion
			kernelCompletion.waitFor();
			
			d_out.read(Game.queue, h_out, true);

			for(int i = 0; i < dynamicSize; i++) {
				if(h_out.get(i) == 1){
					dynamicActors.get(i).setVelocity(new Coordinate(0, 0));
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
	
	void parseChunk(Actor a){
		relevantActors.get(a.renderOrder).add(a);
	}
	

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {

		Game.zoom = Game.zoomMult * gc.getHeight()/1000;
		
		Game.GameTotalTime += delta;
		relevantChunks.removeAllElements();
		relevantChunks.add(Game.world.persistentChunk);
		
		int chunkX = (int) Math.floor(character.location.X / Game.world.chunkRes.X);
		int chunkY = (int) Math.floor(character.location.Y / Game.world.chunkRes.Y);
		if (chunkX < 0) {
			chunkX = 0;
		}
		if (chunkY < 0) {
			chunkY = 0;
		}
		if (chunkX >= Game.world.chunks.length - 1) {
			chunkX = Game.world.chunks.length - 2;
		}
		if (chunkY >= Game.world.chunks[chunkX].length - 1) {
			chunkY = Game.world.chunks[chunkX].length - 2;
		}
		relevantChunks.add(Game.world.chunks[chunkX][chunkY]);
		relevantChunks.add(Game.world.chunks[chunkX + 1][chunkY]);
		relevantChunks.add(Game.world.chunks[chunkX][chunkY + 1]);
		relevantChunks.add(Game.world.chunks[chunkX + 1][chunkY + 1]);
		
		relevantActors.clear();
		for(int i = 0; i < 10; i++) {
			relevantActors.add(new Vector<Actor>());
		}
		// Resetting relevantActors to be a Vector of an actor 
		
		for (int i = 0; i < relevantChunks.size(); i++) {
			for (int x = 0; x < relevantChunks.get(i).actors.size(); x++) {
				if (true) {
					// TODO get actor relevancy equations here.
					relevantActors.get(relevantChunks.get(i).actors.get(x).renderOrder).add(relevantChunks.get(i).actors.get(x) );
				}
			}
		}
		// Adding ALL actors to be relevant (if they are in a relevant chunk).
		
/*	
 * Coordinate[] testCoordinates = new Coordinate[Game.world.chunks.keySet().size()];
 
		
		// Creates a Coordinate array of the keys of the world map.
		
		Game.world.chunks.keySet().toArray(testCoordinates);
		for (int i = 0; i < Game.world.chunks.size(); i++) {
			int screenWidth = gc.getWidth();
			int screenHeight = gc.getHeight();
			if (	testCoordinates[i].X * Game.world.ChunkRes.X - character.location.X - .2 * Game.world.ChunkRes.X < screenWidth / (2 * Game.zoom) &&
					testCoordinates[i].Y * Game.world.ChunkRes.Y - character.location.Y - .2 * Game.world.ChunkRes.Y < screenHeight / (2 * Game.zoom) &&
					testCoordinates[i].X * Game.world.ChunkRes.X - character.location.X + 1.2 * Game.world.ChunkRes.X > -screenWidth / (2 * Game.zoom) &&
					testCoordinates[i].Y * Game.world.ChunkRes.Y - character.location.Y + 1.2 * Game.world.ChunkRes.Y > -screenHeight / (2 * Game.zoom) 
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
				 *  testCoordinates[i] gives the coordinate for the top left hand corner of the chunk.
				 *  
				 *  The chunk coordinate (in actors, not chunks) - the character coordinate gives us how many actors the chunk is away from the character.
				 *  	screenSize/(2 * Game.zoom) gives us the amount of actors between the character and the side of the screen
				 *  	(screenSize/2 for pixels to side, / by Game.zoom to get that in actors to the side (Game.zoom is pixels/actor)
				 *  This gets us the basic equations:
				 *  	testCoordinates[i] * Game.world.ChunkRes - character.location < screenSize / (2 * Game.zoom)
				 *  	testCoordinates[i] * Game.world.ChunkRes - character.location > -screenSize / (2 * Game.zoom)
				 *  
				 *  The multiple of Game.world.ChunkRes is because we want the chunk to be visible if any part of it is on the screen.
				 *  	(Only relevant for lower bounds)
				 *  The .2 is because for some reason, chunks weren't loading instantly, so we had a slight amount of blackness around the edges.
				 *  
				 *  Basically, we need to check if the upper left hand corner of the chunk is more than one chunk away from the screen. (Or just off the screen for lower and right hand bounds)
				 *
				relevantChunks.addElement(Game.world.chunks.get(testCoordinates[i]) );
				
			} // End if
					
		}	// End for

		int minX = 0;
		int maxX = 10;
		int minY = 0;
		int maxY = 10;
		// TODO new min/max equations.
		for (int x = minX; x < maxX; x++) {
			for (int y = minY; y < maxY; y++) {
				relevantChunks.addElement(Game.world.chunks[x][y]);
			}
		}
		for (int chunk = 0; chunk < relevantChunks.size(); chunk++) {
			for (int i1 = 0; i1 < relevantChunks.get(chunk).tickingObjects.size(); i1++) {
				relevantChunks.get(chunk).tickingObjects.get(i1).tick(delta);
			}
		}*/
	//	relevantChunks = Game.world.chunks[i];

		for (int i = 0; i < relevantChunks.size(); i++) {
			// Looping through relevant chunks to get all ticking actors
			for (int x = 0; x < relevantChunks.get(i).tickingObjects.size(); x++) {
				relevantChunks.get(i).tickingObjects.get(x).tick(delta);
			}
		}
		// Loops to tick each ticking object.
		
		
		Input i1 = gc.getInput();

		if (i1.isKeyPressed(Keyboard.KEY_ESCAPE)) {
			sbg.enterState(Game.pause);
		}

		character.setVelocity(new Coordinate(0, 0) );

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
			if (Game.zoom < MAX_ZOOM) {
				Game.zoomMult *= 2;
			}
		}
		if (i1.isKeyPressed(Keyboard.KEY_R)) {
			if (Game.zoom > MIN_ZOOM) {
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
