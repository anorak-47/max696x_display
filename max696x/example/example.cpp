#include <inttypes .h>
#include <max696x .h>
#include <string .h> 
#include <stdio .h>
#include "font_data.h"
#include "bmpfont.h"

//-------------------------------------------------------
// Global variables

// a byte array for forming the display data in memory to be copied to the display
byte planeData[64]; 

// a character string to hold strings to be displayed
char str[9];

// a variable to keep track of the enable/disable state of the display
byte shutdown = 1;

// instantiate the MAX696x driver from the library.
max696x max6960;

//-------------------------------------------------------
// Routine to configure the ATMEGA168

void setup(void)
{
  // Configure serial port
  Serial.begin(19200);
  Serial.flush();
  Serial.print('\n');
  Serial.print('\n');
  Serial.println("Baudrate set to 19200");

  // call the max696x initialisation function
  max6960.init((GPC_PIXEL_INTENSITY_2BPP | GPC_RIPPLESYNC_ENABLED | 
    GPC_DISPLAY_MONO | GPC_ENABLE | GPC_PIXEL_NORMAL),   // Global Panel Config register setting
    0x80,                           // Panel Intensity
    PIS_FULL ,                    // Pixel Intensity Scale
    NUMBER_OF_DRIVER_DEVICES,    // Number of driver devices
    NUMBER_OF_DRIVERS_PER_ROW,   // Number of drivers per row
    PCR_RATE);                   // Frame Rate, calculated from the max696x_conf.h settings

  // a variable to keep track of when the display is enabled or shut down
  shutdown = 0;
  return;
}

//------------------------------------------------
// The main loop

