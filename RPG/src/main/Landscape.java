package main;

import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;
import org.newdawn.slick.SlickException;

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
			
			for (int imageY = 0; imageY < image.getHeight(); imageY++) {
				if (imageData.get(image.getColor(imageX, imageY) ) != null) {
					world.backgroundImages[imageX][imageY] = imageData.get(image.getColor(imageX, imageY));
				} else {
					// If the color is unknown.
					try {
						world.backgroundImages[imageX][imageY] = new Image("res/default.png");
					} catch (SlickException e) {e.printStackTrace();}
				}// End if/else
			}	// End for
		}		// End for
	}

	public Image queryPixel(int x, int y) {
		Color pixColor = loadImage.getColor(x, y);
		
		return imageInfo.get(pixColor);
		
	}

}
