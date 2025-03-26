#include "header/header.h"


DWORD WINAPI writeCordinates(Args_P_t *ar_arg){
   ephem_t eph[EPHEM_ARRAY_SIZE][MAX_SAT];
   int neph,sv, loop;
   loop = -1;
   double dt; 
   label_start:
   if(ar_arg->buf->fexit == 1)
                return 0;
   if(ar_arg->flag_generate == 0){
    usleep(100);
    goto label_start;
   }
    CorectDate(ar_arg);
    while (true){
        if(ar_arg->buf->fexit == 1)
                break;
        if(loop == 30 || loop == -1){
           
            if(loop != -1)
                continue;
            neph = readRinexNavAll(eph, &ar_arg->arg_gps->ionoutc, ar_arg->arg_gps->navfile);
            if (neph==0)
            {
                loop = 0;
                continue;
            }
            
            for (sv=0; sv<MAX_SAT; sv++) 
            {
              for(int k = 0; k < neph; k++){  
                if (eph[k][sv].vflg==1)
                    {
                        if((ar_arg->arg_gps->gmin.sec == 0 && ar_arg->arg_gps->gmin.week == 0) || subGpsTime(ar_arg->arg_gps->gmin,  eph[k][sv].toc) > 0){
                            ar_arg->arg_gps->gmin = eph[k][sv].toc;
                            ar_arg->arg_gps->tmin = eph[k][sv].t;
                        }
                    }
                }
            }

            ar_arg->arg_gps->gmax.sec = 0;
            ar_arg->arg_gps->gmax.week = 0;
            ar_arg->arg_gps->tmax.sec = 0;
            ar_arg->arg_gps->tmax.mm = 0;
            ar_arg->arg_gps->tmax.hh = 0;
            ar_arg->arg_gps->tmax.d = 0;
            ar_arg->arg_gps->tmax.m = 0;
            ar_arg->arg_gps->tmax.y = 0;
            for (sv=0; sv<MAX_SAT; sv++)
            {
              for(int k = 0; k < neph; k++){  
                if (eph[k][sv].vflg == 1)
                    {
                        if((ar_arg->arg_gps->gmax.sec == 0 && ar_arg->arg_gps->gmax.week == 0) || subGpsTime(ar_arg->arg_gps->gmax,  eph[k][sv].toc) < 0){
                        ar_arg->arg_gps->gmax = eph[k][sv].toc;
                        ar_arg->arg_gps->tmax = eph[k][sv].t;
                        }
                    }
                }

            }
            for (int i=0; i<neph; i++)
            {
                for (sv=0; sv<MAX_SAT; sv++)
                {
                    if (eph[i][sv].vflg == 1)
                    {
                        dt = subGpsTime(ar_arg->arg_gps->g0, eph[i][sv].toc);
                        if (dt>=-SECONDS_IN_HOUR && dt<=SECONDS_IN_HOUR)
                        {
                            ar_arg->ieph = i;
                            break;
                        }
                    }
                }

                if (ar_arg->ieph>=0) 
                    break;
            }
           
            if (ar_arg->ieph != -1)
            {
                printf("new rinex success %i\n",neph);
                int sss = sizeof(eph);
                memcpy(ar_arg->arg_gps->eph,eph,sizeof(eph));
                
            }else{
                printf("no rinex correct \n");
            }


                loop = 0;
                
        }
        Sleep(1000);
        loop++;
    }
    return 0;
    
    
}