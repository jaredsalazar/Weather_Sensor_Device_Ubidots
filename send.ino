
char token[] = "GP3z7iWlKYjzg4Kz5KERo8ogBNXMr8"; //add token here
char server[] = "things.ubidots.com";
char path[] = "/api/v1.6/variables/55e10ac476254250c8c8f96c/values";  // Edit Path to include you source key

String send(String val) {

  String var = val;
  String response;
  response = global.POST(var, server, path, token);
  Serial.println(response);
  return response;
}
