// WiFiManager and SPIFFS JSON credential storage from https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/
#include <FS.h> //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson  version 6.x
#include "credentials.h" // import the Slack OAuth token from the other file
#include "custom_values.h" // import the values from the other file
#include "button.h"  // Use debouncing code for buttons from https://www.e-tinkers.com/2021/05/the-simplest-button-debounce-solution/
Button button1;
Button button2; // define the pins & reading functions for the buttons from 'button.h'

// Conenct 3 LEDs and 2 button switches to ESP8266 D1 Mini
#define GREEN_LED_PIN D1
#define YELLOW_LED_PIN D2
#define RED_LED_PIN D3

#define DEBUG true  // set to 'true' if you want to print lines for debugging
#define DEBUG_SERIAL \
  if (DEBUG) Serial


// Following the Slack API for Arduino tutorial https://github.com/witnessmenow/arduino-slack-api
// But use a different fork of the slack-api-library which has the option to get current status from slack https://github.com/shaneapowell/arduino-slack-api/tree/feature/AddGetCurrentStatusCall
#include <ArduinoSlack.h>
#include <ArduinoSlackCert.h>
//initialize variables for Slack OAuth token
//char slackOAuthToken[80];

WiFiClientSecure client;
// Initialize Aruino Slack API library
ArduinoSlack slack(client, slackOAuthToken);


unsigned long delayBetweenRequests = 30000; // Time between requests (1 minute)
unsigned long requestDueTime;    


WiFiServer server(80);  // Set web server port number to 80

//String header;  // Variable to store the HTTP request

bool shouldSaveConfig = false;  //flag for saving data


void setup() {                                           
   DEBUG_SERIAL.println("Startintg setup");
   pinMode(GREEN_LED_PIN, OUTPUT);
   pinMode(YELLOW_LED_PIN, OUTPUT);
   pinMode(RED_LED_PIN, OUTPUT);

   button1.init(D5);
   button2.init(D6);

   Serial.begin(115200); // open the serial port at 115200 bps
   delay(400);
   DEBUG_SERIAL.println("\nInitialized Serial connection at 115200 bps");
   
   
  DEBUG_SERIAL.println("Mounting FS...");
  if (SPIFFS.begin()) {
      DEBUG_SERIAL.println("Mounted file system");
      if (SPIFFS.exists("/config.json")) {
        //file exists, reading and loading
        DEBUG_SERIAL.println("Reading config file");
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile) {
          DEBUG_SERIAL.println("Opened config file");
          size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
          std::unique_ptr<char[]> buf(new char[size]);


          configFile.readBytes(buf.get(), size);
          DynamicJsonDocument doc(1024);
          DeserializationError error = deserializeJson(doc, buf.get());
          serializeJson(doc, Serial);
          if (error) {
            DEBUG_SERIAL.println("\nparsed json");
          } else {
           Serial.println("failed to load json config");
          }
        }
      }
    } else {
      Serial.println("failed to mount FS");
    }
    //end read
    delay(1000);
    WiFiManagerParameter slack_token("slackOAuthToken", "slackOAuthToken", slackOAuthToken, 80);
  
    WiFiManager wifiManager;
    wifiManager.setSaveConfigCallback(saveConfigCallback);    //set config save notify callback
    
    wifiManager.addParameter(&slack_token); //add all your parameters here
    
    //wifiManager.resetSettings(); // Uncomment and run it once, if you want to erase all the stored information
        
    wifiManager.setTimeout(120); //sets timeout (in seconds) until configuration portal gets turned off
  
    wifiManager.autoConnect();
    
    // if you get here you have connected to the WiFi
    Serial.println("Connected to WiFi!");
    
    //save the custom parameters to FS
    strcpy(slackOAuthToken, slack_token.getValue());
    
    if (shouldSaveConfig) {
      Serial.println("Saving config");
      
      DynamicJsonDocument doc(1024);
      // JsonObject& json = jsonBuffer.createObject();
      doc["slackOAuthToken"] = slackOAuthToken;
    
      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println("Failed to open config file for writing");
      }
  
      serializeJson(doc, Serial);
      serializeJson(doc, configFile);
      configFile.close();
      //end save
    }
  
    server.begin();
    DEBUG_SERIAL.println("Started the server");
    
    delay(100);
    client.setFingerprint(SLACK_FINGERPRINT);
    DEBUG_SERIAL.println("Set Slack fingerprint");

    
    delay(200);

}  


