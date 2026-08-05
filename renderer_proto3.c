//gcc renderertest.c -o test -lm

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQ(x) ((x)*(x))

typedef struct object{
	float values[10];
} object;

typedef struct light{
	float pos[3];
	float color[3];
	int intensity;
	
	} light; //maybe add shape at some point? idk
	
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
void vecminus(float vec1[], float vec2[], float vecfinal[]){
	vecfinal[0] = vec1[0] - vec2[0];
	vecfinal[1] = vec1[1] - vec2[1];
	vecfinal[2] = vec1[2] - vec2[2];
}
	
void vecplus(float vec1[], float vec2[], float vecfinal[]){
	vecfinal[0] = vec1[0] + vec2[0];
	vecfinal[1] = vec1[1] + vec2[1];
	vecfinal[2] = vec1[2] + vec2[2];
}

void vecscale(float scalar, float vec[]){
	vec[0] = vec[0] * scalar;
	vec[1] = vec[1] * scalar;
	vec[2] = vec[2] * scalar;
}

void vecnormalize(float vec[]){		
	float length = sqrt((SQ(vec[0])+SQ(vec[1])+SQ(vec[2])));
	vec[0] = (vec[0]/length);
	vec[1] = (vec[1]/length);
	vec[2] = (vec[2]/length);
}

void veccopy(float copier[], float copied[]){
	copier[0] = copied[0];
	copier[1] = copied[1];
	copier[2] = copied[2];
}

float vecmagnitude(float vec[]){ sqrt((SQ(vec[0])+SQ(vec[1])+SQ(vec[2]))); }
	
//++++++++++++

collision check_sphere(float ray[],object sphere, int Nobj, float origin[]){
	collision collide;
	collide.target = Nobj;
	
	float a = float_dotP(ray, ray); 
	
	float pos[3] = {sphere.values[1],sphere.values[2],sphere.values[3]};
	float oripos[3];
	vecminus(origin, pos, oripos);
	float b = (2 * (float_dotP(oripos, ray)));
	//float c = ((-2*float_dotP(origin, pos)) + float_dotP(origin, origin) + float_dotP(pos, pos) - SQ(sphere.values[4])); equivalent to current
	float c = float_dotP(oripos, oripos) - SQ(sphere.values[4]);
	
	if ((SQ(b) - (4*a*c)) < 0){ collide.hit = 0; return collide;}
	
	float T1 = ((-(b) + sqrtf(SQ(b) - (4*a*c))) / (2*a));
	float T2 = ((-(b) - sqrtf(SQ(b) - (4*a*c))) / (2*a));
	
	
	if (T1<0 && T2<0){collide.hit = 0;} 
	else if (T1<0 || T2<0){collide.hit = 1; collide.time = (fmaxf(T1,T2));} 
	else {collide.hit = 1; collide.time = (fminf(T1,T2));} 
	
	
	return (collide); 
	}

//+++++++++++++++++++++++++++++++++++++++

