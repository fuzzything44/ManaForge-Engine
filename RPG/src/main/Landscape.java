package main;

import java.util.HashMap;
import java.util.Map;

import org.newdawn.slick.Color;
import org.newdawn.slick.Image;

public class Landscape extends Object{
	
	
	public Landscape(Image image){
		loadImage = image;
		imageInfo = new HashMap<Color, String>();
		
	}
	
	public Landscape (Image image, Map<Color, String> iI){
		imageInfo  = iI;
		loadImage = image;
	}
	
	public Landscape (Map<Color, String> iI){
		imageInfo = iI;
	}

	public Map<Color, String> imageInfo;
	public Image loadImage;
	
	public String queryPixel(int x, int y){
		Color pixColor = loadImage.getColor(x, y);
		
		return imageInfo.get(pixColor);
		
	}

}
