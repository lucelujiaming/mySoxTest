#include <stdio.h>
#include <string.h>

#include "config_operation.h"

int main(int argc, char ** argv) 
{
    int iRet = 0;

    if(argc == 1)
    {
       create_ini_file("HReg", 100, "test.ini");
       append_ini_file("PReg", 100, "test.ini");
    }

    char value[20] = { 0 };
    iRet = put_ini_key_string("AAA", "Config030", value, "test.ini");

    char newValue[20] = { 0 };
    iRet = get_ini_key_string("AAA", "Config030", newValue, "test.ini");
    printf("newValue=%s\n", newValue);

    if(argc == 3)
    {
        strncpy(value, argv[2], strlen(argv[2]));
        iRet = put_ini_key_string("HReg", argv[1], value, "test.ini");
        iRet = get_ini_key_string("HReg", argv[1], newValue, "test.ini");
        printf("newValue=%s\n", newValue);
    }
    else 
    {
        if(argc == 2)
        {
            strncpy(value, argv[1], strlen(argv[1]));
        }
        else
        {
            strncpy(value, "30", 2);
        }
        iRet = put_ini_key_string("HReg", "Config030", value, "test.ini");
        iRet = get_ini_key_string("HReg", "Config030", newValue, "test.ini");
        printf("newValue=%s\n", newValue);
    }
    iRet = get_ini_key_string("HReg", "Config040", newValue, "test.ini");
    printf("newValue=%s\n", newValue);
}
