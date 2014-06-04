/*
 * El Chip MAX2769 tiene 10 registros de configuracion
 * Se acceden mediante SPI con palabras de 32 bits
 *
 * Los primeros 28 bits son el valor del registro y
 * los ultimos 4 bits son la direccion del mismo.
 *
 * | DATA MSB | --- DATA LSB | ADDR MSB | --- | ADDR LSB |
 * SPI mode=0
 */

#define CONF1_ADDR    (0x00)
#define CONF2_ADDR    (0x01)
#define CONF3_ADDR    (0x02)
#define PLLCONF_ADDR  (0x03)
#define DIV_ADDR      (0x04)
#define FDIV_ADDR     (0x05)
#define STRM_ADDR     (0x06)
#define CLK_ADDR      (0x07)
#define TEST1_ADDR    (0x08)
#define TEST2_ADDR    (0x09)

/*===================================================*/

#define CONF1_CHIPEN ((1<<27)<<4)
#define CONF1_IDLE ((1<<26)<<4)

/* Reserver registers */
#define CONF1_R1 ((8<<22)<<4)
#define CONF1_R2 ((2<<20)<<4)
#define CONF1_R3 ((2<<18)<<4)
#define CONF1_R4 ((1<<16)<<4)

#define CONF1_MIXPOLE_36MHZ ((1<<15)<<4)
#define CONF1_MIXPOLE_13MHZ ((0<<15)<<4)

#define CONF1_LNAMODE_AUTO ((0<<13)<<4)
#define CONF1_LNAMODE_LNA2 ((1<<13)<<4)
#define CONF1_LNAMODE_LNA1 ((2<<13)<<4)
#define CONF1_LNAMODE_OFF  ((3<<13)<<4)

#define CONF1_MIXEN ((1<<12)<<4)
#define CONF1_ANTEN ((1<<11)<<4)

/*IF Center freq*/
/* La info en la DS de MAXIM es bullshit */
/* ver appnote de maxim para calcular donde cae la fcentral
 * que depende del BW del filtro */
//#define CONF1_FCEN_3_092M ((13<<5)<<4) // esto cae en 3.9M  | con BW de 2M5
//#define CONF1_FCEN_4_092M ((11<<5)<<4) // esto cae en 2.34M | esta mal el datasheet (con BW 2M5)

#define CONF1_FCEN_2_340M ((11<<5)<<4) // esto cae en 2.34M | BW 2M5
#define CONF1_FCEN_0_975M ((55<<5)<<4) // esto cae en 0.975M | BW 2M5

//#define CONF1_FCEN_10_00M ((19<<5)<<4)
//#define CONF1_FCEN_CUSTOM ((43<<5)<<4)

/*IF bandwith*/
#define CONF1_FBW_2_50M ((0<<3)<<4)
//#define CONF1_FBW_9_66M ((1<<3)<<4)
//#define CONF1_FBW_4_20M ((2<<3)<<4)

/*3rd or 5th order filter*/
#define CONF1_F3OR5_3 ((1<<2)<<4)
#define CONF1_F3OR5_5 ((0<<2)<<4)

#define CONF1_FCENX_BP ((1<<1)<<4)
#define CONF1_FCENX_LP ((0<<1)<<4)

#define CONF1_FGAIN ((1<<0)<<4)

/*===================================================*/

//#define CONF2_INIT (0x8<<4)
//#define CONF2_INIT (0x055028C<<4)

#define CONF2_IQEN ((1<<27)<<4)
// esto tambien es BS en la hoja de datos
// segun el support de maxim, el MAX2769
// no tiene disponible el sampleo IQ

#define CONF2_GAINREF_33P ((170<<15)<<4)
#define CONF2_GAINREF_16P ((84<<15)<<4)
#define CONF2_GAINREF_61P ((314<<15)<<4)
#define CONF2_GAINREF_CUSTOM ((300<<15)<<4)

#define CONF2_AGCMODE_INDEP_IQ ((0<<11)<<4)
#define CONF2_AGCMODE_GAINREG ((2<<11)<<4)

#define CONF2_FORMAT_UNSIGNED_BIN ((0<<9)<<4)
#define CONF2_FORMAT_SIGN_MAG ((1<<9)<<4)
#define CONF2_FORMAT_TWO_COMP ((2<<9)<<4)

#define CONF2_BITS_1 ((0<<6)<<4)
#define CONF2_BITS_2 ((2<<6)<<4)
#define CONF2_BITS_3 ((4<<6)<<4)

#define CONF2_DRVCFG_CMOS ((0<<4)<<4)
#define CONF2_DRVCFG_ANALOG ((2<<4)<<4)

#define CONF2_R1 ((1<<3)<<4)

/*===================================================*/

//#define CONF3_INIT (0x02F61C0<<4)
//#define CONF3_INIT (0xEAFE1DC<<4)

#define CONF3_GAININ_00dB ((0<<22)<<4)


#define CONF3_GAININ_10dB ((11<<22)<<4)
#define CONF3_GAININ_20dB ((21<<22)<<4)
#define CONF3_GAININ_30dB ((31<<22)<<4)


#define CONF3_GAININ_42dB ((43<<22)<<4)
#define CONF3_GAININ_43dB ((44<<22)<<4)
#define CONF3_GAININ_45dB ((46<<22)<<4)
#define CONF3_GAININ_57dB ((58<<22)<<4)
#define CONF3_GAININ_62dB ((63<<22)<<4)

