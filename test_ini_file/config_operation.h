#ifndef CONFIG_OPERATION_H
#define CONFIG_OPERATION_H

/* 读写的Key必须存在。 */
int get_ini_key_string(const char* title, const char* key, char* buf, const char* filename);
int put_ini_key_string(const char* title, const char* key, char* val, const char* filename);

/*
 * 读写的Key必须存在。因此上需要使用下面的函数预先生成文件内容。
 */
int create_ini_file(const char* title, int iKeyCount, const char* filename);
int append_ini_file(const char* title, int iKeyCount, const char* filename);

#endif
