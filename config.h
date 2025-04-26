//****************************************************************************
// config.h
//
// Tobias Tangemann 2020
//
// Wolfgang Kutscherauer 2024-2025
// Anpassung an ETS SH20 / adopted to ETA SH20 wood heating
// Funktionen zum Auslesen von Konfigurations-Werten aus einer JSON Datei
// Functions to read config values from an JSON file
// Header-File
//
//****************************************************************************
#pragma once

#include <stdbool.h>

typedef struct {
    // serial device /dev/....
    char* device;
    // Pause in Sekunden zwischen den Abfragen
    int delay;
    // path to sqlite database
    const char* database;
    // true to store data in sqlite3 database
    bool withSql;
    // if true: print collected data to stdout
    bool print_result;
    // loglevel based on log.c logging library
    // 0: LOG_TRACE
    // 1: LOG_DEBUG
    // 2: LOG_INFO
    // 3: LOG_WARN
    // 4: LOG_ERROR
    // 5: LOG_FATAL
    int loglevel;
    // if true data will be sent to MQTT server
    bool mqtt_enabled;
    // user for mqtt server
    const char* mqtt_user;
    // password, clear text
    const char* mqtt_password;
    //fqdn or ip v4 of the mqtt server
    const char* mqtt_server;
    // base string for the sensor topics
    const char* mqtt_sensor_base;
    //base string for the actor base;
    const char* mqtt_actor_base;
    //ID-string to identify the client
    const char* mqtt_client_id;
    // true to enable homassitant suppor
    // alwas false as i have no homeassistant
    bool homeassistant_enabled;
    const char* homeassistant_entity_id_base;
} CONFIG;

// function to read values from the JSON file
int parseConfig(const char* file, CONFIG* cfg);
