// mqttWidget.h

#ifndef MQTT_WIDGET_H
#define MQTT_WIDGET_H

#include <Widget.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "utils.h"
#include <WiFiClient.h>
#include <vector>
#include <map>

// Structure to hold individual orb configurations
struct OrbConfig {
    int orbid;               // Orb identifier
    String orbdesc;          // Orb description/title
    uint16_t orbBgColor;    // Background color of the orb
    uint16_t orbTextColor;  // Text color within the orb
    String topicSrc;        // MQTT topic source for data
    int xpostxt;            // x axis label
    int ypostxt;            // y axis label
    int xposval;            // x axis value
    int yposval;            // y axis value
    String orbvalunit;      // value unit
    int orbsize;            // font size
    String jsonField;       // JSON field to extract
    std::map<String, String> lastValuesMap; // Store last values for fields
};

class MQTTWidget : public Widget {
public:
    /**
     * @brief Constructor for MQTTWidget.
     * 
     * @param manager Reference to the ScreenManager.
     * @param host MQTT broker host address.
     * @param port MQTT broker port number.
     */
    MQTTWidget(ScreenManager &manager, const String &host, uint16_t port);

    /**
     * @brief Initializes the MQTT connection and subscribes to the setup topic.
     */
    void setup() override;

    /**
     * @brief Handles MQTT loop to process incoming messages.
     * 
     * @param force Force update flag.
     */
    void update(bool force = false) override;

    /**
     * @brief Draws all orbs on the display based on current data.
     * 
     * @param force Force redraw flag.
     */
    void draw(bool force = false) override;

    /**
     * @brief Handles mode changes (if applicable).
     */
    void changeMode();

    void buttonPressed(uint8_t buttonId, ButtonState state) override;
    String getName() override;   

private:
    // MQTT-related members
    String mqttHost;                // MQTT broker host
    uint16_t mqttPort;              // MQTT broker port
    WiFiClient wifiClient;          // Wi-Fi client for MQTT
    PubSubClient mqttClient;        // MQTT client

    // Configuration from setup topic
    std::vector<OrbConfig> orbConfigs;          // Vector of orb configurations

    // Data storage: maps topicSrc to latest message
    std::map<String, String> orbDataMap;

    // Static callback proxy
    static void staticCallback(char* topic, byte* payload, unsigned int length);

    // Static instance pointer
    static MQTTWidget* instance;

    // Helper functions
    void reconnect();                     // Handle MQTT reconnection
    void callback(char* topic, byte* payload, unsigned int length); // MQTT message callback
    void handleSetupMessage(const String &message); // Process setup JSON
    void subscribeToOrbs();               // Subscribe to all configured orb topics
    uint16_t getColorFromString(const String &colorStr); // Convert color string to uint16_t
    void drawOrb(int orbid);              // Draw a single orb based on orbid
};

#endif // MQTT_WIDGET_H
