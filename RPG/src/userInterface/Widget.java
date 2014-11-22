package userInterface;
/*
 * Start to UI system.
 */
import java.util.Vector;

import main.Coordinate;
import main.Game;
import main.Object;

public class Widget extends Object{
	
	Widget owner;
	
	enum FillType{
		Full,
		Auto
	}
	
	Vector<Widget> childWidgets = new Vector<Widget>();
	
	Coordinate end;
	
	FillType horizonitalFill = FillType.Auto;
	FillType verticalFill = FillType.Auto;
	
	public Widget(Widget owner, FillType fillTypeH, FillType fillTypeV){
		
		horizonitalFill = fillTypeH;
		verticalFill = fillTypeV;
		
		Game.persistentChunk.tickingObjects.addElement(this);
		
		switch(horizonitalFill){
		case Full:
			
		case Auto:
			
		}
	}
	
	public void addWidget(Widget newWidget){
		childWidgets.addElement(newWidget);
		
		newWidget.owner = this;
	}
	
	// creates the root widget. Usually only used once
	public Widget(){
		
	}
	
	public void tick(float DeltaTime){
		super.tick(DeltaTime);
		
		switch(horizonitalFill){
		case Full:
			end.X = owner.end.X;
		case Auto:
			end.X = owner.end.X;
		}
		switch(verticalFill){
		case Full:
			end.Y = owner.end.Y;
		case Auto:
			end.Y = owner.end.Y;
		}
	}
	
	public void setSize(Coordinate size){
		
		end = size;
		
	}
	
}
