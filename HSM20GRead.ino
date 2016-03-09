String* HSM20GRead() {
  String* TH = new String[2];
  int humValue = analogRead(A0); // read humidity value from A0 pin
  float voltage = ((humValue * 5.0) / 1023.0); // convert analog value to voltage

  /**equation for humidity*/
  float hum = (3.71 * pow(voltage, 3))
              - (20.65 * pow(voltage, 2))
              + (64.81 * voltage)
              - 27.44;

  TH[1] = String(hum);
  Serial.print("Humidity: " + TH[1]);
  Serial.println(" %");

  delay(200);

  int tempValue = analogRead(A1); // read temperature value from A1 pin
  float voltage_temp = ((tempValue * 5.0) / 1023.0); // convert analog value to voltage

  /**equation for temperature*/
  float tem = (5.26 * pow(voltage_temp, 3))
              - (27.34 * pow(voltage_temp, 2))
              + (68.87 * voltage_temp)
              - 17.81;

  TH[0] = String(tem);
  Serial.print("Temperature: " + TH[0]);
  Serial.println(" C");

  return TH;
}
