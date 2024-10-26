#include <LPC17xx.h>
#include <stdio.h>
#include <string.h>
#define RS_CTRL 0x00000100  //P0.8
#define EN_CTRL 0x00000200  //PO.9
#define DT_CTRL 0x000000F0  //PO.4 TO P0.7
#define BUZZER_PIN (1<<17)  // P2.13 for buzzer
#define PRESCALE (25000-1) 

unsigned long int temp1=0, temp2=0,i,j,temp ;
unsigned char flag1 =0, flag2 =0;
unsigned int flag;
unsigned char msg[] = "DECODE OR ENCODE";
unsigned long int init_command[] = {
0x30, //8 bit 
0x30, //8 bit 
0x30, //8 bit
0x20, //4 bit
0x28, //use 2 lines
0x0c, //display ON
0x06, //increment cursor
0x01, //clear display
0x80}; //set first line first position

signed int row,col;

unsigned char morse[26][5] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
};

unsigned int seven_alpha[]={
	0x77, //A
	0x7C, //B
	0x39, //C
	0x5E, //D
	0x79, //E
	0x71, //F
	0x3D, //G
	0x76, //H
	0x30, //I
	0x1E, //J
	0x75, //K
	0x38, //L
	0x55, //M
	0x54, //N
	0x5C, //O
	0x73, //P
	0x67, //Q
	0x50, //R
	0x6D, //S
	0x78, //T
	0x3E, //U
	0x1C, //V
	0x1D, //W
	0x64, //X
	0x6E, //Y
	0x5B  //Z
};


unsigned int alpha[26]={
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z'
};

unsigned int MatrixMap[3][3]={
		{0,1,2},
		{3,4,5},
		{6,7,8}
};
	
unsigned char KeyMap[9][5] = {
    {0,0,0,0},  // Key 1 
    {'A', 'B', 'C',   0},  // Key 2: A, B, C
    {'D', 'E', 'F',   0},  // Key 3: D, E, F
    {'G', 'H', 'I',   0},  // Key 4: G, H, I
    {'J', 'K', 'L',   0},  // Key 5: J, K, L
    {'M', 'N', 'O',   0},  // Key 6: M, N, O
    {'P', 'Q', 'R', 'S'},  // Key 7: P, Q, R, S
    {'T', 'U', 'V',   0},  // Key 8: T, U, V
    {'W', 'X', 'Y', 'Z'}   // Key 9: W, X, Y, Z
};	
void encode(void);
void decode(void);
	
void display_lcd(unsigned char[]);
void lcd_write(void);
void port_write(void);
				
void input_keyboard(void);
void scan(void);
				
void delay_lcd(unsigned int r1);
void delayinUS(unsigned int microseconds);
void delayMS(unsigned int milliseconds);

int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	//Configuration for the buzzer
	LPC_PINCON->PINSEL1=0;
	LPC_GPIO0->FIODIR=1<<17;
	LPC_GPIO0->FIOSET=0<<17;
	//configuration for matrix keyboard and seven segment
	LPC_PINCON->PINSEL3 = 0; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL4 = 0; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR = 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR =0; //made input P1.23 to P1.26 (cols)
	delayMS(100000);
	//configuration for LCD
	LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;
	
	
	//select encode or decode(maybe display a select option in lcd)
	while(1){
		unsigned char msg[] = {"ENCODE OR DECODE"};
		unsigned char msg1[] = {"Enter word"};
			display_lcd(msg);
			input_keyboard();
			
			//sprintf(msg1, "Row: %d Col: %d", row, col);
			//display_lcd(msg1);
			//delay_lcd(10000000);
			if(row==0 && col==3){
				unsigned char msg[] = {"Enter letter"};
				display_lcd(msg);
				delay_lcd(10000000);
				encode();
			}else if(row==1 && col==3){
				unsigned char msg[] = {"Enter code"};
				display_lcd(msg);
				delay_lcd(10000000);
				decode();
			}else{
				unsigned char msg[] = {"ERROR"};
				display_lcd(msg);
				delay_lcd(10000000);
			}
	}
}


void encode(void){
	unsigned char msg[] = {"Encoding.."};
	int keyPress1;
	int keyPress2=-1;
	int count=0;
	char currChar;
	unsigned char inputBuffer[16];
	int bufferIndex=0;
display_lcd(msg);
	while(1){
		input_keyboard();
		if(row==0 && col==0){ //escape
			inputBuffer[bufferIndex++] = currChar;
			display_lcd(inputBuffer);
			keyPress2=-1;
		}
		if(row==3 && col==2){
			break;
		}
		else{
			keyPress1 = MatrixMap[row][col];
			if(keyPress2==-1){
				count = 0;
				currChar = KeyMap[keyPress1][count];
				keyPress2=keyPress1;
			}else if(keyPress1==keyPress2){
				currChar = KeyMap[keyPress1][++count];
			}else{
				count =0;
				currChar=' ';
			}
		}
	}
	display_lcd(inputBuffer);
}

