the actual program is in /compile, these files do run but without the /config or /apps it will throw errors


priority:

1st. make terminal emulator -not done due to:
need to make bitmap for fonts
need to map fonts to grid
need to add/remove chars on keyevent

2nd. fix mouse -not done due to stupid bullshit

3rd. figure out how to execute shit -done

4th. config.c && config.json -done

5th. auto recognize keyboard event

6th. integrate file server

7th. app transition



issues:

when setting mouse size to larger than one pixel the get_space and erase_mouse fail and end up shifting the pixels. a fix for this could be using get_pixel_range (a for loop) and if the color value matches the background color we dont map it. 
because its a mouse updating quickly id rather not use a for loop though since its heavy on cpu

draw_highlight makes multiple calls to draw_pixel which is slow and can make boxes flash. this also overwrites anything were trying to highlight. is fixible, i am lazy

printing text is impossible

make app transition (pixelate fade maybe) to allow time for the windower.c to close, clear screen to prevent the cmdline from showing, and load the emulator

buffer screen into memory

make animated screen saver

make the screen timeout lol


