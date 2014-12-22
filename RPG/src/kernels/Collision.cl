__kernel void collide(__global float* dynamicActors, __global float* staticActors, __global short* out, int staticSize, int dynamicSize)
{
	// get IDs
	int xIdx = get_global_id(0), yIdx = get_global_id(1);
	
	if(yIdx < dynamicSize){

		int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
		

		// get data
		if(xIdx < dynamicSize){
			x0 = dynamicActors[xIdx * 2];
			y0 = dynamicActors[xIdx * 2 + 1];
		} else if(xIdx < dynamicSize + staticSize){
			x0 = staticActors[(xIdx - dynamicSize) * 2];
			y0 = staticActors[(xIdx - dynamicSize) * 2 + 1];
			
		}
		
		x1 = dynamicActors[yIdx * 2];
		y1 = dynamicActors[yIdx * 2 + 1];
		
		// check if they are colliding

		/*		(x0, y0) ___________ (x0 + 1, y0)
		 *				|			|
		 *				|			|
		 *				|(x1, y1)___|_______(x1 + 1, y1)
		 *				|		|	|		|
		 *	(x0, y0 + 1)|_______|___| (x0 + 1, y0 + 1)
		 *					    |			|
		 *						|			|
		 *			(x1, y1 + 1)|___________|(x1 + 1, y1 + 1)
		 *
		*/

		if((x0 > x1 && x0 < x1 + 1 && y0 > y1 && y0 < y1 + 1) || (x1 > x0 && x1 < x1 + 1 && y1 > y0 && y1 < y0 + 1) ) {
			out[yIdx] = 1;
		}
	}
}