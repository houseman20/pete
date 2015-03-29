/*
A sketch to control a 5 sets of LEDs depending on the number of button pushes.
 */

// 2015-03-24 :: Added look-up table to linearise the LED intensity.
//               Fixed overflow condition for the delay functions
//               Added method to turn-off all LEDs when moving between buttons

// start global declarations
#define ONE   1
#define TWO   2
#define THREE 3
#define FOUR  4
#define FIVE  5

#define NLEDS 5
#define DEBOUNCE_DELAY 20
#define MIN_DELAY 30
#define NO_DELAY 0
#define ONE_SECOND_DELAY 1000
#define HALF_SECOND_DELAY 500
// These two values define the active state of the switch input.
// Use 'false' for pull low switch, 'true' for pull high switch
#define SWITCH_ACTIVE_STATE false
// Use 'true' for pull low switch, 'false' for pull high switch
#define SWITCH_INACTIVE_STATE true
boolean pushedSwitch = SWITCH_INACTIVE_STATE;
const int buttonPin = 7;
const int l1ed = 5;
const int l2ed = 6;
const int l3ed = 9;       // the pin that the LED is attached to
const int l4ed = 10;
const int l5ed = 11;
const int maxBrightness = 255;
const int minBrightness = 0;
const int fadeAmount = 5;    // how many points to fade the LED by
// Look-up table to linearise the bright against PWM
static unsigned char exp_map[256]={
  0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,
  3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,
  5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,
  9,9,10,10,10,10,10,11,11,11,11,12,12,12,13,13,13,13,
  14,14,14,15,15,15,16,16,16,17,17,18,18,18,19,19,20,
  20,20,21,21,22,22,23,23,24,24,25,26,26,27,27,28,29,
  29,30,31,31,32,33,33,34,35,36,36,37,38,39,40,41,42,
  42,43,44,45,46,47,48,50,51,52,53,54,55,57,58,59,60,
  62,63,64,66,67,69,70,72,74,75,77,79,80,82,84,86,88,
  90,91,94,96,98,100,102,104,107,109,111,114,116,119,
  122,124,127,130,133,136,139,142,145,148,151,155,158,
  161,165,169,172,176,180,184,188,192,196,201,205,210,
  214,219,224,229,234,239,244,250,255
};

int brightness = 0;    // how bright the LED is
int b1rightness = 0;
int b2rightness = 0;
int b3rightness = 0;
int b4rightness = 0;
int b5rightness = 0;
volatile int buttonCount = 1; // Made a volatile to ensure latest version always used.
int downLed = 0;
int ledNumber = 0;
int myled[NLEDS]={l1ed,l2ed,l3ed,l4ed,l5ed}; // define array for the led pins
int longDelay;

