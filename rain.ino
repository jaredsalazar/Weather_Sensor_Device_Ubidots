volatile int tips;
volatile unsigned long lastminute , raininterval, raintime, rainlast;

void rainSensorAttach() {
  pinMode(2, INPUT_PULLUP); // input from wind meters rain gauge sensor
  tips = 0; // initialize tips value to zero
  attachInterrupt(0, rainIRQ, FALLING); // attach external interrupt pins to IRQ functions
  interrupts(); // turn on interrupts
}

int tipsStat() {
  int currentTips = tips;
  Serial.print("current tips: ");
  Serial.println(currentTips);
  if ((millis()) - lastminute >= 60000)
  {
    tips = 0; //reset tips every minute
    lastminute = millis();
  }
  return currentTips;
}

void rainIRQ()
// Count rain gauge bucket tips as they occur
// Activated by the magnet and reed switch in the rain gauge, attached to input D2
{
  raintime = millis(); // grab current time
  raininterval = raintime - rainlast; // calculate interval between this and last event

  if (raininterval > 10) // ignore switch-bounce glitches less than 10mS after initial edge
  {
    tips += 1; //Increase this minute's amount of rain

    rainlast = raintime; // set up for next event
  }
}

void tipReset(){
  tips = 0;
}

