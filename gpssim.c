#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "header/header.h"
#include <windows.h>

int sinTable512[] = {
	   2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
	  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
	  97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
	 140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
	 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
	 209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
	 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
	 245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250,
	 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
	 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
	 230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
	 207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
	 176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
	 138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
	  94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
	  47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
	  -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
	 -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
	 -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
	-140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
	-178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
	-209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
	-232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
	-245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
	-250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
	-245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
	-230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
	-207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
	-176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
	-138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
	 -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
	 -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2
};

int cosTable512[] = {
	 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 246, 245,
	 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232,
	 230, 229, 228, 227, 225, 224, 223, 221, 220, 218, 217, 215, 214, 212, 210, 209,
	 207, 205, 204, 202, 200, 198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178,
	 176, 173, 171, 169, 167, 164, 162, 160, 157, 155, 153, 150, 148, 145, 143, 140,
	 138, 135, 132, 130, 127, 125, 122, 119, 116, 114, 111, 108, 105, 103, 100,  97,
	  94,  91,  89,  86,  83,  80,  77,  74,  71,  68,  65,  62,  59,  56,  53,  50,
	  47,  44,  41,  38,  35,  32,  29,  26,  23,  20,  17,  14,  11,   8,   5,   2,
	  -2,  -5,  -8, -11, -14, -17, -20, -23, -26, -29, -32, -35, -38, -41, -44, -47,
	 -50, -53, -56, -59, -62, -65, -68, -71, -74, -77, -80, -83, -86, -89, -91, -94,
	 -97,-100,-103,-105,-108,-111,-114,-116,-119,-122,-125,-127,-130,-132,-135,-138,
	-140,-143,-145,-148,-150,-153,-155,-157,-160,-162,-164,-167,-169,-171,-173,-176,
	-178,-180,-182,-184,-186,-188,-190,-192,-194,-196,-198,-200,-202,-204,-205,-207,
	-209,-210,-212,-214,-215,-217,-218,-220,-221,-223,-224,-225,-227,-228,-229,-230,
	-232,-233,-234,-235,-236,-237,-238,-239,-240,-241,-241,-242,-243,-244,-244,-245,
	-245,-246,-247,-247,-248,-248,-248,-249,-249,-249,-249,-250,-250,-250,-250,-250,
	-250,-250,-250,-250,-250,-249,-249,-249,-249,-248,-248,-248,-247,-247,-246,-245,
	-245,-244,-244,-243,-242,-241,-241,-240,-239,-238,-237,-236,-235,-234,-233,-232,
	-230,-229,-228,-227,-225,-224,-223,-221,-220,-218,-217,-215,-214,-212,-210,-209,
	-207,-205,-204,-202,-200,-198,-196,-194,-192,-190,-188,-186,-184,-182,-180,-178,
	-176,-173,-171,-169,-167,-164,-162,-160,-157,-155,-153,-150,-148,-145,-143,-140,
	-138,-135,-132,-130,-127,-125,-122,-119,-116,-114,-111,-108,-105,-103,-100, -97,
	 -94, -91, -89, -86, -83, -80, -77, -74, -71, -68, -65, -62, -59, -56, -53, -50,
	 -47, -44, -41, -38, -35, -32, -29, -26, -23, -20, -17, -14, -11,  -8,  -5,  -2,
	   2,   5,   8,  11,  14,  17,  20,  23,  26,  29,  32,  35,  38,  41,  44,  47,
	  50,  53,  56,  59,  62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  91,  94,
	  97, 100, 103, 105, 108, 111, 114, 116, 119, 122, 125, 127, 130, 132, 135, 138,
	 140, 143, 145, 148, 150, 153, 155, 157, 160, 162, 164, 167, 169, 171, 173, 176,
	 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 205, 207,
	 209, 210, 212, 214, 215, 217, 218, 220, 221, 223, 224, 225, 227, 228, 229, 230,
	 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245,
	 245, 246, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250
};


double ant_pat_db[37] = {
	 0.00,  0.00,  0.22,  0.44,  0.67,  1.11,  1.56,  2.00,  2.44,  2.89,  3.56,  4.22,
	 4.89,  5.56,  6.22,  6.89,  7.56,  8.22,  8.89,  9.78, 10.67, 11.56, 12.44, 13.33,
	14.44, 15.56, 16.67, 17.78, 18.89, 20.00, 21.33, 22.67, 24.00, 25.56, 27.33, 29.33,
	31.56
};

int allocatedSat[MAX_SAT];



