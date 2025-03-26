#include "header/header.h"
#include <error.h>
DCB  dcb;
COMMTIMEOUTS 	lpCommTimeouts;
bool Opened = false;
HANDLE ComPort = INVALID_HANDLE_VALUE;
TReadState State;
BYTE buff[100];
HANDLE openPort(char* port, int speed){
    // L"\\\\.\\COM5"
    ComPort = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,	FILE_FLAG_OVERLAPPED, NULL);
        if( ComPort == INVALID_HANDLE_VALUE) {
             printf("COM %s is not open\n",port); 
              printf("error %d\n",GetLastError());
            return NULL;  
        }else{
            printf("COM %s is open\n",port);
            FillMemory(&dcb, sizeof(dcb), 0);
            if(!GetCommState(ComPort, &dcb)){
               printf("Can read DCB\n",port); 
              printf("error %d\n",GetLastError()); 
              return NULL;     
            }else{
                dcb.BaudRate = speed;
             	dcb.ByteSize = 8;
                dcb.Parity = NOPARITY;
                dcb.StopBits = ONESTOPBIT;
                if (!SetCommState(ComPort, &dcb)) {
                    printf("Can't write DCB");
                     return NULL; 
                }
                GetCommTimeouts(ComPort, &lpCommTimeouts);
                lpCommTimeouts.ReadIntervalTimeout = 20000;
                lpCommTimeouts.ReadTotalTimeoutMultiplier =  2000; 
                lpCommTimeouts.ReadTotalTimeoutConstant = 5000;
                lpCommTimeouts.WriteTotalTimeoutMultiplier = 10;
                lpCommTimeouts.WriteTotalTimeoutConstant = 100;
                if (SetCommTimeouts(ComPort, &lpCommTimeouts) == 0) {
		            printf("Can't SetCommTimeouts ");
                     return NULL;
                }
                PurgeComm(ComPort, PURGE_TXCLEAR);
                PurgeComm(ComPort, PURGE_RXCLEAR);
                Opened = true;
            }
            
            return ComPort;
        }  
    return NULL;         
            
}



int writePort(HANDLE *port, LPCVOID buff, DWORD n)
{
	
	DWORD nw = 0;
	OVERLAPPED Overlapped = { 0 };
	Overlapped.Offset = 0;
	Overlapped.OffsetHigh = 0;
	Overlapped.hEvent = NULL;
    const int READ_TIME = 100;
    unsigned long wait = 0, read = 0, state = 0;
	Overlapped.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
   
        WriteFile(*port, buff, n, &nw, &Overlapped);
        wait = WaitForSingleObject(Overlapped.hEvent, READ_TIME);
        if(wait == WAIT_OBJECT_0) 
            if (!GetOverlappedResult(*port, &Overlapped, &nw, true)) {
                printf("ERROR read comport");
                return NULL;
            }
    
    
	return 0;
}

