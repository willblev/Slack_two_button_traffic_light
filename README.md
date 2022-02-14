# Slack two button traffic light
An ESP8266-based Slack status indicator that can read and write a users status using the Slack API.

## Motivation
Working in a shared work space has a ton of benefits! However, there are some drawbacks; for example, if someone needs to focus on an important task, it can be difficult for their coworkers to know that they shouldn't start up a conversation. Having a visual indicator of a teammates availability, be it a post-it note on the back of a chair, putting on headphones, or something else, can be a quick and easy way to communicate the message. We thought that using a traffic light was perfect fit, since it's fairly intuitive to know when someone is available, busy, or should not be disturbed. 


# The traffic light
Here's what the traffic toy light looks like:
<br>

![traffic light toy](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_toy.jpg?raw=true)

<br>

It costs about $2 USD and is available on a variety of sites. It has a PCB with an integrated IC which runs off of 3 coin cells (~4.7v); it has 2 button switches, a small speaker, and three white SMD LEDs. Out of the box, the two buttons activate different modes of music and animations using the three LEDs. 

*Dimensions*
- Height: 13.5 cm(5.31 in)
- Width: 4.2 cm (1.65 in)
- Depth: 4.2 cm (1.65 in)


To open up the traffic light toy, we need to remove 4 small Phillips head screws: 

<br>

![traffic light screws](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_screws.png?raw=true)

<br>

It looks like we can fit a header-less D1 Mini ESP8266 board inside of the main body. We just need to remove the small speaker and trim away a few plastic bits. It is serendipitous that the holes in plastic case behind the speaker will also serve as ventilation to keep the ESP8266 from getting too hot. 


Once we are inside, we need to cut a few wires to remove the speaker and coin battery contacts. We will also cut several traces on the PCB (severing a connection) using a pen knife in order to connect the LEDs to our D1 Mini board. Here’s where we need to cut:

<br>

![traffic light schematic](https://github.com/willblev/Slack_two_button_traffic_light/blob/main/images/traffic_light_schematic.png?raw=true)

<br>

# The code

This project is setup to use [PlatformIO](https://platformio.org/) for builds and dependency management. Once you have installed PlatformIO, you can import this GitHub repository as a project. Once imported, you can build the project (download all dependencies & compile the code) with the following command:

```
platformio run
```

To upload it to a connected device, run:
```
platformio run --target upload
```


# Setting up Slack

For this device to be able to access and modify your Slack status, presence, and DnD settings, we need to create a secret key that it can use to prove to the Slack API that it has permission to do so. We can follow a guide [adapted from these instructions here](https://github.com/witnessmenow/arduino-slack-api#getting-auth-token-this-may-change):

##
- Create a new Slack App [by clicking here](https://api.slack.com/apps)
- Name your app whatever you want, such as "My traffic light app", then set the "Development Slack Workspace" to whichever workspace that you want the traffic light to update your status on
- Look on the lefthand side menu for "OAuth & Permissions"
- Scroll down to the section that says "Scopes", then under "User Token Scopes", click the "Add an OAuth Scope"
- Add the following scopes to have full functionality:

| Enpoint        | scope           |
| ------------- |-------------|
| setPresence      | users:write |
| setCustomStatus      | users.profile:write |
| getCurrentStatus |   users.profile:read |
| <undef> setDnD   |  dnd:write   |
| <undef> getDnD   |  dnd:read    |

- Once you've set these scopes, scroll up to the top of the page and click "Install app to workspace"
- Click "Allow" on the next page
- You will now have an OAuth Access Token up the top of the page, this is referred to as the `SLACK_ACCESS_TOKEN` in the ArduinoSlack library

