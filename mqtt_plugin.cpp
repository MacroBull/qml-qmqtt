#include "mqtt_plugin.h"
#include "MqttClient.h"

#include <qqml.h>

void MqttPlugin::registerTypes(const char *uri)
{
	// @uri SerialPort
	qmlRegisterType<MqttClient>(uri, 0, 1, "MqttClient");
}


