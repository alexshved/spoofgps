#include "header/header.h"

void CalcParam(TSubframe *sb) {
    sb->word = (DWORD*)&sb->data[14];
    sb->satelite = sb->data[7];
    sb->TOW = (sb->word[1] >> (13)) & 0x1FFFF;
    sb->id = (sb->word[1] >> (8)) & 0x7;
    if (sb->id < 4)
        sb->page = -1;
    else {
        sb->page = (sb->word[2] >> 22) & 0x3F;
        if (sb->page > 32) {
            if (sb->page == 52)
                sb->page = 13;
            else if (sb->page == 55)
                sb->page = 17;
            else if (sb->page == 56)
                sb->page = 18;
            else if (sb->page == 63)
                sb->page = 25;
            else
                sb->page = -1;
        }
    }
}

int compare_ints(const int *a, const int *b ){ 
   return *a - *b;
}

int satelite[1024];

void AddSubFrame(TSubframe* sb){
    int i;
    for (i = 0; i < satelitelist->size; ++i) //поищем есть ли в списке
    {
        TSatelite* p = (TSatelite*)&satelitelist->satelite[i];
        int satelite_id = satelitelist->satelite[i]->id;
        if (satelitelist->satelite[i]->id == sb->satelite) {
            break;
        }
    }
    if (i>=satelitelist->size) {
        
        satelitelist->satelite[i] = malloc(sizeof(TSatelite)) ; // новый появился
        satelitelist->satelite[i]->id = sb->satelite;
        satelitelist->satelite[i]->data = malloc(sizeof(TSatSata));
        memset(satelitelist->satelite[i]->data,'\0',sizeof(TSatSata));
        satelitelist->satelite[i]->frame = calloc(42,sizeof(TSatelite*));
        for(int k = 0; k < 42; k++){
           satelitelist->satelite[i]->frame[k] = malloc(sizeof(Tframe));
           satelitelist->satelite[i]->frame[k]->cnt = 3;
            satelitelist->satelite[i]->frame[k]->subframe = calloc(6,sizeof(TSubframe*));
            for(int kk = 0; kk < 6; kk++){
                satelitelist->satelite[i]->frame[k]->subframe[kk] = malloc(sizeof(TSubframe));
                satelitelist->satelite[i]->frame[k]->subframe[kk]->data[0] = 2;
                //  memset(satelitelist->satelite[i]->frame[k]->subframe[kk],'\0',sizeof(TSubframe));
            }
        }
       satelitelist->size++;
       satelite[satelitelist->size-1] = sb->satelite;
       qsort(&satelite, satelitelist->size, sizeof(int) , compare_ints ) ;
        
    }
    int cc = 0;
   
        
    int frm = 1;
    if (sb->page > 0)
        frm = sb->page;
     memcpy(satelitelist->satelite[i]->frame[frm]->subframe[sb->id],sb,sizeof(TSubframe));
    satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->data[1] = 4;
    Tframe *kkk = satelitelist->satelite[i]->frame[frm];
    TSubframe *mpo = satelitelist->satelite[i]->frame[frm]->subframe[sb->id];
    satelitelist->satelite[i]->frame[frm]->cnt = 5;
    memcpy(satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->data,sb->data,56);
    satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->id = -1;
    satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->page = -1;
    satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->satelite = -1;
    satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->word = NULL;
    CalcParam(satelitelist->satelite[i]->frame[frm]->subframe[sb->id]);
    unsigned char bufframe[512];
    memset(bufframe,'\0',512);
    for (int i = 0; i < 56; ++i) {
        ByteToHexStr(bufframe,sb->data[i]);
    }
    writeLogComon("subframe.txt", bufframe, satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->satelite, satelitelist->satelite[i]->frame[frm]->subframe[sb->id]->id, "Recive frame");
    calcdata(satelitelist->satelite[i],sb);
    switch (sb->id) {
        case 1:
            satelitelist->satelite[i]->ready[0] = true;
            break;
        case 2:
            satelitelist->satelite[i]->ready[1] = true;
            break;
        case 3:
            satelitelist->satelite[i]->ready[2] = true;
            break;
        case 4:
            if (sb->page == 13)
                satelitelist->satelite[i]->ready[3] = true;
            else if (sb->page == 18)
                satelitelist->satelite[i]->ready[4] = true;
            else if (sb->page == 25)
                satelitelist->satelite[i]->ready[5] = true;
            break;
        case 5:
            if (sb->page == 24)
                satelitelist->satelite[i]->ready[6] = true;
            else if (sb->page == 25)
                satelitelist->satelite[i]->ready[7] = true;
            break;
    }
    free(sb);
}

void calcdata(TSatelite* satelite, TSubframe* sf)
{
    satelite->data->TOW = sf->TOW;
    if (sf->id == 1)
        calc_sf1(satelite,sf);
    else if (sf->id == 2)
        calc_sf2(satelite,sf);
    else if (sf->id == 3)
        calc_sf3(satelite,sf);
    else if (sf->id == 4) {
        if (sf->page == 13)
            calc_sf4_13(satelite,sf);
        else if (sf->page == 18)
            calc_sf4_18(satelite,sf);
        else if (sf->page == 25)
            calc_sf4_25(satelite,sf);
    } else if (sf->id == 5) {
        if (sf->page == 24)
            calc_sf5_24(satelite,sf);
        else if (sf->page == 25)
            calc_sf5_25(satelite,sf);
    }
}

int ParseInt(byte t0, byte t1, byte t2, byte t3){
    int rezult =  t0 + (t1 << 8) + (t2 << 16) + (t3 << 24);
    return rezult;
} 

short ParseShort(byte t0, byte t1){
   return ((t0 << 8) & 0xff00) | (t1 & 0xff);
}