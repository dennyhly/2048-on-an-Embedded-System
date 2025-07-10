#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef PERIPH_H
#define PERIPH_H


void ADC_init() {
  ADMUX = (1<<REFS0);
	ADCSRA|= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

unsigned int ADC_read(unsigned char chnl){
  	uint8_t low, high;

  	ADMUX  = (ADMUX & 0xF8) | (chnl & 7);
  	ADCSRA |= 1 << ADSC ;
  	while((ADCSRA >> ADSC)&0x01){}
  
  	low  = ADCL;
	high = ADCH;

	return ((high << 8) | low) ;
}

int getDirections() {
  if (ADC_read(0) <= 255) {return 0;} //up
  else if (ADC_read(0) >= 767) {return 1;} //down
  else if (ADC_read(1) >= 767) {return 2;} //left
  else if (ADC_read(1) <= 255) {return 3;} //right
  else {return 4;}
}

#endif