#include <SPI.h>

/*
You will need to connect the 12V pin to the VIN of the Arduino board, 
the GND pin to the GND of the Arduino board, and the DATA pin 
to both the MOSI and MISO pins of the Arduino board.
You will also need a resistor (e.g. 10k ohm) 
between the DATA pin and the GND pin to create a pull-down effect. 
*/

// Define the pins used for SPI communication 
#define CS_PIN 10 // Chip select pin 
#define MOSI_PIN 11 // Master out slave in pin 
#define MISO_PIN 12 // Master in slave out pin 
#define SCK_PIN 13 // Serial clock pin

// Define the number of bytes to read from the alarm panel 
#define DATA_LENGTH 21

// Define the timing parameters of the alarm panel in microseconds 
#define T 500 

// Clock signal period 
#define TP 5000 

// Pause time between messages 
#define TRM 1000 // Transfer time between bytes

// Create a buffer to store the data 
byte data[DATA_LENGTH];

void setup() { // Initialize serial communication at 9600 baud rate 
  Serial.begin(9600);

  // Set the chip select pin as output and set it high 
  pinMode(CS_PIN, OUTPUT); 
  digitalWrite(CS_PIN, HIGH);

  // Set the SPI settings: mode 0, MSB first, external clock 
  SPISettings settings(0, MSBFIRST, EXTERNAL);

  // Initialize SPI communication with the settings 
  SPI.begin(); 
  SPI.beginTransaction(settings);
}

void loop() { 
  // Set the chip select pin low to start communication 
  digitalWrite(CS_PIN, LOW);

  // Wait for the clock line to go from high to low 
  while (digitalRead(SCK_PIN) == HIGH) {}

  // Read DATA_LENGTH bytes from the alarm panel and store them in the buffer 
  for (int i = 0; i < DATA_LENGTH; i++) {
    data[i] = SPI.transfer(0x00); // Send dummy byte to receive data

    // Wait for TRM microseconds before reading the next byte
    delayMicroseconds(TRM);
  }

  // Set the chip select pin high to end communication 
  digitalWrite(CS_PIN, HIGH);

   if (check_checksum(data, DATA_LENGTH)) { 
      // Check if byte 0 is between 0x30 and 0x39 (text panel display update message) 
      if (data[0] >= B00110000 && data[0] <= B00111001) { 
        // Parse and print the text panel display update messages to the serial monitor 
        parse_text_panel_display_update(data, DATA_LENGTH); 
      } // Check if byte 0 is 0x00 (key press data) 
      else if (data[0] == B00000000) {
        // Parse and print the key presses to the serial monitor 
        parse_key_presses(data, DATA_LENGTH); 
      } // Check if byte 0 is 0x3f (broadcast message to all panels) 
      else if (data[0] == B00111111) { 
        // Parse and print the broadcast message to all panels to the serial monitor 
        parse_broadcast_message(data, DATA_LENGTH); 
      } 
      else {
        // Print a message to the serial monitor that this is an unknown message type 
        Serial.println("Unknown message type."); 
      }

    } else { 
      // Print an error message to the serial monitor 
      Serial.println("Invalid checksum!"); 
    }

  // Wait for TP microseconds before reading again 
  delayMicroseconds(TP); 
}

// A function that calculates and verifies the checksum of a given array of bytes 
bool check_checksum(byte* array, int length) {

// Initialize variables to store the checksum values 
byte chksum1 = array[length - 2]; // The first checksum byte is at index length - 2 
byte chksum2 = array[length - 1]; // The second checksum byte is at index length - 1

// Calculate the expected checksum values based on the formula given by the user 
byte expected_chksum2 = 0; 
// Initialize expected_chksum2 as zero 
for (int i = 0; i < length - 2; i++) {
   // Loop through all bytes except the checksum bytes 
   expected_chksum2 += array[i]; 
   // Add each byte to expected_chksum2 
   expected_chksum2 &= B01111111; 
   // Bitwise AND with B01111111 (0x7f) to keep only the lower seven bits 
   //(this is equivalent to modulo by B10000000 (128)) 
   //(this is also equivalent to using a mask of B01111111) 

   } 
   byte expected_chksum1 = B01111111 - expected_chksum2; // Calculate expected_chksum1 as B01111111 (0x7f) minus expected_chksum2

  // Compare the actual checksum values with the expected checksum values 
  if (chksum1 == expected_chksum1 && chksum2 == expected_chksum2) {
   return true; // The checksum is valid 
  } 
  else 
  { 
     return false; // The checksum is invalid 
  } 
}

