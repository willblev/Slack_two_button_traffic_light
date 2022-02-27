#ifndef custom_values_h
#define custom_values_h

enum state {OFF, GREEN,YELLOW,RED};
int btnPressCount = 0;
int btnState = LOW;
int lastBtnState = LOW;
char* currentStatus = "Away";
char* lastStatus = "Away";
char* APIstatus = "";
bool local_upstream = false; // if the traffic light status (local) is more recently updated than the Slack API, local_upstream = true

char* NTPaddress = "pool.ntp.org"; // "es.pool.ntp.org" is for Spain, but we can let the default address decide the closest server to us
int8_t GMTOffsetHours = 1;
int16_t GMTOffsetSeconds = GMTOffsetHours * 3600; 
unsigned long delayBetweenNTPRequests = 60000; // set to 1 minute, can be changed using setUpdateInterval()


int LEDstatus=OFF;

// Try to use emojis of different shapes to help with colorblind/accessibility, since red/green circle can look similar
char* message0 = "Away";
char* emoji0 = ":brb:";
int timeout0 = 0;

char* message1 = "Available";
char* emoji1 = ":large_green_circle:";
int timeout1 =  0; 
                                        
char* message2 = "Busy";
char* emoji2 = ":large_yellow_square:";
int timeout2 = 40 - GMTOffsetSeconds; // setting status to expire after 40 seconds, remembering to return to GMT 0


char* message3 = "Do not disturb";
char* emoji3 = ":octagonal_sign:";
int timeout3 = 5*60 - GMTOffsetSeconds; // setting status to expire after 5 minutes

#endif
