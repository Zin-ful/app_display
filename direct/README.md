priority:

1st. make terminal emulator

2nd. fix mouse

3rd. figure out how to execute shit

4th. config.c && config.json

5th. auto recognize keyboard event

6th. integrate the file server (1.0)

7th. app transition



issues:
when setting mouse size to larger than one pixel the get_space and erase_mouse fail and end up shifting the pixels. a fix for this could be using get_pixel_range (a for loop) and if the color value matches the background color we dont map it. 
because its a mouse updating quickly id rather not use a for loop though since its heavy on cpu

draw_highlight makes multiple calls to draw_pixel which is slow and can make boxes flash. this also overwrites anything were trying to highlight. is fixible, i am lazy

printing text is impossible

make struct for highlighted objects to allow the check and execution of applications

create a way to map programs to screen coordinates

make app transition (pixelate fade maybe) to allow time for the windower.c to close, clear screen to prevent the cmdline from showing, and load the emulator

buffer screen into memory

make animated screen saver

make the screen timeout lol


