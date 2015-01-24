package main;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.lwjgl.BufferUtils;
import org.lwjgl.PointerBuffer;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opencl.CL10;
import org.lwjgl.opencl.CLMem;
import org.lwjgl.opencl.Util;
import org.newdawn.slick.Color;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import actors.Actor;
import actors.DynamicActor;
import actors.PlayerCharacter;
import actors.Projectile;


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


	public Vector<DynamicActor> dynamicActors;
	
	int staticSize, dynamicSize;
	
	final int DYNAMIC_MAX = 100;
	final int STATIC_MAX = 200;
	
	FloatBuffer d_dynamic;
	FloatBuffer d_static;
	
	float[] h_dynamic;
	float[] h_static;
	
	public Play(int statein) {
		state = statein;
	}

	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {

		Game.textures = new HashMap<String, Image>();

		Game.textures.put("res/default.png", new Image("res/default.png"));
		Game.textures.put("res/knight.png", new Image("res/Knight.png"));
		Game.textures.put("res/grass.png", new Image("res/grass.png"));
		Game.textures.put("res/dirt.png", new Image("res/dirt.png"));
		Game.textures.put("res/tree.png", new Image("res/tree.png"));

		Game.world.load("src/res/w1/main.world", new Image("/src/res/w1/TestLandscape.png") );
		
		character = new PlayerCharacter(9, "res/knight.png", new Coordinate(0, 0));	
		character.isPersistent = true;
		character.refreshChunk();
		
		dynamicActors = new Vector<DynamicActor>();
		
		DynamicActor build = new DynamicActor(2, "res/knight.png", new Coordinate(5, 5));
		
		build.collides = true;
		build.isPersistent = true;
		build.refreshChunk();
		
		Actor build1 = new Actor(2, "res/knight.png", new Coordinate(7, 5) );
		
		build1.collides = true;
		build1.isPersistent = true;
		build1.refreshChunk();
				
		Actor testSpawner = new Actor(2, "res/default.png", new Coordinate(7,7) ) {
			int timeUntilSpawn = 0;
			
			@Override
			public void tick(float deltaTime) {
				System.out.println("Spawner Ticked");
				timeUntilSpawn -= deltaTime;
				if (timeUntilSpawn <= 0) {
					timeUntilSpawn = 1000;
					Projectile projectile = new Projectile(location, new Coordinate(0, 3), 3000, "res/tree.png", this);
					projectile.collides = true;
					projectile.refreshChunk();
				}
			}	// End tick	
		}; 
		// Creating a new hardcoded custom actor to spawn projectiles
		testSpawner.doesTick = true;
		testSpawner.refreshChunk();
		
		d_dynamic = BufferUtils.createFloatBuffer(DYNAMIC_MAX);
		h_dynamic = new float[DYNAMIC_MAX];
		
		d_static = BufferUtils.createFloatBuffer(STATIC_MAX);
		h_static = new float[STATIC_MAX];
		
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		

		staticSize = 0;
		dynamicSize = 0;
		dynamicActors.clear();
		
		Map<String, Image> texturesScaled = new HashMap<String, Image>();
		
		// Variables to see how far left/right and up/down we draw images.
		float startX = (float) (character.location.X - 0.5*gc.getWidth()/Game.zoom - 1) ;
		float endX = (float) (character.location.X + 0.5*gc.getWidth()/Game.zoom );
		float startY = (float) ( character.location.Y - 0.5*gc.getHeight()/Game.zoom - 1 );
		float endY = (float) (character.location.Y + 0.5*gc.getHeight()/Game.zoom );
		for (float x = startX; x < endX + Game.zoom; x++) {
			for (float y = startY; y < endY + Game.zoom; y++) {
				try {
					float drawX = (Math.round(x) - startX - 1) * Game.zoom;
					float drawY = (Math.round(y) - startY - 1) * Game.zoom;
					Game.world.backgroundImages[Math.round(x)][Math.round(y)].getScaledCopy(Game.zoom, Game.zoom).draw(drawX, drawY);
				} catch (ArrayIndexOutOfBoundsException e) {}
				// Looping through and drawing all necessary background images.
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
							texturesScaled.put(
									a.displayImage, 
									Game.textures.
									get(a.displayImage).
									getScaledCopy(Game.zoom, Game.zoom) );
							// If the image has no scaled copy yet.
						}
						RenderedObjects ++;
						texturesScaled.get(a.displayImage).draw(x, y);
						// Draw the scaled image at correct position.
						
					}	// End if on screen.
					
					// check for collision

					
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
	
		// OpenCL tick
		
		// Error buffer used to check for OpenCL error that occurred while a command was running
		IntBuffer errorBuff = BufferUtils.createIntBuffer(1);

		// Create a buffer containing our array of numbers, we can use the buffer to create an OpenCL memory object
		d_dynamic.put(h_dynamic);
		d_dynamic.rewind();
		// Create an OpenCL memory object containing a copy of the data buffer
		CLMem dynamicMem = CL10.clCreateBuffer(Game.context, CL10.CL_MEM_WRITE_ONLY | CL10.CL_MEM_COPY_HOST_PTR, d_dynamic, errorBuff);
		// Check if the error buffer now contains an error
		Util.checkCLError(errorBuff.get(0));

		// Create a buffer containing our second array of numbers
		d_static = BufferUtils.createFloatBuffer(STATIC_MAX);
		d_static.put(h_static);
		d_static.rewind();

		// Create an OpenCL memory object containing a copy of the data buffer
		CLMem staticMem = CL10.clCreateBuffer(Game.context, CL10.CL_MEM_WRITE_ONLY | CL10.CL_MEM_COPY_HOST_PTR, d_static, errorBuff);
		// Check if the error buffer now contains an error
		Util.checkCLError(errorBuff.get(0));

		// Create an empty OpenCL buffer to store the result of adding the numbers together
		CLMem resultMem = CL10.clCreateBuffer(Game.context, CL10.CL_MEM_READ_ONLY, DYNAMIC_MAX * 4, errorBuff);
		// Check for any error creating the memory buffer
		Util.checkCLError(errorBuff.get(0));

		// Set the kernel parameters
		Game.collideKernel.setArg(0, dynamicMem);
		Game.collideKernel.setArg(1, staticMem);
		Game.collideKernel.setArg(2, resultMem);
		Game.collideKernel.setArg(3, STATIC_MAX);
		Game.collideKernel.setArg(4, DYNAMIC_MAX);

		// Create a buffer of pointers defining the multi-dimensional size of the number of work units to execute
		final int dimensions = 2; 
		PointerBuffer globalWorkSize = BufferUtils.createPointerBuffer(dimensions);
		globalWorkSize.put(0, DYNAMIC_MAX + STATIC_MAX);
		globalWorkSize.put(1, DYNAMIC_MAX);
		
		// Run the specified number of work units using our OpenCL program kernel
		CL10.clEnqueueNDRangeKernel(Game.queue, Game.collideKernel, dimensions, null, globalWorkSize, null, null, null);
		CL10.clFinish(Game.queue);

		//This reads the result memory buffer
		FloatBuffer resultBuff = BufferUtils.createFloatBuffer(DYNAMIC_MAX * 4);
		// We read the buffer in blocking mode so that when the method returns we know that the result buffer is full
		CL10.clEnqueueReadBuffer(Game.queue, resultMem, CL10.CL_TRUE, 0, resultBuff, null, null);
		// Print the values in the result buffer
		for(int i = 0; i < resultBuff.capacity(); i++) {
			System.out.println("result at " + i + " = " + resultBuff.get(i));
		}
		// This should print out 100 lines of result floats, each being 99.
		CL10.clReleaseMemObject(resultMem);
		CL10.clReleaseMemObject(staticMem);
		CL10.clReleaseMemObject(dynamicMem);
		
		
		// End OpenCL tick
		
		
	
		gr.setColor(Color.black);
		gr.fillRect(0, 0, 1000, 60);
		gr.setColor(Color.white);
		gr.drawString("Rendered objects: " + RenderedObjects, 0, 20);
		gr.drawString("Character location: " + character.location.X + ", " + character.location.Y, 0, 0);
		gr.drawString("Chunks loaded: " + relevantChunks.size() , 0, 35);
		gr.drawString("Zoom level: " + Game.zoom + ", Zoom multiplier: " + Game.zoomMult, 400, 0);
		
		// drawing all the random info we might want on the screen.
		
	}	// End render method.
	
	void parseChunk(Actor a) {
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
		
		for(int i = 0; i < relevantChunks.size(); i++) {
			for(int i1 = 0; i1 < relevantChunks.get(i).actors.size(); i1++) {
				parseChunk(relevantChunks.get(i).actors.get(i1) );
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
				 * screenSize / 2
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
