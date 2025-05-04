//****************************************************************************
// config.c
// Funktionen zum Auslesen von Konfigurations-Werten aus JSON Datei
// Functions to read values from a JSON file.
// Implementation
//
//****************************************************************************
#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _SVID_SOURCE
#include <string.h>

#include <cjson/cJSON.h>
#include "log.h"

int getParameter(cJSON* json, CONFIG* cfg);
int getMqttParameter(const cJSON* mqtt, CONFIG* cfg);

int parseConfig(const char* file, CONFIG* cfg)
{
    int status = 0;
    FILE *fp = fopen(file, "rt");
    log_trace("parseConfig: , file %s", file);
    if (fp == NULL)
    {
        printf("Error opening config file: %s\n", file);
        log_fatal("fatal error opening config file: %s -->stopping", file);
        return 8;
    }

    fseek (fp, 0, SEEK_END);
    int fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *config = malloc(fileLength + 1);

    fread(config, 1, fileLength, fp);
    config[fileLength] = '\0';
    fclose(fp);

    cJSON *json = cJSON_Parse(config);

    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s, returncode 9", error_ptr);
            log_fatal("Error before: %s", error_ptr);
        }

        status = 9;
        goto end;
    }

    status = getParameter(json, cfg);
    if (status != 0) {
        goto end;
    }

    const cJSON* mqtt = cJSON_GetObjectItem(json, "mqtt");
    if (mqtt == NULL || !cJSON_IsObject(mqtt)) {

        printf("Invalid value for mqtt\n");
        log_error("Invalid value for mqtt%s ", "returncode 11");
        status = 11;
        goto end;
    }

    status = getMqttParameter(mqtt, cfg);
    if (status != 0) {
        goto end;
    }


end:
    cJSON_Delete(json);
    free(config);
    return status;
}

int getParameter(cJSON* json, CONFIG* cfg)
{
    cJSON *value;
    log_trace("Started function config.c: ", "getParameter");
    // Serial port
    value = cJSON_GetObjectItem(json, "device");
    log_trace("json device = %s|",value);
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for device\n");
        log_error("Invalid value for device, ", "returncode 11");
        return 10;
    }
    log_info("Read JSON value serial device: ",value->valuestring);
    cfg->device = strdup(value->valuestring);


    // Database path
    log_trace("config.c start reading ","database path");
    value = cJSON_GetObjectItem(json, "database");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            printf("Invalid value for database\n");
            log_error("Invalid value for database ", "returncode 10");
            return 10;
        }
        log_info("Read JSON value for database: ", value->valuestring);
        cfg->database = strdup(value->valuestring);
    }

    // Interval
    log_trace("config.c start reading ","delay");
    value = cJSON_GetObjectItem(json, "delay");
    if (value == NULL || !cJSON_IsNumber(value))
    {
        printf("Invalid value for interval\n");
        log_error("Invalid value for interval, ", "returncode 10");
        return 10;
    }
    log_info("Read JSON value for interval: %d seconds.", value->valueint);
    cfg->delay = value->valueint;

    // read loglevel
    log_trace("config.c start reading ","loglevel");
    value = cJSON_GetObjectItem(json, "loglevel");
    if (value == NULL || !cJSON_IsNumber(value))
    {
        printf("Invalid value for loglevel\n");
        log_error("Invalid value for logelvel, ", "returncode 10");
        return 10;
    }
    log_info("Read loglevel from JSON file: %d", value->valueint);
    cfg->loglevel = value->valueint != 0;
    log_set_level(value->valueint);

    // Print to stdout
    log_trace("config.c start reading: ","Print to stdout");
    value = cJSON_GetObjectItem(json, "print_stdout");
    if (value == NULL || !cJSON_IsBool(value))
    {
        printf("Invalid value for print_stdout\n");
        log_error("Invalid value for print_stdout, ", "returncode 10");
        return 10;
    }

    cfg->print_result = value->valueint != 0;

    return 0;
}

int getMqttParameter(const cJSON* mqtt, CONFIG* cfg)
{
    cJSON *value;
    log_trace("config.c start funtion: ","getMqttParameter");
    // Enabled
    log_trace("config.c start reading: ","mqtt enabled");
    value = cJSON_GetObjectItem(mqtt, "enabled");
    if (value == NULL || !cJSON_IsBool(value))
    {
        printf("Invalid value for mqtt.enabled\n");
        log_error("Invalid value for mqtt.enabled %s", "returncode 10");
        return 10;
    }

    cfg->mqtt_enabled = value->valueint != 0;
    if (cfg->mqtt_enabled == 0) {
        log_trace("config.c read mqtt_enabled = %d", value->valueint);
        return 0;
    }

    // MQTT server
    log_trace("config.c start reading: %s ","mqtt server");
    value = cJSON_GetObjectItem(mqtt, "server");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.server\n");
        log_error("config.c Invalid value for: mqtt.server", "returncode 10");
        return 10;
    }

    cfg->mqtt_server = strdup(value->valuestring);
    log_trace("config.c read mqtt.server: %s", value->valuestring);

    // MQTT client_id
    log_trace("config.c start reading: %s","MQTT client_id");
    value = cJSON_GetObjectItem(mqtt, "client_id");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.client_id\n");
        return 10;
    }

    cfg->mqtt_client_id = strdup(value->valuestring);

    // MQTT user
    log_trace("config.c start reading: ","MQTT user");
    value = cJSON_GetObjectItem(mqtt, "user");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            printf("Invalid value for mqtt.user\n");
            return 10;
        }

        cfg->mqtt_user = strdup(value->valuestring);
    }

    // MQTT password
    log_trace("config.c start reading: ","MQTT password");
    value = cJSON_GetObjectItem(mqtt, "password");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            printf("Invalid value for mqtt.password\n");
            return 10;
        }

        cfg->mqtt_password = strdup(value->valuestring);
    }

    // MQTT sensor_base
    log_trace("config.c start reading: ","MQTT sensor_base");
    value = cJSON_GetObjectItem(mqtt, "sensor_base");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.sensor_base\n");
        return 10;
    }

    cfg->mqtt_sensor_base = strdup(value->valuestring);

        // MQTT actor_base
    log_trace("config.c start reading: ","MQTT actor_base");
    value = cJSON_GetObjectItem(mqtt, "actor_base");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.actor_base\n");
        return 10;
    }

    cfg->mqtt_actor_base = strdup(value->valuestring);
    return 0;
}

