
mridhula k <mridhulak99@gmail.com>
Tue, Mar 27, 2018, 12:05 PM
to srinidhi

#include<reg51.h>
#include<string.h>
 
sbit rs=P2^5;             //Register Select(RS) pin of 16*2 lcd
sbit rw=P2^6;             //Read/Write(RW) pin of 16*2 lcd
sbit en=P2^7;             //Enable(E) pin of 16*2 lcd
 
char count = 0;            // count = 0
char input[13];           // character array of size 12
char ch;   

unsigned int usecCnt=0;
unsigned int msCnt=0;
unsigned int secCnt=0;

void delay(unsigned int);                   //function for creating delay
void cmdwrt(unsigned char);              //function for sending commands to 16*2 lcd display
void datawrt(unsigned char);             //function for sending data to 16*2 lcd display
char rxdata(void);                                //function for receiving a character through serial port of 8051 
void InitTimer0(void);
void UpdateTimeCounters(void);
 
 void main(void)
 {
	 
	 /**  0x38 - Use 2 lines and 5×7 matrix
	      0x01 - Clear Screen
	      0x06 - Increment cursor
	      0x0c - Display ON, Cursor OFF
	      0x80 - Force cursor to the beginning of  1st line
	 **/
                  
unsigned char cmd[]={0x38,0x01,0x06,0x0c,0x80}; //16*2 lcd initialization commands
unsigned char msg[]={"RFID Attendance "};
unsigned char i,k;
unsigned char tag[2][13]={"450041767D0F\0","270012F05792\0"};
unsigned char name[2][13]={"Mridhula    ","SriNidhi    "};
unsigned char display[12]={"Invalid Tag "};


 
                  for(i=0;i<5;i++)     //send commands to 16*2 lcd display one command at a time
                 {
                         cmdwrt(cmd[i]);       //function call to send commands to 16*2 lcd display
                         delay(1);
                 }
 
                for(i=0;i<16;i++)             //send data to 16*2 lcd display one character at a time
                {
                         datawrt(msg[i]);    //function call to display message on 16*2 lcd display
                        delay(1);
                }
								delay(500);
							
								InitTimer0();
 
                 while(1)
                 {
													strcpy(display,"Time Out    ");
                          count=0;
									       /*  0xC2 - Jump to second line, position2 */
                          cmdwrt(0xC2); 
									 
												 UpdateTimeCounters(); 
									 
													 for(i=0;i<5;i++)     //send commands to 16*2 lcd display one command at a time
													{
																cmdwrt(cmd[i]);       //function call to send commands to 16*2 lcd display
																delay(1);
													}
									 
									     //  if( msCnt == 0 )                                       // msCounter becomes zero after exact one sec
												//{
										//			datawrt(0x01);   // Clear screen command
											//		delay(1000);
												//	datawrt((secCnt/10)+0x30);
													//datawrt((secCnt%10)+0x30);
												//}
													
												// continue;
									 
											if (secCnt >=10)
												{
										       for(i=0;i<5;i++)     //send commands to 16*2 lcd display one command at a time
													{
																cmdwrt(cmd[i]);       //function call to send commands to 16*2 lcd display
																delay(1);
													}
													strcpy(display,"Time Out    ");
													for(i=0;i<12;i++)             //send data to 16*2 lcd display one character at a time
													{
														
														datawrt(display[i]);    //function call to display message on 16*2 lcd display
														delay(1);
													}
													continue;
												}
									 
                         while(count<12)                //repeat for 12 times
                         {
                                    input[count]=rxdata(); //receive serial data and store it.
                                    count++;
                         }
												 input[count]='\0';
												 for (k=0;k<2;k++)
												 {
														if(strncmp(input,tag[k],12)==0)
														{
															strncpy(display,name[k],strlen(name[k]));
															break;
														}
												 }
												 
												   for(i=0;i<5;i++)     //send commands to 16*2 lcd display one command at a time
												{
															cmdwrt(cmd[i]);       //function call to send commands to 16*2 lcd display
															delay(1);
												}
 
														for(i=0;i<12;i++)     //send data to 16*2 lcd display one character at a time
                          {
															datawrt(input[i]);                //function call to display rfid card number on 16*2 lcd display
                             delay(1);
                          }
										
													cmdwrt(0xC2); 
													delay(100);
													
														for(i=0;i<12;i++)     //send data to 16*2 lcd display one character at a time
                          {
															datawrt(display[i]);                //function call to display rfid card number on 16*2 lcd display
                             delay(1);
                          }
													
                            delay(100);
														
									 
									  //datawrt(rxdata());
                   }
}
 
void cmdwrt (unsigned char x)
{ 
   P0=x;  //send the command to Port 1 on which 16*2 lcd is connected
   rs=0;  //make RS = 0 for command
   rw=0;  //make RW = 0 for write operation
   en=1;  //send a HIGH to LOW pulse on Enable(E) pin to start commandwrite operation 
   delay(1);
   en=0;
}
 
void datawrt (unsigned char y)
{ 
   P0=y; //send the data to Port 1 on which 16*2 lcd is connected
   rs=1; //make RS = 1 for command
   rw=0; //make RW = 0 for write operation
   en=1; //send a HIGH to LOW pulse on Enable(E) pin to start datawrite operation
   delay(1);
   en=0;
}
 
char rxdata()
{
  while(RI==0);   //wait till RI becomes HIGH
  ch=SBUF;      //copy received data 
	RI=0;           //make RI low
  return ch;      //return the received data to main function.
}
 
void delay(unsigned int z)
{
  unsigned int p,q;
  for(p=0;p<z;p++)    //repeat for 'z' times
  {
    for(q=0;q<1375;q++);   //repeat for 1375 times
  }
}

void InitTimer0(void)
{
	TMOD=0x20;                  //timer 1 , mode 2 ,auto reload timer 0 mode 2
SCON=0x50;                   //8bit data , 1 stop bit , REN enabled
    //start the timer
	TMOD &= 0xF0;    // Clear 4bit field for timer0
	TMOD |= 0x02;    // Set timer0 in mode 2
	
	TH0 = 0x05;      // 250 usec reloading time
	TL0 = 0x05; 
  TH1=0xfd;                  //timer value for 9600 bits per second(bps)
  
	
	ET0 = 1;         // Enable Timer0 interrupts
	EA  = 1;         // Global interrupt enable
	
	TR0 = 1;	// Start Timer 0
	TR1=1;	// First time value
}
void Timer0_ISR (void) interrupt 1     // It is called after every 250usec
{
	
	usecCnt = usecCnt + 250;   // Count 250 usec
	
	if(usecCnt==1000)              // 1000 usec means 1msec 
	{
		msCnt++;
		usecCnt = 0;
	}

	TF0 = 0;     // Clear the interrupt flag
}

void UpdateTimeCounters(void)
{
	if (msCnt==1000)
	{
		secCnt++;
		msCnt=0;
	}

	
}