package main;

import java.lang.Math;

/*
 * The class for a basic coordinate. Cannot tick or implement any classes, just a structure for coordinates
 * 
 * also includes many operations for these coordinates. (To be added on)
 */

public class Coordinate {
	public float X;
	public float Y;
	
	public Coordinate(){
		X = 0;
		Y = 0;
	}
	
	public Coordinate(float Xin, float Yin){
		X = Xin;
		Y = Yin;
	}
	
	public Coordinate(float in){
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
	
	public Coordinate multiply(Coordinate c){
		return new Coordinate(X * c.X, Y * c.Y);
	}
	
	public Coordinate multiply(float f){
		return new Coordinate(X * f, Y * f);
	}
	
	public float Length(){
		
		return (float)Math.sqrt((double)X*(double)X + (double)Y*(double)Y);
		
	}
	
}

