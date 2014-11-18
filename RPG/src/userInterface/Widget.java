package userInterface;

import main.Object;

public class Widget extends Object{
	
	Widget owner;
	
	enum FillType{
		Full,
		Auto
	}
	
	FillType horizonitalFill = FillType.Auto;
	FillType verticalFill = FillType.Auto;
	
	public Widget(Widget owner){
		
	}
	
	// creates the root widget. Usually only used once
	public Widget(){
		
	}
	
}
