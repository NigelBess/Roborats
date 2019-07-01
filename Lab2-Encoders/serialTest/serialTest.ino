int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                if (incomingByte == 49) {
                  Serial.print("I received: ");
                  Serial.println("LOW");
                }
                if (incomingByte == 50) {
                  Serial.print("I received: ");
                  Serial.println("MEDIUM");
                }
                if (incomingByte == 51) {
                  Serial.print("I received: ");
                  Serial.println("HIGH");
                }
        }
}
