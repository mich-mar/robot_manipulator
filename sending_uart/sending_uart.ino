
int counter = 0; // Initialize counter variable
int analogValue_1, analogValue_2, analogValue_3;


void setup() {
  pinMode(A3, INPUT); // Ustawienie GPIO 28 jako wejścia
  pinMode(A2, INPUT); // Ustawienie GPIO 28 jako wejścia
  pinMode(A1, INPUT); // Ustawienie GPIO 28 jako wejścia
  
  Serial.begin(100000);
}

void loop() {
  //counter++;                         
  //Serial.print("Counter: ");        
  //Serial.print(counter);           

  analogValue_1 = analogRead(A3);
  analogValue_2 = analogRead(A2);
  analogValue_3 = analogRead(A1);

  //Serial.print("  Analog Value: ");    // print analog label
  Serial.print(analogValue_1);       // print the mapped analog value
  Serial.print(";");       // print the mapped analog value
  Serial.print(analogValue_2);       // print the mapped analog value
  Serial.print(";");       // print the mapped analog value
  Serial.println(analogValue_3);       // print the mapped analog value
}
