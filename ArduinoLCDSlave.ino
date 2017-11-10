/*
 * Made by Jacob Schnedler, last modified 10/11-17
 * Made for an Arduino Pro Micro controlling a Liquid Crystal display
 * The Arduino running this code will act as slave for w/e device which prints to 
 * the serial port. The code below analyses the code for operators &n as linebreaker 
 * and $ as display clearer. No scrolling is implemented for readability.
 * Use as you like but at your own risk :)
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(15, 14, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);                          // put your LCD parameters here
  lcd.print("Hello - Ready!");
  delay(100);
}

void loop() {
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    // clear the screen
    lcd.clear();
    // read all the available characters
    lcd.setCursor(0, 0);
    lcd.noCursor();
    String text = "";
    while (Serial.available() > 0) {
      text = Serial.readString();
    }

    printToLCD(text);

  }
}

/*
  Operators: &n = \newline , $ = clear
  Automatic handling of wordwrap is implemented.
  Faulsely added newlines in after line one are removed.
*/

void printToLCD(String text) {
  text.trim();
  //Define the two lines we need to write
  String line1 = "";
  String line2 = "";
  Serial.println(text);
  //look for a linebreaker
  int linebreaker = text.indexOf("&n");
  delay(200);
  //if the string starts with $ we will understand
  //this as a clear command
  if (text.substring(0, 1).equals("$")) {
    lcd.clear();
    Serial.println("$ detected");
  }
  //If the linebraker is there it will be above 0, we only want to
  //break lines if the linebreaker exists on line1 thus < 17
  else if (linebreaker > 0 && linebreaker < 16)
  {
    Serial.println("&n found");
    //Fill line1 up untill the linebreaker
    line1 = text.substring(0, text.indexOf("&n"));
    //Fill line2 from after the linebreaker untill the end
    line2 = text.substring(text.indexOf("&n") + 2, text.length());
    line2.trim();
    line1.replace("&n", "  ");
  }
  //If the text is longer than 16, we check for a space on the last spot on line 1
  //and the first on line 2, if either is a space we fill the lines,
  //Otherwise we add a - to break up the word to make the text more readable
  else if (text.length() > 16)
  {
    Serial.println(text.substring(15, 17));
    if (isSpace(text.charAt(15)) || isSpace(text.charAt(16)) || isSpace(text.charAt(17))) {
      Serial.println("space found");
      line1 = text.substring(0, 15);
      line2 = text.substring(16, text.length());
    } else {
      Serial.println("length above 16");
      line1 = text.substring(0, 15);
      line1 += "-";
      line2 = text.substring(15, text.length());
    }
  }
  //If the message is short, we simply print the message
  else if (text.length() < 16)
  {
    Serial.print("one line only");
    line1 = text;
  }

  line2.replace("&n", "");

  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.noCursor();
  lcd.print(line2);
}

