
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 15;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
   Serial.begin(9600);
   Serial.println("Button test begin");
  // initialize the LED pin as an output:     
  //initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
}

void loop(){


  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.println(analogRead(buttonPin));
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
       for(int i=5; i<=7; i++) {
        //Serial.print("Pin "+String(i)+" on ");
        pinMode(i, OUTPUT);      
        digitalWrite(i, HIGH);  
        delay(500);
        //Serial.println("off");
        digitalWrite(i, LOW);  
      } 
  } 
}
