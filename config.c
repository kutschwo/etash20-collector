//****************************************************************************
// config.c
//
// Tobias Tangemann 2020
// Wolfgang Kutscherauer 2025 adopted to ETA sh20 wood heating
// 
// Funktionen zum Auslesen von Konfigurations-Werten aus .json Datei
// Implementation
//
//****************************************************************************
#include "config.h"
#include "log.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _SVID_SOURCE
#include <string.h>

#include <cjson/cJSON.h>

// define function to get common parameter
int getParameter(cJSON* json, CONFIG* cfg);
// define function for mqtt parameter
int getMqttParameter(const cJSON* mqtt, CONFIG* cfg);

int parseConfig(const char* file, CONFIG* cfg)
{
    int status = 0;
    //open json-file 
    FILE *fp = fopen(file, "rt");
    if (fp == NULL)
    {
        log_fatal("Error opening config file: %s", file);
        //printf("Error opening config file: %s\n", file);
        return 8;
    }

    //read the whole file into a char array.
    fseek (fp, 0, SEEK_END);
    int fileLength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *config = malloc(fileLength + 1);

    fread(config, 1, fileLength, fp);
    config[fileLength] = '\0';
    fclose(fp);

    //parse config test and get a pointer to the parsed configuration
    cJSON *json = cJSON_Parse(config);

    //Pointer will be NULL on error.
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            log_fatal("Error before: %s", error_ptr);
        }

        status = 9;
        goto end;
    }

    // call private function that read the known parameter
    status = getParameter(json, cfg);
    if (status != 0) {
        log_error("Error in getParameter, status-code: %d",status);
        goto end;
    }

    //get parameters of group mqtt
    const cJSON* mqtt = cJSON_GetObjectItem(json, "mqtt");
    if (mqtt == NULL || !cJSON_IsObject(mqtt)) {

        printf("Invalid value for mqtt\n");
        log_error("Invalid value for mqtt");
        status = 11;
        goto end;
    }
    // read mqtt parameter with privat function
    status = getMqttParameter(mqtt, cfg);
    if (status != 0) {
        log_error("Error in getMqttParameter, status-code: %d",status);
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

    // Serial port, String
    value = cJSON_GetObjectItem(json, "device");
    if (value == NULL || !cJSON_IsString(value))
    {
        log_error("Invalid value for device");
        return 10;
    }
    log_trace("cfg->device: %s", value->valuestring);
    cfg->device = strdup(value->valuestring);

    // Interval between checks, Number
    value = cJSON_GetObjectItem(json, "delay");
    if (value == NULL || !cJSON_IsNumber(value))
    {
        printf("Invalid value for interval\n");
        log_error("Invalid value for interval");
        return 10;
    }
    cfg->delay = value->valueint;
    log_trace("cfg->delay: %d",cfg->delay);
    
        // loglevel , Number
    value = cJSON_GetObjectItem(json, "loglevel");
    if (value == NULL || !cJSON_IsNumber(value))
    {
        printf("Invalid value for loglevel\n");
        log_error("Invalid value for loglevel");
        return 10;
    }
    cfg->loglevel = value->valueint;
    log_trace("cfg->loglevel: %d",cfg->loglevel);
    
    
    // Database path
    value = cJSON_GetObjectItem(json, "database");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            printf("Invalid value for database\n");
            log_error("Invalid value for database\n");
            return 10;
        }

        cfg->database = strdup(value->valuestring);
        log_trace("cfg->database: %s",cfg->database);
    }

    // Use sqlite, bool
    value = cJSON_GetObjectItem(json, "withSql");
    if (value == NULL || !cJSON_IsBool(value))
    {
        printf("Invalid value for withSql\n");
        log_fatal("Invalid value for withSql");
        return 10;
    }
    log_trace("testmode: %d", value->valueint);
    cfg->withSql = value->valueint != 0;

    // Print results to stdout, bool
    value = cJSON_GetObjectItem(json, "print_stdout");
    if (value == NULL || !cJSON_IsBool(value))
    {
        printf("Invalid value for print_stdout\n");
        log_fatal("Invalid value for print_stdout");
        return 10;
    }
    cfg->print_result = value->valueint != 0;
    log_trace("cfg->print_result: %d",cfg->print_result);
    return 0;


    
} // END int getParameter(cJSON* json, CONFIG* cfg)
    




int getMqttParameter(const cJSON* mqtt, CONFIG* cfg)
{
    cJSON *value;

    // Enabled
    value = cJSON_GetObjectItem(mqtt, "enabled");
    if (value == NULL || !cJSON_IsBool(value))
    {
        printf("Invalid value for mqtt.enabled\n");
        log_error("Invalid value for mqtt.enabled.");
        return 10;
    }
    cfg->mqtt_enabled = value->valueint != 0;
    log_trace("cfg->mqtt_enabled: %d",cfg->mqtt_enabled);
    // if enabled = false --> stop reading config as no mqtt will be used.
    if (cfg->mqtt_enabled == 0) {
        return 0;
    }
    
    // MQTT sensor_base
    value = cJSON_GetObjectItem(mqtt, "sensor_base");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.mqtt_sensor_base \n");
        log_error("Invalid value for mqtt.mqtt_sensor_base.");
        return 10;
    }
    cfg->mqtt_sensor_base = strdup(value->valuestring);
    log_trace("cfg->mqtt_sensor_base: %s",cfg->mqtt_sensor_base);
// END MQTT sensor_base
    
    
    // MQTT actor_base
    value = cJSON_GetObjectItem(mqtt, "actor_base");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.actor_base \n");
        log_error("Invalid value for mqtt.actor_base.");
        return 10;
    }
    cfg->mqtt_actor_base = strdup(value->valuestring);
    log_trace("cfg->mqtt_actor_base: %s",cfg->mqtt_sensor_base);
// END MQTT actor_base

    // MQTT server
    value = cJSON_GetObjectItem(mqtt, "server");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.server\n");
        log_error("Invalid value for mqtt.server");
        return 10;
    }

    cfg->mqtt_server = strdup(value->valuestring);
    log_trace("cfg->mqtt_server: %s",cfg->mqtt_server );

    // MQTT client_id
    value = cJSON_GetObjectItem(mqtt, "client_id");
    if (value == NULL || !cJSON_IsString(value))
    {
        printf("Invalid value for mqtt.client_id\n");
        log_error("Invalid value for mqtt.client_id");
        return 10;
    }

    cfg->mqtt_client_id = strdup(value->valuestring);
    log_trace("mqtt_client_id: %s", cfg->mqtt_client_id);

    // MQTT user
    value = cJSON_GetObjectItem(mqtt, "user");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            printf("Invalid value for mqtt.user\n");
            log_error("Invalid value for mqtt.user");
            return 10;
        }

        cfg->mqtt_user = strdup(value->valuestring);
        log_trace("cfg->mqtt_user: %s", cfg->mqtt_user);
    }

    // MQTT password
    value = cJSON_GetObjectItem(mqtt, "password");
    if (value != NULL && value->valuestring != NULL) {
        if (!cJSON_IsString(value))
        {
            log_error("Invalid value for mqtt.password");
            return 10;
        }

        cfg->mqtt_password = strdup(value->valuestring);
        log_trace("cfg->mqtt_password: %s", cfg->mqtt_password);
    }


    return 0;
} // int getMqttParameter(const cJSON* mqtt, CONFIG* cfg)


