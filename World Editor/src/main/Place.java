package main;

import java.util.LinkedList;
import java.util.Vector;

import org.lwjgl.input.Keyboard;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

public class Place extends BasicGameState {
	public final int state;
	
	public boolean snapToGrid = false;
	// Boolean to determine if it snaps things to the grid.
	enum placeModes {
		background,
		actor,
		delete
	}
	public placeModes placeType = placeModes.background;
	// To determine what you are placing.
	
	
	public Place(int gameState) {
		state = gameState;
	}
	
	@Override
	public void init(GameContainer arg0, StateBasedGame arg1)
			throws SlickException {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		// First we determine relevant chunks.
		Vector<WorldChunk> relevantChunks = new Vector<WorldChunk>();
		relevantChunks.add(Game.workingWorld.getActorChunk(Game.topLeft) );
		// Adds the top left hand corner chunk.
		
		
	}

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int deltaTime)
			throws SlickException {
		if (Game.topLeft.x < 0) {
			// If there is no chunk to the left of the world...
			
			Game.topLeft.x += Game.workingWorld.chunkResX;
			LinkedList<WorldChunk> chunkRow = new LinkedList<WorldChunk>();
			for (int i = 0; i < Game.workingWorld.chunks.getFirst().size(); i++) {
				chunkRow.add(new WorldChunk(Game.workingWorld) );
				// Filling the row of chunks.
			}
			Game.workingWorld.chunks.addFirst(chunkRow);
		}
		
		
		Input input = gc.getInput();
		if (input.isKeyPressed(Keyboard.KEY_G) ) {
			snapToGrid = !snapToGrid;
			// Toggles if it snaps to the grid.
		}
		if (input.isKeyPressed(0) ) {
			// TODO more keyboard shortcuts.
		}
	}

	@Override
	public int getID() {
		return state;
	}
	
}
