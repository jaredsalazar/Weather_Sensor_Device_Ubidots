void blinkSetup(){
  pinMode(7, OUTPUT);
}

void blinker(int interval) {

  digitalWrite(7, HIGH);
  delay(interval);
  digitalWrite(7, LOW);
  delay(interval);
  digitalWrite(7, HIGH);
  delay(interval);
  digitalWrite(7, LOW);
  delay(interval);
}
