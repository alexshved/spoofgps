#include "header/header.h"
#include <time.h>



void MakeRinexFileOld(char* FileName)
{
    char *month[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL",
        "AUG", "SEP", "OCT", "NOV", "DEC" };
    char *rinex = malloc(1024*10);
    memset(rinex,'\0',1024*10);
    strcat(rinex,"     2              NAVIGATION DATA                         RINEX VERSION / TYPE\n");
    strcat(rinex,"CCRINEXN V1.6.0 UX  CDDIS               ");
   	time_t s_time;
    struct tm m_time;
    s_time = time(NULL);
    localtime_s(&m_time,&s_time);
      
    int n = m_time.tm_mday;
    if (n < 10)
      strcat(rinex," ");
   IntToStr(rinex,n);
   strcat(rinex,"-");

    n = m_time.tm_mon;
    strcat(rinex,month[n]);
    strcat(rinex,"-");
    n = m_time.tm_year - 100;
    IntToStr(rinex,n);
    strcat(rinex," ");
    
    n = m_time.tm_hour;
    IntToStr(rinex,n);
    strcat(rinex,":");
    n = m_time.tm_min;
    if (n < 10)
        strcat(rinex,"0");
    IntToStr(rinex,n);    
   
    // while (strlen(rinex) < 19)
    //     strcat(rinex," "); 
    strcat(rinex,"     PGM / RUN BY / DATE\n");     
    strcat(rinex,"IGS BROADCAST EPHEMERIS FILE                                COMMENT             \n");  
    char buf_log[512];
     memset(buf_log,'\0',512);
    strcpy(buf_log,"Create rinex old: ");
   
    //  "-b 16 -s " + sampling.ToString() + " -r " + sampling.ToString() + " -f " + num9.ToString() + " -a 1 -x " + gain.ToString() + " -v -R 10 -Z 106 -G -21 -P " + str + " -B " + rateGenaA.ToString() + " -D " + num1.ToString() + " -K " + correction.ToString() + " -L " + leapSecond.ToString() + (Gena.GenaCmdUseGpsDou ? " -J" : "") + " -V " + Gena.GenaCmdLogLevel.ToString() + " -S " + num2.ToString() + " -N " + num3.ToString() + " -M " + num4.ToString() + " -X " + num6.ToString() + " -Y " + num7.ToString() + " -A " + num8.ToString() + " -u " + num5.ToString() + " -O -W" + brdcCheckInterval.ToString() + " ";


    if (satelitelist->size > 3) {
        // AnsiString us;
        // s = "   ";
		TSatelite* sat;
		for(int i = 0; i < satelitelist->size; i++){
			sat = satelitelist->satelite[i];
			double sr = 0;
			for (int c = 0; c < 4; ++c) {
			    sr += sat->data->ION_ALFA[c];
			}
			if(sr > 0)
                break;
		}
        int z = 4;
        strcat(rinex,"   ");
        for (int i = 0; i < 4; ++i) {
            //if( sat->data->ION_ALFA[i] >= 0) us = " "; else us = "";
             z += FloatToStrROld(rinex,sat->data->ION_ALFA[i], 4);
             strcat(rinex," ");
             z+=2;   
			
        }
        while (z < 65){
            strcat(rinex," ");
            z++;
        }
        strcat(rinex,"ION ALPHA");
        while (z < 76){
            strcat(rinex," ");
            z++;
        }
        strcat(rinex,"\n");  
        strcat(rinex,"   ");      
        z=4;
        for (int i = 0; i < 4; i++) {
             z += FloatToStrROld(rinex,sat->data->ION_BETA[i], 4);
             strcat(rinex," ");
             z+=2;  
        }
        while (z < 65){
             strcat(rinex," ");
            z++;
        }
       
        strcat(rinex,"ION BETA");
        while (z < 76){
            strcat(rinex," ");
            z++;
        }
         z=0;
        strcat(rinex,"\n"); 
        strcat(rinex,"   ");
        int zz = 4;
        zz += FloatToStrROld(rinex,sat->data->A0, 12);
        zz += FloatToStrROld(rinex,sat->data->A1, 12);
        while (zz < 45){
            strcat(rinex," ");
            zz++;
        }
        zz += IntToStr(rinex,(int)sat->data->TOT);
         while (zz < 56){
            strcat(rinex," ");
            zz++;
         }
        z += IntToStr(rinex,(int)sat->data->sf1_WN+2048);
        strcat(rinex," ");
        z++;
		strcat(rinex,"DELTA-UTC: A0,A1,T,W\n");
        strcat(rinex,"\n");
        z=0;  
        zz=0;
        strcat(rinex,"    ");
        z += IntToStr(rinex,(int)sat->data->sf4_18_delta_t_ls);
		z += 4;
       while (z < 60){
            strcat(rinex," ");
            z++;
        }
        z = 0;
         strcat(rinex,"LEAP SECONDS");
          strcat(rinex,"        \n");
       
    }
    
    // - - - - - - - - - - - - - - - -
    int z = 0;
    while (z < 60){
         strcat(rinex," ");
         z++;
    }
     strcat(rinex,"END OF HEADER");
   strcat(rinex,"        \n");
    for(int k = 0; k < satelitelist->size; k++){
    for (int i = 0; i < satelitelist->size; ++i) {
        
        // - 1 - - - - - - - - - - - - - - - - - - - - -
		TSatelite* sat = satelitelist->satelite[i];
		if(sat->id == 0 || satelite[k] != sat->id)
			continue;
		if(!checkDataSatelite(sat))
            continue;
        if (sat->id < 10)
            strcat(rinex," ");
         IntToStr(rinex,sat->id);
        IntToStr(buf_log,sat->id);
        strcat(buf_log," ");
		 __time64_t s_time_s = sat->data->WN*604800 + sat->data->TOC + 315964782;
         struct tm *m_time;
        m_time = gmtime(&s_time_s);
        int w = m_time->tm_year-100;
         strcat(rinex," ");
        IntToStr(rinex,w);
        strcat(rinex, " ");
        w = m_time->tm_mon+1;
        if (w < 10){
            strcat(rinex," ");
        }
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_mday;
        if (w < 10)
           strcat(rinex," ");
        IntToStr(rinex,w);
       strcat(rinex," ");
        w = m_time->tm_hour;
        if (w < 10)
            strcat(rinex," ");
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_min;
        if (w < 10)
            strcat(rinex," ");
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_sec;
        strcat(rinex," 0.0");
        FloatToStrROld(rinex,sat->data->AF0, 12);
        FloatToStrROld(rinex,sat->data->AF1, 12);
        FloatToStrROld(rinex,sat->data->AF2, 12);
        strcat(rinex,"\n");
		// - 2 - - - - - orbit 1 - - - - - - - - - - - - - - - -
		
        strcat(rinex,"   ");
		FloatToStrROld(rinex,sat->data->IODE, 12);
        
		FloatToStrROld(rinex,sat->data->CRS, 12);
       
		FloatToStrROld(rinex,sat->data->DELTA_N, 12);
       
		FloatToStrROld(rinex,sat->data->M0, 12);
		strcat(rinex,"\n");

        // - 3 - - - - - orbit 2 - - - - - - - - - - - - - - - -
        strcat(rinex,"   ");
        FloatToStrROld(rinex,sat->data->CUC, 12);
        
        FloatToStrROld(rinex,sat->data->E, 12);
        
        FloatToStrROld(rinex,sat->data->CUS, 12);
       
        FloatToStrROld(rinex,sat->data->SQRT_A, 12);
        strcat(rinex,"\n");

        // - 4 - - - - - orbit 3 - - - - - - - - - - - - - - - -
        strcat(rinex,"   ");
        FloatToStrROld(rinex,sat->data->TOE, 12);
        
        FloatToStrROld(rinex,sat->data->CIC, 12);
        
        FloatToStrROld(rinex,sat->data->OMEGA_0, 12);
        
        FloatToStrROld(rinex,sat->data->CIS, 12);
        strcat(rinex,"\n");

        // - 5 - - - - - orbit 4 - - - - - - - - - - - - - - - -
       strcat(rinex,"   ");
       FloatToStrROld(rinex,sat->data->I0, 12);
      
       FloatToStrROld(rinex,sat->data->CRC, 12);
      
       FloatToStrROld(rinex,sat->data->omega, 12);
       
       FloatToStrROld(rinex,sat->data->OMEGA_DOT, 12);
       strcat(rinex,"\n");
        // - 6 - - - - - orbit 5 - - - - - - - - - - - - - - - -
        strcat(rinex,"   ");
        FloatToStrROld(rinex,sat->data->IDOT, 12);
        
		FloatToStrROld(rinex,sat->data->sf1_ca_or_p_on, 12);
       

		FloatToStrROld(rinex,sat->data->sf1_WN+2048, 12);
        
		FloatToStrROld(rinex,sat->data->sf1_dfl2pc, 12);
		strcat(rinex,"\n");

        // - 7 - - - - - orbit 6 - - - - - - - - - - - - - - - -
        strcat(rinex,"   ");
        FloatToStrROld(rinex,sat->data->sf1_SV_Accuracy, 12);
        
        FloatToStrROld(rinex,sat->data->sf1_SV_Health, 12);
        
        FloatToStrROld(rinex,sat->data->TGD, 12);
      
        FloatToStrROld(rinex,sat->data->sf1_IODC, 12);
        strcat(rinex,"\n");

        // - 8 - - - - - orbit 7 - - - - - - - - - - - - - - - -
        strcat(rinex,"   ");
        FloatToStrROld(rinex,0, 12);
        
        FloatToStrROld(rinex,0, 12);
        FloatToStrROld(rinex,0, 12);
        FloatToStrROld(rinex,0, 12);
        strcat(rinex,"\n");
    }
    }
	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    hFind = FindFirstFile(".\\RINEX\\OLD", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
        CreateDirectory(".\\RINEX\\OLD",NULL);
    }else
        FindClose(hFind);
	FILE *f_o = NULL;
    char filen[1024];
    memset(filen,'\0',1024);
    sprintf(filen,".\\RINEX\\2\\%s",FileName);
    f_o = fopen(filen, "w");
    fprintf(f_o,"%s",rinex);
    fclose(f_o);
    writeLog("create_rinex.txt", buf_log,"RINEX"); 
    hFind = FindFirstFile("C:\\hgenerator_04", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
        CreateDirectory("C:\\hgenerator_04",NULL);
    }else
        FindClose(hFind);
     hFind = FindFirstFile("C:\\hgenerator_04\\brdc", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE){
        CreateDirectory("C:\\hgenerator_04\\brdc",NULL);
    }else
        FindClose(hFind);   
    f_o = fopen("C:\\hgenerator_04\\brdc\\mode0.brdc", "w");
    fprintf(f_o,"%s",rinex);
    fclose(f_o);  
    free(rinex);    
	

   
}


int FloatToStrROld(char* rezult, double n, int digit)
{
   
    char temp[24];
    char temp1[24];
    memset(temp,'\0',24);
    memset(temp1,'\0',24);
    char test11[24];
    memset(test11,'\0',24);
    if(n>=0)
        temp1[0] = ' ';
       else 
        temp1[0] = '-';
      temp1[1] = '0';
      temp1[2] = '.';    
     if(digit == 12){
        sprintf(temp, "%.11E", n*10);
        // sprintf(test11, "%.11E", n);
     }else{
        sprintf(temp, "%.3E", n*10);
     }
           
           
            
	for(int i = 0, k = 3; i<24 && k < 24;i++){
        if(temp[i] != '.' && (temp[i] != '-' || i > 4)){
            temp1[k] = temp[i];
            k++;
        }
    }
    for(int i=0; i<strlen(rezult); i++){
        if(temp1[i]=='E'){
            temp1[i] = 'D';
            break;
        }
    }
    strcat(rezult,temp1);
   
    return strlen(temp1);
}

