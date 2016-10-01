//
// Example for EC11 class (Rotary Encoder Helper).
// Copyright (C) 2016, Aleh Dzenisiuk. 
// http://github.com/aleh/ec11
//

#include <EC11.hpp>

//
// Here we assume that the pins A and B of a EC-11 rotary encoder are connected to pins 2 and 3 of the Arduino Uno
// board and the pin C is connected to the ground.
//

EC11 encoder;

// Defined as 0 to see how polling-style pin checks behave.
#define DEMO_INTERRUPTS 0

#if DEMO_INTERRUPTS

//
// Interrupt-based example. This is recommended, but it means only pins 2 and 3 can be used with Uno.
//

const int encoderPinA = 2;
const int encoderPinB = 3;

void pinDidChange() {
  encoder.checkPins(digitalRead(encoderPinA), digitalRead(encoderPinB));
}

void prepare() {
  attachInterrupt(0, pinDidChange, CHANGE);
  attachInterrupt(1, pinDidChange, CHANGE);
}

#else

//
// Polling allows to use the encoder with any digital input pin.
//

const int encoderPinA = 2;
const int encoderPinB = 3;

void prepare() {
}

#endif // #if DEMO_INTERRUPTS

void setup() {
  
  Serial.begin(9600);
  Serial.println("EC11 Test");

  // We can use internal pull-up with the encoder pins, assuming pin C is simply grounded.
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  
  prepare();
}

static int value = 0;

void loop() {
  
  EC11Event e;
  if (encoder.read(&e)) {

    // OK, got an event waiting to be handled.
    
    if (e.type == EC11Event::StepCW) {
      value += e.count;
    } else {
      value -= e.count;
    }
    
    Serial.println(value);
  }

#if DEMO_INTERRUPTS

  // Wait quite some time to demonstrate that we can check for events fairly infrequently and still not miss them.
  delay(200);
  
#else

  // With polling-style pin checking we can still read infrequently, but we need to poll the pins often enough.
  for (int i = 0; i < 200; i++) {
    encoder.checkPins(digitalRead(encoderPinA), digitalRead(encoderPinB));
    delay(1);
  }
  
#endif
}