// the setup routine runs once when you press reset:
void setup()  { 
  pinMode(l1ed, OUTPUT);
  pinMode(l2ed, OUTPUT);
  pinMode(l3ed, OUTPUT);
  pinMode(l4ed, OUTPUT);
  pinMode(l5ed, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
} 

// the loop routine runs over and over again forever:
void loop()  { // start of main loop
  int currentButton = buttonCount;
  if (buttonCount == ONE) // fade up each colour then switch it off
  { // if button count = 1
    currentButton = buttonCount;
    Serial.println("Button Count = 1");       // prints a label
//  Serial.print("\t"); // prints a tab
    for (int ledCount = 0 ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of ledCount
      // NOTE:: The 'for' loop termination is now dependent on buttonCount not changing during the count.
      for(int ledbright = minBrightness ; (ledbright <= maxBrightness) && (buttonCount == currentButton); ledbright+=fadeAmount) // fade in (from min to max) 
      { // start of fadeup
        ledNumber = myled[ledCount];
        analogWrite(ledNumber, exp_map[ledbright]);           // sets the value (range from 0 to 255) 
        delayCode(MIN_DELAY);                            // waits for 30 milli seconds to see the dimming effect
        //delay(MIN_DELAY);
      } // end fade up
      analogWrite(ledNumber, 0); // switch off led colour
    } // end of ledCount
    if(buttonCount != currentButton){
      analogWrite(l1ed, 0);
      analogWrite(l2ed, 0);
      analogWrite(l3ed, 0);
      analogWrite(l4ed, 0);
      analogWrite(l5ed, 0);
    }
  } // end of button count = 1
  
  else if (buttonCount == TWO) // fade up leds one by one then switch them all off
  { // if button count = 2
    currentButton = buttonCount;
    Serial.println("Button Count = 2");       // prints a label
    for (int ledCount = 0 ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of ledCount
      for(int ledbright = minBrightness ; (ledbright <= maxBrightness) && (buttonCount == currentButton); ledbright+=fadeAmount) // fade in (from min to max) 
      { // start of fadeup
        analogWrite(myled[ledCount], exp_map[ledbright]);           // sets the value (range from 0 to 255) 
        delayCode(MIN_DELAY);                            // waits for 30 milli seconds to see the dimming effect 
      } // end fade up
    } // end of ledCount
    longDelay = (buttonCount == 4) ? HALF_SECOND_DELAY : NO_DELAY;
    delayCode(longDelay);
    for (int ledCount = 0 ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of led count
      analogWrite(myled[ledCount], 0);    
    } // end of ledCount
    if(buttonCount != currentButton){
      analogWrite(l1ed, 0);
      analogWrite(l2ed, 0);
      analogWrite(l3ed, 0);
      analogWrite(l4ed, 0);
      analogWrite(l5ed, 0);
    }
  } // end of button count = 2
  
  else if (buttonCount == THREE) // leds fade up one by one then fade down one by one.
  {
    currentButton = buttonCount;
    Serial.println("Button Count = 3");       // prints a label
    for (int ledCount = 0 ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of ledCount
      for(int ledbright = minBrightness ; (ledbright <= maxBrightness) && (buttonCount == currentButton); ledbright+=fadeAmount) // fade in (from min to max) 
      { // start of fadeup
        analogWrite(myled[ledCount], exp_map[ledbright]);           // sets the value (range from 0 to 255) 
        delayCode(MIN_DELAY);                            // waits for 30 milli seconds to see the dimming effect 
      } // end fade up
    } // end of ledCount
    // Look at the value of buttonCount, if it has changed then skip the delay and move on.
    longDelay = (buttonCount == FOUR) ? HALF_SECOND_DELAY : NO_DELAY;
    delayCode(longDelay);
    for (int ledCount = minBrightness ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of led count
      for(int ledbright = maxBrightness ; (ledbright >= 0) && (buttonCount == currentButton); ledbright-=fadeAmount) // fade in (from min to max) 
      { // start of fade down 
        analogWrite(myled[ledCount], exp_map[ledbright]);           // sets the value (range from 0 to 255) 
        delayCode(MIN_DELAY);                            // waits for 30 milli seconds to see the dimming effect 
      } // end fade down
    } // end of ledCount
    if(buttonCount != currentButton){
      analogWrite(l1ed, 0);
      analogWrite(l2ed, 0);
      analogWrite(l3ed, 0);
      analogWrite(l4ed, 0);
      analogWrite(l5ed, 0);
    }
 } // end of button count = 3
  
  else if (buttonCount == FOUR) // all LEDs fade up then down together
  { // if button count = 4
    currentButton = buttonCount;
    Serial.println("Button Count = 4");       // prints a label
    for(int ledbright = minBrightness ; (ledbright <= maxBrightness) && (buttonCount == currentButton); ledbright+=fadeAmount) // fade in (from min to max) 
    {
      analogWrite(l1ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l2ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l3ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l4ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l5ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      delayCode(MIN_DELAY);
    } // end of fade up for all colours
    longDelay = (buttonCount == 4) ? ONE_SECOND_DELAY : NO_DELAY;
    delayCode(longDelay);
    for(int ledbright = maxBrightness ; (ledbright >= minBrightness) && (buttonCount == currentButton); ledbright-=fadeAmount) // fade in (from min to max) 
    { // start of fade down 
      analogWrite(l1ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l2ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l3ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l4ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      analogWrite(l5ed, exp_map[ledbright]); // sets the value (range from 0 to 255)
      delayCode(MIN_DELAY);     // <-- Call a function 
    } // end fade down
    longDelay = (buttonCount == 4) ? ONE_SECOND_DELAY : NO_DELAY;
    delayCode(longDelay); // <-- Call a function
    if(buttonCount != currentButton){
      analogWrite(l1ed, 0);
      analogWrite(l2ed, 0);
      analogWrite(l3ed, 0);
      analogWrite(l4ed, 0);
      analogWrite(l5ed, 0);
    }
  } // end of button count = 4
  
  else if (buttonCount == 5) // rolling fade up and down 
  { // if button count = 5
    currentButton = buttonCount;
    Serial.println("Button Count = 5");       // prints a label
    analogWrite(myled[4], 255);
    for (int ledCount = minBrightness ; (ledCount < NLEDS) && (buttonCount == currentButton); ledCount+=1)
    { // start of ledCount
      for (int brightness = minBrightness ; (brightness <=maxBrightness) && (buttonCount == currentButton); brightness+=fadeAmount)
      {
        downLed = (ledCount + 4) % 5;
        analogWrite(myled[ledCount], exp_map[brightness]); // sets the value (range from 0 to 255) 
        analogWrite(myled[downLed], exp_map[255 - brightness]); // sets the value (range from 0 to 255)       
        delayCode(MIN_DELAY); // waits for 30 milli seconds to see the dimming effect
      } // end of for next loop
    } // end of ledCount  
    if(buttonCount != currentButton){
      analogWrite(l1ed, 0);
      analogWrite(l2ed, 0);
      analogWrite(l3ed, 0);
      analogWrite(l4ed, 0);
      analogWrite(l5ed, 0);
    }
 } // end of button count = 5

}// end main loop


void delayCode( unsigned long int delay_time ) {
  unsigned long int delay_end;
  int entryButtonCount;
  boolean inputSwitch = true;
  delay_end = millis() + delay_time;
  // Get the entry value of count.
  entryButtonCount = buttonCount;
  do {
        // Read the state of the switch
        inputSwitch = debounce(buttonPin);

        // We are going to edge detect the switch, i.e. look for it to be '0'
        // then look for it to be a '1'. Use pushedSwitch to retain the state
        // for each time this routine is entered.
        // (It is a global atm, but could be made a static within the function
        //  which is preferable in real applications)
        // Check the stat of the switch has transitioned:
         if( (inputSwitch == SWITCH_ACTIVE_STATE) && !pushedSwitch ) { //It's been pressed
          // Update the global, this will remember the state of the switch
          pushedSwitch = true;
        } else if ( (inputSwitch == SWITCH_INACTIVE_STATE) && pushedSwitch ) { // It's released
          // increment button count on release of switch
          if (buttonCount == 5){
            buttonCount = 1;
          } else {
            buttonCount++;
          }
          // set the pushed switch state to false
          pushedSwitch = false;
        }
  // Exit the loop when delay is exceeded or buttonCount has changed.
  } while ( (millis() < delay_end) && (entryButtonCount == buttonCount) );
}

boolean debounce(int pin)
{
  boolean state;
  boolean previousState;

  previousState = digitalRead(pin);          // store switch state
  for(int counter=0; counter < DEBOUNCE_DELAY; counter++)
  {
      delay(1);                  // wait for 1 millisecond
      state = digitalRead(pin);  // read the pin
      if( state != previousState)
      {   
         counter = 0; // reset the counter if the state changes      
         previousState = state;  // and save the current state
      }
  }
  // here when the switch state has been stable longer than the de-bounce period
  return state;
}
