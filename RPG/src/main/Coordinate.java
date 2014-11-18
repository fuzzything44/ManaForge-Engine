package main;

import java.lang.Math;

/*
 * The class for a basic coordinate. Cannot tick or implement any classes, just a structure for coordinates
 * 
 * also includes many operations for these coordinates. (To be added on)
 */

public class Coordinate {
	public int X;
	public int Y;
	
	public Coordinate(){
		X = 0;
		Y = 0;
	}
	
	public Coordinate(int Xin, int Yin){
		X = Xin;
		Y = Yin;
	}
	
	public Coordinate(int in){
		Y = X = in;
	}
	
	public Coordinate(Coordinate c){
		X = c.X;
		Y = c.Y;
	}
	
	public Coordinate add(Coordinate c){
		
		return new Coordinate(c.X + X, c.Y + Y);
		
	}
	
	public Coordinate subtract(Coordinate c){
		return new Coordinate(X - c.X, Y - c.Y);
	}
	
	public float Length(){
		
		return (float)Math.sqrt((double)X*(double)X + (double)Y*(double)Y);
		
	}
	
}

