#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

Config *Get_Config(char *file)
{
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        return NULL;
    }
    // Allocate the config variable and initialize
    // buffers for config strings
    Config *conf = malloc(sizeof(Config));
    conf->speakers = calloc(255, sizeof(char));
    conf->oxygen_file = calloc(255, sizeof(char));
    conf->headphones = calloc(255, sizeof(char));

    // Read the config file into conf
    char keybuff[20];
    char valbuff[255];
    int count = 0;
    while (1) {
        int ret = fscanf(fp, "%s %255[^\n]\n",keybuff, valbuff);
        if (ret < 1) {
            break;
        }
        if (strcmp(keybuff, "OXYGEN_FILE") == 0) {
            strcpy(conf->oxygen_file, valbuff);
            count++;
        }
        if (strcmp(keybuff, "SPEAKERS") == 0) {
            strcpy(conf->speakers, valbuff);
            count++;
        }
        if (strcmp(keybuff, "HEADPHONES") == 0) {
            strcpy(conf->headphones, valbuff);
            count++;
        }
        if (strcmp(keybuff, "STATE_LOCATION") == 0) {
            conf->state_location = (int)strtol(valbuff, NULL, 10);
            count++;
        }
        if (strcmp(keybuff, "STATE_LENGTH") == 0) {
            conf->state_length = (int)strtol(valbuff, NULL, 10);
            count++;
        }
        if (strcmp(keybuff, "MASK") == 0) {
            conf->mask = (int)strtol(valbuff, NULL, 10);
            count++;
        }
    }
    fclose(fp);
    if (count < 6) {
        Destroy_Config(conf);
        errno = EINVAL;
        fprintf(stderr, "Invalid configuration");
        return NULL;
    }
    return conf;
}

int Write_Config(Config *conf, char filename[])
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

void Destroy_Config(Config *conf)
{
    free(conf->oxygen_file);
    free(conf->speakers);
    free(conf->headphones);
    free(conf);
}
