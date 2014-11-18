package userInterface;

import java.util.Vector;

public class Divider extends Widget {

	Vector<DividerSlot> Slots = new Vector<DividerSlot>();
	
	/*public Divider(Widget owner){
		super(owner);
	}
	*/
	public Divider(){
		super();
	}
	
	public void AddSlot(Widget newWidget){
		Slots.addElement(new DividerSlot(newWidget));
	}
	
	public void tick(float deltaTime){
		
	}
	
}
