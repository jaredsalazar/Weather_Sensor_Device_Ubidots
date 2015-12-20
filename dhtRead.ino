char cTemp[10], cHumid[10];
int DHTFailed = 0;

void dhtRead() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    DHTFailed++;
    if(DHTFailed >= 1500){
      Log("Dht Failed");
      DHTFailed = 0;
    }
    return;
  }

  Serial.println("------------------------------");
  Serial.print("Temperature Celcius = ");
  sprintf(cTemp, "%.2f", t);
  Temperature = cTemp;
  Serial.print(Temperature);
  Serial.println("C");

  Serial.print("Humidity = ");
  sprintf(cHumid, "%.2f", h);
  Humidity = cHumid;
  Serial.print(Humidity);
  Serial.println("%");

  DHTFailed = 0;
}

boolean InitializeDHT() {
  delay(200);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  } else {
    sprintf(cTemp, "%.2f", t);
    Temperature = cTemp;
    sprintf(cHumid, "%.2f", h);
    Humidity = cHumid;
    return true;
  }
}

