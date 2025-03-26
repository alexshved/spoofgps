#include "header/header.h"
#include <stdio.h>
#include <assert.h> 
#ifdef TEST
#define EXECUTE_OR_GOTO(label, ...){}
#else
#define EXECUTE_OR_GOTO(label, ...) \
    if(__VA_ARGS__){ \
        printf("error %i %i\n",fl,__VA_ARGS__);\
        return_code = EXIT_FAILURE; \
        fl++;\
        goto label; \
    }
#endif


int fl = 0;
int initUsd(){
    // int CY_VID_PID = 0; 
   libusb_device_handle* m_hUsb;
   libusb_context * m_ctx;
     printf("init USB\n");  
    libusb_init(&m_ctx);
    libusb_device **list;
    libusb_get_device_list(m_ctx, &list);
    for (size_t idx = 0; list[idx] != NULL; idx ++){       
        struct libusb_device_descriptor desc = {0}; 
        int rc = libusb_get_device_descriptor(list[idx], &desc);
        printf("Vendor:Device = %04x:%04x\n",  desc.idVendor,desc.idProduct);
        if(desc.idVendor == 0x2500 && desc.idProduct == 0x0020){
            
            m_hUsb = libusb_open_device_with_vid_pid(m_ctx,desc.idVendor,desc.idProduct); 
             if (m_hUsb == 0){
                printf("Cannot open device\n");    
             }else{
                printf("Device is open\n");
             }
             libusb_release_interface(m_hUsb,0);
             libusb_close(m_hUsb);
             return 0;
        }
        assert(rc==0);
               
    }
    return -1;   
   
}




