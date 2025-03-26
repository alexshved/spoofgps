#include "header/header.h"
#include <synchapi.h>


DCB  dcbGPS;
COMMTIMEOUTS 	lpCommTimeoutsGPS;

bool OpenedGPS = false;
HANDLE openPortGPS(char * port, int speed){
    // printf("Star com port %s %i %0.0f\n",port,speed,(double)time(NULL));
    char *pp = port;
   HANDLE ComPortGPS = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	FILE_FLAG_OVERLAPPED, NULL);
        if( ComPortGPS == INVALID_HANDLE_VALUE) {
            return NULL;  
        }else{
           
            FillMemory(&dcbGPS, sizeof(dcbGPS), 0);
            if(!GetCommState(ComPortGPS, &dcbGPS)){
              return NULL;     
            }else{
                dcbGPS.BaudRate = speed;
             	dcbGPS.ByteSize = 8;
                dcbGPS.Parity = NOPARITY;
                dcbGPS.StopBits = ONESTOPBIT;
				if (!SetCommState(ComPortGPS, &dcbGPS)) {
                   return NULL; 
                }
                GetCommTimeouts(ComPortGPS, &lpCommTimeoutsGPS);
                lpCommTimeoutsGPS.ReadIntervalTimeout = 0;
                lpCommTimeoutsGPS.ReadTotalTimeoutMultiplier =  0; 
                lpCommTimeoutsGPS.ReadTotalTimeoutConstant = 0;
                lpCommTimeoutsGPS.WriteTotalTimeoutMultiplier = 0;
                lpCommTimeoutsGPS.WriteTotalTimeoutConstant = 0;
                if (SetCommTimeouts(ComPortGPS, &lpCommTimeoutsGPS) == 0) {
		           return NULL;
                }
                // PurgeComm(ComPortGPS, PURGE_TXCLEAR);
                PurgeComm(ComPortGPS, PURGE_RXCLEAR);
               
            }
             printf("Port %s is open %i\n",port,speed);
            return ComPortGPS;
        }  
    return NULL;         
            
}

HANDLE ComPortGPSRead = NULL;

DWORD WINAPI readCordinates(Args_P_t * arg){
    // printf("Start read COM\n");
#ifndef TEST_C
	ComPortGPSRead = openPortGPS(arg->arg_gps->umfile, arg->arg_gps->speedcom);
		if(ComPortGPSRead == NULL){
			arg->buf->fexit = 1;
			return -1;
		}
#endif
	double kj = 0;	
    while(true){
    
    if(arg->buf->fexit == 1)
        break;    
    
	
	   char str[MAX_CHAR];
	   memset(str,'\0',MAX_CHAR);
		lablewe:
        memset(arg->buf_crd,'\0',200);
 #ifndef TEST_C
		if(readPortGPS(&ComPortGPSRead,arg->buf_crd,200) == NULL){
				if(checkDoExit()){
						CloseHandle(ComPortGPSRead);
						arg->buf->fexit = 1;
						return 0;	  
					}
				// printf("Com no read\n");	
				usleep(100);
				goto lablewe;
			}
			
       
		if(strlen(arg->buf_crd) < 50){
            printf("Str < 50\n");
			usleep(10);
            goto lablewe;
        }else{
#else
	usleep(20000);
#endif					
					char *token;
					double llh[3],pos[3];
					char tmp[8];
					char tmp_t[6];
					memset(str,'\0',MAX_CHAR);
					// if(strlen(str) == 0){
						strncpy(str,arg->buf_crd,100);
					 	// printf("cordinates %s\n",str);
					// }
					
 #ifdef TEST_C					
					strcpy(str,"$GPGGA,073415.796,1723.7388,N,01621.7921,W,1,10,00.7,0021,M,0026,M,,*49");
#endif					
				// printf("coord %s",str);
			
				token = strtok(str, ",");
				
				if (strncmp(token+3, "GGA", 3)==0)
				{
					PurgeComm(ComPortGPSRead, PURGE_RXCLEAR);
					
					
					token = strtok(NULL, ","); 
					// time_t gps = time(NULL);
					// struct tm *time_duo =  gmtime(&gps);
					// arg->arg_gps->t0.y = time_duo->tm_year+1900;
					// arg->arg_gps->t0.m = time_duo->tm_mon+1;
					// arg->arg_gps->t0.d = time_duo->tm_mday;
					// arg->arg_gps->t0.hh = time_duo->tm_hour;
					// arg->arg_gps->t0.mm = time_duo->tm_min;
					// arg->arg_gps->t0.sec = time_duo->tm_sec;
					// memset(tmp_t,'\0',6);
					// strncpy(tmp_t,token,2); 
					// arg->arg_gps->t0.hh = atoi(tmp_t);
					// memset(tmp_t,'\0',6);
					// strncpy(tmp_t,token+2,2); 
					// arg->arg_gps->t0.mm = atoi(tmp_t);
					// memset(tmp_t,'\0',6);
					// strncpy(tmp_t,token+4,2); 
					// arg->arg_gps->t0.sec = atoi(tmp_t);
					// double sec_d = round(arg->arg_gps->t0.sec * 10) / 10; 
					// arg->arg_gps->t0.sec = sec_d;
					// addSecond(&arg->arg_gps->t0, arg->arg_gps->correction);
					date2gps(&(arg->arg_gps->t0), &arg->arg_gps->g0);
					
					token = strtok(NULL, ",");
					// printf("token 1 %i %s %s\n",strlen(kx),kx,token);
					if(strlen(kx) != 0){
						strncpy(tmp, kx, 2);
						tmp[2] = 0;
						llh[0] = atof(tmp) + atof(kx+2)/60.0;
					}else{
						
						strncpy(tmp, token, 2);
						tmp[2] = 0;
						
						llh[0] = atof(tmp) + atof(token+2)/60.0;
					}
					
					token = strtok(NULL, ","); 
					// printf("token 1.1 %c %c\n",kx_z,token[0]);
					if ((token[0]=='S' && kx_z == '\0') || kx_z == 'S'){
						llh[0] *= -1.0;	
					}
						

					llh[0] /= R2D;
					
					token = strtok(NULL, ",");
					// printf("token  2 %i %s %s\n",strlen(ky),ky,token);
					if(strlen(ky) != 0){
						strncpy(tmp, ky, 3);
						tmp[3] = 0;
						
						llh[1] = atof(tmp) + atof(ky+3)/60.0;
					}else{
						tmp[3] = 0;
						llh[1] = atof(tmp) + atof(token+3)/60.0;	
					}


					// token_temp = "1721.0000";
					// strncpy(tmp, token_temp, 3);
					strncpy(tmp, token, 3);
					

					token = strtok(NULL, ",");
					// printf("token 2.1 %c %c\n",ky_z,token[0]);
					if ((token[0]=='W'  && ky_z == '\0') || ky_z == 'W')
						llh[1] *= -1.0;

					llh[1] /= R2D; 

					token = strtok(NULL, ","); 
					token = strtok(NULL, ","); 
					token = strtok(NULL, ","); 

					token = strtok(NULL, ",");
					// printf("token z %s %s %.0f\n",token, kz,atof(kz));
					if(strlen(kz) != 0){
						llh[2] = atof(kz);
					}else{
						llh[2] = atof(token);
					}

					token = strtok(NULL, ","); 

					token = strtok(NULL, ",");
					// printf("token z %s %s\n",token,ondul);
					// llh[2] += atof(token);
					if(strlen(ondul) != 0){
						llh[2] += atof(ondul);
					}else{
						llh[2] += atof(token);
					}
					
					

					llh2xyz(llh, pos);
					
					arg->flag_buf = 1;
					// if(kj == 0){
					arg->xyz[0][0] = pos[0];
					arg->xyz[0][1] = pos[1];
					arg->xyz[0][2] = pos[2];
					memset(arg->buf_crd_back,'\0',200);
					strcpy(arg->buf_crd_back,arg->buf_crd);
					writeLogMain("GPG_read.txt", arg->buf_crd_back);
					// }
					

					
					kj++;
					arg->flag_buf = 0;
					
					// usleep(500);
					
				}else{

				}
				if(checkDoExit()){
						CloseHandle(ComPortGPSRead);
						arg->buf->fexit = 1;
						return 0;	  
					}
				usleep(10);
				// arg->flag_buf = 1;
				// arg->flag_buf = 0;
			// OpenedGPS = false;
			// fclose(fp);

#ifndef TEST_C	
            
        }
#endif		
			   
    }
    return 0;       
}