collision checkray(float ray[], object objlist[], int objcount, float origin[]){
	collision finalhit;
	collision checkhit;

	
	for (int n=0; n<objcount; n++){
		int shape = objlist[n].values[0];
		switch (shape){
		
			case 1:
			checkhit = check_sphere(ray, objlist[n], n, origin);
			
		
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

void surfacenormalize (object target, float hitpoint[], float finalvec[]) {
	float vector[3] = {(hitpoint[0] - target.values[1]), (hitpoint[1] - target.values[2]), (hitpoint[2] - target.values[3])};
	vecnormalize(vector);
	
	finalvec[0] = vector[0];
	finalvec[1] = vector[1];
	finalvec[2] = vector[2];
}

//++++++++++++++++++++++++++++++++++
float find_allignement(float vec1[], float vec2[]){
	float dotP = float_dotP(vec1,vec2);
	
	float length1 = sqrt((SQ(vec1[0])+SQ(vec1[1])+SQ(vec1[2])));
	float length2 = sqrt((SQ(vec2[0])+SQ(vec2[1])+SQ(vec2[2])));
	
	float allignement = (dotP/(length1*length2));
	return allignement;
}
//++++++++

float lighting(object target, float hitpoint[], light lightlist[], int n, object objlist[], int objcount)  {
	float surfacenormal[3];
	
	surfacenormalize(target, hitpoint, surfacenormal);
	//printf("%f,%f,%f\n", surfacenormal[0],surfacenormal[1],surfacenormal[2]);
		
	float fulllightray[3] = {lightlist[n].pos[0] - hitpoint[0], lightlist[n].pos[1] - hitpoint[1], lightlist[n].pos[2] - hitpoint[2]};
	
	//printf("%f ; %f ; %f\n", lightlist[n].pos[0],lightlist[n].pos[1],lightlist[n].pos[2]);
	float lightray[3];
	veccopy(lightray, fulllightray);
	//printf("%f,%f,%f\n", lightray[0],lightray[1],lightray[2]);
	
	vecnormalize(lightray);
	//printf("%f,%f,%f\n", lightray[0],lightray[1],lightray[2]);
	
	float lightfactor = find_allignement(lightray, surfacenormal);
	//printf("%f\n", lightfactor);
	if (lightfactor > 0){
		collision lightvision;
		
		float raystart[3];
		float shifter[3];
		veccopy(shifter, surfacenormal);
		vecscale(0.1, shifter);
		vecplus(hitpoint, shifter, raystart);
		
		lightvision = checkray(lightray, objlist, objcount, raystart);
		
		if (lightvision.hit == 1){
			
			float lightdistance = vecmagnitude(fulllightray);
			if (lightdistance > lightvision.time){lightfactor = 0;}
	}
				
}
	


if (lightfactor < 0){lightfactor = 0;}
return lightfactor;
}


//++++++++++++++++++++++
	
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
	S4.values[1] = -15; 	//posx
	S4.values[2] = 10;	//posy
	S4.values[3] = 50;	//posz
	S4.values[4] = 3;	//radius
	S4.values[5] = 64;	//red
	S4.values[6] = 0;	//green
	S4.values[7] = 128;	//blue
	
	//++++++++++++++++++++++++++++ Here add objects to objlist and set objcount to the mount of objects ++++++++++++++++++++++++++++
	object objlist[5];
	int objcount = 4;
	
	objlist[0] = S1;
	objlist[1] = S2;
	objlist[2] = S3;
	objlist[3] = S4;
	
	//++++++++++++++++++++++++++++++++++++++create light sources++++++++++++++++++++++++++++++++
	
	light L1;
	L1.pos[0] = 4;
	L1.pos[1] = 5;
	L1.pos[2] = 5;
	
	
	
	//+++++++++++++ light list ++++++++++++++++++++++
	
	light lightlist[5];
	int lightcount = 1;
	
	lightlist[0] = L1;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
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
		
		vecnormalize(ray);
		
		float origin[3] = {0,0,0};
		hit = checkray(ray, objlist, objcount,origin);
		
		if (hit.hit == 1){ 
			float lightfactor;
			float hitpoint[3];
			
			//lightfactor = 1;
			//printf("%f\n", hit.time);
			
			veccopy(hitpoint, ray);
			vecscale(hit.time, hitpoint);
			
			lightfactor = lighting(objlist[hit.target], hitpoint, lightlist, 0, objlist, objcount);
			printf("%f\n", lightfactor);
			
			framebuffer[l][0] = (objlist[hit.target].values[5]*lightfactor); 
			framebuffer[l][1] = (objlist[hit.target].values[6]*lightfactor); 
			framebuffer[l][2] = (objlist[hit.target].values[7]*lightfactor); 
			}
			
		else{ for(int bac = 0; bac <= 2; bac++){ framebuffer[l][bac] = background; }}
	}
	
	writeimage(window1.resx, window1.resy, window1.colors, framebuffer);
	
	free(framebuffer);
}
