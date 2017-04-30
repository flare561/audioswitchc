#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"

int main(void)
{
    Config_T *conf = Get_Config_T("./audioswitch.conf");
    printf("%s\n%s\n%s\n%d\n%d\n%d\n",
            conf->headphones, conf->speakers, conf->oxygen_file,
            conf->state_location, conf->state_length, conf->mask);
    Destroy_Config_T(conf);
}