void addSecond(datetime_t *time, int sec){
	
	int s = time->sec + sec;
	if(s > 59){
		time->sec = s - 60;
		int m = time->mm + 1;
		if(m > 59){
			time->mm = 0;
			int h =	time->hh + 1;
			if(h > 23){
				time->hh = 0;
			}else
				time->hh = h; 

		}else
			time->mm = m;
	}else 
	time->sec = s;
}

int readPortGPS(HANDLE *ComPortGPS, char * buff, int n){
    // if (!OpenedGPS)
	// 	return NULL;
	DWORD nw = 0;
	OVERLAPPED Overlapped = { 0 };
	Overlapped.Internal = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.Offset = 0;
	Overlapped.OffsetHigh = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.hEvent = NULL;
    const int READ_TIME = 50;
    unsigned long wait = 0, read = 0, state = 0;
	Overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
    
    SetCommMask(*ComPortGPS, EV_RXCHAR);
    WaitCommEvent(*ComPortGPS, &state, &Overlapped); 
   
     
	// for(int i=0;i< 500; i++){
	wait = WaitForSingleObject(Overlapped.hEvent, READ_TIME);
	if(wait == WAIT_OBJECT_0) {
			 
		 	int start = 0, nread = 0;
			int isread = 0;
				goto lablewe;

			lablewe:
			while(start < 2 && nread < MAX_CHAR){
				char read = '0';
				// printf("Star read port  %0.0f\n",(double)time(NULL));
				if(ReadFile(*ComPortGPS, &read, 1, &isread, &Overlapped) == NULL){
				
					
					return 0;
				}
				if(read == '\n' || read == '\r' && start > 0){
					break;
				}  
				if(nread == 3 && read != 'G'){
					start = 0, 
					nread = 0;
					goto lablewe;
				}	
				if(read == '$' && start==1){
					// buff[nread] = '\n';
					break;
				}
						
				if(read == '$')
					start=1;
				if(start > 0)
					buff[nread] = read;	
				
				if(start == 1)
					nread++;	
			}
			// PurgeComm(ComPortGPS, PURGE_TXCLEAR);
                // PurgeComm(ComPortGPS, PURGE_RXCLEAR);
			return (nread);
		//  }else{
		// 	usleep(20);
		//  }
		 	
	}   
	
         

      
    return (0);
} 