// A function that parses and prints the key presses from a given array of bytes 
void parse_key_presses(byte* array, int length) {

  // Initialize variables to store the key press information 
  byte key_code = array[1]; // The second byte is the key type 
  byte key_data = array[2]; // The third byte is the key data

  // Print the key press information to the serial monitor 
  Serial.print("Key press: ");


  // Check the key code and print the corresponding message 
switch (key_code) { 
  case 0x00: // Key code 0:  
  Serial.println("Key 0 pressed"); break; 
  case 0x01: // Key code 1: 
  Serial.println("Key 1 pressed"); break; 
  case 0x02: // Key code 2:
  Serial.println("Key 2 pressed"); break; 
  case 0x03: // Key code 3: 
  Serial.println("Key 3 pressed"); break; 
  case 0x04: // Key code 4: 
  Serial.println("Key 4 pressed"); break; 
  case 0x05: // Key code 5: 
  Serial.println("Key 5 pressed"); break; 
  case 0x06: // Key code 6: 
  Serial.println("Key 6 pressed"); break; 
  case 0x07: // Key code 7: 
  Serial.println("Key 7 pressed"); break; 
  case 0x08: // Key code 8: 
  Serial.println("Key 8 pressed"); break; 
  case 0x09: // Key code 9: 
  Serial.println("Key 9 pressed"); break; 
  case 0x0A: // Key code A: 
  Serial.println("Key A pressed"); break; 
  case 0x0B: // Key code B: 
  Serial.println("Key B pressed"); break; 
  case 0x0C: // Key code C: 
  Serial.println("Key C pressed"); break; 
  case 0x0D: // Key code D: 
  Serial.println("Key D pressed"); break; 
  case 0x0E: // Key code E:  
  Serial.println("Key E pressed"); break; 
  case 0x0F: // Key code F:
  Serial.println("Key F pressed"); break; 
  default: // Unknown key code 
  Serial.println("Unknown key code"); break; 
}

/*switch (key_code) { 
  case B00000000: // Key code 0:  
  Serial.println("Key 0 pressed"); break; 
  case B00000001: // Key code 1: 
  Serial.println("Key 1 pressed"); break; 
  case B00000010: // Key code 2:
  Serial.println("Key 2 pressed"); break; 
  case B00000011: // Key code 3: 
  Serial.println("Key 3 pressed"); break; 
  case B00000100: // Key code 4: 
  Serial.println("Key 4 pressed"); break; 
  case B00000101: // Key code 5: 
  Serial.println("Key 5 pressed"); break; 
  case B00000110: // Key code 6: 
  Serial.println("Key 6 pressed"); break; 
  case B00000111: // Key code 7: 
  Serial.println("Key 7 pressed"); break; 
  case B00001000: // Key code 8: 
  Serial.println("Key 8 pressed"); break; 
  case B00001001: // Key code 9: 
  Serial.println("Key 9 pressed"); break; 
  case B00001010: // Key code A: 
  Serial.println("Key A pressed"); break; 
  case B00001011: // Key code B: 
  Serial.println("Key B pressed"); break; 
  case B00001100: // Key code C: 
  Serial.println("Key C pressed"); break; 
  case B00001101: // Key code D: 
  Serial.println("Key D pressed"); break; 
  case B00001110: // Key code E:  
  Serial.println("Key E pressed"); break; 
  case B00001111: // Key code F:
  Serial.println("Key F pressed"); break; 
  default: // Unknown key code 
  Serial.println("Unknown key code"); break; 
  }
  */


  // Print the key data to the serial monitor in hexadecimal format 
  Serial.print("Key data: "); 
  if (key_data < 0x10) { 
    Serial.print("0"); // Add leading zero if necessary 
  }

  Serial.println(key_data, HEX);
}

void parse_broadcast_message(byte* array, int length) {

  // Initialize variables to store the broadcast message information 
  byte line = array[1]; 
  // The second byte is the line number on all panels 
  char text[17]; // A character array to store the text on all panels

  // Copy the text from the data buffer to the character array 
  for (int i = 0; i < 16; i++) { 
    // Loop through bytes 2 to 17 (16 bytes of ASCII data) 
    text[i] = char(array[i + 2]); // Convert each byte to a character and store it in the array
    }

  // Add a null terminator at the end of the character array 
  text[16] = '\0';

  // Print the broadcast message information to the serial monitor 
  Serial.print("Broadcast message to all panels: ");

  // Print the line number in decimal format 
  Serial.print("Line ");
  Serial.print(line, DEC);

  // Print the text in quotation marks 
  Serial.print(""""); 
  Serial.print(text); 
  Serial.println(""""); 
}


void parse_text_panel_display_update(byte* array, int length) {

  // Initialize variables to store the text panel display update information 
  byte dest = array[3]; // The fourth byte is the destination panel number 
  byte line = array[4]; // The fifth byte is the line number on the panel 
  char text[17]; // A character array to store the text on the panel

  // Copy the text from the data buffer to the character array 
  for (int i = 0; i < 16; i++) { // Loop through bytes 5 to 20 (16 bytes of ASCII data) 
  text[i] = char(array[i + 5]); // Convert each byte to a character and store it in the array 
  }

  // Add a null terminator at the end of the character array 
  text[16] = '\0';

  // Print the text panel display update information to the serial monitor 
  Serial.print("Text panel display update: ");

  // Print the destination panel number in decimal format 
  Serial.print("Panel "); 
  Serial.print(dest, DEC);

  // Print the line number in decimal format 
  Serial.print(" Line "); 
  Serial.print(line, DEC);

  // Print the text in quotation marks 
  Serial.print(""); 
  Serial.print(text); 
  Serial.println(""); 
}


