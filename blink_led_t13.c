#define F_CPU 9600000UL
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <mega8.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#define OUTPUTPWM1 PB0
#define OUTPUTPWM2 PB1
#define BUTTONPLUS PB4
#define BUTTONMINUS PB2
#define BUTTONSWICH PB3

#define HOLDTIME 2048
#define HOLDSPEED 64

/*__eeprom uint8_t PWMVALUE = 255;
uint8_t setting PWMVALUE;*/

int PWM1 = 0;
unsigned int holdCounter = 0;
unsigned int holdSpeedCounter = 0;



void init_io(void);

void init_io(void)
{
  // Пины кнопок
  DDRB &= ~((1<<BUTTONPLUS)|(1<<BUTTONMINUS)|(1<<BUTTONSWICH)); // входы
  PORTB |= (1<<BUTTONPLUS)|(1<<BUTTONMINUS)|(1<<BUTTONSWICH); // подтянуты

  // Пин светодиода
  DDRB |= (1<<OUTPUTPWM1); // выход
  PORTB &= ~(1<<OUTPUTPWM1); // выключен
  //PORTB ^= (1<<OUTPUTPWM1);
}

void processButtons(void);

void processButtons(void){
  if ( (PINB & (1<<BUTTONMINUS)) == 0 ) // если нажата одна из кнопок
  {
    PWM1+=1;
    if(PWM1>255){
      PWM1 = 255;
    }
  }

  if ( (PINB & (1<<BUTTONPLUS)) == 0 ) // если нажата одна из кнопок
  {
    PWM1-=1;
    if(PWM1<0){
      TCNT0 = 0x00;
      PWM1 = 0;
    }
  }

  OCR0A = PWM1;
}

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
  processButtons();
}

ISR(TIM0_OVF_vect) {
  if ( ((PINB & (1<<BUTTONMINUS)) == 0 ) || ( (PINB & (1<<BUTTONPLUS)) == 0 )){
    holdSpeedCounter++;
    if((holdCounter>HOLDTIME) && (holdSpeedCounter>HOLDSPEED)){
      processButtons();
      holdSpeedCounter=0;
    }else{
      
      holdCounter++;
    }
  }else{
    holdCounter = 0;
  }
}

int main(void)
{
  init_io();

  // Таймер для ШИМ:
  TCCR0A = 0xB1; // режим ШИМ, неинверсный сигнал на выходе OC0A, инверсный - на выходе OC0B
  TCCR0B = 0x01; // предделитель тактовой частоты CLK/8
  TCNT0=0; // начальное значение счётчика
  OCR0A=0; // регистр совпадения A
  OCR0B=0; // регистр совпадения B

  TIMSK0 = (1<<TOIE0); // разрешить прерывание по переполнению таймера счетчика Т0
  TCNT0 = 0x00; // обнулить счетный регистр
  
  // Настройка прерываний
  GIMSK |= (1<<PCIE); // Разрешаем внешние прерывания PCINT0.
  PCMSK |= (1<<BUTTONPLUS)|(1<<BUTTONMINUS)|(1<<BUTTONSWICH); // Разрешаем по маске прерывания на ногак кнопок (PCINT3, PCINT4)
  sei();
  
  while(1)
  {
  }
} 
