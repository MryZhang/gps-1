#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<signal.h>
#include<getopt.h>

/* Include D2XX header*/
#include "ftd2xx.h"
#include "max2769.h"

/* para imprimir binario */
/* printf(Bformat,B( foo )); */
#define Bformat "%d%d%d%d%d%d%d%d\n"
#define B(byte) \
	(byte & 0x80 ? 1 : 0),\
	(byte & 0x40 ? 1 : 0),\
	(byte & 0x20 ? 1 : 0),\
	(byte & 0x10 ? 1 : 0),\
	(byte & 0x08 ? 1 : 0),\
	(byte & 0x04 ? 1 : 0),\
	(byte & 0x02 ? 1 : 0),\
	(byte & 0x01 ? 1 : 0)


static int running=1;
uint8_t buffer[4];
int i,j,k;

/* ~IDLE & ~SHDN on pins 26 & 27
 * must be outputs (1) and LD & ANTFLAG on pins 28 & 29
 * must be inputs (0)
 * ~IDLE & ~SHDN must be HI to enable the device
 * con la version bitbang no tengo manera de mover esos pines
 * pero ambos estan en HI por default asi que no hay problema (?)
 */

int8_t buff[65536*2];
int8_t data[65536*4];
uint32_t rd,rdd;

int buff_size;
FILE* report;

#define USAGE_STR \
"================================================================================\n"\
" USAGE:\n"\
"\tgps-setup-n-dump MANDAROTY_ARGUMENTS [OPTIONAL_ARGUMENTS]\n\n"\
" MANDATORY_ARGUMENTS:\n\n"\
"\t-l, --lna <0|1|2> select input. 0 to disconnect, 1 for passive antenna, 2 for active\n\n"\
"\t-f, --freq <4|8> select output BW in mbytes/sec\n"\
"\t\tfor 8M setup FI=2.42MHz, FiltCen=2.340MHz, FiltBW=2.5MHz\n"\
"\t\tfor 4M setup FI=1.02MHz, FiltCen=0.975MHz, FiltBW=2.5MHz\n\n"\
" OPTIONAL_ARGUMENTS:\n\n"\
"\t-g, --gain <0...63> in dB\n\n"\
"\t-c, --config configure MAX2769 chip via SPI\n\n"\
"\t-d, --dump dump data via stdout\n"\
"\t\tdata format: one signed byte per sample, real sampling, 3bits\n\n"\
"\t-m, --mixpole <13|36> select passive filter pole at mixer output. default=13\n\n"\
"\t-o, --order <3|5> filter order selection. default=5\n\n"\
"\t-v, --verbose show info via stderr\n\n"\
"\t-h, --help show this message\n\n"\
" SOME EXAMPLES:\n\n"\
"\tgps-setup-n-dump --lna 2 --freq 8 --gain 63 -c -d | nc -l 1234\n"\
"\t\tsend stream via socket (to gnuradio script for example)\n\n"\
"\tgps-setup-n-dump --lna 2 --freq 8 --gain 63 -d | dd of=/dev/null\n"\
"\t\tsend to dd (to perform speed test)\n\n"\
"================================================================================\n"

int opt_lna;
int opt_profile;
int opt_gain;
int opt_verbose;
int opt_config;
int opt_dump;
int opt_mixpole;
int opt_order;

void dump(void);
void config(void);

FT_STATUS ftStatus_fifo;
FT_HANDLE ftHandle_fifo;
	
void intHandler(int dummy)
{
    running = 0;
}

