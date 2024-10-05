To enable MQTTWidget in your setup, do the following:

Step 1 - add the following to your config.h file
------
#define MQTT_WIDGET_HOST "192.168.3.40" // MQTT broker host
#define MQTT_WIDGET_PORT 1883 // MQTT broker port
#define MQTT_SETUP_TOPIC "info-orbs/setup/orbs" // Setup topic

// Optional MQTT Authentication Credentials
#ifndef MQTT_WIDGET_USER
#define MQTT_WIDGET_USER "user" // Leave empty if authentication is not required
#endif

#ifndef MQTT_WIDGET_PASS
#define MQTT_WIDGET_PASS "pass" // Leave empty if authentication is not required
#endif


Step 2 - publish the following to your MQTT topic as defined above.  Update the below to suit your setup:

{
  "orbs": [
    {
      "orbid": 0,
      "orbdesc": "House battery",
      "orb-bg": "TFT_SILVER",
      "orb-textcol": "TFT_BLACK",
      "topicsrc": "hamqtt/sensor/solis_battery_soc/state",
      "xpostxt": 30,
      "ypostxt": 85,
      "xposval": 90,
      "yposval": 125,
      "orbsize": 2,
      "orbvalunit": "%"
    },
    {
      "orbid": 1,
      "orbdesc": "Solar yield today",
      "orb-bg": "TFT_BLUE",
      "orb-textcol": "TFT_WHITE",
      "topicsrc": "hamqtt/sensor/solis_power_generation_today/state",
      "xpostxt": 10,
      "ypostxt": 85,
      "xposval": 70,
      "yposval": 125,
      "orbsize": 2,
      "orbvalunit": "kWh"
    },
    {
      "orbid": 2,
      "orbdesc": "PV Solar Today",
      "orb-bg": "TFT_BLUE",
      "orb-textcol": "TFT_WHITE",
      "topicsrc": "hamqtt/sensor/solcast_pv_forecast_forecast_today/state",
      "xpostxt": 21,
      "ypostxt": 90,
      "xposval": 35,
      "yposval": 125,
      "orbsize": 2,
      "orbvalunit": "kWh"
    },
    {
      "orbid": 3,
      "orbdesc": "PV Solar Today+1",
      "orb-bg": "TFT_BLUE",
      "orb-textcol": "TFT_WHITE",
      "topicsrc": "hamqtt/sensor/solcast_pv_forecast_forecast_tomorrow/state",
      "xpostxt": 10,
      "ypostxt": 90,
      "xposval": 50,
      "yposval": 125,
      "orbsize": 2,
      "orbvalunit": "kWh"
    },
    {
      "orbid": 4,
      "orbdesc": "Garage temp",
      "orb-bg": "TFT_BLUE",
      "orb-textcol": "TFT_WHITE",
      "topicsrc": "zigbee2mqtt/Garage Temp2",
      "xpostxt": 50,
      "ypostxt": 90,
      "xposval": 70,
      "yposval": 125,
      "orbsize": 2,
      "orbvalunit": "'c",
      "jsonfield": "temperature"
    }
  ]
}

Note: for Orbid 4, there's an additional jsonfield that is used to extract a value from a JSON payload returned from the topic zigbee2mqtt/Garage Temp2.  It looks like this:

{
  "battery": 100,
  "humidity": 56.7,
  "linkquality": 69,
  "temperature": 17.8,
}

------