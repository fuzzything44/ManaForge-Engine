package main;

import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class Landscape extends Object{
	
	public Map<Color, Image> imageInfo;
	public Image loadImage;
	


	public Landscape (Image image, Map<Color, Image> iI, Coordinate origin) {
		imageInfo  = iI;
		loadImage = image;
		
	}
	
	// load an image into chunks -- it is faster than doing a query every frame but it is static.
	public static void loadIntoWorld(World world, Image image, Map<Color, Image> imageData, Coordinate origin) {
		for(int imageX = 0; imageX < image.getWidth(); imageX++) {
			
			for(int imageY = 0; imageY < image.getHeight(); imageY++) {
				world.backgroundImages[imageX][imageY] = imageData.get(image.getColor(imageX, imageY));
			}
			
		}
	}

	public Image queryPixel(int x, int y) {
		Color pixColor = loadImage.getColor(x, y);
		
		return imageInfo.get(pixColor);
		
	}

}
