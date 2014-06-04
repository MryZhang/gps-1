gcc SoftOSGPS.c correlator.c display.c gp2021.c gpsfuncs.c gpsisr.c interfac.c nav_fix.c rinex.c nmea.c FwInter.c -o SoftOSGPS_single_channel -lm -O3 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DSOFT -Wall -Wno-unused-result -Wunused-function -fsigned-char #-DINTERACTIVE # -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -marm -mthumb-interwork


gcc SoftOSGPS.c correlator.c display.c gp2021.c gpsfuncs.c gpsisr.c interfac.c nav_fix.c rinex.c nmea.c FwInter.c -o SoftOSGPS -lm -O3 -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DSOFT -Wall -Wno-unused-result -Wunused-function -fsigned-char -DINTERACTIVE # -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp -marm -mthumb-interwork

gcc discover.c -o discover.o

#gcc -msoft-float SoftOSGPS.c correlator.c display.c gp2021.c gpsfuncs.c gpsisr.c interfac.c nav_fix.c rinex.c -o SoftOSGPS -lm -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DSOFT -Wall -Wno-unused-result -Wunused-function -lsoft-fp -L. # -mcpu=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp -ffast-math -fsingle-precision-constant -marm -mthumb-interwork -mfpu=vfp






#gcc SoftOSGPS.c correlator.c display.c gp2021.c gpsfuncs.c gpsisr.c interfac.c nav_fix.c rinex.c -o SoftOSGPS -lm -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DSOFT -Wall -Wno-unused-result -Wunused-function -mcpu=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp #-ffast-math -fsingle-precision-constant
#clang SoftOSGPS.c correlator.c display.c gp2021.c gpsfuncs.c gpsisr.c interfac.c nav_fix.c rinex.c -o SoftOSGPS -O3 -lm -D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DSOFT -Wall -Wno-unused-result -Wunused-function
