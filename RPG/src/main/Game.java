package main;

import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.opencl.CL;
import org.lwjgl.opencl.CL10;
import org.lwjgl.opencl.CLCommandQueue;
import org.lwjgl.opencl.CLContext;
import org.lwjgl.opencl.CLDevice;
import org.lwjgl.opencl.CLKernel;
import org.lwjgl.opencl.CLPlatform;
import org.lwjgl.opencl.CLProgram;
import org.lwjgl.opencl.Util;
import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;

import java.nio.IntBuffer;
import java.util.List;
import java.util.Map;

public class Game extends StateBasedGame {

	static final int play = 0;
	static final int pause = 1;
	public static float GameTotalTime = 0;

	final int PLATFORM_ID = 0;
	
	public static World world = new World();
	
	public static Landscape landscape;
	
	public static int zoomMult = 20;
	public static int zoom = 48;
	static Map<String, Image> textures;
	
	// OpenCL variables	
	public static CLContext context;
	public static CLPlatform platform;
	public static List<CLDevice> devices;
	public static CLCommandQueue queue;
	public static CLProgram program;	
	public static CLKernel collideKernel;

	
	public Game(String name) throws LWJGLException {
		super(name);
		
		initializeCL();
		
		addState(new Play(play));
		addState(new Pause(pause));
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {

	    

		getState(pause).init(gc, this);
		enterState(play);
		
	}
	
	public static void main(String args[]) {
		AppGameContainer agc;
		
		try {
			
			agc = new AppGameContainer(new Game("RPG Game v.-1.0: Hope you like crashes.") );
			agc.setDisplayMode(800, 600, false);
			agc.start();
			
			
		} catch (SlickException e) {
			e.printStackTrace();
			LogCreator.crashLog(e);
			System.exit(-1);
		} catch (LWJGLException e) {
			e.printStackTrace();
			LogCreator.crashLog(e);
			System.exit(-1);
		}
		
	}

	public void initializeCL() throws LWJGLException { 
		IntBuffer errorBuf = BufferUtils.createIntBuffer(1);
		// Create OpenCL
		CL.create();
		// Get the first available platform
		
		platform = CLPlatform.getPlatforms().get(PLATFORM_ID); 
		
		System.out.println("\nPlatform Name: " + platform.getInfoString(CL10.CL_PLATFORM_NAME));
		System.out.println("Platform Profile: " + platform.getInfoString(CL10.CL_PLATFORM_PROFILE));
		System.out.println("Vendor Name: " + platform.getInfoString(CL10.CL_PLATFORM_VENDOR));
		System.out.println("Version: " + platform.getInfoString(CL10.CL_PLATFORM_VERSION) + "\n");
		
		
		// Run our program on the GPU
		devices = platform.getDevices(CL10.CL_DEVICE_TYPE_ALL);
		
		System.out.println("Name: " + devices.get(0).getInfoString(CL10.CL_DEVICE_NAME));
		System.out.println("Type : " + devices.get(0).getInfoString(CL10.CL_DEVICE_TYPE));
		System.out.println("Avaliable: " + devices.get(0).getInfoBoolean(CL10.CL_DEVICE_AVAILABLE));
		System.out.println("Extensions: " + devices.get(0).getInfoString(CL10.CL_DEVICE_EXTENSIONS));
		System.out.println("Compute Units: " + devices.get(0).getInfoInt(CL10.CL_DEVICE_MAX_COMPUTE_UNITS));
		System.out.println("Execution Capabalities: " + devices.get(0).getInfoString(CL10.CL_DEVICE_EXECUTION_CAPABILITIES));
		
		// Create an OpenCL context, this is where we could create an OpenCL-OpenGL compatible context
		context = CLContext.create(platform, devices, errorBuf);
		// Create a command queue
		queue = CL10.clCreateCommandQueue(context, devices.get(0), CL10.CL_QUEUE_PROFILING_ENABLE, errorBuf);
		// Check for any errors
		Util.checkCLError(errorBuf.get(0)); 
		// Create an OpenCL 'program' from a source code file
		program = CL10.clCreateProgramWithSource(context, Macros.loadText("kernels/Collision.cl"), null);
		// Build the OpenCL program, store it on the specified device
		int error = CL10.clBuildProgram(program, devices.get(0), "", null);
		// Check for any OpenCL errors
		Util.checkCLError(error);
		// Create a kernel instance of our OpenCl program
		collideKernel = CL10.clCreateKernel(program, "collide", null);

	}


	public void destroyCL() {		
		// Destroy our kernel and program
		CL10.clReleaseKernel(collideKernel);
		CL10.clReleaseProgram(program);
		// Destroy the OpenCL context
		destroyCL();
		// Finish destroying anything we created
		CL10.clReleaseCommandQueue(queue);
		CL10.clReleaseContext(context);
		// And release OpenCL, after this method call we cannot use OpenCL unless we re-initialize it
		CL.destroy();
	}

	

}
