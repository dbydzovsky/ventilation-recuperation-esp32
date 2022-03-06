#include "Arduino.h"
#include "Display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "TimeLib.h"
#define OLED_RESET 0
#include "../Dependencies/Dependencies.h"

void drawCircle(Adafruit_SSD1306 * d) {
  for (int16_t i=0; i<d->height(); i+=2) {
    d->drawCircle(d->width()/2, d->height()/2, i, WHITE);
    d->display();
    delay(1);
  }
}

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


Display::Display(Dependencies * deps){
  Adafruit_SSD1306 * display = new Adafruit_SSD1306(OLED_RESET);
  this->d = display;
  this->deps = deps;
}

void Display::setup() {
  this->d->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  this->d->display();
}

 unsigned char wifi2_icon12x10[] =
{
	0b00000000, 0b00000000, //                 
	0b00011111, 0b10000000, //    ######       
	0b00111111, 0b11000000, //   ########      
	0b01100000, 0b01100000, //  ##      ##     
	0b00001111, 0b00000000, //     ####        
	0b00011111, 0b10000000, //    ######       
	0b00010000, 0b10000000, //    #    #       
	0b00000110, 0b00000000, //      ##         
	0b00000110, 0b00000000, //      ##         
	0b00000000, 0b00000000, //                 
};

 unsigned char temperature_icon16x16[] =
{
	0b00000001, 0b11000000, //        ###      
	0b00000011, 0b11100000, //       #####     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00000111, 0b11100000, //      ######     
	0b00000111, 0b00100000, //      ###  #     
	0b00001111, 0b11110000, //     ########    
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00001111, 0b11110000, //     ########    
	0b00000111, 0b11100000, //      ######     
};

 unsigned char warning_icon16x16[] =
{
	0b00000000, 0b10000000, //         #       
	0b00000001, 0b11000000, //        ###      
	0b00000001, 0b11000000, //        ###      
	0b00000011, 0b11100000, //       #####     
	0b00000011, 0b01100000, //       ## ##     
	0b00000111, 0b01110000, //      ### ###    
	0b00000110, 0b00110000, //      ##   ##    
	0b00001110, 0b10111000, //     ### # ###   
	0b00001100, 0b10011000, //     ##  #  ##   
	0b00011100, 0b10011100, //    ###  #  ###  
	0b00011000, 0b10001100, //    ##   #   ##  
	0b00111000, 0b00001110, //   ###       ### 
	0b00110000, 0b10000110, //   ##    #    ## 
	0b01111111, 0b11111111, //  ###############
	0b01111111, 0b11111111, //  ###############
	0b00000000, 0b00000000, //                 
};

 unsigned char nocon_icon16x12[] =
{
	0b00000111, 0b11110000, //      #######    
	0b00011111, 0b11111100, //    ###########  
	0b00111110, 0b00111110, //   #####   ##### 
	0b00111000, 0b01111110, //   ###    ###### 
	0b01110000, 0b11111111, //  ###    ########
	0b01110001, 0b11110111, //  ###   ##### ###
	0b01110011, 0b11000111, //  ###  ####   ###
	0b01110111, 0b10000111, //  ### ####    ###
	0b00111111, 0b00001110, //   ######    ### 
	0b00111110, 0b00011110, //   #####    #### 
	0b00011111, 0b11111100, //    ###########  
	0b00000111, 0b11110000, //      #######    
};
 unsigned char humidity_icon16x16[] =
{
	0b00000000, 0b00000000, //                 
	0b00000001, 0b10000000, //        ##       
	0b00000011, 0b11000000, //       ####      
	0b00000111, 0b11100000, //      ######     
	0b00001110, 0b01110000, //     ###  ###    
	0b00001100, 0b00110000, //     ##    ##    
	0b00011100, 0b00111000, //    ###    ###   
	0b00011000, 0b00011000, //    ##      ##   
	0b00111000, 0b00011100, //   ###      ###  
	0b00111000, 0b00011100, //   ###      ###  
	0b00111000, 0b00011100, //   ###      ###  
	0b00011100, 0b00111000, //    ###    ###   
	0b00011111, 0b11111000, //    ##########   
	0b00001111, 0b11110000, //     ########    
	0b00000011, 0b11000000, //       ####      
	0b00000000, 0b00000000, //                 
};

 unsigned char arrow_down_icon16x16[] =
{
	0b00001111, 0b11110000, //     ########    
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011100, 0b00111000, //    ###    ###   
	0b00011100, 0b00111000, //    ###    ###   
	0b00011100, 0b00111000, //    ###    ###   
	0b01111100, 0b00111110, //  #####    ##### 
	0b11111100, 0b00111111, // ######    ######
	0b11111100, 0b00111111, // ######    ######
	0b01111000, 0b00011110, //  ####      #### 
	0b00111100, 0b00111100, //   ####    ####  
	0b00011110, 0b01111000, //    ####  ####   
	0b00001111, 0b11110000, //     ########    
	0b00000111, 0b11100000, //      ######     
	0b00000011, 0b11000000, //       ####      
	0b00000001, 0b10000000, //        ##       
};

 unsigned char heart_icon16x16[] =
{                
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00111100, 0b01111000, //   ####   ####   
	0b01111110, 0b11111100, //  ###### ######  
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b01111111, 0b11111100, //  #############  
	0b01111111, 0b11111100, //  #############  
	0b00111111, 0b11111000, //   ###########   
	0b00011111, 0b11110000, //    #########    
	0b00001111, 0b11100000, //     #######     
	0b00000111, 0b11000000, //      #####      
	0b00000011, 0b10000000, //       ###    
	0b00000001, 0b00000000, //    	  #
};

 unsigned char co2_icon16x16[] =
{
	0b00001111, 0b11110000, //     ########    
	0b00011111, 0b11111000, //    ##########   
	0b00011111, 0b11111000, //    ##########   
	0b00011100, 0b00111000, //    ###    ###   
	0b00011100, 0b00111000, //    ###    ###   
	0b00011100, 0b00111000, //    ###    ###   
	0b01111100, 0b00111110, //  #####    ##### 
	0b11111100, 0b00111111, // ######    ######
	0b11111100, 0b00111111, // ######    ######
	0b01111000, 0b00011110, //  ####      #### 
	0b00111100, 0b00111100, //   ####    ####  
	0b00011110, 0b01111000, //    ####  ####   
	0b00001111, 0b11110000, //     ########    
	0b00000111, 0b11100000, //      ######     
	0b00000011, 0b11000000, //       ####      
	0b00000001, 0b10000000, //        ##       
};
void Display::act() {
  this->d->clearDisplay();
  // if (digitalRead(btnpin)) {
  //   testdrawcircle();
  //   delay(2000);
  //   this->d->clearDisplay();
  // }
  int seconds = (int) millis() / 1000;
  if (WiFi.status() == WL_CONNECTED) {
    this->d->drawBitmap(52, 0, wifi2_icon12x10, 12, 10, WHITE);
  } else {  
    this->d->drawBitmap(52, 0, nocon_icon16x12, 12, 10, WHITE);
  }

  this->d->setTextSize(1);
  this->d->setTextColor(WHITE);
  
  if (this->deps->timeProvider->isTimeSet()) {
    char hourBuffer[3];
    if (hour() < 10) {
      sprintf(hourBuffer, "0%d", hour());
    } else {
      sprintf(hourBuffer, "%d", hour());
    }
    char minuteBuffer[3];
    if (minute() < 10) {
      sprintf(minuteBuffer, "0%d", minute());
    } else {
      sprintf(minuteBuffer, "%d", minute());
    }
    char timeBuffer[6];
    if (seconds % 2 == 1) {
      sprintf(timeBuffer, "%s:%s", hourBuffer, minuteBuffer);
    } else {
      sprintf(timeBuffer, "%s %s", hourBuffer, minuteBuffer);
    }
    this->d->setCursor(1,0);
    this->d->print(timeBuffer);
  }
  this->d->setTextSize(2);
  this->d->drawBitmap(0,12, temperature_icon16x16, 16, 12, WHITE);
  this->d->setCursor(18,12);
  if (this->deps->outsideTemp->isInitialized()) {  
    this->d->print((int) this->deps->outsideTemp->getAverage());
  } else {
    this->d->print("..");
  }

  if (seconds % 2 == 1) {
    this->d->drawBitmap(44, 14, warning_icon16x16, 16,16,WHITE);
  }
  

  this->d->drawBitmap(0,30, heart_icon16x16, 16,16, WHITE);
  this->d->setCursor(16, 30);
  // this->d->drawLine(18, 46, 62, 46, WHITE); // x, height, width
  if (this->deps->co2Inside->isInitialized()) {
    this->d->print((int) this->deps->co2Inside->getAverage());
  } else {
    this->d->print("....");
  }
  // this->d->setTextColor(BLACK, WHITE); // 'inverted' text
  // this->d->print("hum:");
  // this->d->println(hum);
  // this->d->setTextSize(1);
  // this->d->setTextColor(WHITE);
  // this->d->print("co2:");
  // this->d->println(ppm);
  // this->d->display();
  this->d->display();
}