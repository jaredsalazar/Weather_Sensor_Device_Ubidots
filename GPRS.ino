char server[] = "things.ubidots.com";
char path[] = "/api/v1.6/variables/55e10ac476254250c8c8f96c/values";  // Edit Path to include you source key
char token[] = "GP3z7iWlKYjzg4Kz5KERo8ogBNXMr8"; //add token here
int port = 80; // HTTP
char action[] = "POST ";

unsigned long LastReport = 0;
int reqTime, res_count;

LGPRSClient globalClient;



void gprsStart() {
  Serial.println("Attach to GPRS network");   // Attach to GPRS network - need to add timeout
  while (!LGPRS.attachGPRS("internet2.voicestream.com", "", "")) { //attachGPRS(const char *apn, const char *username, const char *password);
    delay(500);
  }
  LGPRSClient client;
  globalClient = client;
}

String gprsSend(String var) {
  String response = "";

  int num = var.length();             // How long is the payload
  String le = String(num);               //this is to calcule the length of var
  Serial.print("Connect to ");    // For the console - show you are connecting
  Serial.println(server);

  if (globalClient.connect(server, port)) { // if you get a connection, report back via serial:
    Serial.println("connected");  // Console monitoring
    Serial.print(action);                   // These commands build a JSON request for Ubidots but fairly standard
    Serial.print(path);                     // specs for this command here: http://ubidots.com/docs/api/index.html
    Serial.println(" HTTP/1.1");
    Serial.println(F("Content-Type: application/json"));
    Serial.print(F("Content-Length: "));
    Serial.println(le);
    Serial.print(F("X-Auth-Token: "));
    Serial.println(token);
    Serial.print(F("Host: "));
    Serial.println(server);
    Serial.println();
    Serial.println(var);  // The payload defined above
    Serial.println();
    Serial.println((char)26); //This terminates the JSON SEND with a carriage return
    globalClient.print(action);                   // These commands build a JSON request for Ubidots but fairly standard
    globalClient.print(path);                     // specs for this command here: http://ubidots.com/docs/api/index.html
    globalClient.println(" HTTP/1.1");
    globalClient.println(F("Content-Type: application/json"));
    globalClient.print(F("Content-Length: "));
    globalClient.println(le);
    globalClient.print(F("X-Auth-Token: "));
    globalClient.println(token);
    globalClient.print(F("Host: "));
    globalClient.println(server);
    globalClient.println();
    globalClient.println(var);  // The payload defined above
    globalClient.println();
    globalClient.println((char)26); //This terminates the JSON SEND with a carriage return
  } else {
    response = "no connection";
    goto return_now;
  }
  reqTime = millis();
  res_count = 0;
  Serial.println("get response");

  while (res_count < 13) {
    if (globalClient.available()) {
      char c = globalClient.read();
      if (res_count > 8 && res_count < 12) {
        response = response + c;
      }
      Serial.print("/");
      res_count++;
    }
    if (millis() >= reqTime + 14000) {
      response = "timeout"; // don't know how to specify cutoff
      break;
    }
  }

  Serial.printf("\ngot response");
return_now:
  return response;
}

