typedef struct {
    char *oxygen_file;
    char *headphones;
    char *speakers;
    int state_location;
    int state_length;
    int mask;
} Config_T;

Config_T *Make_Config_T();
Config_T *Get_Config_T(char *file);
int Write_Config_T(Config_T *conf, char filename[]);
void Destroy_Config_T(Config_T *conf);
