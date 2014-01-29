#include <Arduino.h>
#include <TM1637Display.h>
#include "OnOffController.h"
#include "Average.h"

// Pin Allocation
#define HEATER       6     // Heater control (on/off)
#define TSENSE       A0    // Temperature sense, analog input
#define DISP_CLK   2    // Display module clock
#define DISP_DIO   3    // Display module data
#define BTN_ONOFF 4   // On/Off Button
#define BTN_PRST1  A1   // Preset 1 Button
#define BTN_PRST2  A2   // Preset 2 Button
#define BTN_PRST3  A3   // Preset 3 Button
#define LED_HEAT   13  // Heater on indicator
#define LED_READY 8   // Ready LED
#define KNOB_1         9      // Setting knob input 1
#define KNOB_2         10      // Setting knob input 2

class IronOnOffController: public OnOffController
{
public:
  IronOnOffController(): OnOffController(0, 700) {}
};

Average readingAvg;
TM1637Display display(DISP_CLK, DISP_DIO);
OnOffController controller(0, 1000);

void setup()
{
  Serial.begin(9600);

  // Set up the I/O pin direction
  pinMode(HEATER, OUTPUT);
  pinMode(BTN_ONOFF, INPUT_PULLUP);
  pinMode(BTN_PRST1, INPUT_PULLUP);
  pinMode(BTN_PRST2, INPUT_PULLUP);
  pinMode(BTN_PRST3, INPUT_PULLUP);  
  pinMode(KNOB_1, INPUT_PULLUP);
  pinMode(KNOB_2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
  readingAvg.init(analogRead(A0));
  
  display.setBrightness(15);
  
  controller.setSetpoint(700);
  
  // Fill the average window up
  int k;
  for(k=0; k < 10; k++) {
    int val = analogRead(A0);
    readingAvg.putValue(val);
    delay(50);
  }
  
  //
  controller.updateSamplingValue(readingAvg.getAverage());
  controller.setOnOffState(true);
    
}

void loop()
{

    int val = analogRead(A0);
    readingAvg.putValue(val);
  
    int temp = readingAvg.getAverage();
    display.showNumberDec(temp);
    controller.updateSamplingValue(temp);

    Serial.print(controllerStateToStr(controller.getState()));  
    Serial.print(" ");
    Serial.println(temp);

  
    delay(100);
  
}
