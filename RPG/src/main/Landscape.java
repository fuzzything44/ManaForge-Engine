package main;

import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class Landscape extends Object{
	
	

	public Landscape (Image image, Map<Color, String> iI, Coordinate origin){
		imageInfo  = iI;
		loadImage = image;
		
	}
	


	public Map<Color, String> imageInfo;
	public Image loadImage;
	
	public String queryPixel(int x, int y){
		Color pixColor = loadImage.getColor(x, y);
		
		return imageInfo.get(pixColor);
		
	}

}
