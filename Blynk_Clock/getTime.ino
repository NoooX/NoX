BLYNK_CONNECTED() 
{
  // Synchronize time on connection
  rtc.begin();
}

void getTime()
{
  hours = hour();
  minutes = minute();
  #ifdef DEBUG 
  Serial.print("Current time: ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.println();
  #endif
}