void loop() {
       
  //local_upstream =false; // assume Slack API is most up-to-date unless button is pressed
        
  if(button1.pressDetected()){
       btnPressCount++; // Count this press
       DEBUG_SERIAL.println("button1 pressed");  
       local_upstream= true;       //as button as been pressed, we know that local is upstream of remote
       requestDueTime = millis() + 3000; // reset the next time to send status to Slack for 3 seconds from last button press detection 
    }  

   if(button2.pressDetected()){
       btnPressCount--; // Count this press, but iterate backwards
       DEBUG_SERIAL.println("button2 pressed");
       if(btnPressCount < 0){ btnPressCount = 3; } // if counter goes into negative integers, reset to top of range (circle back 0-3)
       //as button as been pressed, we know that local is upstream of remote
       local_upstream= true;
       requestDueTime = millis() + 3000; // reset the next time to send status to Slack for 3 seconds from last button press detection 
    } 
 
  btnPressCount = btnPressCount % 4; // Keep count in the range 0 to 3 

    if (millis() > requestDueTime){
        if(local_upstream){updateSlackAPI();}
        delay(100);
        checkSlackStatus();
        requestDueTime = millis() + delayBetweenRequests;
    }

    updateLEDs();
}


void updateLEDs() {
  //DEBUG_SERIAL.println("Updating LEDs");
  
  switch(btnPressCount){
    case 0:   // off 
    {

       //DEBUG_SERIAL.println(message0);
       LEDstatus=OFF;
       currentStatus=message0;
       digitalWrite(GREEN_LED_PIN, LOW);
       digitalWrite(YELLOW_LED_PIN, LOW);
       digitalWrite(RED_LED_PIN, LOW);
       delay(10); 
       break;
    } 
    
    case 1:   // green 
    {
       //DEBUG_SERIAL.println(message1);
       LEDstatus=GREEN;
       currentStatus=message1;
       digitalWrite(GREEN_LED_PIN, HIGH);
       digitalWrite(YELLOW_LED_PIN, LOW);
       digitalWrite(RED_LED_PIN, LOW);
       delay(10);
       break;  
    }
    
    case 2:   // yellow 
    {
       //DEBUG_SERIAL.println(message2);
       LEDstatus=YELLOW;
       currentStatus=message2;
       digitalWrite(GREEN_LED_PIN, LOW);
       digitalWrite(YELLOW_LED_PIN, HIGH);
       digitalWrite(RED_LED_PIN, LOW);
       delay(10);
       break;  
    }
    
    case 3:   // red
    {
       //DEBUG_SERIAL.println(message3);
       LEDstatus=RED;
       currentStatus=message3;
       digitalWrite(GREEN_LED_PIN, LOW);
       digitalWrite(YELLOW_LED_PIN, LOW);
       digitalWrite(RED_LED_PIN, HIGH);
       delay(10);
       break;  
    }
  }
}

void updateSlackAPI() { // if a change has been registered locally, but hasn't been sent to slack yet
   
   SlackProfile profile;

   switch(LEDstatus){
    case 0: // off
    {
      profile = slack.setCustomStatus(message0, emoji0);
      delay(50);
      slack.setPresence(SLACK_PRESENCE_AWAY);
      break;
    }         
    
    case 1: // green
    {
      profile = slack.setCustomStatus(message1, emoji1);
      delay(50);
      slack.setPresence(SLACK_PRESENCE_AUTO);
      break;
    } 

    case 2: // yellow
    { 
      profile = slack.setCustomStatus(message2, emoji2);
      delay(50);
      slack.setPresence(SLACK_PRESENCE_AUTO);
      break;
    } 
    
    case 3: //red
    {
      profile = slack.setCustomStatus(message3, emoji3);
      delay(50);
      slack.setPresence(SLACK_PRESENCE_AUTO);
      break;
    }
  } 
  
    
  local_upstream=false; // now that we've sent the new status to Slack, local and remote are equal again    
}        


void checkSlackStatus() {
  DEBUG_SERIAL.println("sending GET request to Slack API");
  
  SlackProfile profile;
  delay(20);
  slack.getCurrentStatus();
  delay(20);
  char* APIstatus = profile.statusText;
  displayProfile(profile);
  Serial.println(APIstatus);

  if (APIstatus==currentStatus){
    
    DEBUG_SERIAL.println("Status unchanged since last time; doing nothing.");
  } else { // If a change has been registered, but it happened on Slack first

    local_upstream=false;

    if(APIstatus==message0){
      btnPressCount = OFF;
      
    } else if(APIstatus==message1) { 
      btnPressCount = GREEN;
      
    } else if(APIstatus==message2) {
      btnPressCount = YELLOW;
      
    } else if(APIstatus==message3) {
      btnPressCount = RED;
      
    } else {
      Serial.println("Slack status does not match any known state; turning off LEDs.");
      //btnPressCount = OFF;
      
    }
  }     
}

void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void displayProfile(SlackProfile profile)
{
    if (!profile.error)
    {
        Serial.println("--------- Profile ---------");

        Serial.print("Display Name: ");
        Serial.println(profile.displayName);

        Serial.print("Status Text: ");
        Serial.println(profile.statusText);

        Serial.print("Status Emoji: ");
        Serial.println(profile.statusEmoji);

        Serial.print("Status Expiration: ");
        Serial.println(profile.statusExpiration);

        Serial.println("------------------------");
    }
    else
    {
        Serial.println("error getting profile");
    }
}
