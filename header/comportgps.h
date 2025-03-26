#ifndef COMPORTGPS_H
#define COMPORTGPS_H
HANDLE openPortGPS(char * port, int);
int readPortGPS(HANDLE*, char *, int);
extern HANDLE ComPortGPS;
extern bool OpenedGPS;
DWORD WINAPI readCordinates(Args_P_t *);
void addSecond(datetime_t *, int);
#endif