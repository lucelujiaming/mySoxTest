#include <stdio.h>
#include <string.h>

#include "config_operation.h"

#define CONFIG_FILE_NAME     "config_test.ini"

int main(int argc, char ** argv) 
{
    int iRet = 0;
    
    if(argc < 2) 
    {
        printf("./test_ini_file <INIT/ADD/GET/SET/DEL> <Title> <Key> <Value>\n");
        return 1;
    }
    if(strcmp(argv[1], "INIT") == 0)
    {
       create_ram_in_inifile("HReg", 5, CONFIG_FILE_NAME);
       append_ram_in_inifile("PReg", 5, CONFIG_FILE_NAME);
    }
    else if(strcmp(argv[1], "ADD") == 0)
    {
        if(argc < 5) 
        {
            printf("./test_ini_file <ADD> <Title> <Key> <Value>\n");
            return 1;
        }
        iRet = append_ini_key_string(argv[2], argv[3], argv[4], CONFIG_FILE_NAME);
        printf("iRet = %d.\n", iRet);
    }
    else if(strcmp(argv[1], "GET") == 0)
    {
        if(argc < 4) 
        {
            printf("./test_ini_file <GET> <Title> <Key>\n");
            return 1;
        }
        char newValue[20] = { 0 };
        iRet = get_ini_key_string(argv[2], argv[3], newValue, CONFIG_FILE_NAME);
        if(iRet == -1)
        {
            printf("Not Found\n");
        }
        else 
        {
            printf("[%s:%s:%d] newValue=%s\n",  
                            __FILE__, __FUNCTION__, __LINE__, newValue);
        }
    }
    else if(strcmp(argv[1], "SET") == 0)
    {
        if(argc < 5) 
        {
            printf("./test_ini_file <SET> <Title> <Key> <Value>\n");
            return 1;
        }
        iRet = set_ini_key_string(argv[2], argv[3], argv[4], CONFIG_FILE_NAME);
    }
    else if(strcmp(argv[1], "DEL") == 0)
    {
        if(argc < 4) 
        {
            printf("./test_ini_file <SET> <Title> <Key>\n");
            return 1;
        }
        iRet = delete_ini_key_string(argv[2], argv[3], CONFIG_FILE_NAME);
    }
        
    return iRet;
}

//    // insert_ini_key_string
//    strncpy(value, "321", 4);
//    iRet = append_ini_key_string("HReg", "FFF", value, CONFIG_FILE_NAME);
//    strncpy(value, "321", 4);
//    iRet = append_ini_key_string("HReg", "FFF", value, CONFIG_FILE_NAME);
//    strncpy(value, "321", 4);
//    iRet = append_ini_key_string("HReg", "GGGG", value, CONFIG_FILE_NAME);
//    // insert_ini_key_string
//    strncpy(value, "321", 4);
//    iRet = append_ini_key_string("PReg", "FFF", value, CONFIG_FILE_NAME);
//    strncpy(value, "321", 4);
//    iRet = append_ini_key_string("PReg", "FFF", value, CONFIG_FILE_NAME);
//    strncpy(value, "321", 4);