void decode(void){
unsigned char msg[] = {"Decoding.."};
unsigned char msg1[5];
unsigned char inputBuffer[16];
unsigned char outputBuffer[16];
unsigned int ctr=0;
unsigned int ctr2=0;
unsigned int z;

	display_lcd(msg);
	while(1){
	//matrix keyboard input for characters expect an escape sequence after each character
		input_keyboard();
		if(row==2 && col==3) // for the dot
		{
			inputBuffer[ctr++]='.';
			 inputBuffer[ctr] = '\0';
			display_lcd(inputBuffer);
		}
		else if(row==3 && col==3) // for the dash
		{
			inputBuffer[ctr++]='-';
			inputBuffer[ctr] = '\0';
			display_lcd(inputBuffer);
		}
		else if(row==0 && col==0) // for end of character
		{
			unsigned char msg[] = {"Converting"};
					display_lcd(msg);
			inputBuffer[ctr] = '\0';
			for ( z = 0; z < 26; z++) {
        if ((strcmp((const char *)morse[z], (const char *)inputBuffer) == 0))
				{
					
				  outputBuffer[ctr2++]=alpha[z];
					outputBuffer[ctr2] = '\0';
					display_lcd(outputBuffer);
					delayMS(1000);
					inputBuffer[0] = '\0';
					ctr=0;
					break; 
				}
		}
			
		}
		else if(row==3 && col==2)
		{
			break;
	}
}
}
//Should return which key pressed
void input_keyboard(void){
	int Break_flag = 0;
	row=0;
	col=0;
	while(1){
	//poll rows 
 for(row = 0; row <4; row++){
			 if(row == 0) temp = 1<<10;
			 else if(row == 1) temp = 1<< 11;
			 else if(row == 2) temp = 1<< 12;
			 else if(row == 3) temp = 1<< 13;
				//temp=1<<(10+row);
			 LPC_GPIO2->FIOPIN = temp; //WRITING TO PORT 2
			 flag = 0;
			 delayMS(10000);
			 scan();
			 if(flag == 1){ 
				 Break_flag=1;
				 delayMS(2000);
				 break;
			 }				 
		 }
		 if(Break_flag==1)
			 break;
	 }
}

	void scan(void){
		unsigned long temp3;

			temp3 = LPC_GPIO1->FIOPIN;
			temp3 &= 0x07800000;
			if(temp3 != 0x00000000){
				flag = 1;
				if (temp3 == 1 << 23)
				col=0;
				else if (temp3 == 1 << 24)
				col=1;
				else if (temp3 == 1 << 25)
				col=2;
				else if (temp3 == 1 << 26)
				col=3;
			}
	}

void display_lcd(unsigned char msg[])
{
	
	unsigned int k=0;
	
	LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
	flag1 =0;
		for (i=0; i<9;i++)
				{
			temp1 = init_command[i];
			lcd_write();
				}
	flag1 =1; //DATA MODE
		i =0;
		while (msg[i] != '\0')
			{
			temp1 = msg[i];
			lcd_write();
				i+= 1;
			}
		while(k<100){
			k++;
		};
}

void lcd_write(void)
			{
			
				flag2 = (flag1 == 1) ? 0 :((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
				temp2 = temp1 & 0xf0;

		temp2 = temp2 << 19;
		port_write();
if (flag2==0)
	{
		temp2 = temp1 & 0x0f; //26-4+1
		temp2 = temp2 << 23;
		port_write();
			}
}

void port_write(void)
{
		//LPC_GPIO0->FIOMASK = 0xF00FFFF;
		LPC_GPIO0->FIOPIN = temp2;
	if (flag1 == 0)
			LPC_GPIO0->FIOCLR = RS_CTRL;
	else
				LPC_GPIO0->FIOSET = RS_CTRL;
				
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(10000);
	LPC_GPIO0->FIOCLR = EN_CTRL;
		delay_lcd(500000);
}

void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r=0;r<r1;r++);
return;
}


 
 void delayinUS(unsigned int microseconds){
		LPC_TIM0->TCR = 0x02;
		LPC_TIM0->PR = 0; // Set prescaler to the value of 0
		LPC_TIM0->MR0 = microseconds - 1; // Set match register for 10us
		LPC_TIM0->MCR = 0x01; // Interrupt on match
		LPC_TIM0->TCR = 0x01; // Enable timer
		while ((LPC_TIM0->IR & 0x01) == 0); // Wait for interrupt flag
		LPC_TIM0->TCR = 0x00; // Stop the timer
		LPC_TIM0->IR = 0x01; // Clear the interrupt flag
 }
 void delayMS(unsigned int milliseconds) //Using Timer0
 { 
		delayinUS(1000 * milliseconds);
 }
