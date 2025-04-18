"# etash20-collector"

## Credits
Diese Projekt ist abgeleitet vom vbus-collector der Hewell Technology Ltd. 2014 und Tobias Tangemann 2020

## Untersützte Hardware:
Scheitholzkessel ETA SH20 aus dem Baujahren ca. 2006 bist ca. 2012
Es werden die Modelle unterstützt die eine vierzeilige LCD-Anzeige + RS232 Schnittstelle haben.
Die Doku des Datenformates gibt es bei Fa. ETA in Österreich auf Anfrage.

Seit ca. 2012 gibt es eine neue Elektronik, die eine Ethernet-Schnittstelle mit bringt.


## Features
* Sammelt die Daten eines ETA SH20 (anderes Scheitholzkessel sollten auch funktionieren).
* Sendet die Daten an einen MQTT Broker

## HowTo build

Installiere die notwendigen Pakete (raspbian, Debian)
```shell 
$ apt update
$ apt install git build-essential cmake libsqlite3-dev sqlite3 libpaho-mqtt-dev libcjson-dev libcurl4-gnutls-dev
```

Andere Distributionen entsprechend Doku.

Downloade Source Code
```shell 
$ cd ~
$ git clone https://github.com/kutschwo/etash20-collector.git
```

## Kompilieren
```shell
$ $ cd to you build dir
$ make
$ make install
```

## Service installieren

to be done 


## Konfigurations-Datei

to be done  
