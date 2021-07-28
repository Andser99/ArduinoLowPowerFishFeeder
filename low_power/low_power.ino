#include <avr/sleep.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#define HoursToTicks(a) (a * 450) // <a> * 450 gets you the number of 8 second ticks needed for <a> hours
#define DEBUG true

const byte LED = 2;

int counter = 0;
int feedingTicksRequired = HoursToTicks(3);
byte checkpointMod = feedingTicksRequired > 255 ? (feedingTicksRequired / 256) : 1;

void feed ()
{
  pinMode (LED, OUTPUT);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite (LED, HIGH);
    delay (10);
    digitalWrite (LED, LOW);
    delay (5); 
  }
  pinMode (LED, INPUT);
}

void flash ()
{
  //Blink power led
  pinMode (LED, OUTPUT);
  digitalWrite (LED, HIGH);
  delay (1);
  digitalWrite (LED, LOW);
  
  counter++;

#ifdef DEBUG
  Serial.begin(9600);
  Serial.print("Counter = ");
  Serial.println(counter);
  Serial.end();
#endif
  
  if (counter > feedingTicksRequired)
  {
    feed();
    counter = 0;
    EEPROM.write(0, 0);
  }
  else if (counter % checkpointMod == 0)
  {
#ifdef DEBUG
    Serial.begin(9600);
    Serial.print("Saving counter in case of power outage = ");
    Serial.println(counter / checkpointMod);
    Serial.end();
#endif
    EEPROM.write(0, (byte) counter / checkpointMod);
  }
  pinMode (LED, INPUT);
    
}  // end of flash
  
// watchdog interrupt
ISR (WDT_vect) 
{
   wdt_disable();  // disable watchdog
}  // end of WDT_vect
 
void setup ()
{
  // The following saves some extra power by disabling some 
  // peripherals I am not using.
  
  // Disable the ADC by setting the ADEN bit (bit 7)  of the
  // ADCSRA register to zero.
  ADCSRA = ADCSRA & B01111111;
  
  // Disable the analog comparator by setting the ACD bit
  // (bit 7) of the ACSR register to one.
  ACSR = B10000000;
  
  // Disable digital input buffers on all analog input pins
  // by setting bits 0-5 of the DIDR0 register to one.
  // Of course, only do this if you are not using the analog 
  // inputs for your project.
  DIDR0 = DIDR0 | B00111111;
  
  counter = EEPROM.read(0) * checkpointMod;
}

void loop () 
{
 
  flash ();
  
  // disable ADC
  ADCSRA = 0;  

  // clear various "reset" flags
  MCUSR = 0;     
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval 
  WDTCSR = bit (WDIE) | bit (WDP3) | bit (WDP0);    // set WDIE, and 8 seconds delay
  wdt_reset();  // pat the dog
  
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  noInterrupts ();           // timed sequence follows
  sleep_enable();
 
  interrupts ();             // guarantees next instruction executed
  sleep_cpu ();  
  
  // cancel sleep as a precaution
  sleep_disable();
  
 } // end of loop
