#include <mcp2515.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// Define CAN bus pins
const int spiCS = 10; // Chip select pin for CAN bus module

// Define Bluetooth RX and TX pins for master and slave
const int bluetoothRX = 4;
const int bluetoothTX = 3;

// Initialize SoftwareSerial for master and slave Bluetooth communication
SoftwareSerial bluetooth(bluetoothRX, bluetoothTX);

// Initialize MCP2515 for CAN communication
MCP2515 mcp2515(spiCS);

// Define message IDs
const uint32_t requestSpeedID = 0x20;
const uint32_t requestButtonStateID = 0x30;
const uint32_t requestLEDStateID = 0x40;
const uint32_t speedInfoID = 0x28;
const uint32_t buttonChangeID = 0x37;
const uint32_t LEDStateID = 0x47;
const uint32_t LEDControlID = 0x48;

// Define LED pins
const int led1Pin = 8;
const int led2Pin = A3;
const int led3Pin = A1;

// Define button pins
const int button1Pin = A4;
const int button2Pin = A2;
const int button3Pin = A5;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
 bluetooth.begin(38400);

  // Initialize SPI communication
  SPI.begin();

  // Initialize CAN bus module
  mcp2515.reset();
mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
mcp2515.setNormalMode();

  // Set LED pins as outputs
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // Set button pins as inputs with internal pull-ups enabled
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
}

void loop(){
  // Read the state of the pushbutton values
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);

  // Check if each pushbutton is pressed
  if (button1State == LOW) {
    // Turn LED1 on if button 1 is pressed
    digitalWrite(led1Pin, HIGH);
  } else {
    // Turn LED1 off if button 1 is not pressed
    digitalWrite(led1Pin, LOW);
  }

  if (button2State == LOW) {
    // Turn LED2 on if button 2 is pressed
    digitalWrite(led2Pin, HIGH);
  } else {
    // Turn LED2 off if button 2 is not pressed
    digitalWrite(led2Pin, LOW);
  }

  if (button3State == LOW) {
    // Turn LED3 on if button 3 is pressed
    digitalWrite(led3Pin, HIGH);
  } else {
    // Turn LED3 off if button 3 is not pressed
    digitalWrite(led3Pin, LOW);
  }

  // Check for incoming CAN messages
  struct can_frame rxMsg;
  if (mcp2515.readMessage(&rxMsg) == MCP2515::ERROR_OK) {
    // Process the received message
    processCANMessage(rxMsg);
  }

  // Check for incoming messages on master Bluetooth
  if (bluetooth.available() >= 12) {
    // Read the message
    byte message[12];
    bluetooth.readBytes(message, 12);

    // Process the message
    processBluetoothMessage(message);
  }

  // Handle button state requests
  if (checkButtonStateRequest()) {
    // Respond with button state message
    sendButtonStateMessage();
  }

  // Handle LED state requests
  if (checkLEDStateRequest()) {
    // Respond with LED state message
    sendLEDStateMessage();
  }
}

// Message processing logic
void processBluetoothMessage(byte message[]) {
  // Check message format
  if (message[0] == 0x00 && message[11] == 0xFF) {
    // Extract message type and data
    uint16_t messageType = (message[1] << 8) | message[2];
    uint16_t messageData = (message[3] << 8) | message[4];

    // Process message based on message type
    switch (messageType) {
      case 0x0001:
        // Speed (km/h)
        // Process speed data
        break;
      case 0x0002:
        // Engine speed (rpm)
        // Process engine speed data
        break;
      case 0x0003:
        // LED state
        // Extract LED ID and state
        uint8_t ledID = message[3];
        uint8_t ledState = message[4];
        // Process LED state data
        break;
      case 0x0004:
        // Button press
        // Extract button ID and state
        uint8_t buttonID = message[3];
        uint8_t buttonState = message[4];
        // Process button press data
        break;
      case 0x0005:
        // Analogue value (0-1024)
        // Process analogue value data
        break;
      case 0xFFFF:
        // Test
        // Process test message
        break;
      case 0xFFFE:
        // Acknowledge
        // Process acknowledge message
        break;
      default:
        // Unknown message type
        break;
    }
  } else {
    // Invalid message format
    // Ignore or handle error
  }
}

void processCANMessage(const can_frame& msg) {
  switch (msg.can_id) {
    case requestSpeedID:
      // Respond with speed info message
      sendSpeedInfoMessage();
      break;
    // Handle other request messages if needed
    default:
      // Ignore unrecognized messages
      break;
  }
}

bool checkButtonStateRequest() {
  // Check if any button state request is received
  // For demonstration, always return true
  return true;
}

void sendButtonStateMessage() {
  // Create and send the button state message
  can_frame msg;
  msg.can_id = buttonChangeID;
  msg.can_dlc = 3; // Button state message has one byte of data
  msg.data[0] = (digitalRead(button1Pin) == HIGH) ? 1 : 0;
  msg.data[1] = (digitalRead(button2Pin) == HIGH) ? 1 : 0;
  msg.data[2] = (digitalRead(button3Pin) == HIGH) ? 1 : 0; // Convert button state to 1 or 0
  mcp2515.sendMessage(&msg);
}

bool checkLEDStateRequest() {
  // Check if any LED state request is received
  // For demonstration, always return true
  return true;
}

void sendLEDStateMessage() {
  // Create and send the LED state message
  can_frame msg;
  msg.can_id = LEDStateID;
  msg.can_dlc = 3; // LED state message has one byte of data
  msg.data[0] = (digitalRead(led1Pin) == HIGH) ? 1 : 0;
  msg.data[1] = (digitalRead(led2Pin) == HIGH) ? 1 : 0;
  msg.data[2] = (digitalRead(led3Pin) == HIGH) ? 1 : 0; // Convert LED state to 1 or 0
  mcp2515.sendMessage(&msg);
}

void sendSpeedInfoMessage() {
   can_frame msg;
  msg.can_id = speedInfoID;
  msg.can_dlc = 5; // Speed info message has five bytes of data
  msg.data[0] = 0; // Placeholder for unused byte
  msg.data[1] = 0x12; // Speed MSB
  msg.data[2] = 0x34; // Speed LSB
  msg.data[3] = 0x00; // Placeholder for RPM MSB
  msg.data[4] = 0x00; // Placeholder for RPM LSB
  mcp2515.sendMessage(&msg);
}

void controlLED(int led, int state) {
  digitalWrite(led, state);
}
