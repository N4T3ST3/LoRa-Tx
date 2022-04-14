#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>
#include <Wire.h>

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ (Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    915E6

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

//packet packet_counter
int packet_counter = 0;

void setup() {

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  pinMode(25,OUTPUT); // send success, LED trigger
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW); // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH);
   
  Serial.begin(9600);
  while (!Serial); 
  // Initialising the UI will init the display too.
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
   
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  u8x8.drawString(5,20,"LoRa Initializing OK!");
  u8x8.display();
  delay(2000);
}
void loop() {
  
  Serial.print("Sending packet: ");
  Serial.println(packet_counter);
  
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(packet_counter);
  LoRa.endPacket();
  
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.println("LORA SENDER");
  u8x8.setCursor(0,20);
  u8x8.print(packet_counter); 
  u8x8.setCursor(20,40);
  u8x8.print("PACK(S)");     
  u8x8.display();
  
  
  packet_counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  delay(3000);
}