DWORD WINAPI readUSRP(Args_P_t *ar_arg){
    int option = 0;
    FILE *fp = fopen("LOG/log_read_binary.txt", "wb");
    FILE *fptext = fopen("LOG/log_read.txt", "w");
    size_t num_acc_samps;
    size_t n_samples = 1000000;
    int return_code = EXIT_SUCCESS;
    uhd_rx_streamer_handle rx_streamer;
    char error_string[512];
 
    size_t channel = 0;
    uhd_usrp_handle usrp_a;

    int k = uhd_usrp_make(&usrp_a, "");
        if(k != 0){
            printf("usrt error %i\n",k);
        }
     printf("get time 0\n");   
    uhd_usrp_set_time_source(usrp_a,"gpsdo", 0);
    uhd_usrp_set_clock_source(usrp_a,"gpsdo", 0);
    usleep(1000);
    printf("get time 1\n"); 
    uhd_sensor_value_handle value_time;
    printf("get time 2\n"); 
    uhd_sensor_value_make_from_realnum(&value_time,"gps_time",0,"sec","%f");
    uhd_usrp_get_mboard_sensor(usrp_a,"gps_time",0,&value_time);
    printf("get time\n");
    double bt;
    int16_t full_sec;
    uhd_sensor_value_to_realnum(value_time,&bt);
    printf("Start time1 = corect %4d\n", bt);
    time_t now_sec = time(NULL);
     ar_arg->sec_corect = (time_t)(bt - (double)now_sec); 
    uhd_sensor_value_free(&value_time);
   
    printf("uhd_usrp_get_time_now %lf \n",bt);

    

    

    EXECUTE_OR_GOTO(free_usrp,
        uhd_rx_streamer_make(&rx_streamer)
    )

    // Create RX metadata
    uhd_rx_metadata_handle md;
    EXECUTE_OR_GOTO(free_rx_streamer,
        uhd_rx_metadata_make(&md)
    )

    // Create other necessary structs
    uhd_tune_request_t tune_request = {
        .target_freq = ar_arg->arg_usrt->freq,
        .rf_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
        .dsp_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
    };
    uhd_tune_result_t tune_result;

    uhd_stream_args_t stream_args = {
        .cpu_format = "fc32",
        .otw_format = "sc16",
        .args = "",
        .channel_list = &channel,
        .n_channels = 1
    };

    uhd_stream_cmd_t stream_cmd = {
        .stream_mode = UHD_STREAM_MODE_NUM_SAMPS_AND_DONE,
        .num_samps = n_samples,
        .stream_now = true
    };

    size_t samps_per_buff;
    float *buff = NULL;
    void **buffs_ptr = NULL;
  

    // Set rate
    // fprintf(stderr, "Setting RX Rate: %f...\n", ar_arg->arg_usrt->rate);
    fprintf(fptext, "Setting RX Rate: %f...\n", ar_arg->arg_usrt->rate);
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_set_rx_rate(usrp_a, ar_arg->arg_usrt->rate, channel)
    )

    // See what rate actually is
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_get_rx_rate(usrp_a, channel, &ar_arg->arg_usrt->rate)
    )
    // fprintf(stderr, "Actual RX Rate: %f...\n", ar_arg->arg_usrt->rate);
    fprintf(fptext, "Actual RX Rate: %f...\n", ar_arg->arg_usrt->rate);
    // Set gain
    // fprintf(stderr, "Setting RX Gain: %f dB...\n", ar_arg->arg_usrt->gain);
    fprintf(fptext, "Setting RX Gain: %f dB...\n", ar_arg->arg_usrt->gain);
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_set_rx_gain(usrp_a, ar_arg->arg_usrt->gain, channel, "")
    )

    // See what gain actually is
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_get_rx_gain(usrp_a, channel, "", &ar_arg->arg_usrt->gain)
    )
    // fprintf(stderr, "Actual RX Gain: %f...\n", ar_arg->arg_usrt->gain);
    fprintf(fptext, "Actual RX Gain: %f...\n", ar_arg->arg_usrt->gain);
    // Set frequency
    // fprintf(stderr, "Setting RX frequency: %f MHz...\n", ar_arg->arg_usrt->freq/1e6);
    fprintf(fptext, "Setting RX frequency: %f MHz...\r\n", ar_arg->arg_usrt->freq / 1e6);
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_set_rx_freq(usrp_a, &tune_request, channel, &tune_result)
    )

    // See what frequency actually is
    EXECUTE_OR_GOTO(free_rx_metadata,
        uhd_usrp_get_rx_freq(usrp_a, channel, &ar_arg->arg_usrt->freq)
    )
    // fprintf(stderr, "Actual RX frequency: %f MHz...\n", ar_arg->arg_usrt->freq / 1e6);
    fprintf(fptext, "Actual RX frequency: %f MHz...\r\n", ar_arg->arg_usrt->freq / 1e6);
    // Set up streamer
    stream_args.channel_list = &channel;
    EXECUTE_OR_GOTO(free_rx_streamer,
        uhd_usrp_get_rx_stream(usrp_a, &stream_args, rx_streamer)
    )

    // Set up buffer
    EXECUTE_OR_GOTO(free_rx_streamer,
        uhd_rx_streamer_max_num_samps(rx_streamer, &samps_per_buff)
    )
    // fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
    fprintf(fptext, "Buffer size in samples: %zu\n", samps_per_buff);
    buff = malloc(samps_per_buff * 2 * sizeof(float));
    buffs_ptr = (void**)&buff;

    // Issue stream command
    // fprintf(stderr, "Issuing stream command.\n");
    fprintf(fptext, "Issuing stream command.\r\n");
    EXECUTE_OR_GOTO(free_buffer,
        uhd_rx_streamer_issue_stream_cmd(rx_streamer, &stream_cmd)
    )

    // Set rate
 
    while (true) {
        
       label6 :
        if(ar_arg->buf->fl == 1 ){
            usleep(1);
            goto label6; 
        }
        // ar_arg->buf->flr = 1;      
        size_t num_rx_samps = 0;
        EXECUTE_OR_GOTO(close_file,
            uhd_rx_streamer_recv(rx_streamer, buffs_ptr, samps_per_buff, &md, 3.0, false, &num_rx_samps)
        )
        printf("Read byte %i\n",num_rx_samps);
        uhd_rx_metadata_error_code_t error_code;
        EXECUTE_OR_GOTO(close_file,
            uhd_rx_metadata_error_code(md, &error_code)
        )
        if(error_code != UHD_RX_METADATA_ERROR_CODE_NONE){
            fprintf(stderr, "Error code 0x%x was returned during streaming. \n", error_code);
            fprintf(fptext, "Error code 0x%x was returned during streaming. \r\n", error_code);
            // goto close_file;
        }else{
             printf("Read byte %i\n",num_rx_samps);
             fprintf(fptext,"Read byte %i\r\n",num_rx_samps);
        }

        // Handle data
        fwrite(buff, sizeof(float) * 2, num_rx_samps, fp);
        ar_arg->buf->flr = 0;  
        usleep(2000);
        // num_acc_samps += num_rx_samps;
    }

    // Cleanup
    close_file:
        fclose(fp);
        fclose(fptext);
        ar_arg->buf->flr = 0;
    free_buffer:
        buff = NULL;
        buffs_ptr = NULL;    
    free_rx_streamer:
        uhd_rx_streamer_free(&rx_streamer);
    free_rx_metadata:
        uhd_rx_metadata_free(&md);        
    free_usrp:
       if(return_code != EXIT_SUCCESS ){
            uhd_usrp_last_error(usrp_a, error_string, 512);
            fprintf(stderr, "USRP reported the following error: %s\n", error_string);
            fprintf(fptext,"USRP reported the following error: %s\n", error_string);
        }

     free_option_strings:
    uhd_usrp_free(&usrp_a);
    fprintf(stderr, (return_code ? "Failure\n" : "Success\n")); 
     
     return return_code;
}



