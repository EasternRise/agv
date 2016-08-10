#ifndef __MYFATFS_H
#define __MYFATFS_H
#include <stm32f10x.h>
#include "all.h"
#include "MyTypeDef.h"
extern FIL fil;  
extern FATFS fs;
extern FRESULT fatfs_res;
extern UINT bw;
FRESULT Test_f_mkdir(const TCHAR* path);//创建一个新目录,输入目录的路径Test_f_mkdir("/dir");只能一级一级的建立目录	
FRESULT Test_f_deldir(const TCHAR* path);//删除一个目录,输入目录的路径
FRESULT Test_f_write(char *dat);//写入字符
void write_double(double data);//写入double数
void write_float(float data);
void f_mount_off(void );
void f_mount_on( void);
void file_open(const TCHAR *path);
void file_close(void);
void SdTest(void);
#endif
