#ifndef MQTT_H_
#define MQTT_H_

typedef void (*mqtt_handle_data_t)(char *data, int len);

void MQTT_init(void);
void MQTT_appStart(void);
void MQTT_setCallback(void* callback);
void MQTT_subscribe(char *topic);
void MQTT_publish(char *topic, char *data, int len);

#endif /* MQTT_H_*/
