//****************************************************************************
// mqtt.c
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2025 logging eingbaut.
// 
// function to publish data to MQTT server
// implementation
//
//****************************************************************************
#include "mqtt.h"
#include "log.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <MQTTClient.h>


#define QOS         1
#define TIMEOUT     3000L
#define WILL_TOPIC  "/status"

MQTTClient client;
bool already_connected = false;

char* last_will_topic = NULL;
const char* mqtt_sensor_base = NULL;
//const char* mqtt_actor_base = NULL;

int reconnect_mqtt(const CONFIG* cfg)
{
    if (already_connected)
    {
        return true;
    }

    mqtt_sensor_base = cfg->mqtt_sensor_base;

    if (last_will_topic == NULL)
    {
        last_will_topic = malloc(strlen(mqtt_sensor_base) + strlen(WILL_TOPIC) + 1);
        sprintf(last_will_topic, "%s%s", mqtt_sensor_base, WILL_TOPIC);
    }

    MQTTClient_willOptions last_will = MQTTClient_willOptions_initializer;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, cfg->mqtt_server, cfg->mqtt_client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    last_will.topicName = last_will_topic;
    last_will.retained = 1;
    last_will.message = "0";

    conn_opts.will = &last_will;
    conn_opts.keepAliveInterval = 600;
    conn_opts.cleansession = 1;
    conn_opts.connectTimeout = TIMEOUT;

    if (cfg->mqtt_user != NULL)
    {
        conn_opts.username = cfg->mqtt_user;
    }

    if (cfg->mqtt_password != NULL)
    {
        conn_opts.password = cfg->mqtt_password;
    }

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        log_error("Failed to connect, return code %d", rc);
        return false;
    }

    publish_str(last_will_topic, "1");

    already_connected = true;
    return true;
}

void disconnect_mqtt()
{
    log_trace("start disconnect_mqtt ....", "");
    already_connected = false;
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);

    free(last_will_topic);
}

void publish_str(const char *topic, const char *payload)
{
    log_trace("start: publish_str(const char *topic, const char *payload) ....", "");
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (char*)payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 1;

    char* fullTopic = malloc(strlen(mqtt_sensor_base) + 1 + strlen(topic) + 1);
    strcpy(fullTopic, mqtt_sensor_base);
    strcat(fullTopic, "/");
    strcat(fullTopic, topic);
    printf("fullTopic: %s\n", fullTopic);

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, fullTopic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);

    free(fullTopic);
    log_trace("publish_str %s %s", topic, payload);
}

void publish_int(const char *topic, int payload)
{
    char buffer[32];
    snprintf(buffer, 32, "%d", payload);
    publish_str(topic, buffer);
    log_trace("publish_int %s %s", topic, buffer);
}

void publish_double(const char *topic, double payload, const char *format)
{
    char buffer[32];
    snprintf(buffer, 32, format, payload);
    publish_str(topic, buffer);
    log_trace("publish_double %s %s", topic, buffer);
}
