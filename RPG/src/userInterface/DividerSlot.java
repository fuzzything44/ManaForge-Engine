package userInterface;

public class DividerSlot extends Widget{

	public DividerSlot(Widget newWidget) {
		childWidgets.addElement(newWidget);
		
		newWidget.owner = this;
	}
	
}
