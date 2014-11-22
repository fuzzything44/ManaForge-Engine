package main;
/*
 * Pretty much everything for draw needs to be changed-placements need to be fixed, things need to be added, etc.
 * See the section for more notes on what need to be done.
 */


import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import org.lwjgl.input.Keyboard;
import org.newdawn.slick.GameContainer;
import org.newdawn.slick.Graphics;
import org.newdawn.slick.Image;
import org.newdawn.slick.Input;
import org.newdawn.slick.SlickException;
import org.newdawn.slick.state.BasicGameState;
import org.newdawn.slick.state.StateBasedGame;

import buffs.Buff;

public class Play extends BasicGameState {

	int minZoom = 3;
	int maxZoom = 100;
	Vector<WorldChunk> relevantChunks = new Vector<WorldChunk>();
	Buff buff = null;
	
	public final int state;
	public PlayerCharacter character = null;
	public enum rightPaneStates {
		equipment,
		consumables,
		buffs,
		equipmentSpecific,
		consumableSpecific,
		buffSpecific
	}	// What the right pane can display. Specific is the exact buff/consumable/equipment info.
	public int infoPaneSpecific = 1;	// What vector index the specific thing is at.
	public rightPaneStates rightPaneState = rightPaneStates.buffs;
	
	public Play(int statein) {
		state = statein;
	}
	
	@Override
	public void init(GameContainer gc, StateBasedGame sbg)
			throws SlickException {

		
		Game.textures = new HashMap<String, Image>();
		
		Game.textures.put("res/Default.png", new Image("res/Default.png") );
		Game.textures.put("res/Knight.png", new Image("res/Knight.png") );
		Game.textures.put("res/grass.png", new Image("res/grass.png") );

		
		character = new PlayerCharacter("res/Knight.png", new Coordinate(0, 0), 8);
		

		
		for (int i = -100; i < 100; i ++) {
			for (int i1 = -100; i1 < 100; i1++) {
				new Actor("res/grass.png", new Coordinate(i, i1), 0);
			}
		}
		
		Actor a = new Actor(new Coordinate(3, 4) , 2);
		Actor a1 = new Actor(new Coordinate(-5, -7) , 2);
		Actor a2 = new Actor(new Coordinate(-2, 1) , 2);
		Actor a3 = new Actor(new Coordinate(-8, 0) , 2);
		
	}

	@Override
	public void render(GameContainer gc, StateBasedGame sbg, Graphics gr)
			throws SlickException {
		
		Map<String, Image> texturesScaled = new HashMap<String, Image>();
		

		
		int RenderedObjects = 0;
		for(int chunk = 0; chunk < relevantChunks.size(); chunk++){
			for(int i = 0; i < relevantChunks.get(chunk).actors.size(); i++){
				for(int i1 = 0; i1 < relevantChunks.get(chunk).actors.get(i).size(); i1++){
					Actor a = relevantChunks.get(chunk).actors.get(i).get(i1);
					if(a.isRendered){
						int x, y;
						x = (int) (((a.location.X - character.location.X) * Game.zoom) + gc.getWidth()/2);
						y = (int) (((a.location.Y - character.location.Y) * Game.zoom) + gc.getHeight()/2);
						
						if (x > -Game.zoom && y > -Game.zoom && x < gc.getWidth() && y < gc.getHeight() && a.displayImage != null) {
							
							if(!texturesScaled.containsKey(a.displayImage)){
								texturesScaled.put(a.displayImage, Game.textures.get(a.displayImage).getScaledCopy(Game.zoom, Game.zoom) );
							}
							RenderedObjects ++;
							texturesScaled.get(a.displayImage).draw(x, y);
						}
					}
				}
			}
		}
		System.out.println(RenderedObjects);
		
	}	// End render method.

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int delta)
			throws SlickException {
		
		Game.GameTotalTime += delta;
		relevantChunks.clear();

		for(int i = 0; i < Game.world.chunks.size(); i++){
			if(Game.world.chunks.get(i).location.X * Game.zoom > (-Game.zoom * Game.world.ChunkRes.X) && 
					Game.world.chunks.get(i).location.Y * Game.zoom > (-Game.zoom * Game.world.ChunkRes.Y) &&
					Game.world.chunks.get(i).location.X * Game.zoom < gc.getWidth() &&
					Game.world.chunks.get(i).location.Y * Game.zoom < gc.getHeight()){
				relevantChunks.addElement(Game.world.chunks.get(i));
			}
		}
		
		for(int chunk = 0; chunk < relevantChunks.size(); chunk++){
			for (int i = 0; i < relevantChunks.get(chunk).tickingObjects.size(); i++) {
				relevantChunks.get(chunk).tickingObjects.get(i).tick(delta);
			}
		}
		Input i = gc.getInput();
		
		if (i.isKeyPressed(Keyboard.KEY_ESCAPE) ) {
			sbg.enterState(Game.pause);
		}
		
		character.setVelocity(new Coordinate(0) );
		
		if(i.isKeyDown(Keyboard.KEY_W) ) {
			character.setVelocity(new Coordinate(character.getVelocity().X, -character.moveSpeed) );
		}
		if(i.isKeyDown(Keyboard.KEY_A) ) {			
			character.setVelocity(new Coordinate(-character.moveSpeed, character.getVelocity().Y) );
		}
		if(i.isKeyDown(Keyboard.KEY_S) ) {			
			character.setVelocity(new Coordinate(character.getVelocity().X, character.moveSpeed) );
		}
		if(i.isKeyDown(Keyboard.KEY_D) ) {			
			character.setVelocity(new Coordinate(character.moveSpeed, character.getVelocity().Y) );
		}
		if(i.isKeyPressed(Keyboard.KEY_Q) ) {
			if (Game.zoom < maxZoom) {
				Game.zoom *= 2;
			}
		}		
		if(i.isKeyPressed(Keyboard.KEY_R) ) {
			if (Game.zoom > minZoom) {
				Game.zoom *= .5;
			}
		}
		if(i.isKeyPressed(Keyboard.KEY_GRAVE)){
			gc.exit();
		}
	}	// End update method

	@Override
	public int getID() {
		return state;
	}
}
