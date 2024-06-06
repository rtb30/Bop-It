#include <LiquidCrystal_I2C.h> // library for LCD

LiquidCrystal_I2C lcd(0x27,16,2);  // I2C address of LCD chip is 0x27. LCD is 16 chars and 2 lines.

void setup() {
  // set input pins
  pinMode(0, INPUT);     // START button
  pinMode(1, INPUT);     // ANTENNA button
  pinMode(2, INPUT);     // PTT button
  pinMode(3, INPUT);     // TUNING knob
  
  // set output pins
  pinMode(6, OUTPUT);    // Speaker
  pinMode(7, OUTPUT);    // GREEN LED indicator
  pinMode(8, OUTPUT);    // RED LED indicator

  // LCD setup
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   // Set cursor to character 2 on line 0
  lcd.print("Press START");
  lcd.setCursor(2,1);   // Move cursor to character 2 on line 1
  lcd.print("to begin.");
}

void loop() {

  if(digitalRead(0) == 1) { // if START button is pressed

    digitalWrite(7, LOW); // turn off GREEN LED
    digitalWrite(8, LOW); // turn off RED LED

    int action; // variable to keep track for action to be performed
    int elapsedTime; // variable to keep track of time elapsed in each turn
    bool validResponse; // variable to keep track of response correctness

    int maxTime = 3000; // initial maximum time (in ms) allowed for a response
    int score = 0; // inital score

    while(score < 99){ // loop while score is less than the maximum of 99
    
      validResponse = false; // initially set valid response to false
      maxTime = maxTime - 20; // subtract 20 (ms) from maximum time 
      elapsedTime = 0; // set elapsed time to 0 intially

      // display current score on LCD
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Game in progress");
      lcd.setCursor(0,1);
      lcd.print("Score: " + String(score));
      delay(2000); // pause for 2 s (2000 ms)

      action = rand() % 3 + 1; // Randomly generate number from 1 to 3
      // 1: SLIDE IT
      // 2: YELL IT
      // 3: TUNE IT

      lcd.clear();
      
      if (action == 1){ // SLIDE IT case
        // beep once
        analogWrite(6, 127);
        delay(200);
        analogWrite(6, 0);

        // display command on LCD
        lcd.setCursor(2,0);
        lcd.print("SLIDE IT!");
      }
      else if (action == 2){ // YELL IT case
        // beep twice
        analogWrite(6, 127);
        delay(100);analogWrite(6, 127);
        delay(100);
        analogWrite(6, 127);
        delay(100);
        analogWrite(6, 0);

        // display command on LCD
        lcd.setCursor(2,0);
        lcd.print("YELL IT!");
      }
      else if (action == 3){
        // beep three times
        analogWrite(6, 127);
        delay(50);
        analogWrite(6, 0);
        delay(50);
        analogWrite(6, 127);
        delay(50);
        analogWrite(6, 0);
        delay(50);
        analogWrite(6, 127);
        delay(50);
        analogWrite(6, 0);

        // display command on LCD
        lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
        lcd.print("TUNE IT!");
      }

      int antState = digitalRead(1);
      int tuneState = digitalRead(3);

      while(elapsedTime < maxTime){

        if(digitalRead(1) != antState){ // user performs SLIDE IT action
          if(action == 1){ // if SLIDE IT was commanded
            validResponse = true;
            break;
          }
          else{ // if SLIDE IT was not commanded
            break;
          }
        }

        if(digitalRead(2) == 1){ // user performs YELL IT action
          if(action == 2){ // if YELL IT was commanded, proceed to detect microphone input
            int micIn; // define integer for analog microphone input
            int micThresh = 600; // define microphone threshold
            bool doubleBreak = false; // boolean for breaking out of both loops

            for (int i = 0; i < 1000; i++){ // detect mic input for 1 second
              micIn = analogRead(1);
              if(micIn > micThresh) {
                doubleBreak = true;
                break;
              }
              delay(1);
            }

            if (doubleBreak == true){
              validResponse = true;
              break;
            }
            else{
              break;
            }

          }
          else{ // if YELL IT was not commanded
            break;
          }
        }

        if(digitalRead(3) != tuneState){ // user performs TUNE IT action
          if(action == 3){ // if TUNE IT was commanded
            validResponse = true;
            break;
          }
          else{ // if TUNE IT was not commanded
            break;
          }
        }

        delay(1); // delay for 1 ms
        elapsedTime++; // increment elapsed time
      }
      
      if(validResponse == true){
        digitalWrite(7, HIGH); // turn on GREEN LED
        digitalWrite(8, LOW); // turn off RED LED
        score++; //increment score
      }

      if(validResponse == false){
        digitalWrite(7, LOW); // turn off GREEN LED
        digitalWrite(8, HIGH); // turn on RED LED

        lcd.setCursor(0,0);
        lcd.print("GAME OVER!");
        lcd.setCursor(0,1);
        lcd.print("Final Score: " + String(score));

        break; // end the game by breaking out of while loop
      }
    }

    // the following case only occurs when the user has beat the game by reaching a score of 99
    if(validResponse == true){
      lcd.setCursor(0,0);
      lcd.print("GAME BEAT!");
      lcd.setCursor(0,1);
      lcd.print("Final Score: " + String(score));
    }
  }
}