#include "Network.h"
#include <addons/TokenHelper.h>

/*Define these value match to yours*/
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define API_KEY "API_KEY"
#define FIREBASE_PROJECT_ID "PROJECT_ID"
#define USER_EMAIL "USER_EMAIL" // you can add your email on firebase authentication
#define USER_PASSWORD "USER_PASSWORD" // Not your email password, you can set on firebase authentication

static Network *instance = NULL;

Network::Network(){
  instance = this;
}

void Network::initWiFi(){
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void Network::firebaseInit(){
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
}

void Network::firestoreDataUpdate(String humid, String lightLevel, String soilHumid, String temp, unsigned long timestamp){
  if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
    String documentPath = "data/";
    documentPath += String(timestamp);

    FirebaseJson content;

    content.set("fields/humid/integerValue", humid.c_str());
    content.set("fields/lightLevel/integerValue", lightLevel.c_str());
    content.set("fields/soilHumid/integerValue", soilHumid.c_str());
    content.set("fields/temp/integerValue", temp.c_str());
    content.set("fields/timestamp/integerValue", String(timestamp).c_str());

    if(Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "humid,lightLevel,soilHumid,temp,timestamp")){
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      return;
    }else{
      Serial.println(fbdo.errorReason());
    }

    if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw())){
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      return;
    }else{
      Serial.println(fbdo.errorReason());
    }
  }
}