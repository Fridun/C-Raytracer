Hey. So this is a raytracer I've made in C. As of this version it supports only spheres of solid colors, but can support multiple spheres at once. No shadows yet.

This is the structure for a sphere object if you want to make one:
	
	object S1; //sphere 1
	S1.values[0] = 1; 	//shape
	S1.values[1] = 0; 	//posx
	S1.values[2] = 0;	//posy
	S1.values[3] = 10;	//posz
	S1.values[4] = 2;	//radius
	S1.values[5] = 128;	//red
	S1.values[6] = 0;	//green
	S1.values[7] = 0;	//blue
You must make it in the main function, and then both add it to the objlist array as well as making sure that the integer objcount = the amount of objects in the scene. All this must be done before the for loop close to the end of the main function.
	
Z axis: depth, distance straight in front of camera
X axis: left-right
Y axis: up-down

Other notes: I'm not sure whether or not i want to keep that object.values[] structure. Might end up changing it to individual variables. We'll see.
