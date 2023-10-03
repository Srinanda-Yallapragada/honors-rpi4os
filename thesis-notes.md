Working on getting power measurements to work and see if its reliable enough
Also checking if the power consumption difference is clearly noticeable between doing work and not doing working, then i can say for sure that i can atleast measure how much energy is being used properly

Need to make an effective and reliable way to measure the time and power consumption of the raspberry pi


The TTY on linux is ttyUSB0 for me to connect the serial cable through putty

sudo usermod -a -G uucp $USER on arch 

sudo usermod -a -G dailout $USER on ubuntu and fedora



Calibration is tricky, using samsing charger rn to calibrate for 5 v

Generic testing now…


Starting with this code for 3 min using the usb power meter as time tracking
#include "io.h"


void main()
{
int i = 0;
uart_init();
while (1)
{
uart_writeText("Hello world!");
uart_writeText("\n");
i++;
}
}




Now going to try 
#include "io.h"


void main()
{
while (1);
}



For 3 min 

Results 

Using uart output for 3 min + simple addition 17 mAh and 94mWh 


Using while true loop for 3 min 17 mAh and 93mWh 


Still not enough to make it do work……. Try to use a sleep function> wait for event?

#include "io.h"


void main()
{
uart_init();
uart_writeText("Hello World \n");
while (1)
{
{
volatile unsigned long long i
for (i = 0; i < 1000000000ULL; ++i)
{
}
}
}
}



This one ran and gave 18mAh and 97mWh and this is the first time i was able to change the power stats but it could be human error/ variable factors like temperature. Not convinced…


There is a lot of human error in my method rn, will work on it and make it better over time


Now trying the full raspberry pi os and seeing the power stats on that. It should turn on a lot of hardware and software features so im sure i will see something change in the power consumption. 


While the sd card was formatting i had the idea of booting the pi without any sd card in it to see what its power consumption was. Those stats are 


17mAh and 93mWh. Hmm, so a while true really does nothing then… what do i need to write to create a noticeable power difference to a spinlock?!



With the sd card its running raspbian 64 bit, have not set up anything on it…. But literally plugged it in and left it the results are 16mAh and  88mWh  its more efficient? Right ok need to dig in more… Wonder if there is power management in raspbian os 

Raspberry pi has no ACPI which means no sleep or power saving modes… yikes thats not good if i am looking at power efficiency lmao




Next i will try an array in memory to see how that will change the power consumption in C

#include "io.h"


void main()
{
int basic_arr[] = {0, 0, 0, 0, 0, 0, 00, 0, 0};
uart_init();
uart_writeText("Hello World \n");
while (1)
{
{
volatile unsigned long long i;
for (i = 0; i < 1000000000ULL; ++i)
{
for (int j = 0; j < 10; j++)
{
basic_arr[j] += 1;
}
}
}
}
}



This is what i am trying to run this time

17mAh 94mWh hmm no change from  simple while true loop


Moved away from the idea of power consumption, and into developer expereince 

Working with make files is annoying but needed 

Both rust and C have strong tools for code development like auto forrmaters and syntax highlighting 

ASCII rendering in C achieved! 


Possible goals? 
What does it mean to have an OS instead of baremetal code that runs more like a microconroller?  Where to draw the line?

Resource management! Thats where you draw the line

Is it worth investing time into making a shell instead of going for the intense copy pasting code of networking? 

https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/Introduction.md

https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf the docs for register names and stuff

Managing header files in an interesting task in C. Shell.H is an example 
Writing a shell is fun! Moving onto a parser


Sep 19 meeting!
Next steps 
Bare minimum files on the SD card  -> Done!  
A DTB file is a device tree (or devicetree) blob file used by the Linux kernel. It contains binary data that describes a computer's hardware. DTB files allow operating systems to manage a computer's components by telling the operating system what hardware the computer includes. This file was called bootbin.bin on the rp3. Essentially bcm2711-rpi-4-b.dtb is a binary hardware describing file is my understanding

Config.txt is used to set params that make it easier to interact with the rpi4 such as core_freq_min and arm_64bit

fixup.dat, is used to configure the SDRAM partition between the GPU and the CPU

Start.elf It programs the GPU up for display, sorts out all the interfaces etc - prior to jumping to user code.

And our very own kernel.8 
Those are the 5 bare minimum files we are using 


Look into scheduling -> what it takes to implement round robin
Look into process management, Its an os when you manage resources 
Implement string.h -> On the way, added lenstr() and cmpstr()
Implement parser -> Done! Can parse a command from our UART interface
I need a better working shell, and I want to be able to write and read from memory and use that as files create write and delete m aximum file size 
Get to part 5 on rust! -> Done? Kinda, able to run rust code, but have not done a line by line