int main(int argc, char* argv[])
{
	opt_lna=-1;
	opt_profile=-1;
	opt_gain=-1;
	opt_verbose=0;
	opt_config=0;
	opt_dump=0;
	opt_mixpole=13;
	opt_order=5;
	
	buff_size=512;

	static struct option long_options[] = { 
		{"lna", 1, 0, 'l'},
		{"freq", 1, 0, 'f'},
		{"gain", 1, 0, 'g'},
		{"mixpole", 1, 0, 'm'},
		{"order", 1, 0, 'o'},
		{"verbose", 0, 0, 'v'},
		{"help", 0, 0, 'h'},
		{"config", 0, 0, 'c'},
		{"dump", 0, 0, 'd'},
		{NULL, 0, NULL, 0}
	};  
	int option_index = 0;
	int c;

	while ((c = getopt_long(argc, argv, "l:f:g:m:o:vhcd",
		long_options, &option_index)) != -1) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
		case 'l':
			if (sscanf (optarg, "%i", &opt_lna)!=1) { fprintf(stderr,"error - not an integer\n"); }
			if(!((opt_lna==0)  || (opt_lna==1) || (opt_lna==2))){
				fprintf(stderr,"wrong opt %i for lna\n",opt_lna);
				exit(0);
			}
			break;
		case 'f':
			if (sscanf (optarg, "%i", &opt_profile)!=1) { fprintf(stderr,"error - not an integer\n"); }
				if(!((opt_profile==4) || (opt_profile==8))){
				fprintf(stderr,"wrong opt %i for freq\n",opt_profile);
				exit(0);
			}	
			break;
		case 'g':
			if (sscanf (optarg, "%i", &opt_gain)!=1) { fprintf(stderr,"error - not an integer\n"); }
				if((opt_gain<0) || (opt_gain>63)){
				fprintf(stderr,"wrong opt %i for gain\n",opt_gain);
				exit(0);
			}	
			break;
		case 'm':
			if (sscanf (optarg, "%i", &opt_mixpole)!=1) { fprintf(stderr,"error - not an integer\n"); }
				if(!((opt_mixpole==13) || (opt_mixpole==36))){
				fprintf(stderr,"wrong opt %i for mixpole\n",opt_gain);
				exit(0);
			}	
			break;
		case 'o':
			if (sscanf (optarg, "%i", &opt_order)!=1) { fprintf(stderr,"error - not an integer\n"); }
				if(!((opt_order==3) || (opt_order==5))){
				fprintf(stderr,"wrong opt %i for mixpole\n",opt_gain);
				exit(0);
			}	
			break;

		case 'c':
			opt_config=1;
			break;
		case 'd':
			opt_dump=1;
			break;
		case 'v':
			opt_verbose=1;
			break;
		case 'h':
			fprintf(stderr,USAGE_STR);
			break;
		case '?':
			break;
		default:
			fprintf(stderr,"?? getopt returned character code 0%o ??\n", c); 
		}   
	}   
	if (optind < argc) {
		fprintf(stderr,"non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		printf ("\n");
	} 

	if(argc==1)
	{
		printf (USAGE_STR);
	}

	if(opt_lna==-1 || opt_profile==-1)
	{
		fprintf(stderr,"\n\tMissing mandatory arguments. Exiting now!\n\n");
		exit(0);

	}

	signal(SIGINT, intHandler);
	report=stderr;
	if(!opt_verbose)
	{
		report=fopen("/dev/null","w");
	}

	if(opt_config){config();}
	if(opt_dump){dump();}
	return 0;
}


