#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"

// This needs to be moved to config
typedef enum state {
    plugged_active = 0,
    unplugged_active = 1,
    plugged_inactive= 8,
    unplugged_inactive = 9,
} State;

//Declare global conf
Config *conf;


// This is called in a loop every second so
// buff is supplied to avoid allocating memory
// buff should be a char array with length 2
// and ending in a \0
State getState(FILE *fp, char *buff) {
    // If we don't rewind the byte we read is buffered.
    rewind(fp);

    // Get a single char at STATE_LOCATION
    fseek(fp, conf->state_location, SEEK_SET);
    buff[0] = fgetc(fp);
    // The character we read is a single hex digit
    // (half a byte) with the least significant bit
    // indicating whether the jack is plugged in
    // (0 plugged in, 1 unplugged) and the most
    // significant bit indicating whether the front
    // panel is active (0 for active, 1 for inactive)
    // the other bits are irrelevant so we get rid of them
    // Hopefully you don't need to change this one
    return (State)(strtol(buff, NULL, 16) & conf->mask);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Using config file: %s\n", argv[1]);
        conf = Get_Config(argv[1]);
    } else {
        conf = Get_Config("/etc/audioswitch.conf");
    }

    if (conf == NULL) {
        perror("Could not open config file\nExiting...");
        exit(EXIT_FAILURE);
    }

    // Open the file initialize some variables
    // outside the loop to avoid allocations in the
    // loop. Is this worth doing?
    FILE *fp = fopen(conf->oxygen_file, "rb");
    char readchar[] = "0\0";
    State value;
    int command_success;

    if (fp == NULL) {
        perror("Could not open oxygen_file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read the state and switch when the state is invalid.
        value = getState(fp, readchar);
        switch (value) {
            case plugged_inactive:
                command_success = system(conf->headphones);
                break;
            case unplugged_active:
                command_success = system(conf->speakers);
                break;
            default:
                break;
        }
        sleep(1);
    }
}
