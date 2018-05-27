void checkWater() {
  output_value= analogRead(sensor_pin);
  //Serial.println(output_value);
  if (output_value <= 400) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

