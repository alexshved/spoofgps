#ifndef VENSERVER_COM_H
#define VENSERVER_COM_H
#include <windows.h>
HANDLE openPort(char*,int);
int readPort(BYTE *, int);
void PurgeComBuff();
int writePort(HANDLE*, LPCVOID , DWORD);
int ByteToHexStr(char*,BYTE);
typedef enum tReadState {
    rsWait, 
    rsStart, 
    rsRead
    } TReadState;
#endif