#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DFPlayer Setup
// Audio files 1-7 correspond to correct inputs 0-6
// File 8 is for game started, file 9 is for game win, file 10 is for game over
SoftwareSerial SoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini DFPlayer;

// Global variables:
// Game variables
int score = 0;
int scoreMultiplier = 1;
int timeInterval = 0;
int correctInput = 0;
bool success = true;
bool roundFailure = false;
bool gameStartMessage = false;

// Timer variables
unsigned long previousTime = 0, currentTime = 0;



// Inputs
bool joystickLeftInput = false;
bool joystickUpInput = false;
bool joystickRightInput = false;
bool joystickDownInput = false;
bool buttonOneInput = false;
bool buttonTwoInput = false;
bool coinSlotButtonInput = false;
bool startButtonInput = false;

// Pins: 
// Pins for joystick 
int joystickLeftPin = 5;
int joystickUpPin = 6;
int joystickRightPin = 7;
int joystickDownPin = 8;

// Pins for two input buttons and LEDs
int buttonOnePin = 12;
int buttonTwoPin = 3;

// Pins for coin slot button and LED
int coinSlotButtonPin = 4;

// Pin for Start button
int startButtonPin = 9;

// Setup default values for the board
void setup() {
  // lcd setup
  lcd.init();
  lcd.backlight(); 

  // DFPlayer Setup
  SoftwareSerial.begin(9600);
  Serial.begin(9600);

  lcd.print("Loading Game...");
  delay(1000);
  currentTime = millis();

  while (!DFPlayer.begin(SoftwareSerial)) {
    delay(1000);

    if(millis() - currentTime > 10000){
      lcd.print("DFPlayer Error");
      while(true);
    }
  }
  
  lcd.clear();

  DFPlayer.volume(25);

  // pinMode Setups
  pinMode(joystickLeftPin, INPUT);
  pinMode(joystickUpPin, INPUT);
  pinMode(joystickRightPin, INPUT);
  pinMode(joystickDownPin, INPUT);

  pinMode(buttonOnePin, INPUT);
  pinMode(buttonTwoPin, INPUT);

  pinMode(coinSlotButtonPin, INPUT);
  pinMode(startButtonPin, INPUT);


  //pinMode(10,OUTPUT);

  // Use analog input to generate random noise to choose a correct input
  randomSeed(analogRead(0));
}

// Main Loop
void loop() {
  // Read input pins
  readInputs();

  if(joystickUpInput && buttonOneInput){
    scoreMultiplier = 5;
  }

  if(joystickDownInput && buttonTwoInput){
    scoreMultiplier = 1;
  }
  
  if (gameStartMessage == false){
    if(millis()-previousTime>750){
      lcd.print("Press Start");
      lcd.setCursor(0, 1);
      lcd.print("to Begin");
      lcd.setCursor(0, 0); 
      gameStartMessage = true;
      previousTime = millis();
    }
  }
  else{
    if(millis()-previousTime>750){
      lcd.clear();
      gameStartMessage = false;
      previousTime = millis();
    }
  }

  // Start Button Pressed
  if (startButtonInput == true) {
    lcd.clear();
    lcd.print("Game Started");
    DFPlayer.play(8);
    score = 0;
    success = true;
    roundFailure = false;
    timeInterval = 5000;
    delay(2000);    

    // Main game loop after pressing start button
    while (success == true && score <= 99) {
      lcd.clear();
      readInputs();

      // Choose and display the next correct input
      correctInput = random(7);

      // Reset time interval
      previousTime = millis();
      currentTime = millis();
      lcd.print("Correct Input: ");
      lcd.setCursor(0, 1);
      lcd.print(displayCorrectInput(correctInput));
      lcd.setCursor(0, 0);
      DFPlayer.play(correctInput+1); // audio files should be in order, ex: file 1 -> Red Button Sound
      success = false;

      // Loop for each round
      while (currentTime - previousTime < timeInterval) {
        readInputs();
        roundFailure = testInputs(correctInput);

        if (roundFailure || success)
          break;

        currentTime = millis();
      }

      if (success) {
        lcd.clear();
        lcd.print("Correct!");
        delay(1000);
        score+=1*scoreMultiplier;
        
        if (score%20 == 0 && score < 100){
            lcd.clear();            
            lcd.print("Speeding up...");
        	  timeInterval -= 750;
          	delay(1500);
        }
      } 
      else {
        lcd.clear();
        lcd.print("Wrong!");
        delay(1000);
      }
      delay(1000);
      lcd.clear();
    }
    // Game over
    if(score > 99){
      lcd.print("You Win!");
      DFPlayer.play(9);      
      lcd.setCursor(0, 1);
      lcd.print("Score: " + String(score));
      lcd.setCursor(0, 0);
      delay(2500);
    }
    else{
      lcd.print("Game Over");
      DFPlayer.play(10);
      lcd.setCursor(0, 1);
      lcd.print("Score: " + String(score));
      lcd.setCursor(0, 0);
      delay(2500);
    }
    delay(2500);
    lcd.clear();
    gameStartMessage = false;
  }
}

