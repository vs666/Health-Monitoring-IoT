#include <Adafruit_GFX.h>     // Graphics library for drawin on screen
#include <Adafruit_ST7789.h>  // Hardware-specific library
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
static uint32_t tsLastReport = 0; 
#define SCALING 12                                                   // Scale height of trace, reduce value to make trace height \
                                                                     // bigger, increase to make smaller
#define TRACE_SPEED 0.5                                              // Speed of trace across screen, higher=faster
#define TRACE_MIDDLE_Y_POSITION 41                                   // y pos on screen of approx middle of trace
#define TRACE_HEIGHT 64                                              // Max height of trace in pixels
#define HALF_TRACE_HEIGHT TRACE_HEIGHT / 2                           // half Max height of trace in pixels (the trace amplitude)
#define TRACE_MIN_Y TRACE_MIDDLE_Y_POSITION - HALF_TRACE_HEIGHT + 1  // Min Y pos of trace, calculated from above values
#define TRACE_MAX_Y TRACE_MIDDLE_Y_POSITION + HALF_TRACE_HEIGHT - 1  // Max Y pos of trace, calculated from above values

void setup() {
    tft.init();  // initialize a ST7735S chip, for 128x128 display
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(0, 0);



    // Display BPM and O2 titles, these remain on screen, we only erase the trace and the
    // BPM/O2 results, otherwise we can get some flicker
    tft.setTextSize(2);
    tft.setCursor(0, 86);
    tft.print("BPM   O");
    tft.setCursor(92, 86);
    tft.print("%");
    tft.setTextSize(1);
    tft.setCursor(84, 94);
    tft.print("2");  // The small subscriper 2 of O2
    tft.setCursor(1, 0);
    tft.print("XTronical Health Care");
    tft.setTextSize(2);
    tft.drawRect(0, TRACE_MIN_Y - 1, 128, TRACE_HEIGHT + 2, TFT_BLUE); 
}

void loop() {
   
        if (millis() - tsLastReport > 1000)               // If more than 1 second (1000milliseconds) has past
        {                                                 // since getting heart rate and O2 then get some bew values
            tft.fillRect(0, 104, 128, 16, TFT_BLACK);  // Clear the old values
            int BPM = 20;              // Get BPM
            if ((BPM < 60) | (BPM > 110)) {               // If too low or high for a resting heart rate then display in red
                tft.setTextColor(TFT_RED);
            } else {
                tft.setTextColor(TFT_GREEN);  // else display in green
            }
            tft.setCursor(0, 104);  // Put BPM at this position
            tft.print(BPM);         // print BPM to screen
            int O2 = 98;     // Get the O2
            if (O2 < 94)            // If too low then display in red
                tft.setTextColor(TFT_RED);
            else
                tft.setTextColor(TFT_GREEN);  // else green
            tft.setCursor(72, 104);              // Set print position for the O2 value
            tft.print(O2);                       // print it to screen
            tsLastReport = millis();             // Set the last time values got to current time
        }
}
