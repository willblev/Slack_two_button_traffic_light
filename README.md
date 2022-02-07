# Slack_two_button_traffic_light
A physical toy traffic light which functions as a Slack status indicator (read/write)

Here's what the traffic toy light looks like. It can play a tune over a speaker, individually flash the lights, and has and two button switches to control the different modes. While we will remove the speaker to make room for the ESP8266 D1 Mini board; fortunately, the holes in the back of the plastic case which were intended for the speaker will help to keep the temperature of the ESP8266 down. 

<br>

![traffic light toy](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_toy.jpg?raw=true)

<br>

To get inside the traffic light toy, we need to remove 4 small Phillips head screws: 

<br>

![traffic light screws](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_screws.png?raw=true)

<br>

Once we are inside, we will need to cut a few wires, remove the speaker, and cut a few traces on the PCB using a pen knife. Essentially we are going to disable the built-in brain on the PCB, and instead we will use the ESP8266 to read input from the buttons and turn on/off the LEDs.

<br>

![traffic light schematic](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_schematic.png?raw=true)

<br>
