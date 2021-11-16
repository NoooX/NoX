void sucessStart()
{
    uint8_t EEPROMgreen;
    uint8_t EEPROMred;
    uint8_t EEPROMblue; 
    EEPROM.get(1, EEPROMred);
    EEPROM.get(2, EEPROMgreen);
    EEPROM.get(3, EEPROMblue);
    r = EEPROMred;
    g = EEPROMgreen;
    b = EEPROMblue;

   c = strip.Color(EEPROMred, EEPROMgreen, EEPROMblue,0);
   colorWipe(c, 50);
    c = strip.Color(0, 0, 0,0);
   colorWipeRev(c, 50);
    c = strip.Color(EEPROMred, EEPROMgreen, EEPROMblue,0);
   colorWipe(c, 50);
    c = strip.Color(0, 0, 0,0);
   colorWipeRev(c, 50); 
    c = strip.Color(EEPROMred, EEPROMgreen, EEPROMblue,0);
    return;
}

