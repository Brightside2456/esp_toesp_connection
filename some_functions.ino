void buzz_light(int num)
{
  for (int i = 0; i < num; i++)
  {
    // Turn on the buzzer
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(redpin, HIGH);
  delay(200);
  // Turn off the buzzer
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redpin, LOW);
  delay(200); // Wait for 1 second
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(redpin, HIGH);
  delay(200); // Wait for 1 second

  // Turn off the buzzer
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redpin, LOW);
  delay(1000); // Wait for 1 second
  }
}