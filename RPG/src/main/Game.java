package main;

import org.newdawn.slick.AppGameContainer;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.StateBasedGame;

import com.nativelibs4java.opencl.CLContext;
import com.nativelibs4java.opencl.CLKernel;
import com.nativelibs4java.opencl.CLProgram;
import com.nativelibs4java.opencl.CLQueue;
import com.nativelibs4java.opencl.JavaCL;
import com.nativelibs4java.util.IOUtils;

import java.io.IOException;
import java.nio.ByteOrder;
import java.util.Map;

public class Game extends StateBasedGame {

	static final int play = 0;
	static final int pause = 1;
	public static float GameTotalTime = 0;
	
	public static World world = new World();
	
	public static Landscape landscape;
	
	public static int zoomMult = 20;
	public static int zoom = 48;
	static Map<String, Image> textures;
	
	public static CLContext context;
	public static CLQueue queue;
	public static ByteOrder byteOrder;
	public static CLProgram program;
    public static CLKernel collideKern;

	
	public Game(String name) {
		super(name);
		
		addState(new Play(play));
		addState(new Pause(pause));
	}

	@Override
	public void initStatesList(GameContainer gc) throws SlickException {

	    
		try {
			
			context = JavaCL.createBestContext();
		    queue = context.createDefaultQueue();
		    byteOrder = context.getByteOrder();
	        String src = IOUtils.readText(Game.class.getResource("kernels/Collision.cl"));

	        program = context.createProgram(src);
	        
	        collideKern = program.createKernel("collide");
	        
		} catch (IOException e) {
			e.printStackTrace();
			LogCreator.crashLog(e);
			System.exit(1);
		}
		
		getState(pause).init(gc, this);
		enterState(play);
		
	}
	
	public static void main(String args[]) {
		AppGameContainer agc;
		
		try {
			
			agc = new AppGameContainer(new Game("YAY") );
			agc.setDisplayMode(800, 600, false);
			agc.start();
			
			
		} catch (SlickException e) {
			e.printStackTrace();
			LogCreator.crashLog(e);
			System.exit(1);
		}
		
	}
	
	

}