void loop(void)
{ 
  // variables used by the main loop
  byte data = 0;
  byte i = 0;
  byte k = 0;
  byte m = 0;
  byte mode = 0;
  uint8_t temp[5];
  byte digit = DIGIT_0_INTENSITY;
  char inStr [10];  // string for storing serial data
  byte terminalMode = 0;

  Serial.println("Main loop");

  // store the mode of operation of the display, 1 BPP or 2 BPP
  mode = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL) & GPC_PI;

  // test display pixels
  if(max6960.display_test()){
    Serial.println("WARNING: DISPLAY HAS A FAULT!");
  }
  
  // dump the register settings of the display drivers
  max6960.reg_dump();

  // make sure a couple of string variables are zeroed out before we use them
  memset(inStr, 0x00, 10);
  memset(str, 0x00, 9);

  // the default display string
  strcpy(str, "Arduino");

  while(1) {
    // clear the serial input string
    memset(inStr, 0x00, 10);
    
    // wait for plane change pin to change, but don't wait if the 
    // display is shutdown because the plane change pin won't change...
    // This is the INTERRUPT pin
    while(((PINB & 0x01) == 1) && (shutdown == 0));

    // check for any incoming serial data
    if (Serial.available() > 0) {
      i = 0;
      // copy available serial data
      while ((Serial.available() > 0) && (i < 9)) {
        inStr[i] = Serial.read();
        i++;
      }

      // null terminate the string properly
      inStr[i] = 0;

      // echo back the string
      Serial.print("Rx: ");
      Serial.println(inStr);

      // Terminal mode allows you to type to the display, otherwise any
      // data send to the display will be interpreted as a command.
      // Valid commands are:
      // a - select panel digit 0
      // b - select panel digit 1
      // c - clear display
      // d - decrease selected display segment brightness
      // u - increase selected display segment brightness
      // U - increase panel intensity
      // D - decrease panel intensity
      // 1 - select the 1st display segment
      // 2 - select the 2nd display segment
      // 3 - select the 3rd display segment
      // 4 - select the 4th display segment
      // s - toggle shutdown/enable of the display
      // i - toggle invert pixels
      // r - dump the display registers
      // +++ - enter / exit terminal mode
      // R - reset the display

      if (terminalMode == 0) {
        switch (inStr[0]) {
        case 'a' :
          // select panel digit 0
          digit = DIGIT_0_INTENSITY;
          break;
        case 'b' :
          // select panel digit 1
          digit = DIGIT_1_INTENSITY;
          break;
        case 'c' :
          // clear display
          max6960.command_wr(GLOBAL_CLEAR_PLANES, (GCP_REDP0 | GCP_REDP1 | GCP_REDP2 | GCP_REDP3), GLOBAL);
          break;
        case 'u' :
          // increase selected display segment brightness
          data = max6960.command_rd(digit, LOCAL);      
          max6960.command_wr(digit, data + 8, LOCAL);
          //            data = max696x_command_rd(digit, LOCAL);
          break;
        case 'd' :
          // decrease selected display segment brightness
          data = max6960.command_rd(digit, LOCAL);           
          max6960.command_wr(digit, data - 8, LOCAL);
          //            data = max696x_command_rd(digit, LOCAL);
          break;          
        case 'U' :
          // increase Panel Instensity
          data = max6960.command_rd(PANEL_INTENSITY, GLOBAL);        
          max6960.command_wr(PANEL_INTENSITY, data + 8, GLOBAL);
          //            data = max696x_command_rd(PANEL_INTENSITY, GLOBAL);
          break;  
        case 'D' :
          // decrease Panel Instensity
          data = max6960.command_rd(PANEL_INTENSITY, GLOBAL);            
          max6960.command_wr(PANEL_INTENSITY, data - 8, GLOBAL);
          //            data = max696x_command_rd(PANEL_INTENSITY, GLOBAL);
          break;            
        case '1' :
        case '2' :
          // select the 1st or 2nd display segment ie 1st driver ic
          max6960.command_wr(GLOBAL_DRV_INDIR_ADDR, 0x00, LOCAL);
          data = max6960.command_rd(GLOBAL_DRV_INDIR_ADDR, GLOBAL);
          Serial.print("Global Driver Indirect Reg = 0x");
          Serial.println(data, HEX);
          break;
        case '3' :
        case '4' :
          // select the 3rd or 4th display segment ie 1st driver ic
          max6960.command_wr(GLOBAL_DRV_INDIR_ADDR, 0x01, GLOBAL);
          data = max6960.command_rd(GLOBAL_DRV_INDIR_ADDR, GLOBAL);
          Serial.print("Global Driver Indirect Reg = 0x");
          Serial.println(data, HEX);
          break;
        case 'i' :
          // toggle invert pixels
          data = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
          if ((data & GPC_PIXEL_INVERT) == 0) {
            max6960.command_wr(GLOBAL_PANEL_CONFIG, data | GPC_PIXEL_INVERT, GLOBAL); 
          }
          else {
            max6960.command_wr(GLOBAL_PANEL_CONFIG, data & ~(GPC_PIXEL_INVERT), GLOBAL); 
          }
          break;
        case 's' :
          // toggle shutdown/enable of display
          data = max6960.command_rd(GLOBAL_PANEL_CONFIG, GLOBAL);
          if ((data & GPC_ENABLE) == 0) {
            max6960.command_wr(GLOBAL_PANEL_CONFIG, data | GPC_ENABLE, GLOBAL);
            shutdown = 0;
          }
          else {
            max6960.command_wr(GLOBAL_PANEL_CONFIG, data & ~(GPC_ENABLE), GLOBAL);
            shutdown = 1;
          }           
          break;
        case 'r' :
          // register dump
          max6960.reg_dump();
          break;
        case '+' :
          // Enter terminal mode
          if ((inStr[1] == '+') && (inStr[2] == '+') && (inStr[3] == 0)) {
            Serial.println("Entering terminal mode, type '+++' to exit");
            terminalMode = 1;
          }
          break;
        case 'R' :
          // reset the display ICs
          max6960.display_reset();
          break;
        default :
          break;        
        }
      }
      else {
        // Exit terminal mode
        if ((inStr[0] == '+') && (inStr[1] == '+') && (inStr[2] == '+') && (inStr[3] == 0)) {
          Serial.println("Leaving terminal mode, type '+++' to reenter");
          terminalMode = 0;
        }
        else {
          // in terminal mode, so copy received data from inStr to str, which is where
          // string data to be displayed is held
          strncpy (str, inStr, 8);
        }
      }
      // clear the received data buffer
      Serial.flush();
    }

    // clear the Ardunio memory used to hold the display data in CPU memory
    // before sending it to the display memory planes.
    memset(planeData, 0x00, 64);

    // write font data to the display memory
    i = 0; // counter for string data
    k = 0; // counter for planeData array / pixel column counter

    // Loop through the string character by character filling the planeData array
    // with pixel data. This is done differently for 1 BPP and 2 BPP modes.
    //
    // In this example, we loop through the string until a NULL (0 dec) is 
    // encountered indicating we've reached the end of the string or k (the 
    // planeData address counter) is greater than 31 indicating we've reached the
    // final column of display pixels.

    while ((str[i] != 0) && (k < 32)) {
      if (mode == GPC_PIXEL_INTENSITY_1BPP) {
        // 1 BPP mode

        // The display memory maps directly one to one to pixels so we just copy the
        // data for each character straight over into the planeData memory, moving 
        // the address along 5 bytes at a time.  The valid address range is 0x00 
        // (0 dec) to 0x1f (31 dec) for this example.

        // This mode does not seem to care if we write past the last address.
        get_char_data((unsigned char) str[i], &planeData[k]);
      }
      else {
        // 2 BPP mode

        // This mode is prone to data corruption if you go beyond the end of the 
        // display memory, in this case, 64 bytes (addresses 0x00 to 0x3F)

        // Since we're using a character set font written for 1 bit per pixel operation
        // we need to do things differently.

        // first we fetch the data for each character in turn, placing it into
        // a temporary array, 5 bytes long
        get_char_data((unsigned char) str[i], temp);

        // Now we need to write the font data from the temp array into the planeData 
        // array one byte at a time, but we need to do 2 things to the data on the way:
        //
        //    1) convert it from 1 bit per pixel data to 2 bits per pixel data
        //       This is done by the calls to max6960.convert_to_2bpp()
        //
        //    2) arrange the planeData in internal memory so the data is displayed
        //       correctly in 2 bits per pixel mode once it has been transferred to
        //       the display driver memory.  This is somewhat dependent on the size 
        //       of your display and how your display panels are arranged.
        //
        //       Refer to the description of the display driver memory arrangement in:
        //            <arduino base dir>/hardware/libraries/max696x/max696x.h

        for (m = 0; m < CHAR_LEN; m++) {
          // check to see if we're at the final column of the display
          if ((k + m) > 31) {
            // stop the loop if we're at the final column of the display
            break;
          }
          else {
            // Construct the plane data in internal memory prior to transferring it to 
            // the display driver memory.
            //
            // In this example, we put 8-bit 2 BPP data in addresses 0x00 to 0x1f 
            // corresponding to rows 0-3 for columns 0-31 of the display and put
            // 8-bit 2 BPP data in addresses 0x20 to 0x3f corresponding to rows 4-7
            // for columns 0-31.
            // 
            // This basically corresponds to the data for the top 4 pixels of each 
            // character get written to addresses 0 to 31 and the data for the bottom
            // 4 pixels get written to addresses 32 to 63

            planeData[k + m] = max6960.convert_to_2bpp((temp[m] & 0x0F));
            planeData[k + m + 32] = max6960.convert_to_2bpp(((temp[m] & 0xF0) >> 4));
          }
        }
      }
      // move the planeData address counter along by the width of a character in pixels.
      k = k + 5;
      // move the string character counter along by one.
      i++;
    }

    // write to display memory, copy the whole planeData array to the display
    max6960.mem_update(planeData, 64);
  }
}