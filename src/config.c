#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

Config_T *Make_Config_T()
{
    // Allocate the config variable and initialize
    // buffers for config strings
    Config_T *conf = malloc(sizeof(Config_T));
    conf->speakers = calloc(255, sizeof(char));
    conf->oxygen_file = calloc(255, sizeof(char));
    conf->headphones = calloc(255, sizeof(char));
    return conf;
}

Config_T *Get_Config_T(char *file)
{
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        return NULL;
    }

    Config_T *conf = Make_Config_T();
    // Read the config file into conf
    char keybuff[20];
    char valbuff[255];
    int count;
    for (count = 0;;count++) {
        int ret = fscanf(fp, "%s %255[^\n]\n",keybuff, valbuff);
        if (ret < 0) {
            break;
        }
        if (strcmp(keybuff, "OXYGEN_FILE") == 0) {
            strcpy(conf->oxygen_file, valbuff);
        }
        if (strcmp(keybuff, "SPEAKERS") == 0) {
            strcpy(conf->speakers, valbuff);
        }
        if (strcmp(keybuff, "HEADPHONES") == 0) {
            strcpy(conf->headphones, valbuff);
        }
        if (strcmp(keybuff, "STATE_LOCATION") == 0) {
            conf->state_location = (int)strtol(valbuff, NULL, 10);
        }
        if (strcmp(keybuff, "STATE_LENGTH") == 0) {
            conf->state_length = (int)strtol(valbuff, NULL, 10);
        }
        if (strcmp(keybuff, "MASK") == 0) {
            conf->mask = (int)strtol(valbuff, NULL, 10);
        }
    }
    fclose(fp);
    if (count < 6) {
        Destroy_Config_T(conf);
        errno = EINVAL;
        fprintf(stderr, "Invalid configuration");
        return NULL;
    }
    return conf;
}

int Write_Config_T(Config_T *conf, char filename[])
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "OXYGEN_FILE %s", conf->oxygen_file);
    fprintf(fp, "SPEAKERS %s", conf->speakers);
    fprintf(fp, "HEADPHONES %s", conf->headphones);
    fprintf(fp, "STATE_LOCATION %d", conf->state_location);
    fprintf(fp, "STATE_LENGTH %d", conf->state_length);
    fprintf(fp, "MASK %d", conf->mask);
    printf("%s\n", conf->oxygen_file);
    return 0;
}

void Destroy_Config_T(Config_T *conf)
{
    free(conf->oxygen_file);
    free(conf->speakers);
    free(conf->headphones);
    free(conf);
}
