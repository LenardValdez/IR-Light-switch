#include <Arduino.h>
#include <IRremote.h>

//relay pin assignment
#define RELAY_PIN 2
//light switch pin assignment
#define SWITCH_PIN 8
//IF receiver pin assignment
#define IR_RECEIVE_PIN 7
//for light switch sate
int switching_state;

void setup() {
  // set pin for the light switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);

  // set Relay to HIGh every powerloss or reboot (arduino xiao high = off)
  digitalWrite(RELAY_PIN, HIGH);

  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  
  //set switching state on light switch
  if(digitalRead(SWITCH_PIN) == HIGH){
    switching_state = 1;
  }
  else{
    switching_state = 0;
  }
}

//turning Relay high or low (on/off)
void actuateRelay() {
  if(digitalRead(RELAY_PIN) == HIGH){
    digitalWrite(RELAY_PIN, LOW);
  }
  else{
    digitalWrite(RELAY_PIN, HIGH);
  }
}

void loop() {
  
  //check if light switch state
  if ((switching_state == 1) && (digitalRead(SWITCH_PIN) == LOW)){
    //actuate the relay (on or off)
    actuateRelay();
    //flip state
    switching_state = 0;
  }

  if ((switching_state == 0) && (digitalRead(SWITCH_PIN) == HIGH)){
    actuateRelay();
    switching_state = 1;
  }

  if (IrReceiver.decode()) {
    //compensate double sending by my remote
    delay(100);
    // My remote only transmit 0x0 hex commands
    if (IrReceiver.decodedIRData.command == 0x0) {
        // do something
        actuateRelay();
    }
  }
  //allow another IR command to be receive
  IrReceiver.resume();
}