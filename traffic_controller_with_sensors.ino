// Traffic light control for Energia IDE (Arduino framework)
// Tested on TM4C123 LaunchPad
// Reference: http://users.ece.utexas.edu/~valvano/

// Include necessary libraries
#include <Energia.h>

// Define pin mappings
#define WALK_YELLOW_LED   PA_3
#define WALK_RED_LED      PA_2
#define EW_GREEN_LED      PB_0
#define EW_YELLOW_LED     PB_1
#define EW_RED_LED        PB_2
#define NS_GREEN_LED      PB_3
#define NS_YELLOW_LED     PB_4
#define NS_RED_LED        PB_5
#define PEDESTRIAN_SENSOR PA_7 // changed pins from PE2
#define NS_CAR_SENSOR     PA_6 // changed pins from PE1
#define EW_CAR_SENSOR     PA_5 // changed pins from PE0

// Define states
enum State {
  Go_West,
  Wait_West,
  Go_South,
  Wait_South,
  Walk_Ped
};

State currentState = Go_West;

void setup() {
  Serial.begin(9600);
  Serial.println("three sensors available");
  // Initialize pin modes
  pinMode(WALK_YELLOW_LED, OUTPUT);
  pinMode(WALK_RED_LED, OUTPUT);
  pinMode(EW_GREEN_LED, OUTPUT);
  pinMode(EW_YELLOW_LED, OUTPUT);
  pinMode(EW_RED_LED, OUTPUT);
  pinMode(NS_GREEN_LED, OUTPUT);
  pinMode(NS_YELLOW_LED, OUTPUT);
  pinMode(NS_RED_LED, OUTPUT);
  pinMode(PEDESTRIAN_SENSOR, INPUT_PULLUP);
  pinMode(NS_CAR_SENSOR, INPUT_PULLUP);
  pinMode(EW_CAR_SENSOR, INPUT_PULLUP);
}

