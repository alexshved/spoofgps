#include "header/header.h"
#include <time.h>
time_t last_rinex = 0;
int coint_sat = 0;
bool checkRinexCreate(){
	time_t s_time;
    struct tm *m_time;
    s_time = time(NULL);
    m_time = localtime(&s_time);
	  if(satelitelist->size > 3){

			int sz = 0;
            double k = 0;
			for (int i = 0; i < satelitelist->size; ++i) {
				for (int c = 0; c < 4; ++c) {
						if(satelitelist->satelite[i]->data->ION_ALFA[c])
							k += satelitelist->satelite[i]->data->ION_ALFA[c];
						 }
					if(checkDataSatelite(satelitelist->satelite[i]))
					  sz++;
				  }
				// || (double)( s_time - last_rinex ) > 60 
				if(k != 0 && sz > 1 && (coint_sat != sz )){
                    char filename[512];
                    memset(filename,'\0',512);
                    
                    char dirname[512];
                    memset(dirname,'\0',512);
                    sprintf(dirname,".\\RINEX\\2\\%i_%i_%i",m_time->tm_year,m_time->tm_mon,m_time->tm_mday);
                    sprintf(filename,"%i_%i_%i\\%i_%i_%i.txt",m_time->tm_year,m_time->tm_mon,m_time->tm_mday,m_time->tm_hour,m_time->tm_min,m_time->tm_sec);
                    WIN32_FIND_DATA FindFileData;
                    HANDLE hFind;
                    hFind = FindFirstFile(".\\RINEX", &FindFileData);
                    if (hFind == INVALID_HANDLE_VALUE){
                        CreateDirectory(".\\RINEX",NULL);
                    }else
                        FindClose(hFind);
                    hFind = INVALID_HANDLE_VALUE;       
                    hFind = FindFirstFile(".\\RINEX\\2", &FindFileData); 
                    if (hFind == INVALID_HANDLE_VALUE){
                        CreateDirectory(".\\RINEX\\2",NULL);
                    }else
                        FindClose(hFind);
                    hFind = INVALID_HANDLE_VALUE;       
                    hFind = FindFirstFile(".\\RINEX\\2_10", &FindFileData);
                    if (hFind == INVALID_HANDLE_VALUE){
                        CreateDirectory(".\\RINEX\\2_10",NULL);
                    }else
                        FindClose(hFind);    
                    hFind = INVALID_HANDLE_VALUE;   
                     
                    hFind = FindFirstFile(dirname, &FindFileData);
                    if (hFind == INVALID_HANDLE_VALUE){
                        CreateDirectory(dirname,NULL);
                    }else
                        FindClose(hFind); 
                    hFind = INVALID_HANDLE_VALUE;   
                    memset(dirname,'\0',512);    
                    sprintf(dirname,".\\RINEX\\2_10\\%i_%i_%i",m_time->tm_year,m_time->tm_mon,m_time->tm_mday);
                    if (hFind == INVALID_HANDLE_VALUE){
                        CreateDirectory(dirname,NULL);
                    }else
                        FindClose(hFind); 
                    MakeRinexFile(filename);
                    MakeRinexFileOld(filename);
					last_rinex = s_time;
					coint_sat = sz ;
				}


	  }


}

int IntToStr(char*buf,int i){
    char rezult[24];
    memset(rezult,'\0',24);
    sprintf(rezult,"%i",i);
    strcat(buf,rezult);
    return strlen(rezult);
}