#define CONF3_GAININ_(x) ((x<<22)<<4)


#define CONF3_R1 ((1<<21)<<4)

#define CONF3_HILOADEN ((1<<20)<<4)

#define CONF3_R2 ((1<<19)<<4)
#define CONF3_R3 ((1<<18)<<4)
#define CONF3_R4 ((1<<17)<<4)
#define CONF3_R5 ((1<<16)<<4)

#define CONF3_FHIPEN ((1<<15)<<4)

#define CONF3_R6 ((1<<14)<<4)
#define CONF3_R7 ((1<<13)<<4)

#define CONF3_STRMEN ((1<<11)<<4)
#define CONF3_STRMSTART ((1<<10)<<4)
#define CONF3_STRMSTOP ((1<<9)<<4)

#define CONF3_R8 ((7<<6)<<4)

#define CONF3_STRMBITS ((1<<4)<<4) //no entiendo la DS
#define CONF3_STAMPEN ((1<<3)<<4)
#define CONF3_TIMESYNCEN ((1<<2)<<4)
#define CONF3_DATSYNCEN ((1<<1)<<4)
#define CONF3_STRMRST ((1<<0)<<4)

/*===================================================*/


#define PLLCONF_R1 ((1<<27)<<4)

#define PLLCONF_REFOUTEN ((1<<24)<<4)

#define PLLCONF_R1B ((1<<23)<<4)

#define PLLCONF_REFDIV_XTAL_X_2 ((0<<21)<<4)
#define PLLCONF_REFDIV_XTAL_X_1 ((3<<21)<<4)
#define PLLCONF_REFDIV_XTAL_DIV_2 ((2<<21)<<4)
#define PLLCONF_REFDIV_XTAL_DIV_4 ((1<<21)<<4)

#define PLLCONF_IXTAL_NORMAL_C ((1<<19)<<4)
#define PLLCONF_IXTAL_HIGH_C ((3<<19)<<4)

#define PLLCONF_R2 ((0x10<<14)<<4)

#define PLLCONF_LDMUX ((0<<10)<<4)

#define PLLCONF_ICP_1MA ((1<<9)<<4)
#define PLLCONF_ICP_0_5MA ((0<<9)<<4)

#define PLLCONF_PFDEN ((1<<8)<<4)

#define PLLCONF_INT_PLL ((1<<3)<<4)
#define PLLCONF_PWRSAV ((1<<2)<<4)

/*===================================================*/

// 8M
#define DIV_NDIV_8 ((1573<<13)<<4)
#define DIV_RDIV_8 ((16<<3)<<4)


// 4M
#define DIV_NDIV_4 ((1476<<13)<<4)
#define DIV_RDIV_4 ((15<<3)<<4)




// sugerencia de MB que funcina bien
//#define DIV_NDIV ((1375<<13)<<4)
//#define DIV_RDIV ((14<<3)<<4)


//#define DIV_NDIV ((1573<<13)<<4)
//#define DIV_RDIV ((16<<3)<<4)


// 16M PLANB
//#define DIV_NDIV ((1571<<13)<<4)
//#define DIV_RDIV ((1<<3)<<4)

// 16.368M (default value)
//#define DIV_NDIV ((1536<<13)<<4)
//#define DIV_RDIV ((16<<3)<<4)

/*===================================================*/
// para trabajar con IF 2.34 hay que cambiar el filtro
// al que en la DS dice 4.092
// para que IF caiga en 2.34 con XTAL/2 =8M

// RDIV=8
// fdiv=1573


/*===================================================*/
/*
---------------------------------------------------
con XTAL de 16.368

16.368/16=1.023

1.023*1536 = 1571.328

1571.328 + 4.092 = 1575.42
            ^------LF

---------------------------------------------------
XX con XTAL de 16.000 <--- esto no esta bueno
                           porque el divisor es
                           muy grande y el pll
                           no funciona bien en
                           esa zona
XX 16.000/125=0.128

XX 0.128 * 12276 = 1571.328

XX  1571.328 + 4.092 = 1575.42

XX RDIV=125
XX NDIV=12276

---------------------------------------------------
PLANB:    | esto da algo masomenos
RDIV=1
NDIV=1571


otra opcion es que quede en banda base
16.000/69 = 0.23188405797101

0.23188405797101 * 6794 = 1575.420289

En esta opcion tambien queda el pll
con un divisor grande y aparte para
samplear en I es mejor hacerlo en FI
con un BP en lugar de en baseband con LPF

*/

/*===================================================*/
/* Default Values for this registers */
#define FDIV_INIT (0x8000070<<4)
#define STRM_INIT (0x8000000<<4)
#define CLK_INIT (0x10061B2<<4)
#define TEST1_INIT (0x1E0F401<<4)
#define TEST2_INIT (0x28C0402<<4)
/*===================================================*/


typedef struct{
    uint32_t CONF1;
    uint32_t CONF2;
    uint32_t CONF3;
    uint32_t PLLCONF;
    uint32_t DIV;
    uint32_t FDIV;
    uint32_t STRM;
    uint32_t CLK;
    uint32_t TEST1;
    uint32_t TEST2;
} MAX2769;

