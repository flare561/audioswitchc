#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"

int main(void)
{
    Config *conf = Get_Config("./audioswitch.conf");
    printf("%s %s %s %d %d %d",
            conf->headphones, conf->speakers, conf->oxygen_file,
            conf->state_location, conf->state_length, conf->mask);
    Destroy_Config(conf);
}

