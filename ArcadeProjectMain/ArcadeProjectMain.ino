#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(32, 16, 2);

// Global variables:
// Game variables
int score = 0;
int timeInterval = 0;
int correctInput = 0;
bool success = true;
bool roundFailure = false;

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
int buttonOnePin = 2;
int buttonTwoPin = 3;

// Pins for coin slot button and LED
int coinSlotButtonPin = 4;

// Pin for Start button
int startButtonPin = 9;

// Setup default values for the board
void setup() {
  lcd.init();
  lcd.backlight();  
  Serial.begin(9600);

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

  // Start Button Pressed
  if (startButtonInput == true) {
    score = 0;
    success = true;
    roundFailure = false;
    timeInterval = 3500;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start");
    delay(2000);    

    // Main game loop after pressing start button
    while (success == true && score < 99) {
      lcd.clear();
      readInputs();

      // Choose and display the next correct input
      correctInput = random(6);

      // Reset time interval
      previousTime = millis();
      currentTime = millis();
      lcd.print("Correct Input: ");
      lcd.setCursor(0, 1);
      lcd.print(displayCorrectInput(correctInput));
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
        score++;
        //timeInterval -= 150;
      } else {
        lcd.clear();
        lcd.print("Wrong!");
      }
      delay(1000);
      lcd.clear();
    }
    lcd.print("Score: " + String(score));
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
      return "Coin";
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

