/*
      YET-INTERNET-RADIO-WITH-ESP32
      *** MP3 & MIDI TEST *** 
*/

#include <Arduino.h>

#include <Adafruit_VS1053.h>    // don't forget to connect miso/mosi/sck
#define VS_CS     5       // VS1053 chip select pin (output) = Control Chip Select Pin (for accessing SPI Control/Status registers) --> XCS
#define VS_DCS    33      // VS1053 Data/chip/command select pin (output) --> XDSV sau XDCS
#define VS_DREQ   35      // VS1053 Data request, ideally an Interrupt pin: Player asks for more  data, --> DREQ
#define VS_RESET  -1      // VS1053 reset pin (output) --> XRET ; not necessary with esp32, wired to EN pin on ESP32
#define CARDCS    -1      // SD Card command select pin (output); not used
Adafruit_VS1053_FilePlayer vsPlayer = Adafruit_VS1053_FilePlayer(VS_RESET, VS_CS, VS_DCS, VS_DREQ, CARDCS);

#define VS_BUFFER_SIZE      32       // how many bytes are sent to VS1053 into one batch

byte mp3[] = {
    0xff, 0xe3, 0x18, 0xc4, 0x00, 0x07, 0xf8, 0x01, 0xd9, 0x70, 0x08, 0x44, 0x94, 0xcd, 0x7d, 0x52, 0x1a, 0xe9, 0xa0, 0x58, 0xce, 0x87, 0xed, 0x76, 
    0x6b, 0xf4, 0xf9, 0xbf, 0xe2, 0xc6, 0x3f, 0xbb, 0xff, 0xe9, 0x7f, 0xff, 0xf5, 0xa0, 0x23, 0xfc, 0x98, 0x11, 0xbf, 0xfa, 0x90, 0xb3, 0x43, 0x63, 
    0xf5, 0xb4, 0xdb, 0x31, 0xdc, 0x86, 0xcd, 0xc9, 0xbf, 0x39, 0xa3, 0xe7, 0x00, 0xff, 0xd3, 0x4f, 0xe7, 0x3f, 0x21, 0x7f, 0xff, 0xf5, 0x1c, 0x2e, 
    0xff, 0xe3, 0x18, 0xc4, 0x1b, 0x08, 0xb0, 0x06, 0x66, 0xf8, 0x00, 0x44, 0x00, 0xfa, 0xff, 0xd3, 0xff, 0xf6, 0x75, 0x3b, 0xa6, 0x99, 0xa5, 0x29, 
    0x3c, 0x76, 0x25, 0x9f, 0xcb, 0x67, 0x66, 0x62, 0x18, 0x78, 0x76, 0xf3, 0x94, 0xa4, 0xd2, 0x97, 0xbd, 0xe6, 0x66, 0x6e, 0xbc, 0x48, 0x12, 0x0f, 
    0x13, 0x88, 0x60, 0x0e, 0x06, 0xcb, 0x80, 0x40, 0x3f, 0x0e, 0x06, 0x02, 0x5a, 0x13, 0xaf, 0xd2, 0x6f, 0x7b, 0xcc, 0xe5, 0x29, 0xd9, 0x46, 0x1c, 
    0xff, 0xe3, 0x18, 0xc4, 0x33, 0x15, 0xe9, 0xde, 0x8c, 0x00, 0x03, 0x0a, 0x3c, 0xd3, 0xb5, 0xfe, 0x43, 0x88, 0x0a, 0x09, 0x8f, 0x0e, 0x7f, 0xd4, 
    0xf3, 0xbf, 0xff, 0x57, 0x14, 0x13, 0x16, 0x03, 0x87, 0x04, 0xc0, 0x3a, 0x79, 0x01, 0x01, 0x70, 0x22, 0x59, 0x08, 0x00, 0xff, 0xff, 0xff, 0x91, 
    0x1b, 0x7d, 0xfd, 0x56, 0xb1, 0xae, 0x60, 0x04, 0x7d, 0xee, 0x13, 0xa5, 0xff, 0x9a, 0x6b, 0x11, 0x17, 0x0d, 0xa4, 0x86, 0xc3, 0x51, 0x20, 0x02, 
    0xff, 0xe3, 0x18, 0xc4, 0x16, 0x11, 0x12, 0x62, 0xc0, 0x00, 0x02, 0x0e, 0x48, 0xcc, 0xaf, 0xff, 0xed, 0xf7, 0x8e, 0x92, 0x1d, 0x26, 0x67, 0xff, 
    0xff, 0xf9, 0x87, 0x96, 0x1e, 0x1b, 0x35, 0xff, 0xfe, 0xba, 0xbf, 0x44, 0x24, 0x58, 0x70, 0x42, 0x09, 0x00, 0x40, 0x9c, 0x5e, 0x10, 0x33, 0xff, 
    0x28, 0x2c, 0x03, 0x01, 0x90, 0xfa, 0x33, 0x2e, 0x80, 0x04, 0xe4, 0x15, 0xfa, 0x6a, 0x31, 0x2f, 0x02, 0x9c, 0x4b, 0x1f, 0x4b, 0xb5, 0xcb, 0xff, 
    0xff, 0xe3, 0x18, 0xc4, 0x0c, 0x0f, 0x3a, 0xa2, 0xc4, 0x00, 0x03, 0x4a, 0x44, 0xfa, 0x89, 0xa0, 0x7c, 0x0e, 0x2c, 0x03, 0x7f, 0xff, 0xfd, 0x4c, 
    0x03, 0x87, 0xde, 0x4d, 0xbf, 0xff, 0xfd, 0x08, 0xdf, 0xff, 0xfd, 0x08, 0x26, 0x63, 0x80, 0x62, 0x63, 0x04, 0x1c, 0xd4, 0x91, 0x05, 0xc4, 0x48, 
    0x2d, 0x95, 0xbf, 0xb0, 0xc2, 0x14, 0x4c, 0x5f, 0xfb, 0x6a, 0x28, 0xa0, 0x40, 0x4e, 0x00, 0x05, 0xb0, 0xc9, 0xe1, 0xd7, 0x2c, 0x1c, 0x32, 0xe7, 
    0xff, 0xe3, 0x18, 0xc4, 0x0a, 0x0e, 0xb2, 0xf6, 0xda, 0x50, 0x02, 0x8a, 0x78, 0x3d, 0x14, 0xe3, 0xbf, 0xff, 0x51, 0x70, 0xd2, 0x42, 0x42, 0x63, 
    0x7f, 0xff, 0xe8, 0x58, 0x30, 0x15, 0x06, 0x0a, 0x69, 0xbf, 0xff, 0xfe, 0xe2, 0xa1, 0x82, 0x65, 0x27, 0xff, 0xff, 0x43, 0x82, 0x80, 0x56, 0x2f, 
    0xff, 0xff, 0xe8, 0xa6, 0x74, 0x7c, 0xbf, 0xff, 0xfa, 0x0d, 0x2f, 0x37, 0xff, 0xb2, 0x00, 0x00, 0x58, 0x87, 0x57, 0x59, 0x36, 0xff, 0xff, 0xff, 
    0xff, 0xe3, 0x18, 0xc4, 0x0a, 0x0f, 0x42, 0x97, 0x0b, 0x19, 0x47, 0x50, 0x02, 0xea, 0x78, 0x46, 0x07, 0xac, 0x3f, 0x64, 0xfc, 0xfb, 0x7f, 0x75, 
    0x02, 0x41, 0x24, 0x3e, 0xff, 0xff, 0xe8, 0x49, 0x62, 0xee, 0x22, 0x40, 0x44, 0xdf, 0xff, 0xff, 0xf6, 0x28, 0x36, 0xff, 0xff, 0xfd, 0xcd, 0xcd, 
    0x6f, 0xff, 0xff, 0x9a, 0xc6, 0x90, 0x8f, 0x46, 0x8d, 0xff, 0xac, 0xf1, 0x20, 0xef, 0xfa, 0xef, 0xff, 0xfe, 0xbf, 0xf0, 0x23, 0x43, 0x8b, 0x13, 
    0xff, 0xe3, 0x18, 0xc4, 0x08, 0x0e, 0xb9, 0xea, 0xa0, 0x01, 0x8d, 0x68, 0x00, 0xd2, 0x3b, 0x5d, 0x2f, 0xd4, 0x8f, 0xc8, 0xbd, 0x52, 0xb1, 0x81, 
    0xfe, 0x37, 0xbf, 0xe4, 0xb7, 0xf1, 0x66, 0x29, 0x2d, 0xab, 0xd4, 0x6c, 0x4d, 0x20, 0xff, 0x3c, 0x4d, 0x74, 0x3f, 0x48, 0x67, 0x31, 0x35, 0xf5, 
    0x2f, 0x8e, 0xc4, 0x9f, 0x50, 0x8e, 0xff, 0xaa, 0x2e, 0x67, 0xf0, 0xdf, 0xfe, 0xd7, 0x7f, 0x27, 0x96, 0x33, 0x22, 0x86, 0x53, 0x93, 0xe2, 0x5d, 
    0xff, 0xe3, 0x18, 0xc4, 0x08, 0x0b, 0x00, 0x02, 0x82, 0xd9, 0xc0, 0x10, 0x02, 0x78, 0x34, 0xa0, 0x69, 0xe2, 0x50, 0xdf, 0x12, 0xf1, 0x17, 0x82, 
    0xab, 0x0d, 0x7f, 0x5b, 0x88, 0x95, 0x3a, 0x25, 0x77, 0xfe, 0x0a, 0xf8, 0x6f, 0x83, 0x5f, 0xe0, 0xd6, 0xb0, 0x55, 0xdf, 0xca, 0x9d, 0xff, 0xfc, 
    0x1a, 0x4c, 0x41, 0x4d, 0x45, 0x33, 0x2e, 0x39, 0x39, 0x2e, 0x35, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };

byte midi[] = {
    0x4D,0x54,0x68,0x64,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x01,0xE0,0x4D,0x54,0x72,0x6B,0x00,0x00,0x02,0xC3,0x00,0xFF,0x51,0x03,0x07,0xA1,0x20,0x00,0x90,0x3C,
    0x64,0x78,0x80,0x3C,0x64,0x00,0x90,0x3C,0x64,0x82,0x68,0x80,0x3C,0x64,0x00,0x90,0x3C,0x64,0x78,0x80,0x3C,0x64,0x00,0x90,0x41,0x64,0x83,0x60,0x80,0x41,0x64,0x00,
    0x90,0x41,0x64,0x83,0x60,0x80,0x41,0x64,0x00,0x90,0x43,0x64,0x83,0x60,0x80,0x43,0x64,0x00,0x90,0x43,0x64,0x83,0x60,0x80,0x43,0x64,0x00,0x90,0x48,0x64,0x85,0x50,
    0x80,0x48,0x64,0x00,0x90,0x45,0x64,0x81,0x70,0x80,0x45,0x64,0x00,0x90,0x41,0x64,0x82,0x68,0x80,0x41,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x45,
    0x64,0x82,0x68,0x80,0x45,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x3E,0x64,0x83,0x60,0x80,0x3E,0x64,0x00,0x90,0x46,0x64,0x87,0x40,0x80,0x46,0x64,
    0x00,0x90,0x43,0x64,0x82,0x68,0x80,0x43,0x64,0x00,0x90,0x40,0x64,0x78,0x80,0x40,0x64,0x00,0x90,0x41,0x64,0x87,0x40,0x80,0x41,0x64,0x83,0x60,0x90,0x41,0x64,0x82,
    0x68,0x80,0x41,0x64,0x00,0x90,0x43,0x64,0x78,0x80,0x43,0x64,0x00,0x90,0x45,0x64,0x83,0x60,0x80,0x45,0x64,0x00,0x90,0x45,0x64,0x83,0x60,0x80,0x45,0x64,0x00,0x90,
    0x45,0x64,0x83,0x60,0x80,0x45,0x64,0x00,0x90,0x46,0x64,0x82,0x68,0x80,0x46,0x64,0x00,0x90,0x45,0x64,0x78,0x80,0x45,0x64,0x00,0x90,0x45,0x64,0x83,0x60,0x80,0x45,
    0x64,0x00,0x90,0x43,0x64,0x83,0x60,0x80,0x43,0x64,0x83,0x60,0x90,0x43,0x64,0x82,0x68,0x80,0x43,0x64,0x00,0x90,0x45,0x64,0x78,0x80,0x45,0x64,0x00,0x90,0x46,0x64,
    0x83,0x60,0x80,0x46,0x64,0x00,0x90,0x46,0x64,0x83,0x60,0x80,0x46,0x64,0x00,0x90,0x46,0x64,0x83,0x60,0x80,0x46,0x64,0x00,0x90,0x48,0x64,0x82,0x68,0x80,0x48,0x64,
    0x00,0x90,0x46,0x64,0x78,0x80,0x46,0x64,0x00,0x90,0x45,0x64,0x87,0x40,0x80,0x45,0x64,0x83,0x60,0x90,0x48,0x64,0x82,0x68,0x80,0x48,0x64,0x00,0x90,0x48,0x64,0x78,
    0x80,0x48,0x64,0x00,0x90,0x48,0x64,0x83,0x60,0x80,0x48,0x64,0x00,0x90,0x45,0x64,0x82,0x68,0x80,0x45,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x48,
    0x64,0x83,0x60,0x80,0x48,0x64,0x00,0x90,0x45,0x64,0x82,0x68,0x80,0x45,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x3C,0x64,0x87,0x40,0x80,0x3C,0x64,
    0x83,0x60,0x90,0x3C,0x64,0x78,0x80,0x3C,0x64,0x00,0x90,0x3C,0x64,0x82,0x68,0x80,0x3C,0x64,0x00,0x90,0x3C,0x64,0x78,0x80,0x3C,0x64,0x00,0x90,0x43,0x64,0x87,0x40,
    0x80,0x43,0x64,0x00,0x90,0x46,0x64,0x83,0x60,0x80,0x46,0x64,0x00,0x90,0x43,0x64,0x82,0x68,0x80,0x43,0x64,0x00,0x90,0x40,0x64,0x78,0x80,0x40,0x64,0x00,0x90,0x43,
    0x64,0x83,0x60,0x80,0x43,0x64,0x00,0x90,0x41,0x64,0x83,0x60,0x80,0x41,0x64,0x00,0x90,0x3F,0x64,0x87,0x40,0x80,0x3F,0x64,0x00,0x90,0x3E,0x64,0x83,0x60,0x80,0x3E,
    0x64,0x00,0x90,0x41,0x64,0x82,0x68,0x80,0x41,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x41,0x64,0x83,0x60,0x80,0x41,0x64,0x00,0x90,0x40,0x64,0x82,
    0x68,0x80,0x40,0x64,0x00,0x90,0x41,0x64,0x78,0x80,0x41,0x64,0x00,0x90,0x43,0x64,0x8B,0x20,0x80,0x43,0x64,0x81,0x70,0x90,0x43,0x64,0x81,0x70,0x80,0x43,0x64,0x00,
    0x90,0x44,0x64,0x85,0x50,0x80,0x44,0x64,0x00,0x90,0x44,0x64,0x81,0x70,0x80,0x44,0x64,0x00,0x90,0x44,0x64,0x82,0x68,0x80,0x44,0x64,0x00,0x90,0x44,0x64,0x78,0x80,
    0x44,0x64,0x00,0x90,0x46,0x64,0x82,0x68,0x80,0x46,0x64,0x00,0x90,0x48,0x64,0x78,0x80,0x48,0x64,0x00,0x90,0x43,0x64,0x8B,0x20,0x80,0x43,0x64,0x00,0x90,0x44,0x64,
    0x81,0x70,0x80,0x44,0x64,0x00,0x90,0x43,0x64,0x81,0x70,0x80,0x43,0x64,0x00,0x90,0x41,0x64,0x85,0x50,0x80,0x41,0x64,0x00,0x90,0x41,0x64,0x81,0x70,0x80,0x41,0x64,
    0x00,0x90,0x41,0x64,0x81,0x70,0x80,0x41,0x64,0x00,0x90,0x44,0x64,0x81,0x70,0x80,0x44,0x64,0x00,0x90,0x43,0x64,0x82,0x68,0x80,0x43,0x64,0x00,0x90,0x41,0x64,0x78,
    0x80,0x41,0x64,0x00,0x90,0x41,0x64,0x83,0x60,0x80,0x41,0x64,0x00,0x90,0x40,0x64,0x83,0x60,0x80,0x40,0x64,0x00,0xFF,0x2F,0x00 };


void playArray(byte arr[], int endPointer) {

  unsigned int tmpPointer = 0;
  Serial.printf("--> Playing %u bytes of data.\r\n", endPointer); 
  
  byte tmp[VS_BUFFER_SIZE];
  while (tmpPointer <= endPointer) {    
         for (int i = 0; i < VS_BUFFER_SIZE; i++) { tmp[i] = arr[i+tmpPointer] ;  } 
         while (!vsPlayer.readyForData()) { yield(); }      // wait a few ms to finish what's currently playinh
         if (digitalRead(VS_DREQ) == HIGH) {                // if VS is ready for feeding data
               vsPlayer.playData(tmp, VS_BUFFER_SIZE);
               tmpPointer = tmpPointer + VS_BUFFER_SIZE;
               }
   }
}

void setup() {
  
  Serial.begin(115200);
  if (!vsPlayer.begin()) { Serial.println(F("--> ERROR: VS1053 not found.")); while(1) {} } 
  vsPlayer.softReset(); delay(150); vsPlayer.setVolume(0, 0); // 0 = max vol

  playArray(mp3, sizeof(mp3)); 
  delay(1000);
  playArray(midi, sizeof(midi));

}

void loop() {  }
