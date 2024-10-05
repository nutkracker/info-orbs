// XmasWidget.h

#ifndef XMAS_WIDGET_H
#define XMAS_WIDGET_H

#include "Widget.h"
#include <TFT_eSPI.h>
#include <vector>
#include <ctime>

// Structure to represent a snowflake
struct Snowflake {
    int x;
    int y;
    int speed;
    uint16_t color;
};

class XmasWidget : public Widget {
public:
    XmasWidget(ScreenManager &manager);
    void setup() override;
    void update(bool force) override;
    void draw(bool force) override;
    void changeMode() override;

private:
    // Time-related
    int daysUntilXmas();
    bool isXmasDay();
    void resetCountdown();

    // Holly animation
    unsigned long lastHollyUpdate;
    int hollyFrame;
    void drawHolly();

    // Snow animation
    std::vector<Snowflake> snowflakes;
    unsigned long lastSnowUpdate;
    void drawSnow();

    // Display message
    void displayCountdown();
    void displayMerryChristmas();

    // Utility
    TFT_eSPI &getDisplay();

    // Animation parameters
    const int MAX_SNOWFLAKES = 100;
};

#endif // XMAS_WIDGET_H
