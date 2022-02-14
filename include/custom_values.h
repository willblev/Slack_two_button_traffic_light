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

int LEDstatus=OFF;

// Try to use emojis of different shapes to help with colorblind/accessibility, since red/green circle can look similar
char* message0 = "Away";
char* emoji0 = ":brb:";

char* message1 = "Available";
char* emoji1 = ":large_green_circle:";
      
char* message2 = "Busy";
char* emoji2 = ":large_yellow_square:";

char* message3 = "Do not disturb";
char* emoji3 = ":octagonal_sign:";


#endif
