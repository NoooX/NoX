void updateTime(int hours, int minutes, uint32_t color){

  int numb[10][7] = {{1, 1, 1, 0, 1, 1, 1}, // 0
                     {1, 0, 0, 0, 1, 0, 0}, // 1
                     {1, 1, 0, 1, 0, 1, 1}, // 2
                     {1, 1, 0, 1, 1, 1, 0}, // 3
                     {1, 0, 1, 1, 1, 0, 0}, // 4
                     {0, 1, 1, 1, 1, 1, 0}, // 5
                     {0, 1, 1, 1, 1, 1, 1}, // 6
                     {1, 1, 0, 0, 1, 0, 0}, // 7
                     {1, 1, 1, 1, 1, 1, 1}, // 8
                     {1, 1, 1, 1, 1, 1, 0}  // 9
                    }; 
                    
  int numb2[10][7] = {{1, 1, 1, 0, 1, 1, 1}, // 0
                      {0, 0, 1, 0, 0, 0, 1}, // 1
                      {1, 1, 0, 1, 0, 1, 1}, // 2
                      {0, 1, 1, 1, 0, 1, 1}, // 3
                      {0, 0, 1, 1, 1, 0, 1}, // 4
                      {0, 1, 1, 1, 1, 1, 0}, // 5
                      {1, 1, 1, 1, 1, 1, 0}, // 6
                      {0, 0, 1, 0, 0, 1, 1}, // 7
                      {1, 1, 1, 1, 1, 1, 1}, // 8
                      {0, 1, 1, 1, 1, 1, 1}  // 9
                     };
                    
  uint8_t first = hours % 10;
  uint8_t secound = hours / 10;
  uint8_t thrid = minutes / 10;
  uint8_t forth = minutes % 10;

  strip.setPixelColor(0, color);
  strip2.setPixelColor(0, color);
  
  for (int i = 0; i < 7; i++){
    if (numb[first][i]) 
    strip.setPixelColor(i+1, color);
    else
    strip.setPixelColor(i+1, 0);
  }

  for (int i = 0; i < 7; i++){
    if (numb[secound][i]) 
    strip.setPixelColor(i+8, color);
    else
    strip.setPixelColor(i+8, 0);
  }

  for (int i = 0; i < 7; i++){
    if (numb2[thrid][i]) 
    strip2.setPixelColor(i+1, color);
    else
    strip2.setPixelColor(i+1, 0);
  }

  for (int i = 0; i < 7; i++){
    if (numb2[forth][i]) 
    strip2.setPixelColor(i+8, color);
    else
    strip2.setPixelColor(i+8, 0);
  }
  strip.show();
  strip2.show();
}

