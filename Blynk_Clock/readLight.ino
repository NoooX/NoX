void ReadLight()
{
  float brightnessValue;
  float newValue = analogRead(SUN_PIN);
  newValue = map(newValue, 0, 1023, 0, 100);
  #ifdef DEBUG2
  rdebugDln("ReadValue value: %f\n" ,newValue);
  #endif
  brightnessValue = smoothing(arr, newValue);
  brightnessValue = brightnessValue/100;
  brightnessValue = 0.9909809 + (0.06153342 - 0.9909809)/(1 + pow((brightnessValue/0.7309053), 6.528228));
  //548639 + (0.1181893 - 548639)/(1 + pow((brightnessValue/58.05919), 3.218475));

  //0.9909809 + (0.06153342 - 0.9909809)/(1 + pow(brightnessValue/0.7309053), 6.528228))
  #ifdef DEBUG
  newValue = newValue/100;
  Serial.print(newValue);
  Serial.print(",");
  Serial.println(brightnessValue);
  #endif
  
  if (brightnessValue > 1)
    brightnessValue = 1;
  yield();
  #ifdef DEBUG2
  rdebugDln("Brightness value: %f\n" ,brightnessValue);
  #endif
  if (AutoBrightness)
  {
    if (r == 0 && g == 0 && b == 0)
    {
      uint8_t EEPROMgreen;
      uint8_t EEPROMred;
      uint8_t EEPROMblue; 
      EEPROM.get(1, EEPROMred);
      EEPROM.get(2, EEPROMgreen);
      EEPROM.get(3, EEPROMblue);
      #ifdef DEBUG
      Serial.println("EEPROM:");
      Serial.println(EEPROMred);
      Serial.println(EEPROMgreen);
      Serial.println(EEPROMblue);
      #endif

      c = strip.Color(EEPROMgreen*brightnessValue, EEPROMred*brightnessValue, EEPROMblue*brightnessValue, 0);
    }
    else
    {
      c = strip.Color(r*brightnessValue, g*brightnessValue, b*brightnessValue, 0);
    }
  }
  else 
  {
    c = strip.Color(r, g, b);
  }
}

int smoothing(int *arr, int newVaule)
{
  int temp[100] = {0};
  unsigned int sum = 0;
  
  for (int i = 1; i < 100; i++)
  {
    temp[i-1] = arr[i];
    sum = sum + temp[i-1];
  }
  sum = sum + newVaule;
  temp[99] = newVaule;
  sum = sum / 100;
for (int i = 0; i < 100; i++)
  {
    arr[i] = temp[i];
  }

  return sum;
}
