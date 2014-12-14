package main;

import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class Landscape extends Object{
	
	

	public Landscape (Image image, Map<Color, String> iI, Coordinate origin){
		imageInfo  = iI;
		loadImage = image;
		
	}
	
	// load an image into chunks -- it is faster than doing a query every frame but it is static.
	public static void loadIntoChunks(World world, Image image, Map<Color, String> imageData, Coordinate origin) {
		for(int imageX = 0; imageX < image.getWidth(); imageX++) {
			
			for(int imageY = 0; imageY < image.getHeight(); imageY++) {
				Actor a = Macros.actor(imageData.get(image.getColor(imageX, imageY)), new Coordinate (imageX, imageY) );
				world.addActor(a);
			}
			
		}
	}

	public Map<Color, String> imageInfo;
	public Image loadImage;
	
	public String queryPixel(int x, int y){
		Color pixColor = loadImage.getColor(x, y);
		
		return imageInfo.get(pixColor);
		
	}

}
