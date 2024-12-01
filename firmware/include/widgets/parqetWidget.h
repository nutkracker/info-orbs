#ifndef PARQET_WIDGET_H
#define PARQET_WIDGET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <globalTime.h>

#include "model/parqetDataModel.h"
#include "widget.h"
#include "utils.h"

#define PARQET_MODE_COUNT 10
#define PARQET_MAX_STOCKNAME_LINES 3

class ParqetWidget : public Widget
{
public:
    ParqetWidget(ScreenManager &manager);
    void setup() override;
    void update(bool force = false) override;
    void draw(bool force = false) override;
    void buttonPressed(uint8_t buttonId, ButtonState state) override;
    String getName() override;

private:
    String getTimeframe();
    void updatePortfolio();
    void updatePortfolioChart();
    void displayStock(int8_t displayIndex, ParqetHoldingDataModel &stock, uint32_t backgroundColor, uint32_t textColor);
    ParqetDataModel getPortfolio();
    void clearScreen(int8_t displayIndex, int32_t background);
    void displayClock(int8_t displayIndex, uint32_t background, uint32_t color, String extra, uint32_t extraColor);

    GlobalTime *m_time;

    unsigned long m_stockDelay = 15 * 60 * 1000; // default to 15m between updates
    unsigned long m_stockDelayPrev = 0;

    unsigned long m_cycleDelay = 30 * 1000; // cycle through pages (for more than 4/5 stocks) every 30 seconds
    unsigned long m_cycleDelayPrev = 0;

    unsigned long m_clockDelay = 60 * 1000; // update the clock every minute
    unsigned long m_clockDelayPrev = 0;

    String m_modes[PARQET_MODE_COUNT] = {"today", "1w", "1m", "3m", "6m", "1y", "3y", "mtd", "ytd", "max"}; // Possible timeframes: today, 1w, 1m, 3m, 6m, 1y, 3y, mtd, ytd, max
    int8_t m_curMode = 0;

    boolean m_showClock = true;              // Show clock on first screen
    boolean m_showTotalScreen = true;        // Show a total portfolio screen
    boolean m_showTotalValue = false;        // Show your total portfolio value
    boolean m_showTotalChart = true;         // Show performance chart for total (if we have more than 7 datapoints, ie. not for "today")
    String m_overrideTotalChartToday = "1w"; // Show this chart for "today" to have a chart there as well, set to empty string to disable
    boolean m_showValues = false;            // Show current price (false) or value in portfolio (true)

    ParqetDataModel m_portfolio;
    int m_holdingsDisplayFrom = 0;
    boolean m_changed = false;
    boolean m_everDrawn = false; // Track if our widget was ever drawn (to distinguish between an onboot and an onwidget update)
};
#endif // PARQET_WIDGET_H
