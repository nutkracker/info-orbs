// XmasWidget.cpp

#include "widgets/XmasWidget.h"

// Constructor
XmasWidget::XmasWidget(ScreenManager &manager)
    : Widget(manager),
      lastHollyUpdate(0),
      hollyFrame(0),
      lastSnowUpdate(0) {
    // Initialize snowflakes
    for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
        Snowflake flake;
        flake.x = random(0, 240); // Assuming screen width is 240
        flake.y = random(-240, 0); // Start above the screen
        flake.speed = random(1, 3);
        flake.color = TFT_WHITE;
        snowflakes.push_back(flake);
    }
}

// Setup method
void XmasWidget::setup() {
    Serial.println("XmasWidget: Setup started");
    // Initialize display settings if needed
    m_manager.selectScreen(2);
    TFT_eSPI &display = getDisplay();
    display.setRotation(0);
    display.fillScreen(TFT_BLACK);
    Serial.println("XmasWidget: Setup completed");
}

// Update method
void XmasWidget::update(bool force) {
    // Check if it's time to update animations
    unsigned long currentMillis = millis();

    // Update holly animation every second
    if (currentMillis - lastHollyUpdate >= 1000) {
        hollyFrame = (hollyFrame + 1) % 2; // Toggle between two frames
        lastHollyUpdate = currentMillis;
        draw(true);
    }

    // Update snow animation if it's Christmas Day
    if (isXmasDay()) {
        if (currentMillis - lastSnowUpdate >= 100) { // Adjust speed as needed
            // Update snowflakes positions
            for (auto &flake : snowflakes) {
                flake.y += flake.speed;
                if (flake.y > 240) { // Assuming screen height is 240
                    flake.y = random(-50, 0);
                    flake.x = random(0, 240);
                }
            }
            lastSnowUpdate = currentMillis;
            draw(true);
        }
    }
}

// Draw method
void XmasWidget::draw(bool force) {

    if (force)
    {
        TFT_eSPI &display = getDisplay();
            display.fillScreen(TFT_BLACK);

            if (isXmasDay()) {
                displayMerryChristmas();
                drawSnow();
            } else {
                drawHolly();
                displayCountdown();
            }
        }
    }
 

// ChangeMode method
void XmasWidget::changeMode() {
    // Implement mode changes if applicable
}

// Calculate days until Christmas
int XmasWidget::daysUntilXmas() {
    time_t now = time(nullptr);
    struct tm *timeInfo = localtime(&now);

    int currentYear = timeInfo->tm_year + 1900;
    struct tm xmas = {0};
    xmas.tm_year = currentYear - 1900;
    xmas.tm_mon = 11; // December (0-based)
    xmas.tm_mday = 25;

    time_t xmasTime = mktime(&xmas);
    double seconds = difftime(xmasTime, now);
    int days = seconds / 86400;

    if (days < 0) { // Christmas already passed this year
        // Optionally, set to next year's Christmas
        xmas.tm_year += 1;
        xmasTime = mktime(&xmas);
        seconds = difftime(xmasTime, now);
        days = seconds / 86400;
    }

    return days;
}

// Check if today is Christmas Day
bool XmasWidget::isXmasDay() {
    time_t now = time(nullptr);
    struct tm *timeInfo = localtime(&now);
    return (timeInfo->tm_mon == 11) && (timeInfo->tm_mday == 25);
}

// Reset countdown after January 2nd
void XmasWidget::resetCountdown() {
    // This function can be expanded if needed
    // Currently, daysUntilXmas() handles the reset logic
}

// Draw Holly Animation
void XmasWidget::drawHolly() {
    Serial.println("XmasWidget: Drawing enhanced holly");
    TFT_eSPI &display = getDisplay();

    // Define central position
    int centerX = display.width() / 2;
    int centerY = display.height() / 2 - 40; // Adjust vertically as needed

    // Parameters for leaves and berries
    int leafWidth = 40;
    int leafHeight = 60;
    int berryRadius = 6;

    // Optional: Clear only the holly area instead of the entire screen
    // For simplicity, we'll clear the screen
    display.fillScreen(TFT_BLACK);

    // Draw left leaf
    display.fillTriangle(
        centerX, centerY,
        centerX - leafWidth, centerY + leafHeight,
        centerX, centerY + leafHeight - 20,
        TFT_GREEN
    );

    // Draw right leaf
    display.fillTriangle(
        centerX, centerY,
        centerX + leafWidth, centerY + leafHeight,
        centerX, centerY + leafHeight - 20,
        TFT_GREEN
    );

    // Draw berries on left leaf
    display.fillCircle(centerX - leafWidth + 15, centerY + leafHeight - 25, berryRadius, TFT_RED);
    display.fillCircle(centerX - leafWidth + 25, centerY + leafHeight - 15, berryRadius, TFT_RED);

    // Draw berries on right leaf
    display.fillCircle(centerX + leafWidth - 15, centerY + leafHeight - 25, berryRadius, TFT_RED);
    display.fillCircle(centerX + leafWidth - 25, centerY + leafHeight - 15, berryRadius, TFT_RED);

    // Optional: Add shine or highlights for a more festive look
    display.drawLine(centerX, centerY, centerX - leafWidth, centerY + leafHeight, TFT_WHITE);
    display.drawLine(centerX, centerY, centerX + leafWidth, centerY + leafHeight, TFT_WHITE);

    Serial.println("XmasWidget: Enhanced holly drawn");
}

// Display Countdown Message
void XmasWidget::displayCountdown() {
    TFT_eSPI &display = getDisplay();
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(2);
    display.setTextDatum(MC_DATUM); // Middle Center Datum
    display.setCursor(120, 180); // Adjust position as needed

    int days = daysUntilXmas();
    String countdownMsg = String(days) + " Days Until Christmas";
    display.println(countdownMsg);
}

// Display Merry Christmas Message
void XmasWidget::displayMerryChristmas() {
    TFT_eSPI &display = getDisplay();
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(2);
    display.setTextDatum(MC_DATUM); // Middle Center Datum
    display.setCursor(120, 120); // Adjust position as needed

    String msg = "Merry Christmas!";
    display.println(msg);
}

// Draw Snow Animation
void XmasWidget::drawSnow() {
    TFT_eSPI &display = getDisplay();
    for (const auto &flake : snowflakes) {
        display.drawPixel(flake.x, flake.y, flake.color);
    }
}

// Utility to get display reference
TFT_eSPI &XmasWidget::getDisplay() {
    return m_manager.getDisplay();
}
