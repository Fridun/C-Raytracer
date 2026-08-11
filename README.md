I compile like this:
gcc rendererproto.c -o renderer -lm

Then run ./renderer 

This will create a file called image.ppm that you can open in most image viewing programs

I would recommend increasing the resolution if you want a nice looking image, although 1000x1000 is fine for debugging or editing the scene. But details in reflections can be invisible/very small, and all edges look kinda rough/pixely.
