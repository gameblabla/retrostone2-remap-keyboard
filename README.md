This is a small program that makes use of uinput to remap the buttons on the RG-351P.

The OGA uses GPIO for its keys.
However the RG-351P uses an internal USB-HID chip and therefore, libgo2 doesn't use it
since the RG-351P is mapped the same as libgo2 would.

But this causes an issue for SDL 1.2 games that rely on the keyboard because now we can't
remap the GPIO...

TODO :
Make a detection program for OGA clones