void loop() {
  int btn1 = digitalRead(PEDESTRIAN_SENSOR);
  int btn2 = digitalRead(NS_CAR_SENSOR);
  int btn3 = digitalRead(EW_CAR_SENSOR);

  if (btn1 == LOW){
    Serial.println("pedestrian sensor button pressed");
  } else if (btn2  == LOW){
    Serial.println("North / South Sensor pressed");
  } else if (btn3  == LOW){
    Serial.println("East / West Sensor pressed");
  } else {
    Serial.println("NO button pressed ");
  }

 
  // Set traffic lights based on current state
  switch(currentState) {
  case Go_West:
    // Set west green light, other lights red
    digitalWrite(EW_GREEN_LED, HIGH);
    digitalWrite(EW_YELLOW_LED, LOW);
    digitalWrite(EW_RED_LED, LOW);
    digitalWrite(NS_GREEN_LED, LOW);
    digitalWrite(NS_YELLOW_LED, LOW);
    digitalWrite(NS_RED_LED, HIGH);
    digitalWrite(WALK_YELLOW_LED, LOW);
    digitalWrite(WALK_RED_LED, HIGH);
    break;
  case Wait_West:
    // Set west yellow light, other lights red
    digitalWrite(EW_GREEN_LED, LOW);
    digitalWrite(EW_YELLOW_LED, HIGH);
    digitalWrite(EW_RED_LED, LOW);
    digitalWrite(NS_GREEN_LED, LOW);
    digitalWrite(NS_YELLOW_LED, LOW);
    digitalWrite(NS_RED_LED, HIGH);
    digitalWrite(WALK_YELLOW_LED, LOW);
    digitalWrite(WALK_RED_LED, HIGH);
    break;
  case Go_South:
    // Set south green light, other lights red
    digitalWrite(EW_GREEN_LED, LOW);
    digitalWrite(EW_YELLOW_LED, LOW);
    digitalWrite(EW_RED_LED, HIGH);
    digitalWrite(NS_GREEN_LED, HIGH);
    digitalWrite(NS_YELLOW_LED, LOW);
    digitalWrite(NS_RED_LED, LOW);
    digitalWrite(WALK_YELLOW_LED, LOW);
    digitalWrite(WALK_RED_LED, HIGH);
    break;
  case Wait_South:
    // Set south yellow light, other lights red
    digitalWrite(EW_GREEN_LED, LOW);
    digitalWrite(EW_YELLOW_LED, LOW);
    digitalWrite(EW_RED_LED, HIGH);
    digitalWrite(NS_GREEN_LED, LOW);
    digitalWrite(NS_YELLOW_LED, HIGH);
    digitalWrite(NS_RED_LED, LOW);
    digitalWrite(WALK_YELLOW_LED, LOW);
    digitalWrite(WALK_RED_LED, HIGH);    
    break;
  case Walk_Ped:
    // Pedestrian walk signal, all car lights red
    digitalWrite(WALK_RED_LED, LOW);
    digitalWrite(EW_GREEN_LED, LOW);
    digitalWrite(EW_YELLOW_LED, LOW);
    digitalWrite(EW_RED_LED, HIGH);
    digitalWrite(NS_GREEN_LED, LOW);
    digitalWrite(NS_YELLOW_LED, LOW);
    digitalWrite(NS_RED_LED, HIGH);
    // Pedestrian walk signal blinking
    digitalWrite(WALK_YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, LOW);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, LOW);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, LOW);
    delay(500);
    digitalWrite(WALK_YELLOW_LED, HIGH);
    break;
  }
 
  // Wait for a short time (adjust as needed)
  delay(5000); // 5 seconds per state
 
  // Transition to next state (cyclic)
  switch(currentState) {
  case Go_West:
    // Check if pedestrian is requesting to cross
    if (digitalRead(PEDESTRIAN_SENSOR) == LOW) {
       Serial.println("Pedestrian State");
      currentState = Walk_Ped;
    } 
    else if (digitalRead(NS_CAR_SENSOR) == LOW){
      Serial.println("North South State");
      currentState = Go_South;
    }    
    else if (digitalRead(EW_CAR_SENSOR) == LOW){
      Serial.println("East West State");
      currentState = Go_West;
    }
    else {
      currentState = Wait_West;
    }
    //currentState = Wait_West;
    break;
  case Wait_West:
    // Check if it's time to go south or if cars are waiting
    if (digitalRead(PEDESTRIAN_SENSOR) == LOW) {
       Serial.println("Pedestrian State");
      currentState = Walk_Ped;
    }
    else if (digitalRead(NS_CAR_SENSOR) == LOW){
      Serial.println("North South State");
      currentState = Go_South;
    }    
    else if (digitalRead(EW_CAR_SENSOR) == LOW){
      Serial.println("East West State");
      currentState = Go_West;
    }
    else {
      currentState = Go_South;
    }
    //currentState = Go_South;
    break;
  case Go_South:
    // Check if it's time to allow pedestrians to walk or if cars are waiting
    if (digitalRead(PEDESTRIAN_SENSOR) == LOW) {
       Serial.println("Pedestrian State");
      currentState = Walk_Ped;
    }
    else if (digitalRead(NS_CAR_SENSOR) == LOW){
      Serial.println("North South State");
      currentState = Go_South;
    }    
    else if (digitalRead(EW_CAR_SENSOR) == LOW){
      Serial.println("East West State");
      currentState = Go_West;
    }
    else {
      currentState = Wait_South;
    }
    break;
    //currentState = Wait_South;
    break;
  case Wait_South:
    // Check if it's time for pedestrians to walk or if cars are still waiting
    if (digitalRead(PEDESTRIAN_SENSOR) == LOW) {
       Serial.println("Pedestrian State!");
      currentState = Wait_South;
    } 
    else if (digitalRead(NS_CAR_SENSOR) == LOW){
      Serial.println("North South State");
      currentState = Go_South;
    }    
    else if (digitalRead(EW_CAR_SENSOR) == LOW){
      Serial.println("East West State");
      currentState = Go_West;
    }
    else {
      currentState = Walk_Ped;
    }
    //currentState = Walk_Ped;
    break;
  case Walk_Ped:
    // Check if pedestrians are still crossing or if it's time for cars to go west
    if (digitalRead(PEDESTRIAN_SENSOR) == LOW) {
       Serial.println("Pedestrian State");
      currentState = Walk_Ped;
    } 
    else if (digitalRead(NS_CAR_SENSOR) == LOW){
      Serial.println("North South State");
      currentState = Go_South;
    }    
    else if (digitalRead(EW_CAR_SENSOR) == LOW){
      Serial.println("East West State");
      currentState = Go_West;
    }
    else {
      currentState = Go_West;
    }
    //currentState = Go_West;
    break;
  }  
}
