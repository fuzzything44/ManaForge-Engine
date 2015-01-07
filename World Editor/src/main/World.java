package main;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;


public class World {
	Map<Coordinate, Image> backgroundImages = new HashMap<Coordinate, Image>();
	
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	
	public World() {
		
	}
	
	public void save(String folderName) {
		Set<Color> usedColors = new HashSet<Color>();
		
		// Create an image here.
		Image image = null;
		for (int x = minX; x < maxX; x++) {
			for (int y = minY; y < maxY; y++) {
				// Add actor to image color.
			}
		}
		
	}
}