int convert_utc_tm_to_time_t (){
   long int ttime;
   ttime = time(NULL);
   printf("Время: %s\n",ctime(&ttime) );
   return 0;
}
void MakeRinexFile(char* FileName)
{
    char *month[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL",
        "AUG", "SEP", "OCT", "NOV", "DEC" };
    char *rinex = malloc(1024*10);
    memset(rinex,'\0',1024*10);
    strcat(rinex,"     2.10           N: GPS NAV DATA                         RINEX VERSION / TYPE\n");
    strcat(rinex,"MYPROGRAM V.1.0     AIUB                ");
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
   
    while (strlen(rinex) < 20)
        strcat(rinex," "); 
    strcat(rinex,"      PGM / RUN BY / DATE\n");     
    strcat(rinex,"IGS BROADCAST EPHEMERIS FILE                                COMMENT\n");  
    char buf_log[512];
     memset(buf_log,'\0',512);
    strcpy(buf_log,"Create rinex new: ");
    if (satelitelist->size > 3) {
        // AnsiString us;
        // s = "   ";
		TSatelite* sat;
         for(int k = 0; k < satelitelist->size; k++){
            for(int i = 0; i < satelitelist->size; i++){
                sat = satelitelist->satelite[i];
                if(sat->id == 0 || satelite[k] != sat->id)
			        continue;
                double sr = 0;
                for (int c = 0; c < 4; ++c) {
                    sr += sat->data->ION_ALFA[c];
                }
                if(sr > 0)
                    break;
            }
         }
        int z = 4;
        strcat(rinex,"    ");
        for (int i = 0; i < 4; ++i) {
            //if( sat->data->ION_ALFA[i] >= 0) us = " "; else us = "";
             z += FloatToStrR(rinex,sat->data->ION_ALFA[i], 4);
             strcat(rinex,"  ");
             z+=2;   
			
        }
        while (z < 68){
            strcat(rinex," ");
            z++;
        }
        strcat(rinex,"ION ALPHA\n");
        strcat(rinex,"    ");      
        z=4;
        for (int i = 0; i < 4; i++) {
             z += FloatToStrR(rinex,sat->data->ION_BETA[i], 4);
             strcat(rinex,"  ");
             z+=2;  
        }
        while (z < 68){
             strcat(rinex," ");
            z++;
        }
        z=0;
        strcat(rinex,"ION BETA\n");
        strcat(rinex,"    ");
        int zz = 4;
        zz += FloatToStrR(rinex,sat->data->A0, 12);
        strcat(rinex," ");
        zz += FloatToStrR(rinex,sat->data->A1, 12);
        strcat(rinex,"   ");
        //if(sat->data->TOT > 0) us = " "; else us = "";
        //us = us + FloatToStrF(sat->data->TOT , ffExponent, 6, 0);
       
        z += IntToStr(rinex,(int)sat->data->TOT);
        while (z < 11){
            strcat(rinex," ");
            z++;
        }
        z += IntToStr(rinex,(int)sat->data->sf1_WN+2048);
        strcat(rinex," ");
        z++;
		// while (z < 9){
        //     strcat(rinex," ");
        //     zz+=z;
        // }
			z=0;

        
        strcat(rinex,"DELTA-UTC: A0,A1,T,W\n");  
        zz=0;
        strcat(rinex,"    ");
        z += IntToStr(rinex,(int)sat->data->sf4_18_delta_t_ls);
		
        z += 4;
       
        while (z < 60){
            strcat(rinex," ");
            z++;
        }
        z = 0;
         strcat(rinex,"LEAP SECONDS\n");
       
    }
    int z = 0;
    while (z < 60){
         strcat(rinex," ");
         z++;
    }
     strcat(rinex,"END OF HEADER\n");
   
    for(int k = 0; k < satelitelist->size; k++){
    for (int i = 0; i < satelitelist->size; ++i) {
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
            strcat(rinex,"0");
        }
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_mday;
        if (w < 10)
           strcat(rinex,"0");
        IntToStr(rinex,w);
       strcat(rinex," ");
        w = m_time->tm_hour;
        if (w < 10)
            strcat(rinex,"0");
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_min;
        if (w < 10)
            strcat(rinex,"0");
        IntToStr(rinex,w);
        strcat(rinex," ");
        w = m_time->tm_sec;
        if (w < 10)
            strcat(rinex,"0");
        IntToStr(rinex,w);
        strcat(rinex,".0");
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->AF0, 12);
       strcat(rinex," ");
        FloatToStrR(rinex,sat->data->AF1, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,0, 12);
        strcat(rinex,"\n");
		strcat(rinex,"    ");
		FloatToStrR(rinex,sat->data->IODE, 12);
         strcat(rinex," ");
		FloatToStrR(rinex,sat->data->CRS, 12);
       strcat(rinex," ");
		FloatToStrR(rinex,sat->data->DELTA_N, 12);
       strcat(rinex," ");
		FloatToStrR(rinex,sat->data->M0, 12);
		strcat(rinex,"\n");
        strcat(rinex,"    ");
        FloatToStrR(rinex,sat->data->CUC, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->E, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->CUS, 12);
       strcat(rinex," ");
        FloatToStrR(rinex,sat->data->SQRT_A, 12);
        strcat(rinex,"\n");
        strcat(rinex,"    ");
        FloatToStrR(rinex,sat->data->TOE, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->CIC, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->OMEGA_0, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->CIS, 12);
        strcat(rinex,"\n");

        // - 5 - - - - - orbit 4 - - - - - - - - - - - - - - - -
       strcat(rinex,"    ");
       FloatToStrR(rinex,sat->data->I0, 12);
      strcat(rinex," ");
       FloatToStrR(rinex,sat->data->CRC, 12);
      strcat(rinex," ");
       FloatToStrR(rinex,sat->data->omega, 12);
       strcat(rinex," ");
       FloatToStrR(rinex,sat->data->OMEGA_DOT, 12);
       strcat(rinex,"\n");
        // - 6 - - - - - orbit 5 - - - - - - - - - - - - - - - -
        strcat(rinex,"    ");
        FloatToStrR(rinex,sat->data->IDOT, 12);
        strcat(rinex," ");
		FloatToStrR(rinex,sat->data->sf1_ca_or_p_on, 12);
       strcat(rinex," ");

		FloatToStrR(rinex,sat->data->sf1_WN+2048, 12);
        strcat(rinex," ");
		FloatToStrR(rinex,sat->data->sf1_dfl2pc, 12);
		strcat(rinex,"\n");

        // - 7 - - - - - orbit 6 - - - - - - - - - - - - - - - -
        strcat(rinex,"    ");
        FloatToStrR(rinex,sat->data->sf1_SV_Accuracy, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->sf1_SV_Health, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,sat->data->TGD, 12);
       strcat(rinex," ");
        FloatToStrR(rinex,sat->data->sf1_IODC, 12);
        strcat(rinex,"\n");

        // - 8 - - - - - orbit 7 - - - - - - - - - - - - - - - -
        strcat(rinex,"    ");
        FloatToStrR(rinex,sat->data->TOW, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,0, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,0, 12);
        strcat(rinex," ");
        FloatToStrR(rinex,0, 12);
        strcat(rinex,"\n");
    }
    }
	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    char filen[1024];
    memset(filen,'\0',1024);
    sprintf(filen,".\\RINEX\\2_10\\%s",FileName);
	FILE *f_o = NULL;
    f_o = fopen(filen, "w");
    fprintf(f_o,"%s",rinex);
    writeLog("create_rinex.txt", buf_log,"RINEX"); 
    fclose(f_o); 
    free(rinex);    
	
}


