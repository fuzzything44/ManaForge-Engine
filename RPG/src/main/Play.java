package main;

/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.bridj.Pointer;
import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.PointerBuffer;
import org.lwjgl.input.Keyboard;
import org.lwjgl.opencl.CL;
import org.lwjgl.opencl.CL10;
import org.lwjgl.opencl.CLCommandQueue;
import org.lwjgl.opencl.CLContext;
import org.lwjgl.opencl.CLDevice;
import org.lwjgl.opencl.CLKernel;
import org.lwjgl.opencl.CLMem;
import org.lwjgl.opencl.CLPlatform;
import org.lwjgl.opencl.CLProgram;
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
	
	int staticSize, dynamicSize;
	
	public static CLContext context;
	public static CLPlatform platform;
	public static List<CLDevice> devices;
	public static CLCommandQueue queue;

	
	public Play(int statein) {
		state = statein;
	}

	public static void doSumExample() throws LWJGLException {
		// Create our OpenCL context to run commands
		initializeCL();
		// Create an OpenCL 'program' from a source code file
		CLProgram sumProgram = CL10.clCreateProgramWithSource(context, loadText("kernels/sumKernel.cls"), null);
		// Build the OpenCL program, store it on the specified device
		int error = CL10.clBuildProgram(sumProgram, devices.get(0), "", null);
		// Check for any OpenCL errors
		Util.checkCLError(error);
		// Create a kernel instance of our OpenCl program
		CLKernel sumKernel = CL10.clCreateKernel(sumProgram, "sum", null);

		// Used to determine how many units of work to do
		final int size = 100;
		// Error buffer used to check for OpenCL error that occurred while a command was running
		IntBuffer errorBuff = BufferUtils.createIntBuffer(1);

		// Create our first array of numbers to add to a second array of numbers
		float[] tempData = new float[size];
		for(int i = 0; i < size; i++) {
			tempData[i] = i;
		}
		// Create a buffer containing our array of numbers, we can use the buffer to create an OpenCL memory object
		FloatBuffer aBuff = BufferUtils.createFloatBuffer(size);
		aBuff.put(tempData);
		aBuff.rewind();
		// Create an OpenCL memory object containing a copy of the data buffer
		CLMem aMemory = CL10.clCreateBuffer(context, CL10.CL_MEM_WRITE_ONLY | CL10.CL_MEM_COPY_HOST_PTR, aBuff, errorBuff);
		// Check if the error buffer now contains an error
		Util.checkCLError(errorBuff.get(0));

		// Create our second array of numbers
		for(int j = 0, i = size-1; j < size; j++, i--) {
			tempData[j] = i;
		}
		// Create a buffer containing our second array of numbers
		FloatBuffer bBuff = BufferUtils.createFloatBuffer(size);
		bBuff.put(tempData);
		bBuff.rewind();

		// Create an OpenCL memory object containing a copy of the data buffer
		CLMem bMemory = CL10.clCreateBuffer(context, CL10.CL_MEM_WRITE_ONLY | CL10.CL_MEM_COPY_HOST_PTR, bBuff, errorBuff);
		// Check if the error buffer now contains an error
		Util.checkCLError(errorBuff.get(0));

		// Create an empty OpenCL buffer to store the result of adding the numbers together
		CLMem resultMemory = CL10.clCreateBuffer(context, CL10.CL_MEM_READ_ONLY, size*4, errorBuff);
		// Check for any error creating the memory buffer
		Util.checkCLError(errorBuff.get(0));

		// Set the kernel parameters
		sumKernel.setArg(0, aMemory);
		sumKernel.setArg(1, bMemory);
		sumKernel.setArg(2, resultMemory);
		sumKernel.setArg(3, size);

		// Create a buffer of pointers defining the multi-dimensional size of the number of work units to execute
		final int dimensions = 1; 
		PointerBuffer globalWorkSize = BufferUtils.createPointerBuffer(dimensions);
		globalWorkSize.put(0, size);
		// Run the specified number of work units using our OpenCL program kernel
		CL10.clEnqueueNDRangeKernel(queue, sumKernel, dimensions, null, globalWorkSize, null, null, null);
		CL10.clFinish(queue);

		//This reads the result memory buffer
		FloatBuffer resultBuff = BufferUtils.createFloatBuffer(size);
		// We read the buffer in blocking mode so that when the method returns we know that the result buffer is full
		CL10.clEnqueueReadBuffer(queue, resultMemory, CL10.CL_TRUE, 0, resultBuff, null, null);
		// Print the values in the result buffer
		for(int i = 0; i < resultBuff.capacity(); i++) {
			System.out.println("result at " + i + " = " + resultBuff.get(i));
		}
		// This should print out 100 lines of result floats, each being 99.

		// Destroy our kernel and program
		CL10.clReleaseKernel(sumKernel);
		CL10.clReleaseProgram(sumProgram);
		// Destroy our memory objects
		CL10.clReleaseMemObject(aMemory);
		CL10.clReleaseMemObject(bMemory);
		CL10.clReleaseMemObject(resultMemory);
		// Destroy the OpenCL context
		destroyCL();
	}


	public static void initializeCL() throws LWJGLException { 
		IntBuffer errorBuf = BufferUtils.createIntBuffer(1);
		// Create OpenCL
		CL.create();
		// Get the first available platform
		platform = CLPlatform.getPlatforms().get(0); 
		// Run our program on the GPU
		devices = platform.getDevices(CL10.CL_DEVICE_TYPE_GPU);
		// Create an OpenCL context, this is where we could create an OpenCL-OpenGL compatible context
		context = CLContext.create(platform, devices, errorBuf);
		// Create a command queue
		queue = CL10.clCreateCommandQueue(context, devices.get(0), CL10.CL_QUEUE_PROFILING_ENABLE, errorBuf);
		// Check for any errors
		Util.checkCLError(errorBuf.get(0)); 
	}


	public static void destroyCL() {
		// Finish destroying anything we created
		CL10.clReleaseCommandQueue(queue);
		CL10.clReleaseContext(context);
		// And release OpenCL, after this method call we cannot use OpenCL unless we re-initialize it
		CL.destroy();
	}


	public static String loadText(String name) {
		if(!name.endsWith(".cls")) {
			name += ".cls";
		}
		BufferedReader br = null;
		String resultString = null;
		try {
			// Get the file containing the OpenCL kernel source code
			File clSourceFile = new File(Play.class.getClassLoader().getResource(name).toURI());
			// Create a buffered file reader for the source file
			br = new BufferedReader(new FileReader(clSourceFile));
			// Read the file's source code line by line and store it in a string builder
			String line = null;
			StringBuilder result = new StringBuilder();
			while((line = br.readLine()) != null) {
				result.append(line);
				result.append("\n");
			}
			// Convert the string builder into a string containing the source code to return
			resultString = result.toString();
		} catch(NullPointerException npe) {
			// If there is an error finding the file
			System.err.println("Error retrieving OpenCL source file: ");
			npe.printStackTrace();
		} catch(URISyntaxException urie) {
			// If there is an error converting the file name into a URI
			System.err.println("Error converting file name into URI: ");
			urie.printStackTrace();
		} catch(IOException ioe) {
			// If there is an IO error while reading the file
			System.err.println("Error reading OpenCL source file: ");
			ioe.printStackTrace();
		} finally {
			// Finally clean up any open resources
			try {
				br.close();
			} catch (IOException ex) {
				// If there is an error closing the file after we are done reading from it
				System.err.println("Error closing OpenCL source file");
				ex.printStackTrace();
			}
		}

		// Return the string read from the OpenCL kernel source code file
		return resultString;
	}
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {

		Game.textures = new HashMap<String, Image>();

		Game.textures.put("res/default.png", new Image("res/default.png"));
		Game.textures.put("res/Knight.png", new Image("res/Knight.png"));
		Game.textures.put("res/grass.png", new Image("res/grass.png"));
		Game.textures.put("res/dirt.png", new Image("res/dirt.png"));
		Game.textures.put("res/tree.png", new Image("res/tree.png"));

		Game.world.load("src/res/w1/main.world", new Image("/src/res/w1/TestLandscape.png") );
		
		character = new PlayerCharacter(9, "res/Knight.png", new Coordinate(0, 0));	
		character.isPersistent = true;
		character.refreshChunk();
		
		dynamicActors = new Vector<DynamicActor>();
		
		DynamicActor build = new DynamicActor(2, "res/Knight.png", new Coordinate(5, 5));
		
		build.collides = true;
		build.isPersistent = true;
		build.refreshChunk();
		
		Actor build1= new Actor(2, "res/Knight.png", new Coordinate(7, 5));
		
		build1.collides = true;
		build1.isPersistent = true;
		build1.refreshChunk();
		
		try {
			doSumExample();
		} catch (LWJGLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
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
