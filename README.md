# ESP8266-GardenSensor
IoT sensor that samples and publishes soil moisture data to Adafruit IO.
The device wakes up every 15 minutes, connects using MQTT, collects a sample, publishes and sleeps. 

This should roughly run for 165 days on a 9V cell under ideal conditions. 

FUTURE PLANS:
1) Temperature and Humidity Sensing: Only reason these don't exist now is that I didn't have a sensor lying around. 
2) Dynamic Sleep Mode: Sleep mode is mapped to the average rate of change of quantity in question. 
