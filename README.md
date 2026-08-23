I compile like this:
gcc MAIN.c liverenderer.c Fenster.c -o <insert name> -lm -lX11

Then run ./<insert name>

This will open a window and display the rendered image. As of right now ive made my own little upscaling formula, so you can change the ratio of original to new resolution with the scale variable in MAIN.c . 

As of right now you cant even notice that its updating the image live since nothing changes in the scene, but eventually there will be ways to interact with the scene.

IMPORTANT: Since ive made the display library with X11, it obviously wont work unless thats what your pc is using.
