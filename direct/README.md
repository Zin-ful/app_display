**the actual program is in /compile, these files do run but without the /config or /apps it will throw errors
to recognize keyboard make sure to reference the correct event in keyboard.c use ls -l in /dev/input/wtver-folder-should-be-here**

priority:

1st. make terminal emulator - translate keycode in keyboard for use across other programs

2nd. auto recognize keyboard event

3rd. integrate file server

4th. app transition



issues:

for some reason other programs dont recognize key events when i use inpt_event.code even tho its extern

when changing to difference screen sizes the mouse wont go the full x:y of the screen. im not exactly sure why since the screen boundaries are set properly. seems to be a mouse issue and not a window issue

because of the problem above you can only access certain cells for program execution. make sure to chmod 777 all apps or else youll run into a permission error

make app transition (pixelate fade maybe) to allow time for the windower.c to close, clear screen to prevent the cmdline from showing, and load the emulator

make animated screen saver

make the screen timeout lol