// Reads all input pins into bool variables
void readInputs() {
  joystickLeftInput = (digitalRead(joystickLeftPin) == HIGH);
  joystickUpInput = (digitalRead(joystickUpPin) == HIGH);
  joystickRightInput = (digitalRead(joystickRightPin) == HIGH);
  joystickDownInput = (digitalRead(joystickDownPin) == HIGH);
  buttonOneInput = (digitalRead(buttonOnePin) == HIGH);
  buttonTwoInput = (digitalRead(buttonTwoPin) == HIGH);
  coinSlotButtonInput = (digitalRead(coinSlotButtonPin) == HIGH);
  startButtonInput = (digitalRead(startButtonPin) == HIGH);
}

/* Tests if inputs are correct based on correctInput
 * buttonOne - 0, buttonTwo - 1, coinSlotButton - 2,  
 * joystickLeft - 3, joystickUp - 4, joystickRight - 5, joystickDown - 6
 */
bool testInputs(int correctInput) {
  bool failureStatus = false;

  switch (correctInput) {
    case 0:
      success = buttonOneInput && !(buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      failureStatus = (buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      break;
    case 1:
      success = buttonTwoInput && !(buttonOneInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      failureStatus = (buttonOneInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      break;
    case 2:
      success = coinSlotButtonInput && !(buttonOneInput || buttonTwoInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      failureStatus = (buttonOneInput || buttonTwoInput || joystickLeftInput || joystickUpInput || joystickRightInput || joystickDownInput);
      break;
    case 3:
      success = joystickLeftInput && !(buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickUpInput || joystickRightInput || joystickDownInput);
      failureStatus = (buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickUpInput || joystickRightInput || joystickDownInput);
      break;
    case 4:
      success = joystickUpInput && !(buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickRightInput || joystickDownInput);
      failureStatus = (buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickRightInput || joystickDownInput);
      break;
    case 5:
      success = joystickRightInput && !(buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickDownInput);
      failureStatus = (buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickDownInput);
      break;
    case 6:
      success = joystickDownInput && !(buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput);
      failureStatus = (buttonOneInput || buttonTwoInput || coinSlotButtonInput || joystickLeftInput || joystickUpInput || joystickRightInput);
      break;
  }

  return failureStatus;
}

String displayCorrectInput(int correctInput) {
  switch (correctInput) {
    case 0:
      return "Red Button";
      break;
    case 1:
      return "Green Button";
      break;
    case 2:
      return "Insert Coin";
      break;
    case 3:
      return "Joystick Left";
      break;
    case 4:
      return "Joystick Up";
      break;
    case 5:
      return "Joystick Right";
      break;
    case 6:
      return "Joystick Down";
      break;
  }
  return "";
}

// Timer Setup
void timerSetup() {
  cli();  //stop interrupts

  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0

  // set compare match register (Change this to change interrupt frequency)
  OCR1A = 5124;  // = (16*10^6) / (1*1024) - 1 (must be <65536)

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();  //allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  Serial.println(millis());
}
