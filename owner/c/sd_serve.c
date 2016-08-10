#include "all.h"
#include "string.h"
#include "stdio.h"
#include "ff.h"
#include "sd_serve.h"


static const char mon_name[12][3] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static int get_mon(char *date)
{
	int i;
	for(i=0; i<12; i++)
		if(strncmp(date, mon_name[i], 3)==0)
			return ++i;
	return 0;
}
static int get_mday(char date[])
	{return (date[4]-'0')*10 + (date[5]-'0');}
static int get_year(char date[])
	{return (date[7]-'0')*1000 + (date[8]-'0')*100 + (date[9]-'0')*10 + (date[10]-'0');}
static int get_hour(char time[])
	{return (time[0]-'0')*10 + (time[1]-'0');}
static int get_min(char time[])
	{return (time[3]-'0')*10 + (time[4]-'0');}
static int get_sec(char time[])
	{return (time[6]-'0')*10 + (time[7]-'0');}
 
FRESULT set_timestamp (
    char *obj,     /* Pointer to the file name */
    int year,
    int month,
    int mday,
    int hour,
    int min,
    int sec
)
{
    FILINFO fno;

    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno);
}

FRESULT f_timestamp ( char *obj )    /* Pointer to the file name */
{
    int year  = get_year(__DATE__);
    int month = get_mon(__DATE__);
    int mday  = get_mday(__DATE__); 
    int hour  = get_hour(__TIME__);
    int min   = get_min(__TIME__);
    int sec   = get_sec(__TIME__);
    FILINFO fno;

    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno);
}


/*?????????n???*/
char * left(char *dst,char *src, int n)
 {
     char *p = src;
     char *q = dst;
     int len = strlen(src);
     if(n>len) n = len;
     /*p += (len-n);*/   /*????n?????*/
     while(n--) *(q++) = *(p++);
     *(q++)='\0'; /*?????????*/
     return dst;
 }
 

void f_nameDeal(char *p_char)
{
	
 	while(*p_char != 0)
 	{
 		if(*p_char==' ' || *p_char==':' || *p_char=='-' || *p_char=='(' || *p_char==')')
 			*p_char = '_'	;
        p_char++;
 	}
}



void f_namecrt(const char *f_name)
{
    static char buf[128] = {0};
    int i=0;

    for(i=0; i<sizeof(buf)/sizeof(buf[0]); i++)
        buf[i]=0;

//    sprintf(buf, "%s_%d.txt", f_name, rt_tick_get()/200);
    sprintf(buf, "%s_%d.txt", f_name, 0);
    f_nameDeal(&buf[0]);
}


FRESULT res;
void set_dir(void)
{
    
    FILINFO fno={0};
    int i=0;
    DIR dir;
    char buf[50]={0};
    
    while(1)
    {
        //sprintf(buf, "%s_%3d", __DATE__, i);
        sprintf(buf, "%s", __DATE__);
//        left(buf,buf,6);
        sprintf(buf, "%s_%2d", buf, i);
        f_nameDeal(buf);
        res = f_stat(buf, &fno);
        if (res!=FR_OK)
            break;
        i++;
    }
//     sprintf(buf, "%s_%3d", __DATE__, i);
//     f_nameDeal(buf);
    res = f_mkdir((const char *)buf);
    res = f_opendir(&dir, (const char *)buf);
    res = f_closedir(&dir);
    res = f_timestamp((char *)buf);
    res = f_chdir((const char *)buf);
}

void sd_test(void)
{
    SD_Initialize();
    f_mount_on();
    set_dir();
    
    res = f_mkdir("Feb_14_2016_0");
    res = f_mkdir("sub1");
   res = f_mkdir("sub1/sub2");
   res = f_mkdir("sub1/sub2/sub3");

    file_open("1.txt");
    f_printf(&fil,"%s","hello world");
    file_close();
    f_timestamp("1.txt");
    f_mount_off();

}
 
