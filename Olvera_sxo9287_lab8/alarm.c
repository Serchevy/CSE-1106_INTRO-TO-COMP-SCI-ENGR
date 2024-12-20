// Burglar Alarm Application
// Jason Losh

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -std=c99 -o alarm alarm.c udp.c sysfs_gpio.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp
#include <poll.h>            // poll
#include <unistd.h>          // usleep
#include "sysfs_gpio.h"      // custom virtual file system gpio library
#include "udp.h"             // custom udp library

#define SERVER_LISTENER_PORT 4096

#define DOOR 17
#define MOTION 27

#define TIMEOUT_SECONDS 25

bool isAnyKeyPressed()
{
    struct pollfd pfd;
    pfd.fd = 1;
    pfd.events = POLLIN;
    poll(&pfd, 1, 1);
    return pfd.revents == POLLIN;
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    char *remoteIp;
    int remotePort;
    bool armed = false;
    bool quit = false;
    int timeout = TIMEOUT_SECONDS;

    // Verify arguments are good
    bool goodArguments = (argc == 2);
    if (goodArguments)
        remoteIp = argv[1];
    if (!goodArguments)
    {
        printf("usage: alarm IPV4_ADDRESS\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        exit(EXIT_FAILURE);
    }

    // Set remote port
    remotePort = SERVER_LISTENER_PORT;

    // Setup pins as inputs
    // TODO: Add code to configure the door button 
    //       and motion sensor pins are inputs
    gpioInput(DOOR);
    gpioInput(MOTION);

    // Send disarmed message
    sendData(remoteIp, remotePort, "disarmed");

    // Loop until 'q' is pressed on keyboard
    while (!quit)
    {
        // TODO: Add code to read the door and motion inputs
        //       and send messages and display on screen
            if ((gpioRead(DOOR)) && armed) {
                printf("Door Open\n");
                sendData(remoteIp, remotePort, "door");
            }
            if ((gpioRead(MOTION)) && armed) { 
                printf("Motion Detected\n");
                sendData(remoteIp, remotePort, "motion");
            }
        
        // If key pressed on keyboard, get key and process
        // If 'a', arm system
        // If 'd', disarm system
        // If 'q', quit
        if (isAnyKeyPressed())
        {
            char c = fgetc(stdin);
            switch(c)
            {
                // TODO: Add code to look for q input and exit loop
                // TODO: Add code to look for a or d and handle arm/disarm
                case 'a':
                    printf("Arming System\n");
                    armed = true;
                    sendData(remoteIp, remotePort, "armed");
                    break;
                case 'd':
                    printf("Disarming System\n");
                    armed = false;
                    sendData(remoteIp, remotePort, "disarmed");
                    break;
                case 'q':
                    quit = true;
                    break;
                default:
                    printf("No...\n");
                    break;
            }
        }
        if (!quit)
        {
            // Sleep for 1 second
            usleep(1000000);
            
            // TODO: Add to code decrement the timeout by one
            timeout--;
            // TODO: When the timeout reaches zero:
            //       - send a pulse message to keep connection alive
            //       - reload the timeout variable with TIMEOUT_SECONDS
            if (timeout == 0) {
                printf("Sending Pulse\n");
                sendData(remoteIp, remotePort, "pulse");
                timeout = TIMEOUT_SECONDS;
            }
            
        }
    }
    return EXIT_SUCCESS;
}
