//STORAGE GLOBAL
easyLStorage sd;
String tempStorage = "temp.log";
String Statlog = "stat.log";

void StorageStart() {
  sd.start(tempStorage);
  sd.start(Statlog);
}

void checkResponse(String val, String response) {
  if (response != "200" && response != "201" && response != "400") {
    sd.cacheData(val, tempStorage);
    Log(response);
  }
}

void Log(String _stat) {
  String stat = String(rtcNow()) + " : " + _stat;
  sd.cacheData(stat, Statlog);
}

boolean checkSD() {
  String data = sd.getData(tempStorage);
  if (data == "") {
    return false;
  } else {
    String response = gprsSend(data);
    if (response == "200" || response == "201") {
      sd.deleteData(tempStorage);
      return true;
    } else if (response == "400") {
      Serial.println("Bad Request... Deleting Data");
      sd.deleteData(tempStorage);
      return true;
    } else {
      Log(response);
    }
  }
}

