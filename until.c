#include <string.h>
#include "header/header.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define NTH_BIT(n) (1 << n)

#define TEST_ANY_BITS(x, y) (((x) & (y)) != 0)
#define TEST_ALL_BITS(x, y) (((x) & (y)) == (y))

#define TEST_NTH_BIT(x, n) TEST_ANY_BITS(x, NTH_BIT(n))


double byteToDouble(BYTE temp){
    WORD data = temp;
     writeLogFrameData("omega_dot.txt",&data, 1,"data     ",2);
    int fl = 0;
     if(TEST_NTH_BIT(data,7))
        fl = 1;
    data <<= 1;        
    double rezult = (double)data;
    long long int *corect = (long long int*)&rezult;
    if(fl == 0){
       *corect &= 0x7fffffffffffffff;     
    }else{
       *corect |= 0x8000000000000000;
    }
    
   return rezult;
}

double wordToDouble(WORD temp){
    WORD data = temp;
     writeLogFrameData("omega_dot.txt",&data, 1,"data     ",2);
    int fl = 0;
     if(TEST_NTH_BIT(data,15))
        fl = 1;
    data <<= 1;        
    double rezult = (double)data;
    long long int *corect = (long long int*)&rezult;
    if(fl == 0){
       *corect &= 0x7fffffffffffffff;     
    }else{
       *corect |= 0x8000000000000000;
    }
    
   return rezult;
}


// void doubleToDate(double dt, char *buf, ArgGps_t *arggps){
//    memset(buf,'\0',24);
//    time_t gps = (time_t)dt;
//    struct tm *time_duo = gmtime(&gps);
//    arggps->t0.y = time_duo->tm_year+1900;
// 	arggps->t0.m = time_duo->tm_mon+1;
// 	arggps->t0.d = time_duo->tm_mday;
// 	arggps->t0.hh = time_duo->tm_hour;
// 	arggps->t0.mm = time_duo->tm_min;
// 	arggps->t0.sec = (double)time_duo->tm_sec;
//    arggps->t0.sec = floor(arggps->t0.sec);
// 	date2gps(arggps->t0, &arggps->g0);
//    // sprintf(buf,"%0.2i/%0.2i/%0.2i,%0.2i:%0.2i:%0.2i",time_duo->tm_year-100+2000,time_duo->tm_mon,time_duo->tm_mday,time_duo->tm_hour,time_duo->tm_min,time_duo->tm_sec);
// }


double dwordToDouble3(DWORD temp){
    DWORD data = temp;
    
    int fl = 0;
     if(TEST_NTH_BIT(data,7))
        fl = 1;    
    data <<= 1;
    data &= 0xff;
   
   
    double rezult = (double)data;
    long long int *corect = (long long int*)&rezult;
    if(fl == 0){
       *corect &= 0x7fffffffffffffff;     
    }else{
       *corect |= 0x8000000000000000;
    }
    char we[124];
    memset(&we,'\0',124);
    sprintf(we,"rezult %E ",rezult);
     writeLogFrameData("omega_dot.txt",&rezult, 1,we,8);
   return rezult;


}



double dwordToDouble2(DWORD temp){
    DWORD data = temp;
    
    int fl = 0;
     if(TEST_NTH_BIT(data,21))
        fl = 1;    
    data <<= 1;
    data &= 0x3fffff;
   
   
    double rezult = (double)data;
    long long int *corect = (long long int*)&rezult;
    if(fl == 0){
       *corect &= 0x7fffffffffffffff;     
    }else{
       *corect |= 0x8000000000000000;
    }
    char we[124];
    memset(&we,'\0',124);
    sprintf(we,"rezult %E ",rezult);
     writeLogFrameData("omega_dot.txt",&rezult, 1,we,8);
   return rezult;


}


double dwordToDouble(DWORD temp){
    DWORD data = temp;
    
    int fl = 0;
     if(TEST_NTH_BIT(data,23))
        fl = 1;    
    data <<= 2;
    data &= 0xffffff;
   
   
    double rezult = (double)data;
    long long int *corect = (long long int*)&rezult;
    if(fl == 0){
       *corect &= 0x7fffffffffffffff;     
    }else{
       *corect |= 0x8000000000000000;
    }
    char we[124];
    memset(&we,'\0',124);
    sprintf(we,"rezult %E ",rezult);
     writeLogFrameData("omega_dot.txt",&rezult, 1,we,8);
   return rezult;


}

void printDoubleBit(char *buf, char* value, int size){
    char*   bits = (char*)value; 
    int     array[size*8];
    char bf[5]; 

    for (int i = 0, k = size * 8 - 1; i < size; ++i) {
        for (char j = 0; j < 8; ++j, --k){
            array[k] = (bits[i] >> j) & 1; 
        }
    }
        

    for(int i = 0; i< size*8; i++){
            memset(bf,'\0',5);
            sprintf(bf,"%i",array[i]);
            strcat(buf,bf); 
    }     
            //  strcat(buf,'\0'); 

}

void writeLogMain(char* filename, char* data ){
    char bufp[512];
    memset(bufp,'\0',512);
    sprintf(bufp,"Event %s ",data);
    writeLog(filename, bufp,"MAIN");
}

void writeLogComon(char* filename, char* data, int satelite, int frame, char* coment){
    char bufp[512];
    memset(bufp,'\0',512);
    sprintf(bufp,"%s Satelite:%.2i Frame:%.2i %s",coment,satelite,frame,data);
    writeLog(filename, bufp,"MAIN");
}

void writeLogFrameData(char* filename, char* data, int satelite, char* coment, int size){
    char bufp[512];
    memset(bufp,'\0',512);
    sprintf(bufp,"%s Satelite:%.2i bit:",coment,satelite);
    printDoubleBit(bufp, data, size);
    writeLog(filename, bufp,"DATA");
}