int FloatToStrR(char* rezult, double n, int digit)
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
      temp1[1] = '.';   
     if(digit == 12){
        sprintf(temp, "%.11E", n*10);
      
     }else{
        sprintf(temp, "%.3E", n*10);
     }
           
           
            
	for(int i = 0, k = 2; i<24 && k < 24;i++){
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

bool checkDataSatelite(TSatelite* sat)
{
	int re = 0;
	if(sat->data->AF0!=0) re++;
	if(sat->data->AF1!=0) re++;
//	if(sat->data->AF2==0) re++;
   if(sat->data->IODE!=0) re++;
   if(sat->data->CRS!=0) re++;
   if(sat->data->DELTA_N!=0) re++;
   if(sat->data->M0!=0) re++;
   if(sat->data->CUC!=0) re++;
   if(sat->data->E!=0) re++;
   if(sat->data->CUS!=0) re++;
   if(sat->data->SQRT_A!=0) re++;
   if(sat->data->TOE!=0) re++;
   if(sat->data->CIC!=0) re++;
   if(sat->data->OMEGA_0!=0) re++;
   if(sat->data->CIS!=0) re++;
   if(sat->data->I0!=0) re++;
   if(sat->data->CRC!=0) re++;
   if(sat->data->omega!=0) re++;
   if(sat->data->OMEGA_DOT != 0) re++;
   if(sat->data->IDOT!=0) re++;
   if(sat->data->sf1_ca_or_p_on!=0) re++;
   if(sat->data->sf1_WN!=0) re++;

   if(sat->data->TGD!=0) re++;//
   if(sat->data->sf1_IODC!=0) re++;

   if(re == 23){
		return true;
	}else{
	 return false;
	 }


}