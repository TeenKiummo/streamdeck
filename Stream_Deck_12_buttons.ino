// ---------------------------------
// Key definitions
#define BUTTON_KEY1 KEY_F13
#define BUTTON_KEY2 KEY_F14
#define BUTTON_KEY3 KEY_F15
#define BUTTON_KEY4 KEY_F16
#define BUTTON_KEY5 KEY_F17
#define BUTTON_KEY6 KEY_F18
#define BUTTON_KEY7 KEY_F19
#define BUTTON_KEY8 KEY_F20
#define BUTTON_KEY9 KEY_F21
#define BUTTON_KEY10 KEY_F22
#define BUTTON_KEY11 KEY_F23
#define BUTTON_KEY12 KEY_F24

// Pin definitions
#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4
#define BUTTON_PIN4 5
#define BUTTON_PIN5 6
#define BUTTON_PIN6 7
#define BUTTON_PIN7 8
#define BUTTON_PIN8 9
#define BUTTON_PIN9 10
#define BUTTON_PIN10 16
#define BUTTON_PIN11 14
#define BUTTON_PIN12 15
// ---------------------------------

#include "Keyboard.h"

// Button helper class for handling press/release and debouncing
class button {
  public:
  const char key;
  const uint8_t pin;

  button(uint8_t k, uint8_t p) : key(k), pin(p){}

  void press(boolean state){
    if(state == pressed || (millis() - lastPressed  <= debounceTime)){
      return; // Evita di gestire il rimbalzo del pulsante
    }

    lastPressed = millis();

    state ? Keyboard.press(key) : Keyboard.release(key);    
    pressed = state;
  }

  void update(){
    press(!digitalRead(pin));
  }

  private:
  const long debounceTime = 30;
  unsigned long lastPressed;
  boolean pressed = 0;
} ;

// Button objects, organized in array
button buttons[] = {
  {BUTTON_KEY1, BUTTON_PIN1},
  {BUTTON_KEY2, BUTTON_PIN2},
  {BUTTON_KEY3, BUTTON_PIN3},
  {BUTTON_KEY4, BUTTON_PIN4},
  {BUTTON_KEY5, BUTTON_PIN5},
  {BUTTON_KEY6, BUTTON_PIN6},
  {BUTTON_KEY7, BUTTON_PIN7},
  {BUTTON_KEY8, BUTTON_PIN8},
  {BUTTON_KEY9, BUTTON_PIN9},
  {BUTTON_KEY10, BUTTON_PIN10},
  {BUTTON_KEY11, BUTTON_PIN11},
  {BUTTON_KEY12, BUTTON_PIN12},
};

const uint8_t NumButtons = sizeof(buttons) / sizeof(button);
const uint8_t ledPin = 18;  // LED collegato al pin 15
bool ledState = true;       // Stato dei LED (acceso/spento)
bool lastButtonState = HIGH;  // Stato precedente del bottone
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // Delay per debouncing

void setup() { 
  // Safety check. Ground pin #1 (RX) to cancel keyboard inputs.
  pinMode(1, INPUT_PULLUP);
  if(!digitalRead(1)){
    failsafe();
  }

  // Configura il pin dei LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Imposta LED acceso all'inizio

  // Configura i pin dei pulsanti
  for(int i = 0; i < NumButtons; i++){
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
}

void loop() {
  for(int i = 0; i < NumButtons; i++){
    buttons[i].update();  // Aggiorna lo stato di tutti i pulsanti
  }

  // Controllo specifico per il pulsante associato a F21 (BUTTON_PIN9)
  int reading = digitalRead(BUTTON_PIN9);

  // Verifica il cambio di stato del pulsante (per debouncing)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Se il tempo di debounce è passato, controlla lo stato del pulsante
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Se il pulsante è premuto (LOW), inverte lo stato del LED
    if (reading == LOW) {
      toggleLed();
    }
  }

  lastButtonState = reading;  // Aggiorna lo stato del pulsante
}

// Funzione per accendere/spegnere il LED
void toggleLed() {
  if (ledState) {
    digitalWrite(ledPin, LOW);  // Spegni i LED
  } else {
    digitalWrite(ledPin, HIGH); // Accendi i LED
  }
  ledState = !ledState;  // Cambia lo stato del LED
}

void failsafe(){
  for(;;){} // Se c'è un errore, rimani bloccato qui
}
