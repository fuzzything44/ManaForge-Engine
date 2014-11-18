package userInterface;

import java.util.Vector;

public class Divider extends Widget {

	Vector<DividerSlot> Slots = new Vector<DividerSlot>();
	
	public Divider(Widget owner){
		super(owner);
	}
	
	public Divider(){
		super();
	}
	
	
	
}
