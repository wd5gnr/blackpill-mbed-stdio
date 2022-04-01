/* How to use USBSerial for printf on a Blackpill and possibly other mbed targets

-- Williams
 */

#include "mbed.h"
#include "USBSerial.h"


// Blinking rate in milliseconds
std::chrono::milliseconds rate=500ms;



    // Note: init to false requires connect to enumerate
    // If true, you must connect to the board to start
USBSerial usbSerial(false);



// If you want to use printf etc with USB Serial...
// HOWEVER. USBSerial must be connected prior to elaboration or 
// you need to call clearerr on the standard file handles after 
// a connect
namespace mbed
{
    FileHandle *mbed_override_console(int fd)
    {
        return &usbSerial;
    }

}

// Not really isspace but good enough for us
#define isspace(x) ((x)<=32)


// Main thread
int main()
{
    unsigned i=0, connected, lastconnected=0;
    char c=' ',in;
    usbSerial.connect();  // connect USB port
    // make sure we get 1 character at a time on input
    setvbuf(stdin,NULL,_IONBF,0);
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);
    

// If you redirect stdin/stdout etc to USBSerial, you have a problem
// When there is no serial port open at run time. The stream gets an error and
// then nothing will happen until you clear it.
// So the trick is to occasionally clear errors on stdout/stdin/stderr

    printf("Here we go!\r\n");  // Get started
  


    while (true) {
        led = !led;   // flip LED if output is true
        connected=usbSerial.connected();
        if (connected && ! lastconnected)
        {
            clearerr(stdout);
            clearerr(stderr);
            clearerr(stdin);
            
        }
        lastconnected=connected;
        if (usbSerial.available()>=1) 
            {
                in=getchar();
                if (!isspace(in)) c=in;
            }
        printf("%d - %c\r\n",++i,c);
        ThisThread::sleep_for(rate);  // sleepy time
    }
}
