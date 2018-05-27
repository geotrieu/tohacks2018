void checkWater() {
  // Pulls data from the humidity sensor 
  output_value= analogRead(sensor_pin);
  //Serial.println(output_value);
  // Checks if there is water 
  if (output_value <= 400) {
    // Turn LED on
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn the LED off
    digitalWrite(ledPin, LOW);
  }
}

