const int guessButtonPin = 8;  // the number of the pushbutton pin
const int ledRedSvog = 4;      // the number of the LED pin
const int ledGreen = 5;
const int highestGuess = 9;     // the highest number a user can guess
const int AMOUNT_GUESSES = 6;   //the max amount of guesses
const int submitButtonPin = 9;  // the number of the pushbutton pin

// Variables will change:
int guess = -1;                    // The variable that will hold the current guess of the user (Starts at -1 because user  can guess 0 )
int guessIndex = 0;                // keepsTrack of how manny guesses have been made
int ledState = LOW;                // the current state of the output pin
int buttonState;                   // the current reading from the input pin
int lastButtonState = HIGH;        // the previous reading from the input pin
int submitButtonState;             // the current submitReading from the input pin
int lastSubmitButtonState = HIGH;  // the previous submitReading from the input pin
int serialGuess;
String message;
char received;
unsigned long lastDebounceTime = 0;        // the last time the output pin was toggled
unsigned long debounceDelay = 50;          // the debounce time; increase if the output flickers
unsigned long lastSubmitDebounceTime = 0;  // the last time the output pin was toggled
unsigned long submiyDebounceDelay = 50;    // the debounce time; increase if the output flickers

int guessArray[AMOUNT_GUESSES];                        // Holds the code guessed by the  user
int codeArray[AMOUNT_GUESSES] = { 5, 6, 8, 6, 0, 2 };  //Holds the correct code

void checkCode() {
  Serial.print("Checking code: ");

  for (int i = 0; i < AMOUNT_GUESSES; i++) {
    Serial.print(guessArray[i]);
    Serial.print("-");
  }
  Serial.println("!");
  int correctGuesses = 0;
  for (int i = 0; i < AMOUNT_GUESSES; i++) {
    if (codeArray[i] == guessArray[i]) {
      correctGuesses++;
    }
  }

  if (correctGuesses == AMOUNT_GUESSES) {
    Serial.println("Code correct");
    digitalWrite(ledRedSvog, LOW);
    digitalWrite(ledGreen, HIGH);
    delay(1000);
    digitalWrite(ledGreen, LOW);
  } else {
    Serial.println("Code incorrect");
    for (int i = 0; i < AMOUNT_GUESSES; i++) {
      digitalWrite(ledRedSvog, HIGH);
      delay(500);
      digitalWrite(ledRedSvog, LOW);
      delay(500);
    }
  }
  reset();
}

void addToCode(int guess) {
  if (guessIndex == AMOUNT_GUESSES) {
    //check the pincode
    checkCode();
  } else {
    Serial.print("Number ");
    Serial.print(guess);
    Serial.print(" ");
    Serial.print("Submitted ");
    Serial.print("on index ");
    Serial.println(guessIndex);
    guessArray[guessIndex] = guess;
    guessIndex++;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(submitButtonPin, INPUT_PULLUP);
  pinMode(guessButtonPin, INPUT_PULLUP);
  pinMode(ledRedSvog, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  digitalWrite(ledRedSvog, HIGH);
  
  Serial.begin(9600);
  Serial.println("Voer pincode in!");
}

void loop() {
  if (Serial.available() > 0) {
    char received = Serial.read();

    if (received == '\n')  // Message is finished, so process message.
    {
      Serial.println("Arduino Received: " + message);  // Only for debugging.
      addToCode(message.toInt());

      message = "";  // Message is processed, so clear message.
    } else           // Message is not finished yet, so add the received character to message.
    {
      message += received;
    }
  }

  //END SERIAL LOGIC
  //BUTTON LOGIC
  //GuessButton
  int reading = digitalRead(guessButtonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        if (guess < highestGuess) {

          guess++;
          Serial.print("Current number= ");
          Serial.println(guess);
        } else if (guess = highestGuess) {
          guess = -1;
          Serial.println(guess);
        }
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  //SubmitButton
  int submitReading = digitalRead(submitButtonPin);
  if (submitReading != lastSubmitButtonState) {
    lastSubmitDebounceTime = millis();
  }
  if ((millis() - lastSubmitDebounceTime) > submiyDebounceDelay) {

    if (submitReading != submitButtonState) {
      submitButtonState = submitReading;
      if (submitButtonState == LOW) {
        addToCode(guess);
      }
    }
  }
  // save the submitReading. Next time through the loop, it'll be the lastSubmitButtonState:
  lastSubmitButtonState = submitReading;
}



void reset() {
  guess = -1;
  guessIndex = 0;
  for (int i = 0; i < AMOUNT_GUESSES; i++) {
    guessArray[i] = 0;
    digitalWrite(ledRedSvog, LOW);
  }
}