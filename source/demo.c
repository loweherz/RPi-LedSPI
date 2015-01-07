/*
	demo.c - Demo example for LED APA SPI for Raspberry PI
	
	Authors:	
	Mirko Mancin - mirko.mancin@gmail.com
	Marco Mambiarni - marco.mambriani1@gmail.com
	
	Created on 16/12/2014
*/

#include <stdio.h>
#include <wiringPiSPI.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>


void initBuff();
void milliSleep(int);
int kbhit(void);
unsigned char buff[88];
int num_state[4];

int main()
{
	 //SPI Parameter
     int chan=1;
     int speed=512000;
     int i;
     char state;
     int seq_state;
     int up=1;

     if( wiringPiSPISetup (chan, speed)==-1)
     {
         printf("Could not initialise SPI\n");
         return(0);
     }
     printf("When ready hit enter.\n");
     (void) getchar();// remove the CR

	 //prepare buffer with begin and end byte for SPI Led controller - check datasheet for more information
     initBuff();
     printf("- 'a' demo alternate\n");
     printf("- 'r' red\n");
     printf("- 'g' green\n");
     printf("- 'b' blue\n");
     printf("- 'm' misto\n");
     printf("- 'm' misto\n");
	 printf("- 'p' demo pong")
     printf("- 'q' turn off\n");
     printf("- '0' quit\n\n\n");


     while(1)
     {

         if (kbhit())
         {
            state = getc(stdin);
            seq_state=0;
         }

         switch(state)
         {
                case'a': //R-G-B
                    if (seq_state == 0)
                    {
                        //RED
                        for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=0;
                            buff[4*i+6]=0;
                            buff[4*i+7]=255;
                        }
                        seq_state++;
                        wiringPiSPIDataRW (chan,buff,88);
                    }
                    else if (seq_state == 1)
                    {
                        //GREEN
                        for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=0;
                            buff[4*i+6]=255;
                            buff[4*i+7]=0;
                        }
                        seq_state++;
                        wiringPiSPIDataRW (chan,buff,88);
                    }
                     else if (seq_state == 2)
                    {
                        //BLUE
                        for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=255;
                            buff[4*i+6]=0;
                            buff[4*i+7]=0;
                        }
                        seq_state=0;
                        wiringPiSPIDataRW (chan,buff,88);
                    }
                    milliSleep(1000);
                    break;
                case'r':
                    for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=0;
                            buff[4*i+6]=0;
                            buff[4*i+7]=255;
                        }
                        wiringPiSPIDataRW (chan,buff,88);
                    break;
				case'g':
                    for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=0;
                            buff[4*i+6]=255;
                            buff[4*i+7]=0;
                        }
                        wiringPiSPIDataRW (chan,buff,88);
                    break;
                case'b':
                    for(i = 0; i<20;i++)
                        {
                            buff[4*i+4]=0xf0;
                            buff[4*i+5]=255;
                            buff[4*i+6]=0;
                            buff[4*i+7]=0;
                        }
                        wiringPiSPIDataRW (chan,buff,88);
                    break;
				case'm':
                    for(i = 0; i<20;i++)
                    {
                         buff[4*i+4]=0xf0;
                         if(i%3==0)
                         {
                             buff[4*i+5]=0;
                             buff[4*i+6]=0;
                             buff[4*i+7]=255;
                         }
                         else if(i%3==1)
                         {
                             buff[4*i+5]=0;
                             buff[4*i+6]=255;
                             buff[4*i+7]=0;
                         }
                         else if(i%3==2)
                         {
                             buff[4*i+5]=255;
                             buff[4*i+6]=0;
                             buff[4*i+7]=0;
                         }
                    }
                    wiringPiSPIDataRW (chan,buff,88);
                    break;
				case'p':
                    for(i = 0; i<20;i++)
                    {
                          buff[4*i+4]=0xf0;
                          buff[4*i+5]=0;
                          buff[4*i+6]=0;
                          buff[4*i+7]=0;
                    }
                    buff[4*seq_state+5]=255;
                    buff[4*seq_state+6]=255;
                    buff[4*seq_state+7]=255;

                    if ((seq_state<=19)&&(up==1))
                    {
                        if (seq_state<19) seq_state++;
                        else up=0;
                    }
                    if ((seq_state>=0)&&(up==0))
                    {
                        if (seq_state>0) seq_state--;
                        else up=1;
                    }
                    wiringPiSPIDataRW (chan,buff,88);
                    milliSleep(100);
                    break;
                case 'q':
                    //TURN OFF
                    for(i = 0; i<20;i++)
                    {
                         buff[4*i+4]=0xf0;
                         buff[4*i+5]=0;
                         buff[4*i+6]=0;
                         buff[4*i+7]=0;
                    }
                    wiringPiSPIDataRW (chan,buff,88);
                    break;
                case '0':
                    //TURN OFF
                    for(i = 0; i<20;i++)
                    {
                         buff[4*i+4]=0xf0;
                         buff[4*i+5]=0;
                         buff[4*i+6]=0;
                         buff[4*i+7]=0;
                    }
                    wiringPiSPIDataRW (chan,buff,88);
                    return(0);
                    break;


         }
     }
}


void initBuff()
{
    int i;

    buff[0]=0;
    buff[1]=0;
    buff[2]=0;
    buff[3]=0;

    buff[87]=255;
    buff[86]=255;
    buff[85]=255;
    buff[84]=255;
}

int kbhit(void)
{
   struct termios oldt, newt;
   int ch;
   int oldf;

   tcgetattr(STDIN_FILENO, &oldt);
   newt = oldt;
   newt.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
   oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
   fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

   ch = getchar();

   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
   fcntl(STDIN_FILENO, F_SETFL, oldf);

   if(ch != EOF)
   {
      ungetc(ch, stdin);
      return 1;
   }

   return 0;
}

void milliSleep(int millisec)
{
        int i;
        for (i=0;i<millisec;i++)
        {
                usleep(1000);
        }
}



