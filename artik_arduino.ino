#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
String firstString = "";
String secondString = "";
boolean isFirst = true;
boolean stringComplete = false;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};
byte rowPins[ROWS] = {7, 12, 11, 9};
byte colPins[COLS] = {8, 6, 10};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
	Serial.begin(9600);

	lcd.begin();

	lcd.backlight();
	printLcd("All-Door", "Booting....");

}

void loop(){
	char key = keypad.getKey();

	if (key){
		Serial.print(key);
	}

	if (stringComplete) {
		lcd.clear();
		printLcd(firstString, secondString);
		firstString = "";
		secondString = "";
		stringComplete = false;
	}

}

void serialEvent() {

	while (Serial.available()) {    

		char inChar = (char)Serial.read();

		if (inChar == '\n') {
			stringComplete = true;
			isFirst = true;
		} else {
			if (inChar == ':') {
				isFirst = false;
				continue;
			}
			if(isFirst) {
				firstString += inChar;
			} else {
				secondString += inChar;
			}         
		}
	}
}

void printLcd(String firstStr, String secondStr) {
	int firstLen = firstStr.length();
	int secondLen = secondStr.length();
	int firstCursor = ( 16 - firstLen ) / 2;
	int secondCursor = ( 16 - secondLen ) / 2;

	lcd.clear();

	lcd.setCursor(firstCursor, 0);
	lcd.print(firstStr);
	lcd.setCursor(secondCursor, 1);
	lcd.print(secondStr);
}


