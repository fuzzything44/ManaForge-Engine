#version 330 core
layout(location = 0) in vec2 vertLocationIn;
layout(location = 1) in vec2 vertTexCoordIn;

uniform int currentTile;
uniform int tiles; // the number of horizonital and vertical tiles

uniform mat3 MVPmat;
uniform float renderOrder;

out vec2 fragTexCoord;

void main()
{
	
	gl_Position.xyw = MVPmat * vec3(vertLocationIn, 1.f);
	gl_Position.z = float(renderOrder - 256) / 256;
	
	// calculate the texture coordinates
	int row = currentTile / tiles;
	int column = currentTile % tiles;
	
	float uvCoordsPerTile = 1.f / float(tiles);
	
	fragTexCoord = vec2 (
		(vertTexCoordIn.x / float(tiles)) + (column * uvCoordsPerTile),
		((vertTexCoordIn.y / float(tiles)) + (row * uvCoordsPerTile))
	);
	
}