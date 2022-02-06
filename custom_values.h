#ifndef custom_values_h
#define custom_values_h

enum state {OFF, GREEN,YELLOW,RED};
int btnPressCount = 0;
int btnState = LOW;
int lastBtnState = LOW;
char* currentStatus = "Away";
char* lastStatus = "Away";
bool local_upstream = false; // if the traffic light status (local) is more recently updated than the Slack API, local_upstream = true

int LEDstatus=OFF;


char* message0 = "Away";
char* emoji0 = ":brb:";

char* message1 = "Available";
char* emoji1 = ":large_green_circle:";
      
char* message2 = "Busy";
char* emoji2 = ":large_yellow_circle:";

char* message3 = "Do not distub";
char* emoji3 = ":octagonal_sign:";


#endif