void config(void)
{

	fprintf(report,"==========================================\n");
		
	/* Open  channel 0 (SPI y GPIOs) en el channel 1 esta la FIFO*/
	
	fprintf(report,"========== MAX2769B Config Tool ==========\n");
        
	fprintf(report,"Opening device for BitBang ... \t");
        ftStatus_fifo = FT_Open(0,&ftHandle_fifo);
        if (ftStatus_fifo == FT_OK) {
                fprintf(report,"[ OK  ]\n");
        }else{
                fprintf(report,"[ERROR] (%i %i)\n",ftStatus_fifo,(unsigned int)ftHandle_fifo);
        exit(0);
        }

        fprintf(report,"Setting BitMode ... \t\t");
        ftStatus_fifo = FT_SetBitMode(ftHandle_fifo,0x0F,0x1);//D0-D3 salida D4-D7 entrada bitbang mode  
        if (ftStatus_fifo == FT_OK) {
                fprintf(report,"[ OK  ]\n");
        }else{
                fprintf(report,"[ERROR] (%i %i)\n",ftStatus_fifo,(unsigned int)ftHandle_fifo);
        exit(0);
        }

        fprintf(report,"Setting BitBang rate ... \t");
        ftStatus_fifo = FT_SetBaudRate(ftHandle_fifo,300);  // 300*16 = 4k8 
        if (ftStatus_fifo == FT_OK) {
                fprintf(report,"[ OK  ]\n");
        }else{
                fprintf(report,"[ERROR] (%i %i)\n",ftStatus_fifo,(unsigned int)ftHandle_fifo);
        exit(0);
        }

	MAX2769 GPS;

	GPS.CONF1 = 0;
	GPS.CONF1 |= CONF1_CHIPEN;
	GPS.CONF1 |= CONF1_R1;
	GPS.CONF1 |= CONF1_R2;
	GPS.CONF1 |= CONF1_R3;
	GPS.CONF1 |= CONF1_R4;

	if(opt_mixpole==13){GPS.CONF1 |= CONF1_MIXPOLE_13MHZ;}	
	if(opt_mixpole==36){GPS.CONF1 |= CONF1_MIXPOLE_36MHZ;}	
	
	//GPS.CONF1 |= CONF1_LNAMODE_AUTO;
	/*con modo AUTO selecciona la activa solo si
	  drena corriente de ANTBIAS */
	/*LNA2 es el de la antena activa*/
	if(opt_lna==0){GPS.CONF1 |= CONF1_LNAMODE_OFF;}	
	if(opt_lna==1){GPS.CONF1 |= CONF1_LNAMODE_LNA1;}	
	if(opt_lna==2){GPS.CONF1 |= CONF1_LNAMODE_LNA2;}	

	GPS.CONF1 |= CONF1_MIXEN;
	GPS.CONF1 |= CONF1_ANTEN;//not mandatory
	

	if(opt_profile==8){GPS.CONF1 |= CONF1_FCEN_2_340M;}	
	if(opt_profile==4){GPS.CONF1 |= CONF1_FCEN_0_975M;}	
	
	GPS.CONF1 |= CONF1_FBW_2_50M;

	if(opt_order==3){GPS.CONF1 |= CONF1_F3OR5_3;}	
	if(opt_order==5){GPS.CONF1 |= CONF1_F3OR5_5;}	

	GPS.CONF1 |= CONF1_FCENX_BP;
	GPS.CONF1 |= CONF1_FGAIN;
	GPS.CONF1 |= CONF1_ADDR;


	GPS.CONF2 = 0;
	GPS.CONF2 |= CONF2_IQEN;
	GPS.CONF2 |= CONF2_GAINREF_16P;
	//GPS.CONF2 |= CONF2_GAINREF_33P;
	

	if(opt_gain==-1){GPS.CONF2 |= CONF2_AGCMODE_INDEP_IQ;}
	if(opt_gain!=-1){GPS.CONF2 |= CONF2_AGCMODE_GAINREG;}
	
	GPS.CONF2 |= CONF2_FORMAT_UNSIGNED_BIN;
	
	GPS.CONF2 |= CONF2_BITS_3;
	GPS.CONF2 |= CONF2_DRVCFG_CMOS;
	GPS.CONF2 |= CONF2_R1;
	GPS.CONF2 |= CONF2_ADDR;

	GPS.CONF3 = 0;

	GPS.CONF3 |= CONF3_GAININ_(opt_gain);

	GPS.CONF3 |= CONF3_FHIPEN;
	GPS.CONF3 |= CONF3_R1;
	GPS.CONF3 |= CONF3_R2;
	GPS.CONF3 |= CONF3_R3;
	GPS.CONF3 |= CONF3_R4;
	GPS.CONF3 |= CONF3_R5;
	GPS.CONF3 |= CONF3_R6;
	GPS.CONF3 |= CONF3_R7;
	GPS.CONF3 |= CONF3_R8;
	GPS.CONF3 |= CONF3_ADDR;

	GPS.PLLCONF = 0;	
	GPS.PLLCONF |= PLLCONF_REFOUTEN;
	//GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_X_2;
	//GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_X_1;
	
	if(opt_profile==8){GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_DIV_2;}	
	if(opt_profile==4){GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_DIV_4;}	

	//GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_DIV_2;
	//GPS.PLLCONF |= PLLCONF_REFDIV_XTAL_DIV_4;
	GPS.PLLCONF |= PLLCONF_IXTAL_NORMAL_C;
	GPS.PLLCONF |= PLLCONF_INT_PLL;
	GPS.PLLCONF |= PLLCONF_R1;
	GPS.PLLCONF |= PLLCONF_R1B;
	GPS.PLLCONF |= PLLCONF_R2;
	GPS.PLLCONF |=  PLLCONF_ADDR;

	GPS.DIV = 0;
	
	if(opt_profile==8){
	GPS.DIV |= DIV_NDIV_8;
	GPS.DIV |= DIV_RDIV_8;
	}
	if(opt_profile==4){
	GPS.DIV |= DIV_NDIV_4;
	GPS.DIV |= DIV_RDIV_4;
	}
	
	GPS.DIV |= DIV_ADDR;

	//GPS.FDIV = FDIV_INIT;
	//GPS.FDIV |= FDIV_ADDR;

	//GPS.STRM = STRM_INIT;
	//GPS.STRM |= STRM_ADDR;

	//GPS.CLK = CLK_INIT;
	//GPS.CLK |= CLK_ADDR;

	//GPS.TEST1 = TEST1_INIT | TEST1_ADDR;
	//GPS.TEST2 = TEST2_INIT | TEST2_ADDR;

	fprintf(report,"CONF1:   0x%08X\n",GPS.CONF1);
	fprintf(report,"CONF2:   0x%08X\n",GPS.CONF2);
	fprintf(report,"CONF3:   0x%08X\n",GPS.CONF3);
	fprintf(report,"PLLCONF: 0x%08X\n",GPS.PLLCONF);
	fprintf(report,"DIV:     0x%08X\n",GPS.DIV);
	//printf("FDIV:    0x%08X\n",GPS.FDIV);
	//printf("STRM:    0x%08X\n",GPS.STRM);
	//printf("CLK:     0x%08X\n",GPS.CLK);
	//printf("TEST1:   0x%08X\n",GPS.TEST1);
	//printf("TEST2:   0x%08X\n",GPS.TEST2);

	#define BUFS (8*4)
	#define WORDS (5)
	#define CS_MASK (0x08)
	#define CLK_MASK (0x01)
	// 5 palabras spi de 8*4 bits


	uint8_t spi_buffer[WORDS][BUFS]={0};
	uint8_t aux;
	size_t ret;
	// spi_buffer[0] = X X X X CS X DATA CLK 

	uint32_t * ptr;
	ptr=(uint32_t*)&GPS;
	for(i=0;i<WORDS;i++)
	{	
			
		buffer[0]=((*ptr)>>24)&0xFF;
		buffer[1]=((*ptr)>>16)&0xFF;
		buffer[2]=((*ptr)>>8)&0xFF;
		buffer[3]=(*ptr)&0xFF;
		// esta basura de los buffers quedo heredada de cuando habia libmpsse y el spi write se hacia de a bytes
		// fprintf(report,"%08X %08X %02X %02X %02X %02X ... ",(int)ptr,*ptr,buffer[0],buffer[1],buffer[2],buffer[3]);
		// si se rompe todo, checkear que este bien el endianness con este printf
		for(j=0;j<4;j++)
		{
			for(k=0;k<8;k++)
			{
				spi_buffer[i][8*j+k]|=((buffer[j]>>(7-k))%2)<<1;
			}
		}
		ptr++;
		usleep(1000);

	}
	aux=CS_MASK;
	ftStatus_fifo = FT_Write(ftHandle_fifo,&aux,1,&ret); //dejo /CS en HI
	//printf(Bformat,B(aux));

	usleep(3000);

	uint8_t actual_bitbang=spi_buffer[0][0];

	for(i=0;i<WORDS;i++)
	{
		actual_bitbang=actual_bitbang&=~CS_MASK;
                ftStatus_fifo = FT_Write(ftHandle_fifo,&actual_bitbang,1,&ret); //dejo /CS en LO
		//printf(Bformat,B(actual_bitbang));
		usleep(5000);
		for(j=0;j<BUFS;j++)
		{	
			FT_Write(ftHandle_fifo,&spi_buffer[i][j],1,&ret); 	
		//	printf(Bformat,B(spi_buffer[i][j]));
			usleep(2000);
			
			actual_bitbang=(spi_buffer[i][j]) | CLK_MASK;
			FT_Write(ftHandle_fifo,&actual_bitbang,1,&ret); 	
		//	printf(Bformat,B(actual_bitbang));
			usleep(2000);

			FT_Write(ftHandle_fifo,&spi_buffer[i][j],1,&ret); 	
		//	printf(Bformat,B(spi_buffer[i][j]));
			usleep(2000);

		}
		actual_bitbang=actual_bitbang|=CS_MASK;
		actual_bitbang&=~CLK_MASK;
		ftStatus_fifo = FT_Write(ftHandle_fifo,&actual_bitbang,1,&ret); //dejo /CS en HI y CLK en LO
		//printf(Bformat,B(actual_bitbang));
		usleep(5000);
	}

	fprintf(report,"\nClosing device ... \t\t");
	FT_Close(ftHandle_fifo);
	fprintf(report,"[ OK  ]\n");
	
	fprintf(report,"==========================================\n");

}