void PurgeComBuff(){
	PurgeComm(ComPort, PURGE_TXCLEAR);
	PurgeComm(ComPort, PURGE_RXCLEAR);
}
TSateliteList *satelitelist;
void crateRinex(Args_P_t *ar_arg){
   

    if(openPort(ar_arg->comport,ar_arg->sped_port)){
        writeLogMain("main_log.txt", "Start read port" );
        satelitelist = malloc(sizeof(TSateliteList));
        satelitelist->satelite = calloc(24,sizeof(TSatelite*));
        satelitelist->fripos = 0;
        satelitelist->size = 0;
        
        unsigned char* buf = (unsigned char*)malloc(1024*2);
      
        BYTE marker[4] = { 0xB5, 0x62, 0x02, 0x13 };
        int pos = 0;
        PurgeComBuff();
        State = rsWait;
        int packlen = 0;
       char bufframe[24];
        while(true){
           
            BYTE b;
            int result  =  readPort(&b,1);
           
            if (result < 1) {
                usleep(20);
                continue;
            }
            if (State == rsWait) {
                if (b != marker[pos]) {
                    pos = 0;
                    continue;
                } else {
                    buff[pos] = b;
                    pos++;
                    if (pos >= 4) {
                        State = rsRead;
                    }
                    continue;
                }
            } else if (State == rsRead) {
                buff[pos] = b;
                pos++;
                if (pos == 7) {
                    WORD* n = (WORD*)&buff[4];
                    packlen = n[0];
                    packlen += 8;
                }
               			
                if ((pos >= 100) || ((packlen > 0) && (pos >= packlen))) {
                    pos = 0;
					State = rsWait;
					// checkRinexCreate();
					if (packlen == 56) {
						unsigned char data_h[52];
                        unsigned char data_r[512];
                        memset(data_r,'\0',512);
                        data_r[0];
                        for (int i = 0; i < packlen; ++i) {
                           
							ByteToHexStr(data_r,buff[i]);
							if(i>1 && i<55)
								data_h[i-2] = buff[i];

						}
                         printf("Recive subframe %s ",data_r);

						TSubframe *sb = malloc(sizeof(TSubframe));
                        memcpy(sb->data, buff, 56);
                        sb->id = -1;
                        sb->page = -1;
                        sb->satelite = -1;
                        sb->word = NULL;
                        writeLogMain("main_log.txt", "Create subframe" );
                        CalcParam(sb);
						AddSubFrame(sb);
                        checkRinexCreate();
					}
					packlen = 0;
					continue;
                }
            }

             usleep(200);
		

        }
        // fclose(df);
        writeLogMain("main_log.txt", "Close port" );
        free(buf);
        CloseHandle(ComPort);
        clearListSatelite();
        
    }
}

void clearListSatelite(){
    for(int i = 0; i < satelitelist->size; i++){
        for(int k; k < 42; k++){
            for(int c = 0; c<6;c++){
                if(satelitelist->satelite[i]->frame[k]->subframe[c]){
                    free(satelitelist->satelite[i]->frame[k]->subframe[c]); 
                }
            }
           free(satelitelist->satelite[i]->frame[k]->subframe);
           if(satelitelist->satelite[i]->frame[k] != NULL)
             free(satelitelist->satelite[i]->frame[k]); 
        }
        free(satelitelist->satelite[i]->data);
        free(satelitelist->satelite[i]->frame);
        if(satelitelist->satelite[i] != NULL)
            free(satelitelist->satelite[i]);
    }
    free(satelitelist->satelite);
    free(satelitelist);
}

int ByteToHexStr(char *buf, BYTE n){
    char rezult[24];
    memset(rezult,'\0',24);
    char buff[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
        'B', 'C', 'D', 'E', 'F' };
    BYTE b1, b2 = 0;
    b1 = (n >> 4) & 0x0F;
    b2 = n & 0x0F;
    sprintf(rezult,"%c%c",buff[b1],buff[b2]);
    strcat(buf,rezult);
    return strlen(rezult);
}

int readPort(BYTE * buff, int n){
    if (!Opened)
		return 0;
     
	DWORD nw;
	OVERLAPPED Overlapped = { 0 };
	Overlapped.Internal = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.Offset = 0;
	Overlapped.OffsetHigh = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.hEvent = NULL;
    
	// if (os_NT) {
		ReadFile(ComPort, buff, n, &nw, &Overlapped);
		// WaitForSingleObject(ComPort, INFINITE);
		if (!GetOverlappedResult(ComPort, &Overlapped, &nw, true)) {
			printf("ERROR read comport");
            return 0;
        }
    PurgeComm(ComPort, PURGE_TXCLEAR);    
    // } else
    //     ReadFile(ComPort, buff, n, &nw, NULL);

    if (nw < n) {
        printf("-> Read %i %i\n",(int)nw,(int)n);
        return (0);
    }
    return (nw);
}         