#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

const char OUTPUT_FILE[] = "/proc/asound/DG/oxygen";
const int STATE_LOCATION = 551;
const char COMMAND[] = "amixer set %s %s";
const char HEADPHONES[] = "'Stereo Headphones FP'";
const char SPEAKERS[] = "'Stereo Headphones'";
const char OUTPUT[] = "'Analog Output'";

enum state {
    plugged_active = 0,
    unplugged_active = 1,
    plugged_inactive= 8,
    unplugged_inactive = 9,
};

int getState(FILE *fp, char *buff) 
{
    rewind(fp);
    fseek(fp, STATE_LOCATION, SEEK_SET);
    buff[0] = fgetc(fp);
    return (int)strtol(buff, NULL, 16) & 0x9;
}

int switchDevice(const char device[])
{
    char buff[100];
    sprintf(buff, COMMAND, OUTPUT, device);
    return system(buff);
}

int main()
{
	FILE *fp = fopen(OUTPUT_FILE, "rb");
	char readchar[] = "0\0";
	enum state value;

	while (1) 
	{
		value = getState(fp, readchar);
        switch (value) {
            case plugged_inactive:
                switchDevice(HEADPHONES);
                break;
            case unplugged_active:
                switchDevice(SPEAKERS);
                break;
        }
		sleep(1);
	}
}
