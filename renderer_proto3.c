//gcc renderertest.c -o test -lm

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SQ(x) ((x)*(x))

typedef struct object{
	int type;
	int col[3];
	union {
		struct {
			float radius;
			float pos[3];
		} sphere;
		
	} shape;
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

float vecmagnitude(float vec[]){ 
	float magnitude = sqrt((SQ(vec[0])+SQ(vec[1])+SQ(vec[2])));
	return magnitude;
	}
	
//++++++++++++

collision check_sphere(float ray[],object target, int Nobj, float origin[]){
	collision collide;
	collide.target = Nobj;
	
	float a = float_dotP(ray, ray); 
	
	float pos[3] = {target.shape.sphere.pos[0],target.shape.sphere.pos[1],target.shape.sphere.pos[2] };
	float oripos[3]; 
	vecminus(origin, pos, oripos);
	float b = (2 * (float_dotP(oripos, ray)));
	//float c = ((-2*float_dotP(origin, pos)) + float_dotP(origin, origin) + float_dotP(pos, pos) - SQ(sphere.values[4])); equivalent to current
	float c = float_dotP(oripos, oripos) - SQ(target.shape.sphere.radius);
	
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
		//int shape = objlist[n].type;
		switch (objlist[n].type){
		
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
	float vector[3] = {(hitpoint[0] - target.shape.sphere.pos[0]), (hitpoint[1] - target.shape.sphere.pos[1]), (hitpoint[2] - target.shape.sphere.pos[2])};
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
			//printf("hit\n");
			float lightdistance = vecmagnitude(fulllightray);
			//printf("lightdistance: %f\n", lightdistance);
			//printf("light time: %f\n", lightvision.time);
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
	object S1;
	S1.type = 1;
	S1.col[0] = 128;	//red
	S1.col[1] = 0; 		//green
	S1.col[2] = 0; 		//blue
	S1.shape.sphere.pos[0] = 0;
	S1.shape.sphere.pos[1] = 0;
	S1.shape.sphere.pos[2] = 10;
	S1.shape.sphere.radius = 2;
	
	object S2;
	S2.type = 1;
	S2.col[0] = 0;		//red
	S2.col[1] = 0; 		//green
	S2.col[2] = 128; 	//blue
	S2.shape.sphere.pos[0] = 4;
	S2.shape.sphere.pos[1] = 2;
	S2.shape.sphere.pos[2] = 15;
	S2.shape.sphere.radius = 2;
	
	object S3;
	S3.type = 1;
	S3.col[0] = 0;		//red
	S3.col[1] = 128; 	//green
	S3.col[2] = 0; 		//blue
	S3.shape.sphere.pos[0] = -4;
	S3.shape.sphere.pos[1] = -3;
	S3.shape.sphere.pos[2] = 8;
	S3.shape.sphere.radius = 2;
	
	object S4;
	S4.type = 1;
	S4.col[0] = 64;	//red
	S4.col[1] = 0; 		//green
	S4.col[2] = 128; 		//blue
	S4.shape.sphere.pos[0] = -15;
	S4.shape.sphere.pos[1] = 10;
	S4.shape.sphere.pos[2] = 50;
	S4.shape.sphere.radius = 3;

	object S5;
	S5.type = 1;
	S5.col[0] = 0;	//red
	S5.col[1] = 128; 		//green
	S5.col[2] = 128; 		//blue
	S5.shape.sphere.pos[0] = 5.5;
	S5.shape.sphere.pos[1] = 20;
	S5.shape.sphere.pos[2] = 75;
	S5.shape.sphere.radius = 10;
	
	object S6;
	S6.type = 1;
	S6.col[0] = 255;	//red
	S6.col[1] = 255; 		//green
	S6.col[2] = 0; 		//blue
	S6.shape.sphere.pos[0] = -12;
	S6.shape.sphere.pos[1] = 20;
	S6.shape.sphere.pos[2] = 80;
	S6.shape.sphere.radius = 10;
	

	//++++++++++++++++++++++++++++ Here add objects to objlist and set objcount to the mount of objects ++++++++++++++++++++++++++++
	object objlist[6];
	int objcount = 6;
	
	objlist[0] = S1;
	objlist[1] = S2;
	objlist[2] = S3;
	objlist[3] = S4;
	objlist[4] = S5;
	objlist[5] = S6;
	//++++++++++++++++++++++++++++++++++++++create light sources++++++++++++++++++++++++++++++++
	
	light L1;
	L1.pos[0] = 1;
	L1.pos[1] = 1;
	L1.pos[2] = 5.5;
	
	light L2;
	L2.pos[0] = -1;
	L2.pos[1] = 0;
	L2.pos[2] = 5.5;
	
	
	//+++++++++++++ light list ++++++++++++++++++++++
	
	light lightlist[5];
	int lightcount = 2;
	
	lightlist[0] = L1;
	lightlist[1] = L2;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	Camera Camera1;
	Camera1.Depth = 1;
	Camera1.Winx = 1;
	Camera1.Winy = 1;

	struct window window1;
	window1.resx = 4000;
	window1.resy = 4000;
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
			float templight;
			float hitpoint[3];
			
			//lightfactor = 1;
			//printf("%f\n", hit.time);
			
			veccopy(hitpoint, ray);
			vecscale(hit.time, hitpoint);
			
			vecplus(hitpoint, origin, hitpoint);
			
			for (int i = 0; i < lightcount; i++){
				templight = lighting(objlist[hit.target], hitpoint, lightlist, i, objlist, objcount);
				
				if(i==0){lightfactor = templight;}
				else{lightfactor = lightfactor + templight;}
			}
			
			
			framebuffer[l][0] = (objlist[hit.target].col[0]*lightfactor); 
			framebuffer[l][1] = (objlist[hit.target].col[1]*lightfactor); 
			framebuffer[l][2] = (objlist[hit.target].col[2]*lightfactor); 
			}
			
		else{ for(int bac = 0; bac <= 2; bac++){ framebuffer[l][bac] = background; }}
	}
	
	writeimage(window1.resx, window1.resy, window1.colors, framebuffer);
	
	free(framebuffer);
}
