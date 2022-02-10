# Slack_two_button_traffic_light
A physical Slack status indicator (read/write) in the shape of a little traffic light

Working at an office in 2022, sometimes it is hard to know if someone is very busy or is free for a chat, since everyone almost always has headphones in place. This traffic light is an easily interpretable way of knowing when someone is free, busy, or very busy. 

Here's what the traffic toy light looks like. It can play a tune over a speaker, individually flash the lights, and has and two button switches to control the different modes. While we will remove the speaker to make room for the ESP8266 D1 Mini board; fortunately, the holes in the back of the plastic case which were intended for the speaker will help to keep the temperature of the ESP8266 down. 

*Dimensions*:
- Height: 13.5 cm(5.31 in)
- Width: 4.2 cm (1.65 in)
- Depth: 4.2 cm (1.65 in)

It can snugly fit a D1 Mini ESP8266 board inside of the main body, but you need to trim away a few plastic bits for it to be able to close with the D1 Mini inside. 
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

# Development
This project is setup to use [PlatformIO](https://platformio.org/) for builds and depenedency management. To build the project run
```
platformio run
```

To upload it to a device run:
```
platformio run --target upload
```
