#include <LPC17xx.h>

void encode();
void decode();
void display_lcd();
void input_keyboard();

void main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	//configuration
	
	//select encode or decode(maybe display a select option in lcd)
	while(){ // not selected 
		
	}
}

void encode(void){
	//loop input from keyboard till we get an escape character maybe(maybe display a select option in lcd)
	
	//take keyboard input 
	
	//select between seven segment/lcd
	
	// display on seven segment/lcd
}

void decode(void){
	//matrix keyboard input for characters expect an escape sequence after each character?
	
	//select between buzzer and led
	
	// output on buzzer using timers to decide the length of the beep? + led maybe?
}


//Should return which key pressed
void input_keyboard(void){
	//poll rows 
	
	//poll columns 
	
}

void display_lcd(void){
	
}