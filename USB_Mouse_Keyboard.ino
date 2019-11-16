/*
 * File name    : USB_Mouse_Keyboard.ino
 * Version      : 1.00.00
 * Platform     : ChipKit Pi board (USB bootloader version)
 * Designer     : Jorge Andrade R.
 * Date         : November 15th, 2019
 * Company      : Tecnologico de Monterrey, Puebla
 * Description  : This program emulates a mouse and a keyboard with the ChipKit Pi board.
 *              - The mouse pointer will move automatically from left to right and viceversa.
 *                The LED in pin 3 will turn on and of with every direction change.
 *              - The digital button in pin 18 will send a text message as a keyboard, as
 *                long as the button is pressed. Use Notepad or similar to capture the text.
 */
#include <USB.h>        // Required library for USB specific functions

#define myButton  18    // Pin definition for built-in push button
#define myLED     3     // Pin definition for built-in LED

bool buttonStatus = 1;  // Variable to save the button status
bool mouseDirec   = 1;  // Variable to save the mouse direction, 0 = left, 1 = right
bool LEDstatus    = 1;  // Variable to save the LED status
int  cycles       = 0;  // Variable to count movements per direction

USBFS usbDriver;        // Define the board as an USB Full-Speed peripheral

USBManager    USB(usbDriver, 0x03E8, 0x03F1); // 0x0403,0xA662); // 0x04D8, 0x00E0);
                              // (Full Speed, ProductID, VendorID)
HID_Mouse     Mouse;          // Instance to create an emulated mouse
HID_Keyboard  Keyboard;       // Instance to create an emulated keyboard

void setup() {
  USB.addDevice(Mouse);       // Add mouse
  USB.addDevice(Keyboard);    // Add keyboard
  USB.begin();                // Enable emulated USB peripherals
  pinMode (myLED, OUTPUT);    // LED as output
  pinMode (myButton, INPUT);  // Push button as input
}

void loop() {
  if (mouseDirec == 1) {
    Mouse.move(15, 0, 0);     // move 15 relative points to the right
  }
  if (mouseDirec == 0) {
    Mouse.move(-15, 0, 0);    // move 15 relative points to the left
  }
  digitalWrite(myLED, LEDstatus); // Update LED
  delay(33);                  // time in ms between movements
  cycles++;                   // keep count of cycles
  if (cycles == 20) {         // 20 movements in a direction?
    mouseDirec = !mouseDirec; // change mouse direction
    LEDstatus = !LEDstatus;   // change LED status
    cycles = 0;               // reset cycles
  }
  buttonStatus = digitalRead(myButton);
  if (buttonStatus == 0){     // Button is pressed
    Keyboard.println("Hi from emulated USB keyboard!");
  }
}
