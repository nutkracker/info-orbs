#include "widgets/clockWidget.h"

#include <config_helper.h>
#include <globalTime.h>

ClockWidget::ClockWidget(ScreenManager& manager) : Widget(manager) {
}

ClockWidget::~ClockWidget() {
}

void ClockWidget::setup() {
    m_lastDisplay1Digit = "-1";
    m_lastDisplay2Digit = "-1";
    m_lastDisplay4Digit = "-1";
    m_lastDisplay5Digit = "-1";
}

void ClockWidget::draw(bool force) {
    GlobalTime* time = GlobalTime::getInstance();
    
    if (m_lastDisplay1Digit != m_display1Digit || force) {
        displayDigit(0, m_display1Digit, 7, 5, FOREGROUND_COLOR);
        m_lastDisplay1Digit = m_display1Digit;
        if (SHADOWING != 1 &&m_display1Digit == " ") {
            m_manager.clearScreen(0);
        }
    }
    if (m_lastDisplay2Digit != m_display2Digit || force) {
        displayDigit(1, m_display2Digit, 7, 5, FOREGROUND_COLOR);
        m_lastDisplay2Digit = m_display2Digit;
    }
    if (m_lastDisplay4Digit != m_display4Digit || force) {
        displayDigit(3, m_display4Digit, 7, 5, FOREGROUND_COLOR);
        m_lastDisplay4Digit = m_display4Digit;
    }
    if (m_lastDisplay5Digit != m_display5Digit || force) {
        displayDigit(4, m_display5Digit, 7, 5, FOREGROUND_COLOR);
        m_lastDisplay5Digit = m_display5Digit;
    }

    if (m_secondSingle != m_lastSecondSingle || force) {
        if (m_secondSingle % 2 == 0) {
            displayDigit(2, ":", 7, 5, FOREGROUND_COLOR, false);
        } else {
            displayDigit(2, ":", 7, 5, BG_COLOR, false);
        }
#if SHOW_SECOND_TICKS == true        
        displaySeconds(2, m_lastSecondSingle, TFT_BLACK);
        displaySeconds(2, m_secondSingle, FOREGROUND_COLOR);
#endif
        m_lastSecondSingle = m_secondSingle;
        if (!FORMAT_24_HOUR && SHOW_AM_PM_INDICATOR) {
            displayAmPm(FOREGROUND_COLOR);
        }
    }
}

void ClockWidget::displayAmPm(uint32_t color) {
    GlobalTime* time = GlobalTime::getInstance();
    m_manager.selectScreen(2);
    TFT_eSPI& display = m_manager.getDisplay();
    display.setTextDatum(MC_DATUM);
    display.setTextSize(4);
    display.setTextColor(color, TFT_BLACK, true);
    String am_pm = time->isPM() ? "PM" : "AM";
    display.drawString(am_pm, SCREEN_SIZE - 50, SCREEN_SIZE / 2, 1);
}

void ClockWidget::update(bool force) {
    if (millis() - m_secondTimerPrev < m_secondTimer && !force) {
        return;
    }

    GlobalTime* time = GlobalTime::getInstance();
    m_hourSingle = time->getHour();

    m_minuteSingle = time->getMinute();
    m_secondSingle = time->getSecond();

    if (m_lastHourSingle != m_hourSingle || force) {
        if (m_hourSingle < 10) {
            if (FORMAT_24_HOUR) {
                m_display1Digit = "0";
            } else {
                m_display1Digit = " ";
            }
        } else {
            m_display1Digit = int(m_hourSingle/10);
        }
        m_display2Digit = m_hourSingle % 10;

        m_lastHourSingle = m_hourSingle;
    }

    if (m_lastMinuteSingle != m_minuteSingle || force) {
        String currentMinutePadded = String(m_minuteSingle).length() == 1 ? "0" + String(m_minuteSingle) : String(m_minuteSingle);

        m_display4Digit = currentMinutePadded.substring(0, 1);
        m_display5Digit = currentMinutePadded.substring(1, 2);

        m_lastMinuteSingle = m_minuteSingle;
    }
}

void ClockWidget::changeMode() {
    GlobalTime* time = GlobalTime::getInstance();
    time->setFormat24Hour( !time->getFormat24Hour() );
    draw(true);
}

void ClockWidget::buttonPressed(uint8_t buttonId, ButtonState state) {
    if (buttonId == BUTTON_OK && state == BTN_SHORT)
        changeMode();
}

void ClockWidget::displayDigit(int displayIndex, const String& digit, int font, int fontSize, uint32_t color, bool shadowing) {
    m_manager.selectScreen(displayIndex);
    TFT_eSPI& display = m_manager.getDisplay();
    display.setTextDatum(MC_DATUM);
    display.setTextSize(fontSize);
    if (shadowing && font == 7) {
        display.setTextColor(BG_COLOR, TFT_BLACK);
        display.drawString("8", SCREEN_SIZE / 2, SCREEN_SIZE / 2, font);
        display.setTextColor(color);
    } else {
        display.setTextColor(color, TFT_BLACK);
    }
    display.drawString(digit, SCREEN_SIZE / 2, SCREEN_SIZE / 2, font);
}

void ClockWidget::displayDigit(int displayIndex, const String& digit, int font, int fontSize, uint32_t color) {
    this->displayDigit(displayIndex, digit, font, fontSize, color, SHADOWING);
}

void ClockWidget::displaySeconds(int displayIndex, int seconds, int color) {
    m_manager.reset();
    m_manager.selectScreen(displayIndex);
    TFT_eSPI& display = m_manager.getDisplay();
    if (seconds < 30) {
        display.drawSmoothArc(SCREEN_SIZE / 2, SCREEN_SIZE / 2, 120, 110, 6 * seconds + 180, 6 * seconds + 180 + 6, color, TFT_BLACK);
    } else {
        display.drawSmoothArc(SCREEN_SIZE / 2, SCREEN_SIZE / 2, 120, 110, 6 * seconds - 180, 6 * seconds - 180 + 6, color, TFT_BLACK);
    }
}

String ClockWidget::getName() {
    return "Clock";
}