DWORD WINAPI gpssim(Args_P_t *ar_arg){

	int fl_start = 0;
	FILE *fp;
	int sv;
	ephem_t eph[EPHEM_ARRAY_SIZE][MAX_SAT];
	int i;
	channel_t chan[MAX_CHAN];
	double elvmask = 0.0; 
	int ip,qp;
	int iTable;
	short *iq_buff = NULL;
	signed char *iq8_buff = NULL;
	gpstime_t grx;
	double delt;
	int isamp;
	int numd;
	int nmeaGGA = FALSE;
	int umLLH = FALSE;
	int iq_buff_size;
	int data_format;
	int result;
	int gain[MAX_CHAN];
	double path_loss;
	double ant_gain;
	double ant_pat[37];
	int ibs; 
	double dt;
	int igrx;
	int timeoverwrite = ar_arg->arg_gps->timeoverwrite; 
	ar_arg->arg_gps->samp_freq = floor(ar_arg->arg_gps->samp_freq/10.0);
	iq_buff_size = (int)ar_arg->arg_gps->samp_freq; 
	ar_arg->arg_gps->samp_freq *= 10.0;
	delt = 1.0/ar_arg->arg_gps->samp_freq;
	labale_eph:	
	if(ar_arg->arg_gps->eph[0][0].vflg == -1){
		usleep(100);
		goto labale_eph;
	}
	numd = readNmeaGGA(ar_arg);
	 fprintf(stderr, "Start time = %4d/%02d/%02d,%02d:%02d:%02.0f (%d:%.0f)\n", 
		    ar_arg->arg_gps->t0.y, ar_arg->arg_gps->t0.m, ar_arg->arg_gps->t0.d, ar_arg->arg_gps->t0.hh, ar_arg->arg_gps->t0.mm, ar_arg->arg_gps->t0.sec, ar_arg->arg_gps->g0.week, ar_arg->arg_gps->g0.sec);
	iq_buff = calloc(2*iq_buff_size, 2);
	if (iq_buff==NULL)
	{
		fprintf(stderr, "ERROR: Failed to allocate 16-bit I/Q buffer.\n");
		fprintf(ar_arg->arg_gps->fptext,"ERROR: Failed to allocate 16-bit I/Q buffer.\n");
		exit(1);
	}

	if (data_format==SC08)
	{
		iq8_buff = calloc(2*iq_buff_size, 1);
		if (iq8_buff==NULL)
		{
			fprintf(stderr, "ERROR: Failed to allocate 8-bit I/Q buffer.\n");
			fprintf(ar_arg->arg_gps->fptext,"ERROR: Failed to allocate 8-bit I/Q buffer.\n");
			exit(1);
		}
	}
	else if (data_format==SC01)
	{
		iq8_buff = calloc(iq_buff_size/4, 1); 
		if (iq8_buff==NULL)
		{
			fprintf(stderr, "ERROR: Failed to allocate compressed 1-bit I/Q buffer.\n");
			fprintf(ar_arg->arg_gps->fptext, "ERROR: Failed to allocate compressed 1-bit I/Q buffer.\n");
			exit(1);
		}
	}

	

	for (i=0; i<MAX_CHAN; i++)
		chan[i].prn = 0;

	
	for (sv=0; sv<MAX_SAT; sv++)
		allocatedSat[sv] = -1;

	grx = incGpsTime(ar_arg->arg_gps->g0, (double)ar_arg->arg_gps->correction);

	allocateChannel(chan, ar_arg->arg_gps->eph[ar_arg->ieph], ar_arg->arg_gps->ionoutc, grx, ar_arg->arg_gps->xyz[0], elvmask, ar_arg->arg_gps->leapSecond);
	// if(fl_start == 0)
	int sat_c = 0;
	for(i=0; i<MAX_CHAN; i++)
	{
		if (chan[i].prn>0){
			fprintf(stderr, "Satelite %02d %6.1f %5.1f %11.1f %5.1f\n", chan[i].prn, 
				chan[i].azel[0]*R2D, chan[i].azel[1]*R2D, chan[i].rho0.d, chan[i].rho0.iono_delay);
				sat_c++;
		}
			
	}
	for (i=0; i<37; i++)
		ant_pat[i] = pow(10.0, -ant_pat_db[i]/20.0);

	while(true)
	{
		if (ar_arg->buf->fexit == 1){
			ar_arg->buf->fexit = 1;
			break;
		} 
		for (i=0; i<MAX_CHAN; i++)
		{
			if (ar_arg->buf->fexit == 1){
				ar_arg->buf->fexit = 1;
				break;
			} 
			if (chan[i].prn>0)
			{
				range_t rho;
				sv = chan[i].prn-1;
				computeRange(&rho, ar_arg->arg_gps->eph[ar_arg->ieph][sv], &ar_arg->arg_gps->ionoutc, grx, ar_arg->arg_gps->xyz[0]);
				chan[i].azel[0] = rho.azel[0];
				chan[i].azel[1] = rho.azel[1];
				computeCodePhase(&chan[i], rho, 0.1);
#ifndef FLOAT_CARR_PHASE
				chan[i].carr_phasestep = (int)round(512.0 * 65536.0 * chan[i].f_carr * delt);
#endif
				
				path_loss = 20200000.0/rho.d;
				ibs = (int)((90.0-rho.azel[1]*R2D)/5.0);
				ant_gain = ant_pat[ibs];
				gain[i] = (int)(path_loss*ant_gain*128.0); 
			}
		}

		for (isamp=0; isamp<iq_buff_size; isamp++)
		{
			int i_acc = 0;
			int q_acc = 0;
			for (i=0; i<MAX_CHAN; i++)
			{
				if (chan[i].prn>0)
				{
#ifdef FLOAT_CARR_PHASE
					iTable = (int)floor(chan[i].carr_phase*512.0);
#else
					iTable = (chan[i].carr_phase >> 16) & 0x1ff; 
#endif
					ip = chan[i].dataBit * chan[i].codeCA * cosTable512[iTable] * gain[i];
					qp = chan[i].dataBit * chan[i].codeCA * sinTable512[iTable] * gain[i];
					i_acc += ip;
					q_acc += qp;
					chan[i].code_phase += chan[i].f_code * delt;
					if (chan[i].code_phase>=CA_SEQ_LEN)
					{
						chan[i].code_phase -= CA_SEQ_LEN;
						chan[i].icode++;
						if (chan[i].icode>=20) 
						{
							chan[i].icode = 0;
							chan[i].ibit++;
						
							if (chan[i].ibit>=30) 
							{
								chan[i].ibit = 0;
								chan[i].iword++;
								
							}
							chan[i].dataBit = (int)((chan[i].dwrd[chan[i].iword]>>(29-chan[i].ibit)) & 0x1UL)*2-1;
						}
					}
					chan[i].codeCA = chan[i].ca[(int)chan[i].code_phase]*2-1;
#ifdef FLOAT_CARR_PHASE
					chan[i].carr_phase += chan[i].f_carr * delt;

					if (chan[i].carr_phase >= 1.0)
						chan[i].carr_phase -= 1.0;
					else if (chan[i].carr_phase<0.0)
						chan[i].carr_phase += 1.0;
#else
					chan[i].carr_phase += chan[i].carr_phasestep;
#endif
				}
			}
			i_acc = (i_acc+64)>>7;
			q_acc = (q_acc+64)>>7;
			iq_buff[isamp*2] = (short)i_acc;
			iq_buff[isamp*2+1] = (short)q_acc;
		}
			label1 :
			if (ar_arg->buf->fexit == 1){
				ar_arg->buf->fexit = 1;
				break;
			} 
			 if(ar_arg->buf->fl == 0 && ar_arg->buf->wsiz == 0){
			 	ar_arg->buf->fl = 1;
				memcpy(ar_arg->buf->buf,iq_buff, 4*iq_buff_size);
				ar_arg->buf->wsiz = iq_buff_size ;
				// ar_arg->buf->wsiz = 2*iq_buff_size ;
				ar_arg->buf->fl = 0;
			 }else{
				goto label1;
			}
		igrx = (int)(grx.sec*10.0+0.5);
		numd = readNmeaGGA(ar_arg);
		 if (igrx%300==0) 
		 {
			for (i=0; i<MAX_CHAN; i++)
			{
				if (chan[i].prn>0)
					generateNavMsg(grx, &chan[i], 0);
			}

			for (sv=0; sv<MAX_SAT; sv++)
			{
				if (ar_arg->arg_gps->eph[ar_arg->ieph+1][sv].vflg==1)
				{
					dt = subGpsTime(ar_arg->arg_gps->eph[ar_arg->ieph+1][sv].toc, grx);
					if (dt<SECONDS_IN_HOUR  )
					{
						ar_arg->ieph++;
						 if (ar_arg->ieph >= EPHEM_ARRAY_SIZE) {
                            ar_arg->ieph = 0;
                        }
						for (i=0; i<MAX_CHAN; i++)
						{
							
							if (chan[i].prn!=0) 
								eph2sbf(ar_arg->arg_gps->eph[ar_arg->ieph][chan[i].prn-1], ar_arg->arg_gps->ionoutc, chan[i].sbf,ar_arg->arg_gps->leapSecond);
						}
					}
						
					break;
				}
			 }
			
			allocateChannel(chan, ar_arg->arg_gps->eph[ar_arg->ieph], ar_arg->arg_gps->ionoutc, grx, ar_arg->arg_gps->xyz[0], elvmask,ar_arg->arg_gps->leapSecond);
			
		}
		// allocateChannel(chan, ar_arg->arg_gps->eph[ar_arg->ieph], ar_arg->arg_gps->ionoutc, grx, ar_arg->arg_gps->xyz[0], elvmask,ar_arg->arg_gps->leapSecond);
		//  else
		// printf("time week %i sek %.0f\n",grx.week,grx.sec);
		grx = incGpsTime(grx, 0.1);

		fflush(stdout);
		
	}
	free(iq_buff);
	if(data_format==SC01 || data_format==SC08){
		free(iq8_buff);
	}
	fclose(fp);
	if(ComPortGPSRead != NULL)
		CloseHandle(ComPortGPSRead);
	
	labelexit :
	ar_arg->buf->fexit = 1;
	return 0;
}
