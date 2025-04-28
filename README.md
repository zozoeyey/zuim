# zuim 🕹️
## Description
Zuim is a tangible communication tool designed to make emotional engagement visible during virtual and hybrid meetings. It offers participants a subtle, non-verbal way to share how they're feeling and signal when they need support, helping teams stay more connected even when physical cues are missing.
Through a simple dial and button, users can express their current level of engagement or raise a quiet request for question without interrupting the flow of conversation. Meanwhile, an ambient LED display captures the collective atmosphere of the meeting, making it easy for both participants and observers to sense energy levels and respond accordingly.
Whether it's a one-on-one meeting, a small group discussion, or a large webinar, Zoom Board fosters a more intuitive, human-centered meeting experience—supporting emotional awareness, lowering barriers to feedback, and creating space for every voice to be seen. 
## Project Features:
- Temperature Dial: Rotary encoder maps user input to a red → yellow → green color range.
- Question Button: Button toggles question state, causing a change in blinking speed of dedicated LEDs (indicates urgency and number).
- LED Stripe: Displays an average "room temperature" and highlights question activity.
## Libraries Used:
- Modulino.h: Library to read rotary encoder input
- FastLED.h: Control WS2812-based RGB LED strips interactions
## Setup Instructions:
1. Install libraries <br>
In Arduino IDE, go to Sketch > Include Library > Manage Libraries, and search for and install:
- FastLED
- Modulino
2. Connect Hardware <br>
- Modulino Knob connected to digital input pins (as specified in the code)
- RGB LEDs (WS2812 strip) connected to pin 6
- Physical RGB LEDs connected to pins 2, 3, and 4 (LED 1) and pins 9, 10, and 11 (LED 2)
3. Upload Code
- Open the Arduino .ino file
- Connect your Arduino (Uno, Nano, etc.)
- Select the correct port and board type
- Upload
4. Usage Instructions <br>
- Turn the dial to adjust the "temperature" you feel in the meeting. You will see your own indicated temperature shown on LED 1.
- Press the button to signal a question. You will see the state of your own questions on LED 2.
- When someone else or you have active questions, LEDs 3 and 4 in the stripe will blink in purple with different frequency based on the number of total questions.
- Observe the LED stripe: It changes color gradually to show the average temperature across all users in the meeting.
5. File Instructions <br>
- small_meeting_scenario is designed for simulation of smaller meetings (one-on-one or small group discussions), where the temperature of the whole meeting updates every 10 seconds, and the frequency of question pulsing is slower.
- large_meeting_scenario is designed for simulation of larger meetings (more than 10 people or large webinars), where the temperature of the whole meeting updates every 5 seconds, and the frequency of question pulsing is faster.
## Credits
Team Members: Zoey Yan, Jennifer Long, Jenna Stephenson


