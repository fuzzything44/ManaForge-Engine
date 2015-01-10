package main;

import org.newdawn.slick.Image;

public class Actor {
	public final String actorName;
	public final Image actorImage;
	public Coordinate actorLocation;
	public Actor(String name, Image image, Coordinate location) {
		actorName = name;
		actorImage = image;
		actorLocation = location;
	}
}