void writeLog(char* filename, char* body, char * dir){
     BOOL bErrorFlag = FALSE;
    HANDLE hFind;
     WIN32_FIND_DATA FindFileData;
    char file[256];
    memset(file,'\0',256);
    sprintf(file,".\\LOG\\%s",filename);
    
    hFind = FindFirstFile(".\\LOG", &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE){
        CreateDirectory(".\\LOG",NULL);
        }else
            FindClose(hFind);
    
    memset(file,'\0',256);
    sprintf(file,".\\LOG\\%s",dir);
    hFind = INVALID_HANDLE_VALUE;
     hFind = FindFirstFile(file, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE){
        CreateDirectory(file,NULL);
        }else
            FindClose(hFind);

    memset(file,'\0',256);
    sprintf(file,".\\LOG\\%s\\%s",dir,filename);

    FILE  *f_o  = fopen(file, "a+");        
       
    time_t s_time;
    struct tm *m_time;
    s_time = time(NULL);
    m_time = localtime(&s_time);
    // char bufwrite[1024];
    // memset(bufwrite,'\0',1024);
    // int bytewrite;
    // sprintf(bufwrite,"Date %i:%i:%i %i:%i:%i message %s\n",m_time->tm_year-100, m_time->tm_mon+1, m_time->tm_mday, m_time->tm_hour, m_time->tm_min, m_time->tm_sec, body);
    fprintf(f_o, "Date %i:%.2i:%.2i %.2i:%.2i:%.2i message %s\n",m_time->tm_year-100, m_time->tm_mon+1, m_time->tm_mday, m_time->tm_hour, m_time->tm_min, m_time->tm_sec, body);
   
    fclose(f_o);
     
}

void initConfig(){
   memset(kx,'\0',11);
   memset(ky,'\0',11);
   memset(kz,'\0',11);
   FILE  *f_o  = fopen("c:\\hgenerator_04\\config.ini", "r");
   if(f_o != NULL){
      char buf[12];
      memset(buf,'\0',12);
      while((fgets(buf,12,f_o)) != NULL){
                 
         switch (buf[0])
		   {
         case 'x':
          strncpy(kx,&buf[2],9);
          break;
         case 'y':
            strncpy(ky,&buf[2],10);
            break;
         case 'z':
            strncpy(kz,&buf[2],4);
            break;
          case 'o':
            strncpy(ondul,&buf[2],4);
            break;   
         case 'h':
            kx_z = buf[2];
            break;
         case 'd':
            ky_z = buf[2];
            break;          
         default:
					
			break;  
         } 
        
      }
     fclose(f_o);
   }
}

bool checkDoExit(){
   FILE  *f_o  = fopen("c:\\hgenerator_04\\gps_control.txt", "r");
   if(f_o != NULL){
       char buf[100];
   memset(buf,'\0',100);
   fread(&buf,100,1,f_o);
   fclose(f_o);
   if(strstr(buf,"do_exit") != NULL){
      return true;
   }else
      return false;
   
   }else
      return  false; 
}

void paramprint(char** param, int cn){
   char *buf = malloc(sizeof(char)*20*cn);
   memset(buf,'\0',sizeof(char)*20*cn);
   for(int i=1;i<cn;i++){
       strcat(buf,param[i]);
        strcat(buf," ");
   }
   writeLogMain("param_start.txt", buf);
   printf("param start: %s\n",buf);
   free(buf);

};

void print_help(void){
    fprintf(stderr, "Example using UHD's\n\n"

                    "Options USRP:\n"
                    "    -a (device args)\n"
                    "    -f (frequency in Hz)\n"
                    "    -r (sample rate in Hz)\n"
                    "    -q (gain)\n"
                    "    -n (number of samples to receive)\n\n"
                    
                     "Options U-blox:\n"
                    "    -j speed com port\n"
                    "    -p COM port\n\n"
                  
                   
                    "Options gps-sdr-sim:\n"
                    "  -e <gps_nav>     RINEX navigation file for GPS ephemerides (required)\n"
		            "  -u <user_motion> User motion file in ECEF x, y, z format (dynamic mode)\n"
		            "  -x <user_motion> User motion file in lat, lon, height format (dynamic mode)\n"
		            "  -g <nmea_gga>    NMEA GGA stream (dynamic mode)\n"
		            "  -c <location>    ECEF X,Y,Z in meters (static mode) e.g. 3967283.154,1022538.181,4872414.484\n"
		            "  -l <location>    Lat, lon, height (static mode) e.g. 35.681298,139.766247,10.0\n"
		            "  -t <date,time>   Scenario start time YYYY/MM/DD,hh:mm:ss\n"
		            "  -T <date,time>   Overwrite TOC and TOE to scenario start time\n"
		            "  -d <duration>    Duration [sec] (dynamic mode max: %.0f, static mode max: %d)\n"
		            "  -o <output>      I/Q sampling data file (default: gpssim.bin)\n"
		            "  -s <frequency>   Sampling frequency [Hz] (default: 2600000)\n"
		            "  -b <iq_bits>     I/Q data format [1/8/16] (default: 16)\n"
		            "  -i               Disable ionospheric delay for spacecraft scenario\n\n"
		            
                   
                    "General options\n"
                    "    -k enable treads example 1234\n"
                    "       1 - create rinex\n"
                    "       2 - create data flow\n"
                    "       3 - send usrp\n"
                    "       4 - read data usrp\n"
                    "    -v (enable verbose prints)\n"
                    "    -h (print this help message)\n\n"   
                    ,((double)USER_MOTION_SIZE) / 10.0, STATIC_MAX_DURATION);
};