issues:
when setting mouse size to larger than one pixel the get_space and erase_mouse fail and end up shifting the pixels. a fix for this could be using get_pixel_range (a for loop) and if the color value matches the background color we dont map it. 
because its a mouse updating quickly id rather not use a for loop though since its heavy on cpu

draw_highlight makes multiple calls to draw_pixel which is slow and can make boxes flash. this also overwrites anything were trying to highlight. is fixible, i am lazy

printing text is impossible

make struct for highlighted objects to allow the check and execution of applications
