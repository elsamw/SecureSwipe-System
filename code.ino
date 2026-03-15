#include <LiquidCrystal_I2C.h> // LCD Library

#include <RFID.h> // |
                 //  | RC522 Libraries
#include <SPI.h> //  |
//LCD SETUP
LiquidCrystal_I2C lcd(0x27,16,2); // store the LCD in variable "lcd"
//RC522 SETUP
RFID rf(10,9); // store the rfid in variable "rf"
//PINS
int redLedPin = 3; // red led pin
int greenLedPin = 2; // green led pin
int buzzerPin = 4; // buzzer pin
// Authorized card UID 
byte authorizedUID[4] = {0x53, 0xD5, 0x09, 0x35}; //(put your card UID in the variable 0xA, 0xB, 0xC, 0xD)
void setup(){
  pinMode(redLedPin,OUTPUT); //declare red led pin as output
  pinMode(greenLedPin,OUTPUT); //declare green led pin as output
  pinMode(buzzerPin,OUTPUT); //declare buzzer pin as output
  Serial.begin(9600); // start the serial monitor
  lcd.init(); // initialize the LCD
  lcd.backlight(); //turn on backlight
  lcd.setCursor(0,0); //start in the (first line,first square)
  lcd.print("Swipe your card"); // welcome message
  SPI.begin(); //starts the SPI communication between the Arduino and the RFID module (order matters begin then init)
  rf.init(); //initialize the RFID
}

void loop(){
  if(rf.isCard()){
    if(rf.readCardSerial()){
      // clear the lcd from prevois text
      lcd.clear();
      // compare the card UID
      bool authorized = true;
      for(int i=0;i<4;i++){
        if(rf.serNum[i]!=authorizedUID[i]){
          authorized = false;
          break;
        }
      }

      if(authorized){
        // Athorized card
        lcd.setCursor(4,0);
        lcd.print("Welcome");
        digitalWrite(greenLedPin,1);
        digitalWrite(redLedPin,0);
        tone(buzzerPin, 1000, 150); // first pip
        delay(200);
        tone(buzzerPin, 1000, 150); // second pip
        delay(200);
        noTone(buzzerPin);
      }else{
        // Unthorized Card
        lcd.print("Access Denied");
        digitalWrite(greenLedPin,0);
        digitalWrite(redLedPin,1);
        tone(buzzerPin, 800, 400); // first long
        delay(450);
        tone(buzzerPin, 800, 400); // second long
        delay(450);
        tone(buzzerPin, 800, 200); // short piip
        delay(250);
        noTone(buzzerPin);
      }
      delay(1000); // wait before clearing LEDs
      digitalWrite(greenLedPin,0);
      digitalWrite(redLedPin,0);
      lcd.clear();
      lcd.print("Swipe your card");
    }
    rf.halt(); // stop reading card until removed
  }
}
