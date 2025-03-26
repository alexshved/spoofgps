#ifndef VENSERVER_USRP_H
#define VENSERVER_USRP_H
#include <winusb.h>

#define CY_VID_PID 
int initUsd();
DWORD WINAPI startUSRP(Args_P_t *ar_arg);
DWORD WINAPI readUSRP(Args_P_t *ar_arg);
DWORD WINAPI gpssim(Args_P_t *ar_arg);
DWORD WINAPI startGpssim(Args_P_t *ar_arg);
extern char *optarg;
// #define TEST
#endif