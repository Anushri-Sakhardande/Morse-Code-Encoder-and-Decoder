#include <LPC17xx.h>

#define RS_CTRL 0x08000000 //P0.27
#define EN_CTRL 0x10000000 //P0.28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines

unsigned long int temp1=0, temp2=0,i,j,temp ;
unsigned char flag1 =0, flag2 =0;
unsigned int flag;
unsigned char msg[] = {"DECODE OR ENCODE"};
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};

unsigned int row, col;

unsigned char morse[26][4] = {
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
	
unsigned char KeyMap[10][6] = {
    { '0',   0,   0,   0,   0,   0 },  // Key 1 
    { '1', 'A', 'B', 'C',   0,   0 },  // Key 2: A, B, C
    { '2', 'D', 'E', 'F',   0,   0 },  // Key 3: D, E, F
    { '3', 'G', 'H', 'I',   0,   0 },  // Key 4: G, H, I
    { '4', 'J', 'K', 'L',   0,   0 },  // Key 5: J, K, L
    { '5', 'M', 'N', 'O',   0,   0 },  // Key 6: M, N, O
    { '6', 'P', 'Q', 'R', 'S',   0 },  // Key 7: P, Q, R, S
    { '7', 'T', 'U', 'V',   0,   0 },  // Key 8: T, U, V
    { '8', 'W', 'X', 'Y', 'Z',   0 }   // Key 9: W, X, Y, Z
};

	
	
void encode(void);
void decode(void);
	
void display_lcd(unsigned char[]);
void lcd_write(void);
void port_write(void);
				
void input_keyboard(void);
				
void delay_lcd(unsigned int r1);

int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	//TODO: Decide on the configurations
	
	//configuration for matrix keyboard and seven segment
	LPC_PINCON->PINSEL0 = 0; //P0.4 to P0.11 GPIO data lines
	LPC_GPIO0->FIODIR = 0xFFFFFFFF;//Port 0 output
	LPC_PINCON->PINSEL3 = 0; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL4 = 0; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR = 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR =0; //made input P1.23 to P1.26 (cols)
	
	//configuration for LCD
	LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
	
	//configuration for LED
	

	
	//select encode or decode(maybe display a select option in lcd)
	while(1){
			display_lcd("ENCODE OR DECODE");
			input_keyboard();
			
			if(row==0 && col==3){
				display_lcd("Enter your sentence");
				encode();
			}else if(row==1 && col==3){
				display_lcd("Enter the code");
				decode();
			}else{
				display_lcd("ERROR");
			}
	}
}


void encode(void){
	while(1){
		input_keyboard();
		
		 char keyBuffer[4]; //The input buffer.
		 int  keyIndex = 0; //The index into keyBuffer. 
		 int keyPress1 = -1, keyPress2; // keyboard inputs.
		 int numKeyPresses; // The number of times the same key has been pressed this phase.



			 /* keyboard processing loop. */
			 while ( 1 )
			 {
				 // Phase timed out.  Commit current character.
				 keyBuffer[keyIndex++] = KeyMap[keyPress1][numKeyPresses - 1];
				 keyPress1 = -1;
				}
				if ( ( keyPress2 = KeyAvailable() ) > -1 )
				{
				 // Key pressed.
				 if ( ( keyPress2 != keyPress1 ) && ( -1 != keyPress1 ) )
				 {
					// Different than last key.  Commit current character and start a new one.
					keyBuffer[keyIndex++] = KeyMap[keyPress1][numKeyPresses - 1];
					numKeyPresses = 1; // Yes, I"m using 1-based indexing.  Deal.

				 }else if ( keyPress2 == keyPress1 )
				 {
					// Pressed same key multiple times in same phase. 
					numKeyPresses = ((numKeyPresses) % KEYMAPROWSIZE) + 1;
					if ( 0 == KeyMap[keypress2][numKeyPresses - 1] )
					{
					 //Loop back to first 'valid' character associated with this key.
					 numKeyPresses = 1;
					}
				 }else // -1 == keyPress1 
				 {
					// Pressed new key. Start new phase. 
					numKeyPresses = 1;
				 }
				 keyPress1 = keyPress2.
				}
			 } 
		
		// display on seven segment
	}
}

void decode(void){
	
	while(1){
	//matrix keyboard input for characters expect an escape sequence after each character?
	input_keyboard();
	
	
	// output on buzzer using timers to led
}
}

//Should return which key pressed
void input_keyboard(void){
	//poll rows 
	while(1){
		for(row=0;row<4;row++){
						if(row == 0)
						temp = 1 << 10;
						else if(row == 1)
						temp = 1 << 11;
						else if(row == 2)
						temp=1 << 12;
						else if(row == 3)
						temp = 1 << 13;

						LPC_GPIO2->FIOPIN = temp;
						flag = 0;
							
						unsigned int temp3 = LPC_GPIO1->FIOPIN;
						temp3 &= 0x07800000;
						if(temp3 != 0x00000000){
							flag = 1;
							if (temp3 == 1 << 23)
							col=0;
							else if (temp3 == 1 << 24)
							col=1;
							else if (temp3 == 1 << 25)
							col=2;
							else if (temp3 == 1 << 25)
							col=3;
						}
						
						if(flag == 1){
								break;
						}
			}
	}
}

//display string onto lcd
void display_lcd(unsigned char msg[]){
	//write to lcd
	flag1 =0;//Command
	for (i=0; i<9;i++)
	{
		temp1 = init_command[i];
		lcd_write(); //send Init commands to LCD
	}
	flag1 =1;//Data
	i =0;
	while (msg[i++] != '\0')
	{
		temp1 = msg[i];
		lcd_write();//Send data bytes
	}
	while(1);
}

void lcd_write(void)
{
	flag2 = (flag1 == 1) ? 0 :((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
	temp2 = temp1 & 0xf0;
	temp2 = temp2 << 19;//data lines from 23 to 26. Shift left (26-8+1) times so that higher digit is sent on P0.26 to P0.23
	port_write(); // Output the higher digit on P0.26-P0.23
	if (!flag2) // Other than command 0x30, send the lower 4-bt also
	{
		temp2 = temp1 & 0x0f; //26-4+1
		temp2 = temp2 << 23;
		port_write(); // Output the lower digit on P0.26-P0.23
	}
}

void port_write(void)
{
	LPC_GPIO0->FIOPIN = temp2;
	if (flag1 == 0)
		LPC_GPIO0->FIOCLR = RS_CTRL; // Select command register
	else
		LPC_GPIO0->FIOSET = RS_CTRL; //Select data register
	
	LPC_GPIO0->FIOSET = EN_CTRL; //Apply -ve edge on Enable
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	delay_lcd(5000);
}

void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r=0;r<r1;r++);
	return;
}