//gcc renderertest.c -o test -lm

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQ(x) ((x)*(x))

typedef struct object{
	float values[10];
} object;

typedef struct light{float values[8];} light; //shape, xyzpos, rgb, intensity, ...(no space with current mem)
	
typedef struct Camera{
	int Winx;
	int Winy;
	int Depth;	
} Camera;

struct window{
	int resx;
	int resy;
	int colors;
};

typedef struct collision{int hit; float time; int target; } collision;

//-------------------------------------------------------------
	
float float_dotP(float vec1[], float vec2[]){return ((vec1[0] * vec2[0]) + (vec1[1] * vec2[1]) + (vec1[2] * vec2[2]));}

//++++

collision check_sphere(float ray[],object sphere, int Nobj){
	collision collide;
	collide.target = Nobj;
	
	float a = (SQ(ray[0])+SQ(ray[1])+SQ(ray[2]));
	float pos[3] = {sphere.values[1],sphere.values[2],sphere.values[3]};
	float b = (-2 * (float_dotP(pos, ray)));
	float c = (SQ(pos[0]) + SQ(pos[1]) + SQ(pos[2]) - SQ(sphere.values[4]));
	
	if ((SQ(b) - (4*a*c)) < 0){ collide.hit = 0; return collide;}
	float T1 = ((-(b) + sqrtf(SQ(b) - (4*a*c))) / (2*a));
	float T2 = ((-(b) - sqrtf(SQ(b) - (4*a*c))) / (2*a));
	if (T1<0 && T2<0){collide.hit = 0;} 
	else if (T1<0 || T2<0){collide.hit = 1; collide.time = (fmaxf(T1,T2));} 
	else {collide.hit = 1; collide.time = (fminf(T1,T2));} 
	return (collide); }

//++++

collision checkray(float ray[], object objlist[], int objcount){
	collision finalhit;
	collision checkhit;

	
	for (int n=0; n<objcount; n++){
		int shape = objlist[n].values[0];
		switch (shape){
		
			case 1:
			checkhit = check_sphere(ray, objlist[n], n);
			
		
		if (n==0){finalhit = checkhit;}
		else{
			if (finalhit.hit == 0 && checkhit.hit == 1){finalhit = checkhit;}
			else if(finalhit.hit == 1 && checkhit.hit == 1 && finalhit.time > checkhit.time){finalhit = checkhit;}
		}
		}
	}
	

	return finalhit;
	}
	
//++++
	
void writeimage(int resx, int resy, int colors, int frame[][3]){
	FILE *im;
	
	im = fopen("image.ppm", "w");
	
	fprintf(im, "P3\n%d %d\n%d\n", resx, resy, colors);
	
	for (int p = 0; p < (resx*resy); p++){
		int colR = frame[p][0];
		int colG = frame[p][1];
		int colB = frame[p][2];
		fprintf(im, "%d %d %d ", colR, colG, colB); }
	
	fclose(im);

	printf("image write done\n");
	
}
//----------------------------------------------------------------------

void main(){
	
	//+++++++++++++++++++++  ADD OBJECTS HERE +++++++++++++++++++++++++++++
	object S1; //sphere 1
	S1.values[0] = 1; 	//shape
	S1.values[1] = 0; 	//posx
	S1.values[2] = 0;	//posy
	S1.values[3] = 10;	//posz
	S1.values[4] = 2;	//radius
	S1.values[5] = 128;	//red
	S1.values[6] = 0;	//green
	S1.values[7] = 0;	//blue
	
	object S2; //sphere 2
	S2.values[0] = 1; 	//shape
	S2.values[1] = 4; 	//posx
	S2.values[2] = 2;	//posy
	S2.values[3] = 15;	//posz
	S2.values[4] = 2;	//radius
	S2.values[5] = 0;	//red
	S2.values[6] = 0;	//green
	S2.values[7] = 128;	//blue
	
	object S3; //sphere 3
	S3.values[0] = 1; 	//shape
	S3.values[1] = -4; 	//posx
	S3.values[2] = -3;	//posy
	S3.values[3] = 8;	//posz
	S3.values[4] = 2;	//radius
	S3.values[5] = 0;	//red
	S3.values[6] = 128;	//green
	S3.values[7] = 0;	//blue
	
	object S4; //sphere 4
	S4.values[0] = 1; 	//shape
	S4.values[1] = -5; 	//posx
	S4.values[2] = 1;	//posy
	S4.values[3] = 20;	//posz
	S4.values[4] = 3;	//radius
	S4.values[5] = 0;	//red
	S4.values[6] = 0;	//green
	S4.values[7] = 0;	//blue
	
	//++++++++++++++++++++++++++++ Here add objects to objlist and set objcount to the mount of objects ++++++++++++++++++++++++++++
	object objlist[5];
	int objcount = 4;
	
	objlist[0] = S1;
	objlist[1] = S2;
	objlist[2] = S3;
	objlist[3] = S4;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	Camera Camera1;
	Camera1.Depth = 1;
	Camera1.Winx = 1;
	Camera1.Winy = 1;

	struct window window1;
	window1.resx = 1000;
	window1.resy = 1000;
	window1.colors = 255;

	int background = 255;
	
	int (*framebuffer)[3] = malloc(window1.resx * window1.resy * 3 * sizeof(int));
	
	
	float ray[3];
	ray[2] = Camera1.Depth;
	for (int l = 0; l<(window1.resx*window1.resy); l++){
		collision hit;
		ray[1] = ((0.5 * Camera1.Winy) - ((float)Camera1.Winy / (window1.resy-1)) * (l/window1.resx));
		ray[0] = ((-0.5 * Camera1.Winx) + (((float)Camera1.Winx / (window1.resx-1)) * (l - (window1.resx * (l/window1.resx)))));
		
		float length = sqrt((SQ(ray[0])+SQ(ray[1])+SQ(ray[2])));
		ray[0] = (ray[0]/length);
		ray[1] = (ray[1]/length);
		ray[2] = (ray[2]/length);
		
		
		hit = checkray(ray, objlist, objcount);
		
		if (hit.hit == 1){ framebuffer[l][0] = objlist[hit.target].values[5]; framebuffer[l][1] = objlist[hit.target].values[6]; framebuffer[l][2] = objlist[hit.target].values[7]; }
		else{ for(int bac = 0; bac <= 2; bac++){ framebuffer[l][bac] = background; }}
	}
	
	writeimage(window1.resx, window1.resy, window1.colors, framebuffer);
	
	free(framebuffer);
}
