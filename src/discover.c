#include <stdio.h>
#include <string.h>

FILE* f;
unsigned char fname[8];
unsigned char buffer[8];
unsigned int i,j;
unsigned int max_sig, min_sig, max_time, min_time;
unsigned char t5[100];
unsigned char aux[50];
unsigned char failcount;
unsigned char shortans[10]={0};

int main(void)
{
   j=0;
   min_sig=300;
   max_sig=0;
   min_time=300;
   failcount=0;
   strcpy(t5,"./SoftOSGPS -m 4 ");
   strcat(t5,"-f ../dump-ros.raw ");



   for(i=1;i<=32;i++)
   {
      sprintf(fname,"rpn%i",i);
      f=fopen(fname,"r");
      if(f==NULL){failcount++;}
      if(fread(buffer,3,1,f)==1)
      {
         //buffer[0] es el tiempo en segundos en que aparece el sate
         if(buffer[0]<min_time){min_time=buffer[0];}
	 
         if(buffer[2]<min_sig){min_sig=buffer[2];}
         if(buffer[2]>max_sig){max_sig=buffer[2];}
         //printf("SAT: %i, %i, %i, %i \n",buffer[1],i,4+((i-1)>>3),((i-1)%8));

	shortans[7-((i-1)>>3)]|=1<<((i-1)%8);

if(j<10){
         sprintf(aux,"-%i %i ",j,i);
         strcat(t5,aux);
}

         j++;
      }

   fclose(f);

   }
	
// fail count indica si se pudieron abrir ok los archivos
// short ans va a ser: cant sates, min time, min sig, max sig y desp una mask de 32 bits con los sates que se vieron
shortans[0]=j;
shortans[1]=min_time;
shortans[2]=min_sig;
shortans[3]=max_sig;



//./SoftOSGPS -m 4 -f ../dump-ros.raw -0 3 -1 6 -2 14 -3 19 -4 22 -5 27

//./SoftOSGPS -m 4 -f ../dump-ros.raw -0 3 -1 6 -2 14 -3 19 -4 22 -5 27 666>tout 1>/dev/null

printf("%s 666>tout 1>/dev/null\n",t5);


write(666,&failcount, 1);
write(666,shortans, sizeof(shortans));
   //printf("MinTime: %i\n MinSig: %i - MaxSig: %i\n",min_time,min_sig,max_sig);


   return 0;
}
