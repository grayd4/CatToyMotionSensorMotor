#include <Servo.h>

// Use an ultrasonic sensor to activate a servo when anything is close enough


#include <SR04.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#include "Stepper.h"
#include "IRremote.h"
#include "SR04.h"

/*----- Variables, Pins -----*/
#define STEPS  2048           // Number of steps per revolution of Internal shaft
#define SPEED 13              // Speed to rotate at
#define TRIGGER_ROTATIONS 1   // Number of times to rotate when triggered

#define VOL_PLUS 0xFFA857     // Volume plus pin
#define VOL_MIN 0xFF629D      // Volume minus pin
#define FAST_FORWARD 0xFFC23D // Fast Forward
#define REWIND 0xFF22DD       // Rewind
#define POWER 0xFFA25D        // Power Button
#define ALT_POWER 0x
#define ZERO 0xFF6897

#define TRIG_PIN 7            // Pins for utrasonic sensor
#define ECHO_PIN 6

#define SERVO_PIN 9           // Pin servo control is attached to
#define ROTATE_DEGREES 300

int receiver = 12;            // Signal Pin of IR receiver to Arduino Digital Pin 6
bool isOn = false;            // Whether the device is tracking movement or no
int pos = 0;                  // Servo position

enum rotate_direction {clockwise = 1, counterclockwise = -1, none = 0};      // Direction types
enum rotate_direction dir = clockwise;                    // Current direction of rotation


/*-----( Declare objects )-----*/
// Setup of proper sequencing for Motor Driver Pins
// In1, In2, In3, In4 in the sequence 1-3-2-4

//Stepper stepper(STEPS, 8, 10, 9, 11);
Servo servo;                // Create servo object
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

// Check for input from the remote and change the direction variable accordingly
void checkIsOn()
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    isOn = !isOn;
    Serial.println(results.value, HEX);
    
    irrecv.resume(); // receive the next value
  }
}

// Return the distance perceived by the ultrasonic sensor, in cm
long getDistance()
{
  long distance = sr04.Distance();
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

// Return whether something is close enough to begin an action
bool isClose()
{
  if(getDistance() < 100)     // 100 cm
  {
    return true;
  }
  return false;
}

// Rotate the stepper motor X number of times
void spin(int numRotations)
{
  
  for (int i = 0; i < numRotations; i++)
  {
    int newPos = random(0, 180);
    servo.write(newPos);
    delay(min(newPos * 25, 3000));
    
    for (pos = newPos; pos > 0; pos--) {
      servo.write(pos);
      delay(5);
    }
  }

}

void stepperSpin()
{/*
  for (int i = 0; i < numRotations; i++)
  {
    stepper.step(dir * STEPS);          // Forward
    stepper.step(-dir * (STEPS / 4));   // 1/4 backward
  }

  // Reset the stepper motor
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);      */
}

// Run at the beginning of every turning-on
void setup()
{ 
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  irrecv.enableIRIn(); // Start the receiver
  // initialize the serial port:
  Serial.begin(9600);
  //stepper.setSpeed(SPEED);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// Run forever
void loop()
{

  // Turn on or off according to input
  checkIsOn();
    
  if (isOn)
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);   

    // If something triggered the ultrasonic sensor
    if (isClose())
    {
      // Spin the motor a set number of times
      spin(TRIGGER_ROTATIONS);  
      delay(500);
    }
  } else
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, LOW);   
  }

}/* --end main loop -- */
