typedef struct {
    char *oxygen_file;
    char *headphones;
    char *speakers;
    int state_location;
    int state_length;
    int mask;
} Config;

Config *Get_Config(char *file);
Config Get_Default_Config(void);
int Write_Config(Config *conf, char filename[]);
void Destroy_Config(Config *conf);
