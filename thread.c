#include "header/header.h"
#include <time.h>

int create_Thred(char **arg_device, int len_arg_device){

    ArgGps_t arggps;
   
    arggps.staticLocationMode = FALSE;

    arggps.fptext = fopen("LOG/log_message.txt", "w");
    memset(arggps.outfile,'\0',300);
    strcpy(arggps.outfile, "LOG/message_binary.txt");
    arggps.timeoverwrite = TRUE;
    arggps.verb = FALSE;
    arggps.duration = (double)500/10; 
    arggps.ionoutc.enable = TRUE;
    arggps.samp_freq = 2.5e6;
	arggps.data_format = SC16;
    arggps.leapSecond = 18;
    arggps.nmeaGGA = TRUE;
    arggps.gmax.sec = 0;
    arggps.gmin.sec = 0;
    arggps.eph[0][0].vflg = -1;
    arggps.g0.sec = 0;
   
    memset(arggps.navfile,'\0',100);
    strcpy(arggps.navfile, "C:\\hgenerator_04\\brdc\\mode0.brdc");
    memset(arggps.umfile,'\0',100);
    strcpy(arggps.umfile,"COM7");
    arggps.speedcom = 115200; 
	arggps.g0.week = -1; 
    ARGUSRP_t argusrp;
    argusrp.freq = 500e6;
    argusrp.rate = 1e6;
    argusrp.gain = 50.0;
    argusrp.total_num_samps = 0;
    char thread_p[124];
    int rinex = 0;
    int create = 0;
    int sendusrp = 0;
    int readusrp = 0;
    char arg[256];
    char port[124];
    memset(port,'\0',124);
    int result;
    int sped_port = 9600;
   

#ifndef TEST

   
#else
   time_t now_sec = time(NULL);
   double bt =  now_sec + 100; 
   arggps.sec_corect = (time_t)(bt - (double)now_sec);
#endif


    while ((result=getopt( len_arg_device,arg_device,"a:k:j:p:f:n:r:q:e:u:x:g:c:l:o:s:b:T:t:d:P:B:R:Z:G:D:K:L:V:S:M:X:Y:A:OJivh"))!=-1)
	{
		switch (result)
		{
         case 'L':
          arggps.leapSecond = atol(optarg);
          break;  
         case 'K':
          arggps.correction = atoi(optarg);  
         break;   
         case 'B':
          arggps.speedcom = atoi(optarg);
         break;
         case 'P':
            memset(arggps.umfile,'\0',100);
            strcpy(arggps.umfile,"COM");
            strcat(arggps.umfile,optarg);
         break;

        case 'k':
            memset(thread_p,'\0',124);
            strcpy(thread_p, optarg);
            if(strchr(thread_p,(int)'1') != NULL )
               rinex = 1;
            if(strchr(thread_p,(int)'2') != NULL )
               create = 1;
            if(strchr(thread_p,(int)'3') != NULL )
               sendusrp = 1;   
            if(strchr(thread_p,(int)'4') != NULL )
               readusrp = 1;       
			break;
        case 'j':
            
            sped_port = atoi(optarg);
			break;     
        case 'p':
            
            strcpy(port, optarg);
			break;    
		case 'a':
            // memset(arg,'\0',256);
            // strcpy(arg, optarg);
			break;
		case 'f':
			argusrp.freq = atof(optarg);
            break;
        case 'r':
            argusrp.rate = atof(optarg);
            break;
        case 'x':
            argusrp.gain = atof(optarg);
            break;
		break;
         case 'n':
            // argusrp.gain = atof(optarg);
            break;
		break;
		
		case 'e':
            memset(arggps.navfile,'\0',100);
            strcpy(arggps.navfile, optarg);
			break;
		case 'u':
            // memset(arggps.umfile,'\0',100);
			// strcpy(arggps.umfile, optarg);
			// arggps.nmeaGGA = FALSE;
			// arggps.umLLH = FALSE;
			break;
		
		case 'g':
            
			// arggps.nmeaGGA = TRUE;
			break;
		case 'c':
			
			arggps.staticLocationMode = TRUE;
			sscanf(optarg,"%lf,%lf,%lf",&arggps.xyz[0][0],&arggps.xyz[0][1],&arggps.xyz[0][2]);
			break;
		case 'l':
			arggps.staticLocationMode = TRUE;
			sscanf(optarg,"%lf,%lf,%lf",&arggps.llh[0],&arggps.llh[1],&arggps.llh[2]);
			arggps.llh[0] = arggps.llh[0] / R2D; // convert to RAD
			arggps.llh[1] = arggps.llh[1] / R2D; // convert to RAD
			llh2xyz(arggps.llh,arggps.xyz[0]); // Convert llh to xyz
			break;
		case 'o':
            memset(arggps.outfile,'\0',300);
			strcpy(arggps.outfile, optarg);
			break;
		case 's':
			arggps.samp_freq = atof(optarg);
			if (arggps.samp_freq<1.0e6)
			{
				fprintf(stderr, "ERROR: Invalid sampling frequency.\n");
				fprintf(arggps.fptext,"ERROR: Invalid sampling frequency.\n");
				exit(1);
			}
			break;
		case 'b':
			arggps.data_format = atoi(optarg);
			if (arggps.data_format!=SC01 && arggps.data_format!=SC08 && arggps.data_format!=SC16)
			{
				fprintf(stderr, "ERROR: Invalid I/Q data format.\n");
				fprintf(arggps.fptext,"ERROR: Invalid I/Q data format.\n");
				exit(1);
			}
			break;
		case 'T':
			arggps.timeoverwrite = TRUE;
			if (strncmp(optarg, "now", 3)==0)
			{
				time_t timer;
				struct tm *gmt;
				
				time(&timer);
				gmt = gmtime(&timer);

				arggps.t0.y = gmt->tm_year+1900;
				arggps.t0.m = gmt->tm_mon+1;
				arggps.t0.d = gmt->tm_mday;
				arggps.t0.hh = gmt->tm_hour;
				arggps.t0.mm = gmt->tm_min;
				arggps.t0.sec = (double)gmt->tm_sec;

				date2gps(&arggps.t0, &arggps.g0);
				
				break;
			}
		case 't':
			sscanf(optarg, "%d/%d/%d,%d:%d:%lf", &arggps.t0.y, &arggps.t0.m, &arggps.t0.d, &arggps.t0.hh, &arggps.t0.mm, &arggps.t0.sec);
			if (arggps.t0.y<=1980 || arggps.t0.m<1 || arggps.t0.m>12 || arggps.t0.d<1 || arggps.t0.d>31 ||
				arggps.t0.hh<0 || arggps.t0.hh>23 || arggps.t0.mm<0 || arggps.t0.mm>59 || arggps.t0.sec<0.0 || arggps.t0.sec>=60.0)
			{
				fprintf(stderr, "ERROR: Invalid date and time.\n");
				fprintf(arggps.fptext,"ERROR: Invalid date and time.\n");
				exit(1);
			}
			arggps.t0.sec = floor(arggps.t0.sec);
			date2gps(&arggps.t0, &arggps.g0);
			break;
		case 'd':
			arggps.duration = atof(optarg);
			break;
		case 'i':
			// arggps.ionoutc.enable = FALSE; // Disable ionospheric correction
			break;
		case 'v':
			arggps.verb = TRUE;
			break;
		case 'h':
			print_help();
			exit(0);
		default:
					
			break;
		}
	}
    int th_c = 0;
#ifdef NMEAGGA 
    th_c++;
#endif
    if(rinex == 1)
    th_c++;
    if(create == 1)
    th_c++;
    if(sendusrp == 1)
    th_c++;
    if(readusrp == 1)
    th_c++;
#ifdef NMEAGGA
    if(th_c == 1)
#else    
    if(th_c == 0)
#endif
{
        th_c = 3;
#ifdef NMEAGGA 
    th_c++;
#endif
        create = 1;
        sendusrp = 1;
        readusrp = 1; 
    }
    HANDLE  *threads = (HANDLE*)malloc(th_c * sizeof(HANDLE));
    Args_P_t *args = (Args_P_t*)malloc(sizeof(Args_P_t));
    BufThreds_t *buf = malloc(sizeof(BufThreds_t));
    BufRinex_t *bufrinex =  malloc(sizeof(BufRinex_t));
    memset(args->buf_crd,'\0',200);
    memset(args->buf_crd_back,'\0',200);
    args->flag_generate = 0;
    args->ieph = -1;
    args->flag_buf = 0;
    buf->wsiz = 0;
    buf->fexit = 0;
    buf->fl = 0;
    buf->flr = 0;
    buf->buf = calloc(520000*4,4);
    bufrinex->fl = 0;
    bufrinex->wsiz = 0;
    bufrinex->buf = malloc(1024*1000);
    if(sendusrp == 1 || readusrp == 1){

        
    }
    
        args->buf = buf;
        args->arg_gps = &arggps;
        args->arg_usrt = &argusrp;
        args->rinex = bufrinex;
        args->comport = &port;
        args->sped_port = sped_port;
        args->sec_corect = 9990000.99;
   
       
    // }
    int cp = 0;
    
    
#ifdef NMEAGGA    
     threads[0] = CreateThread(NULL,4096,writeCordinates,args, 0,NULL );
     cp++;
#endif    

    while(cp<th_c){
        if(readusrp == 1){
            
             threads[cp] = CreateThread(NULL,4096,readCordinates,args, 0,NULL );
             readusrp = 0;
             cp++;
             continue;
        }    
        if(rinex == 1){
             threads[cp] = CreateThread(NULL,4096,crateRinex,args, 0,NULL );
             rinex = 0;
             cp++;
             
             continue;
        }
        if(create == 1){
             threads[cp] = CreateThread(NULL,4096,gpssim,args, 0,NULL );
             create = 0;
             cp++;
             continue;
        }
        if(sendusrp == 1){
            sendusrp = 0;
            threads[cp] = CreateThread(NULL,4096,startUSRP,args, 0,NULL );
             cp++;
             continue;
        }
          
    }
          

    WaitForMultipleObjects(th_c, threads, TRUE, INFINITE);
    for(int i = 0; i<th_c;i++){
    CloseHandle(threads[i]);
    
    }
    //  free(args);
    free(bufrinex->buf);
    free(bufrinex);
    free(threads);
    fclose(arggps.fptext);
    free(args);
    free(buf->buf);
    free(buf);
   
    return 0;
}