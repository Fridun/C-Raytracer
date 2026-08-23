#ifndef LIVERENDERER_H
#define LIVERENDERER_H


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>


//structs

typedef struct object{
	int type;
	int col[3];
	float reflectivity;
	union {
		struct {
			float radius;
			float pos[3];
		} sphere;
		
		struct { 
			float normal[3];
			float offset;
		} plane;
		
	} shape;
} object;

typedef struct light{
	float pos[3];
	float color[3];
	int intensity;
	
	} light; //maybe add light shapes at some point? idk
	
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

typedef struct array3{float array[3];} array3;

// functions

float float_dotP(float vec1[], float vec2[]);

void vecminus(float vec1[], float vec2[], float vecfinal[]);

void vecplus(float vec1[], float vec2[], float vecfinal[]);

void vecscale(float scalar, float vec[]);

void vecnormalize(float vec[]);

void veccopy(float copier[], float copied[]);

float vecmagnitude(float vec[]);

collision check_plane(float ray[], object target, int Nobj, float origin[]);

collision check_sphere(float ray[],object target, int Nobj, float origin[]);

collision checkray(float ray[], object objlist[], int objcount, float origin[]);

void sphere_surfacenormalize (object target, float hitpoint[], float finalvec[]);

float find_allignement(float vec1[], float vec2[]);

float lighting(object target, float hitpoint[], light lightlist[], int n, object objlist[], int objcount);

array3 reflect(float ray[], object target, float hitpoint[], object objlist[], int objcount, int background[], float cameraorigin[], light lightlist[], int lightcount, int reflectionlimit, int reflectioncount);

array3 findcolor(float ray[], object target, collision lastcollision,float origin[], object objlist[], int objcount, int background[], light lightlist[], int lightcount, int reflectionlimit, int reflectioncount);

void render(uint32_t *framebuffer, int res[]);


#endif