void dump(void)
{

        fprintf(report,"Opening FIFO ... \t\t");
	ftStatus_fifo = FT_Open(1,&ftHandle_fifo);
	if (ftStatus_fifo == FT_OK) {
                fprintf(report,"[ OK  ]\n");
	}else{
                fprintf(report,"[ERROR] (%i %i)\n",ftStatus_fifo,(unsigned int)ftHandle_fifo);
        exit(0);
	}

        fprintf(report,"Setting Latency Timer ... \t");
	ftStatus_fifo = FT_SetLatencyTimer(ftHandle_fifo, 2);
	if (ftStatus_fifo != FT_OK) {
		fprintf(report, "[ERROR] (status=%d)\n", ftStatus_fifo);
		FT_Close(ftHandle_fifo);
		exit(0);
  	}
  	fprintf(report, "[ OK  ]\n");


       /* fprintf(report,"Setting timeouts ... \t\t");
	ftStatus_fifo = FT_SetTimeouts(ftHandle_fifo, 1, 0);
	if (ftStatus_fifo != FT_OK) {
		fprintf(report, "[ERROR] (status=%d)\n", ftStatus_fifo);
		FT_Close(ftHandle_fifo);
		exit(0);
  	}
  	fprintf(report, "[ OK  ]\n");
	*/
        fprintf(report,"Setting USB transfer size ... \t");
	ftStatus_fifo = FT_SetUSBParameters(ftHandle_fifo, 0x10000, 0x10000);
	if (ftStatus_fifo != FT_OK) {
		fprintf(report, "[ERROR] (status=%d)\n", ftStatus_fifo);
		FT_Close(ftHandle_fifo);
		exit(0);
  	}
  	fprintf(report, "[ OK  ]\n");

	/*
        fprintf(report,"Setting Baud rate ... \t");
        ftStatus_fifo = FT_SetBaudRate(ftHandle_fifo,115200);  // 115200*16 =1.843200M 
        if (ftStatus_fifo == FT_OK) {
                fprintf(report,"[ OK  ]\n");
        }else{
                fprintf(report,"[ERROR] (%i %i)\n",ftStatus_fifo,ftHandle_fifo);
        exit(0);
        }
	*/

        fprintf(report,"Setting flow control mode ... \t");
	ftStatus_fifo = FT_SetFlowControl(ftHandle_fifo, FT_FLOW_RTS_CTS, 0, 0);
	if (ftStatus_fifo != FT_OK) {
		fprintf(report, "[ERROR] (status=%d)\n", ftStatus_fifo);
		FT_Close(ftHandle_fifo);
		exit(0);
	}
  	fprintf(report, "[ OK  ]\n");

	/* Purge the receive buffer on the FTDI device of any old data. */

        fprintf(report,"Purging RX buffer ... \t\t");
	ftStatus_fifo = FT_Purge(ftHandle_fifo, FT_PURGE_RX);
	if (ftStatus_fifo != FT_OK) {
		fprintf(report, "[ERROR] (status=%d)\n", ftStatus_fifo);
		FT_Close(ftHandle_fifo);
		exit(0);
	}
  	fprintf(report, "[ OK  ]\n");

	FT_ResetDevice(ftHandle_fifo);
	FT_Purge(ftHandle_fifo,3);
	

        fprintf(report,"Setting BitMode ... \t\t");
	ftStatus_fifo = FT_SetBitMode(ftHandle_fifo,0,0x40);
	if (ftStatus_fifo == FT_OK) {
		fprintf(report,"[ OK  ]\n");
	}else{
		fprintf(report,"[ERROR]\n");
	}


	buff_size=1024*16;
	while(running)
	{
		ftStatus_fifo = FT_GetQueueStatus(ftHandle_fifo,&rd);
		if (ftStatus_fifo == FT_OK){
			if(rd>1024*60){rd=0;fprintf(stderr,"O");FT_Purge(ftHandle_fifo,3);}
            // si pierdo una muestra descarto todo porque ese bloque para GPS
            // ya no sirve
			if(rd>buff_size)
			{
				ftStatus_fifo = FT_Read(ftHandle_fifo,buff,rd,&rdd);
				if ((ftStatus_fifo == FT_OK) && (rd==rdd)){
#if 1
                    for(i=0;i<rd;++i)
                    //for(i=0;i<57724;++i)
				    {
						    //convert
						    data[2*i+1]  = 2*(((buff[i]&0x08)>>3) |
                            ((buff[i]&0x02)>>0) | ((buff[i]&0x01)<<2))-7;
						    data[2*i+0]= 2*(((buff[i]&0x80)>>7) |
                            ((buff[i]&0x20)>>4) | ((buff[i]&0x10)>>2))-7;
						    //			Q1		I0			I1
			    //			data[2*i]=(data[2*i]*2)-7;
			    //			data[2*i+1]=(data[2*i+1]*2)-7;
                    }
				    fwrite(data,1,rd*2,stdout);
#else
                    for(i=0;i<57724;++i)
                    {
                        float a=1.234;
                        a*=2.2;
                    }
					fwrite(data,1,rd*2,stdout);
#endif
                }else{
					fprintf(report,"some problem reading\n");
				}
			}//else{fprintf(stderr,"."); usleep(10);}
		}else{
			FT_Close(ftHandle_fifo);
			exit(0);
		}
	}

	fprintf(report,"\nClosing device ... \t\t");
	FT_Close(ftHandle_fifo);
	fprintf(report,"[ OK  ]\n");

}


