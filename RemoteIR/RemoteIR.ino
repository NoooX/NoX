#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <IRremote.h>


const int ON_OFF                    = 4;  // PIN3
const int MODE_1                    = 3;  // PIN2
const int MODE_2                    = 1;  // PIN6
const int MODE_3                    = 2;  // PIN7
const int IRLED                     = 0;  // PIN5

uint8_t triggered = 255;

void setup() {

    
    pinMode(ON_OFF, INPUT_PULLUP);
    pinMode(MODE_1, INPUT_PULLUP);
    pinMode(MODE_2, INPUT_PULLUP);
    pinMode(MODE_3, INPUT_PULLUP);
    
    digitalWrite(ON_OFF, HIGH);
    digitalWrite(MODE_1, HIGH);
    digitalWrite(MODE_2, HIGH);
    digitalWrite(MODE_3, HIGH);
    
    pinMode(IRLED, OUTPUT);

    
    IrSender.begin(IRLED, ENABLE_LED_FEEDBACK);

    
    } // setup

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT1);                   // Use PB1 as interrupt pin
    PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    PCMSK |= _BV(PCINT4);                   // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT1);                  // Turn off PB1 as interrupt pin
    PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    PCMSK &= ~_BV(PCINT4);                  // Turn off PB4 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    if (digitalRead(ON_OFF) == LOW)
      triggered = 1;
    else if(digitalRead(MODE_1) == LOW)
      triggered = 2;
    else if(digitalRead(MODE_3) == LOW)
      triggered = 4;
    else if(digitalRead(MODE_2) == LOW)
      triggered = 3;
    }

void loop() {
  cli();

  switch(triggered)
  {
    case 1:
      do
      {
        IrSender.sendNEC(0x80, 0x1A, 0);
        delay(500);
      }while(digitalRead(ON_OFF) == LOW);
      break;
    case 2:
      do
      {
        IrSender.sendNEC(0x80, 0x4, 0);
        delay(500);
      }while(digitalRead(MODE_1) == LOW);
      break;
    case 3:
      do
      {
        IrSender.sendNEC(0x80, 0x6, 0);
        delay(500);
        digitalWrite(MODE_2, HIGH);
      }while(digitalRead(MODE_2) == LOW);
      break;
    case 4:
      do
      {
        IrSender.sendNEC(0x80, 0xA, 0);
        delay(500);
      }while(digitalRead(MODE_3) == LOW);
      break;
    default:
      break;
  }
  digitalWrite(ON_OFF, HIGH);
  digitalWrite(MODE_1, HIGH);
  digitalWrite(MODE_2, HIGH);
  digitalWrite(MODE_3, HIGH);
  sei(); 
  sleep();
}
