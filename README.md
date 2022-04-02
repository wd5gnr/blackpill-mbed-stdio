# Example of using USBSerial for console

by Al Williams

If you construct a normal USBSerial object, the first argument of the constructor defaults to TRUE. This causes the program to hang
until you connect a terminal. That's great if you are debugging but not so good when you just want a port avaialble sometimes.

In addition, on some Linux systems, when the terminal does connect, the terminal will hang on a race condition until you forcibly kill
it and restart it. So you are better off leaving the port disconnected.

But there's a problem. If you use the USBSerial as your console, the first printf or other call to the dead port will tell the C library
something is wrong and it will then give up. So you won't see any output. You would think this could happen if you shut down the terminal
while the program was running but testing shows that is not the case. Once you can write to terminal once, it stays on. However, I
wrote my code to clear errors any time the state goes from disconnected to connected.

There are a few interlocking pieces as shown in this code.

1) Create a USBSerial object that is not connected.
2) Provide the mbed_override_console function. This will be called 3 times for stdout, stderr, stdin and we will return the serial port in each case.
3) Connect the USB serial port using .connect()
4) If you want input a character at a time, call setvbuf on stdin (optional, of course)
5) Somewhere at some point you need to call clearerr on the streams you are using. You have a few choices:

A-Call it periodically which is harmless and when the terminal connects you'll be in business

B-Call it when usbSerial first shows as connected (the approach in this code)

C-Do A or B in its own thread

D-Doubtless you can dream up some other way to do it, too

Keep in mind that it isn't the USB connection you have to make, it is the terminal opening the serial port the first time.

Also keep in mind that almost none of this is necessary if you don't use the standard I/O functions. In other words, use
usbSerial.printf(...) and the like and you can avoid all of this. Still...

Example
==
The example code blinks an LED, reads a keyboard character if available, and prints a count and the last character it saw using printf.

Build environment
==
The Blackpill isn't directly supported by mbed. Most people tell you to use the Nucelo target for the F411 CPU, but that has a few quirks.

If you use the desktop "Mbed Studio" you can follow these directions to set up for a proper build environment for Blackpill:

https://os.mbed.com/users/hudakz/code/BLACKPILL_Custom_Target/graph/






