#include "header/header.h"

char kx[11];
char  kx_z = '\0';
char ky[11];
char  ky_z = '\0';
char kz[11];
char ondul[11];

int main(int argc, char** argv) {
   initConfig();
   paramprint(argv, argc);
   create_Thred(argv,argc);
   return 0;
}
