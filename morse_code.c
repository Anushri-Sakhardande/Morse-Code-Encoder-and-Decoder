#include <LPC17xx.h>

#define RS_CTRL 0x08000000 //P0.27
#define EN_CTRL 0x10000000 //P0.28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines

unsigned long int temp1=0, temp2=0,i,j,temp ;
unsigned char flag1 =0, flag2 =0;
unsigned char msg[] = {"DECODE OR ENCODE"};
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};

unsigned int row, col;

unsigned char morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};
unsigned int seven_alpha[]={0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D,0x76, 0x30, 0x1E, 0x75, 0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D, 0x64, 0x6E, 0x5B};
	
	
void encode(void);
void decode(void);
	
void display_lcd(void);
void lcd_write(void);
void port_write(void);

	
void input_keyboard(void);

void main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	//configuration for matrix keyboard and seven segment
	LPC_PINCON->PINSEL0 = 0; //P0.4 to P0.11 GPIO data lines
	LPC_GPIO0->FIODIR = 0xFFFFFFFF;//Port 0 output
	LPC_PINCON->PINSEL3 = 0; //P1.23 to P1.26 MADE GPIO
	LPC_PINCON->PINSEL4 = 0; //P2.10 t P2.13 made GPIO
	LPC_GPIO2->FIODIR = 0x00003C00; //made output P2.10 to P2.13 (rows)
	LPC_GPIO1->FIODIR =0; //made input P1.23 to P1.26 (cols)
	
	//configuration for LCD
	LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTR;
	
	//configuration for buzzer
	

	
	//select encode or decode(maybe display a select option in lcd)
	while(1){
			display_lcd();
			
			input_keyboard();
			
			if(row )
	}
}

void encode(void){
	//loop input from keyboard till we get an escape character maybe(maybe display a select option in lcd)
	
	//take keyboard input 
	
	// display on seven segment/lcd
}

void decode(void){
	//matrix keyboard input for characters expect an escape sequence after each character?
	
	
	
	while(1){

		
	
	//select between buzzer and led
	
	// output on buzzer using timers to decide the length of the beep? + led maybe?
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
						scan();
						if(flag == 1)

						temp2 = SEVEN_CODE[row][col];
						LPC_GPIO0->FIOMASK=0xFFF87FFF;
						LPC_GPIO0->FIOPIN = FIRST_SEG;
						temp2 = temp2 << 4;
						LPC_GPIO0->FIOMASK=0xFFFFF00F;
						LPC_GPIO0->FIOPIN = temp2;
						break;
			}
	}
}

//display string onto lcd
void display_lcd(void){
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