DWORD WINAPI startUSRP(Args_P_t *ar_arg){
    char log[256];
    // printf("startUSRP %i | %i | %i\n",ar_arg->buf->fl,ar_arg->buf->flr,ar_arg->buf->wsiz);
    // printf("settings gain:%f rate:%f freq::%f\n",ar_arg->arg_usrt->gain, ar_arg->arg_usrt->rate, ar_arg->arg_usrt->freq);
    memset(log,'\0',256);
    sprintf(log,"startUSRP %i | %i | %i\nsettings gain:%f rate:%f freq::%f",ar_arg->buf->fl,ar_arg->buf->flr,ar_arg->buf->wsiz,ar_arg->arg_usrt->gain, ar_arg->arg_usrt->rate, ar_arg->arg_usrt->freq);
    writeLog("usrp_main.txt", log, "MAIN");
    // if(ar_arg->arg_usrt->readcomport == 1){
    //     openPortGPS("COM9");
    // }
    int option = 0;
    
    char* device_args = NULL;
    size_t channel = 0;
    uhd_tx_streamer_handle tx_streamer;
    
    bool verbose = false;
    int return_code = EXIT_SUCCESS;
    char error_string[512];
    FILE *fptext = fopen("LOG/log_write.txt", "w");
   
    uhd_sensor_value_handle value_time;
#ifndef TEST    
    uhd_usrp_handle usrp_a;
    int k = uhd_usrp_make(&usrp_a, "");
        if(k != 0){
            printf("usrt error %i\n",k);
        }
#endif
    //  printf("start get time: \n");
    EXECUTE_OR_GOTO(free_usrp,uhd_usrp_set_time_source(usrp_a,"gpsdo", 0));
    EXECUTE_OR_GOTO(free_usrp,uhd_usrp_set_clock_source(usrp_a,"gpsdo", 0));
    usleep(1000);
   
    EXECUTE_OR_GOTO(free_usrp,uhd_sensor_value_make_from_realnum(&value_time,"gps_time",0,"sec","%f"));
    EXECUTE_OR_GOTO(free_usrp,uhd_usrp_get_mboard_sensor(usrp_a,"gps_time",0,&value_time));
    //  printf("start get time1: \n");
     
    double bt;
    
    EXECUTE_OR_GOTO(free_usrp,uhd_sensor_value_to_realnum(value_time,&bt));
    // printf("start get time2: \n");
    time_t now_sec = time(NULL);// - (60 * 60 * 24 * 4 + (60*60*5));
    // printf("start get time3: \n");
     ar_arg->sec_corect = (double)(bt - now_sec); 
     double sec_corect = bt - now_sec;
    // printf("ar_arg->sec_corect: %.0f | %lld | %u | %u\n",bt,now_sec,ar_arg->sec_corect,sec_corect);
    EXECUTE_OR_GOTO(free_usrp,uhd_sensor_value_free(&value_time));
    // int16_t full_sec;
    //  char dtgp[24];
    // doubleToDate(bt, &dtgp[0],&arggps);
    //  printf("gps time:%ld \n",bt);
    // int64_t full_secs_out;
    // double frac_secs_out;
    // uhd_usrp_get_time_last_pps(usrp_a,0,&full_secs_out,&frac_secs_out);
    // printf("full_secs_out:%lld frac_secs_out:%f\n",full_secs_out,frac_secs_out);
    EXECUTE_OR_GOTO(free_usrp,
        uhd_tx_streamer_make(&tx_streamer)
    )

    // Create TX metadata
    uhd_tx_metadata_handle md;
    EXECUTE_OR_GOTO(free_tx_streamer,
        uhd_tx_metadata_make(&md, false, 0, 0.1, true, false)
    )
 
    // Create other necessary structs
    uhd_tune_request_t tune_request = {
        .target_freq = ar_arg->arg_usrt->freq,
        .rf_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO,
        .dsp_freq_policy = UHD_TUNE_REQUEST_POLICY_AUTO
    };
    uhd_tune_result_t tune_result;

    uhd_stream_args_t stream_args = {
        .cpu_format = "fc32",
        .otw_format = "sc16",
        .args = "",
        .channel_list = &channel,
        .n_channels = 1
    };

    size_t samps_per_buff;
    

    // Set rate
    // fprintf(stderr, "Setting TX Rate: %f...\n", ar_arg->arg_usrt->rate);
    fprintf(fptext,"Setting TX Rate: %f...\n", ar_arg->arg_usrt->rate);
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_set_tx_rate(usrp_a, ar_arg->arg_usrt->rate, channel)
    )
 
    // See what rate actually is
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_get_tx_rate(usrp_a, channel, &ar_arg->arg_usrt->rate)
    )
    // fprintf(stderr, "Actual TX Rate: %f...\n", ar_arg->arg_usrt->rate);
    fprintf(fptext,"Actual TX Rate: %f...\n", ar_arg->arg_usrt->rate);
    // Set gain
    // fprintf(stderr, "Setting TX Gain: %f db...\n", ar_arg->arg_usrt->gain);
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_set_tx_gain(usrp_a, ar_arg->arg_usrt->gain, 0, "")
    )
 
    // See what gain actually is
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_get_tx_gain(usrp_a, channel, "", &ar_arg->arg_usrt->gain)
    )
    // fprintf(stderr, "Actual TX Gain: %f...\n", ar_arg->arg_usrt->gain);
    fprintf(fptext,"Actual TX Gain: %f...\n", ar_arg->arg_usrt->gain);
    // Set frequency
    // fprintf(stderr, "Setting TX frequency: %f MHz...\n", ar_arg->arg_usrt->freq / 1e6);
    fprintf(fptext,"Setting TX frequency: %f MHz...\n", ar_arg->arg_usrt->freq / 1e6);
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_set_tx_freq(usrp_a, &tune_request, channel, &tune_result)
    )
 
    // See what frequency actually is
    EXECUTE_OR_GOTO(free_tx_metadata,
        uhd_usrp_get_tx_freq(usrp_a, channel, &ar_arg->arg_usrt->freq)
    )
    // fprintf(stderr, "Actual TX frequency: %f MHz...\n", ar_arg->arg_usrt->freq / 1e6);
    fprintf(fptext,"Actual TX frequency: %f MHz...\n", ar_arg->arg_usrt->freq / 1e6);
    // Set up streamer
    stream_args.channel_list = &channel;
    EXECUTE_OR_GOTO(free_tx_streamer,
        uhd_usrp_get_tx_stream(usrp_a, &stream_args, tx_streamer)
    )
 
    // Set up buffer
    EXECUTE_OR_GOTO(free_tx_streamer,
        uhd_tx_streamer_max_num_samps(tx_streamer, &samps_per_buff)
    )
    // fprintf(stderr, "Buffer size in samples: %zu\n", samps_per_buff);
    fprintf(fptext,"Buffer size in samples: %zu\n", samps_per_buff);
    // float* buff = NULL;
    const void** buff = calloc(sizeof(float), samps_per_buff * 2);
    // buff = calloc(sizeof(float), samps_per_buff * 2);
    // buffs_ptr = calloc(sizeof(float), samps_per_buff * 2);
    
 #ifdef TEST
    samps_per_buff = 4080;
    // Ctrl+C will exit loop
   
 #endif    
    
   

    // Actual streaming
    
    FILE *fp = fopen(".\\LOG\\semd_data.bin","wb");
    uint64_t wsiz = 0;
    ar_arg->flag_generate = 1;  
    while(1) {
        
        label4 :
        if( ar_arg->buf->fexit == 1)
            break;
       
       
        if(ar_arg->buf->wsiz == 0 || ar_arg->buf->fl == 1 ){
           
            goto label4; 
        }
        ar_arg->buf->fl = 1;
        
        float *buf = malloc(ar_arg->buf->wsiz*4);
        memcpy(buf,ar_arg->buf->buf,ar_arg->buf->wsiz*4);
        wsiz = ar_arg->buf->wsiz;
        ar_arg->buf->wsiz = 0;
        ar_arg->buf->fl = 0;  

        uint64_t k = 0;
        uint64_t num_acc_samps = 0;
        while(k  < wsiz){
           if( ar_arg->buf->fexit == 1)
            break;
           size_t num_samps_sent = 0; 
           size_t send_s = samps_per_buff ;
           if(wsiz - num_acc_samps  < send_s)
                send_s = wsiz - num_acc_samps;
            
            float* buff = calloc(send_s*2,sizeof(float));
            const void** buffs_ptr = &buff;
             for(int i = 0; i < (send_s*2); i+=2, k++){
                buff[i]   = buf[k];
                buff[i+1] = 0;
            }
 #ifndef TEST          
            EXECUTE_OR_GOTO(free_buff,
                uhd_tx_streamer_send(tx_streamer, buffs_ptr , send_s , &md, 0.1, &num_samps_sent)
            )
            
            if(num_samps_sent != send_s){
               UHD_LOG_ERROR("TX-STREAM","error send");
            }
#endif
            free(buff);
            num_acc_samps += send_s;
        }
        free(buf);
        wsiz = 0;    
        
       
       
    //     printf("data %lf %s\n",bt,dtgp);
    }
    
 #ifndef TEST     
    free_buff:

        // ar_arg->buf->wsiz = 0;
       
    free_tx_streamer:
        if(verbose){
            fprintf(stderr, "Cleaning up TX streamer.\n");
        }

        uhd_tx_streamer_free(&tx_streamer);

    free_tx_metadata:
        if(verbose){
            fprintf(stderr, "Cleaning up TX metadata.\n");
        }
        uhd_tx_metadata_free(&md);

    free_usrp:
        if(verbose){
            fprintf(stderr, "Cleaning up USRP.\n");
        }
        if(return_code != EXIT_SUCCESS && usrp_a != NULL){
            uhd_usrp_last_error(usrp_a, error_string, 512);
            fprintf(stderr, "USRP reported the following error: %s\n", error_string);
        }
        uhd_usrp_free(&usrp_a);
#endif
    free_option_strings:
        if(device_args)
            free(device_args);
    fclose(fptext);        
    free(buff);
    fprintf(stderr, (return_code ? "Failure\n" : "Success\n"));
    fclose(fp);
    
    return return_code;
}