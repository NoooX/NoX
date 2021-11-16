// These functions will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin V0/V1/V2

BLYNK_WRITE(V0)
{
  EEPROM.write(1, r);
  EEPROM.write(2, g);
  EEPROM.write(3, b);
  #ifdef DEBUG 
  Serial.println("Saved:");
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  #endif
  EEPROM.commit();
}


BLYNK_WRITE(V1)
{
  r = param[0].asInt(); // assigning incoming value from pin V1 to a variable
  g = param[1].asInt(); // assigning incoming value from pin V1 to a variable
  b = param[2].asInt(); // assigning incoming value from pin V1 to a variable
  // process received value

  #ifdef DEBUG 
  Serial.println("Changed:");
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  #endif

  ReadLight();
  updateTime(hours, minutes, c);
}

BLYNK_WRITE(V2)
{
  bool buttonState = param.asInt();

  if (buttonState)
  {
    AutoBrightness = true;
    Serial.println(AutoBrightness);
    ReadLight();
  	updateTime(hours, minutes, c);
  }
  else
  {
    AutoBrightness = false;
    Serial.println(AutoBrightness);
    ReadLight();
  	updateTime(hours, minutes, c);
  }
}

BLYNK_WRITE(V3)
{
  EEPROM.get(1, r);
  EEPROM.get(2, g);
  EEPROM.get(3, b);

  ReadLight();
  updateTime(hours, minutes, c);

  if (param.asInt())
  {
  	#ifdef DEBUG 
  	Serial.println("RESTART");
  	#endif
  	previousMillis2 = millis();
  }
  else 
  {
  	currentMillis2 = millis();
  	if (currentMillis2 - previousMillis2 > 5000)
  	{
  		ESP.restart();
  	}
  	#ifdef DEBUG 
  	Serial.println("NOT RESTART");
  	#endif
  }
}
BLYNK_WRITE(V4)
{
  ota_flag = true;
}
