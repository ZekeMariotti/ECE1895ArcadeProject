// Global variables:
// Game variables
int score = 0;
int timeInterval = 0;
int correctInput = 0;
bool success = true;

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
// Pins for joystick (Left - 5, Up - 6, Right - 7, Down - 8)
int joystickLeftPin = 5;
int joystickUpPin = 6;
int joystickRightPin = 7;
int joystickDownPin = 8;

// Pins for two input buttons and LEDs
int buttonOnePin = 1;
int buttonTwoPin = 2;

// Pins for coin slot button and LED
int coinSlotButtonPin = 0;

// Pin for Start button
int startButtonPin = 3;

// Setup default values for the board
void setup() {
  Serial.begin(9600);
  // TODO: LCD setup
  int lcdSetup = NULL;

  // pinMode Setups
  pinMode(joystickLeftPin, INPUT);
  pinMode(joystickUpPin, INPUT);
  pinMode(joystickRightPin, INPUT);
  pinMode(joystickDownPin, INPUT);

  pinMode(buttonOnePin, INPUT);
  pinMode(buttonTwoPin, INPUT);

  pinMode(coinSlotButtonPin, INPUT);
  pinMode(startButtonPin, INPUT);

  // Use analog input to generate random noise to choose a correct input
  randomSeed(analogRead(0));
}

// Main Loop
void loop() {
  // Read input pins
  readInputs();

  if (startButtonInput == true) {
    score = 0;
    success = true;
    timeInterval = 17000;
    Serial.println("Test1");

    while (success = true && score < 99) {
      readInputs();

      correctInput = random(6);

      if (buttonTwoInput == true) {
        Serial.println("Button One True");
      }
    }
  }
}

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
