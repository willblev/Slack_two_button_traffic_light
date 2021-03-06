#include <Arduino.h>
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
#define BUTTON_1_PIN D5
#define BUTTON_2_PIN D6

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

void saveConfigCallback () {
  Serial.println("Setting shouldSaveConfig = true");
  shouldSaveConfig = true;
}

void updateLEDs() {
  
  switch(btnPressCount){
    case 0:   // off 
    {

       DEBUG_SERIAL.println("Changing LEDs to off");
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
       DEBUG_SERIAL.println("Changing LEDs to green.");
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
       DEBUG_SERIAL.println("Changing LEDs to yellow.");
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
       DEBUG_SERIAL.println("Changing LEDs to red.");
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
   
   DEBUG_SERIAL.println("Setting a new status on Slack.");
   
   SlackProfile profile;

   switch(LEDstatus){  // depending on what color the LEDs currently are, change the Slack status to... 
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


void checkSlackStatus() { // get the current remote status info from Slack
  
  DEBUG_SERIAL.println("Getting current status info from Slack API.");
 
  SlackProfile profile;
  slack.getCurrentStatus();
  APIstatus = profile.statusText;

  if (APIstatus==currentStatus){ // if the current status locally is the same as on Slack, no action needed.
    
    DEBUG_SERIAL.println("Status unchanged since last time; doing nothing.");
  
  } else { // If a status change has been detected on Slack, update the traffic light
    DEBUG_SERIAL.println("STATUS CHANGE DETECTED ON SLACK");

    if(APIstatus==message0){
      DEBUG_SERIAL.println("Status changed to off");
      btnPressCount = OFF;
      
    } else if(APIstatus==message1) { 
      DEBUG_SERIAL.println("Status changed to green");
      btnPressCount = GREEN;
      
    } else if(APIstatus==message2) {
      DEBUG_SERIAL.println("Status changed to yellow");
      btnPressCount = YELLOW;
      
    } else if(APIstatus==message3) {
      DEBUG_SERIAL.println("Status changed to red");
      btnPressCount = RED;
      
    } else {
      DEBUG_SERIAL.println("Slack status does not match any known state.");
      //btnPressCount = OFF;
      
    }

  }  
  //displayProfile(profile);
  DEBUG_SERIAL.println(APIstatus);
  currentStatus=APIstatus;   
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

void setup() {                                           
   DEBUG_SERIAL.println("Startintg setup");
   pinMode(GREEN_LED_PIN, OUTPUT);
   pinMode(YELLOW_LED_PIN, OUTPUT);
   pinMode(RED_LED_PIN, OUTPUT);

   button1.init(BUTTON_1_PIN);
   button2.init(BUTTON_2_PIN);

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
          DynamicJsonDocument config_json_doc(1024);
          DeserializationError error = deserializeJson(config_json_doc, buf.get());
          serializeJson(config_json_doc, Serial);
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
      
      DynamicJsonDocument config_json_doc(1024);
      config_json_doc["slackOAuthToken"] = slackOAuthToken;
    
      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println("Failed to open config file for writing");
      }
  
      serializeJson(config_json_doc, Serial);
      serializeJson(config_json_doc, configFile);
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
  // FIRST we detect button presses, and if they are detected, we set local_upstream to true        
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
 
  btnPressCount = btnPressCount % 4; // Keep count in the range 0 to 3 since we only have 4 states

    if (millis() > requestDueTime){  //once enough time has elapsed since the last request, we can send
        if(local_upstream){
          updateSlackAPI();  // if the change happened locally, we send the new status to Slack
        } else {     
          checkSlackStatus();  // if no change was detected locally, see what current status on Slack is
          }
        requestDueTime = millis() + delayBetweenRequests;
    }

    if(LEDstatus != btnPressCount){updateLEDs();} // update the LEDs if the status has changed
}

