void HSM20GRead() {
  int humValue = analogRead(A0); // read humidity value from A0 pin
  float voltage = ((humValue * 5.0) / 1023.0); // convert analog value to voltage

  /**equation for humidity*/
  float hum = (3.71 * pow(voltage, 3))
              - (20.65 * pow(voltage, 2))
              + (64.81 * voltage)
              - 27.44;

  Humidity = String(hum);
  Serial.print("Humidity: " + Humidity);
  Serial.println(" %");

  delay(200);

  int tempValue = analogRead(A1); // read temperature value from A1 pin
  float voltage_temp = ((tempValue * 5.0) / 1023.0); // convert analog value to voltage

  /**equation for temperature*/
  float tem = (5.26 * pow(voltage_temp, 3))
              - (27.34 * pow(voltage_temp, 2))
              + (68.87 * voltage_temp)
              - 17.81;

  Temperature = String(tem);
  Serial.print("Temperature: " + Temperature);
  Serial.println(" C");
}

