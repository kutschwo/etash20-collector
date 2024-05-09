//****************************************************************************
// config.h
//
// Tobias Tangemann 2020
//
// Wolfgang Kutscherauer 2024
// Anpassung an ETS SH20
// Funktionen zum Auslesen von Konfigurations-Werten aus .json Datei
// Header-File
//
//****************************************************************************
#pragma once

#include <stdbool.h>

typedef struct {
    char *device;  // serial device /dev/....
    int delay;     // Pause in Sekungen zwischen den Abfragen

    const char* database;
    bool withSql;

    bool print_result;
    bool verbose;

    bool mqtt_enabled;
    const char* mqtt_user;
    const char* mqtt_password;
    const char* mqtt_server;
    const char* mqtt_base_topic;
    const char* mqtt_client_id;

    bool homeassistant_enabled;
    const char* homeassistant_entity_id_base;
} CONFIG;

int parseConfig(const char* file, CONFIG* cfg);
