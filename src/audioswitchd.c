#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Change these to make it work with other soundcards (maybe)
// They'll be moved to a config file soon (hopefully)
#define OUTPUT_FILE "/proc/asound/DG/oxygen"
#define STATE_LOCATION 551
#define COMMAND "amixer -c 1 set 'Analog Output' %s"
#define HEADPHONES "'Stereo Headphones FP'"
#define SPEAKERS "'Stereo Headphones'"

// This might have to be changed too
// This should maybe be a bitfield or possibly just macros
typedef enum state {
    plugged_active = 0,
    unplugged_active = 1,
    plugged_inactive= 8,
    unplugged_inactive = 9,
} State;

// This is called in a loop every second so
// buff is supplied to avoid allocating memory
// buff should be a char array with length 2
// and ending in a \0
State getState(FILE *fp, char *buff)
{
    // If we don't rewind the byte we read is buffered.
    rewind(fp);
    // Get a single char at STATE_LOCATION
    fseek(fp, STATE_LOCATION, SEEK_SET);
    buff[0] = fgetc(fp);
    // The character we read is a single hex digit
    // (half a byte) with the least significant bit
    // indicating whether the jack is plugged in
    // (0 plugged in, 1 unplugged) and the most
    // significant bit indicating whether the front
    // panel is active (0 for active, 1 for inactive)
    // the other bits are irrelevant so we get rid of them
    // Hopefully you don't need to change this one
    return (State)(strtol(buff, NULL, 16) & 0x9);
}

// Should device be const char* device?
int switchDevice(const char device[])
{
    // call amixer set <OUTPUT> <DEVICE>
    // Is it worth trying to change the device
    // directly rather than using an external program?
    char buff[100];
    sprintf(buff, COMMAND, device);
    return system(buff);
}

int main()
{
    // Open the file initialize some variables
    // outside the loop to avoid allocations in the
    // loop. Is this worth doing?
    FILE *fp = fopen(OUTPUT_FILE, "rb");
    char readchar[] = "0\0";
    State value;

    // Maybe I should have TRUE/FALSE macros too?
    while (1)
    {
        // Read the state and switch when the state is invalid.
        value = getState(fp, readchar);
        switch (value) {
            case plugged_inactive:
                switchDevice(HEADPHONES);
                break;
            case unplugged_active:
                switchDevice(SPEAKERS);
                break;
            default:
                break;
        }
        sleep(1);
    }
}
