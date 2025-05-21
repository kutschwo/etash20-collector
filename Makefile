############
# Makefile #
############

# C source files
##SOURCES = log.c config.c  etash20.c  kbhit.c   mqtt.c  serial.c  sqlite.c sh20prnfuncs.c main.cc
SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
SOURCES =  etash20.cpp config.cpp  main.cpp
# Optimization
OPT = -O3 -flto

# For Debugging
#OPT = -g

TARGET = etash20-collector

#===================================================================================

GIT_VERSION := "$(shell git describe --long --always --tags)"

CC = g++
CFLAGS =  $(OPT) -c -Wall -flto=auto -DGIT_VERSION=\"$(GIT_VERSION)\" -DLOG_USE_COLOR 

# uncomment if you want to store values to
# a sqlite3 database
CFLAGS += -D__SQLITE__

#LDFLAGS = -LcJSON/build/ -Lpaho.mqtt.c/build/src/ $(OPT) -fuse-linker-plugin -lcurl -lsqlite3 -lpaho-mqtt3c -lcjson -lpthread -lm
LDFLAGS =  -lm -ljsoncpp -lserial -lspdlog -lfmt
#-l:libpaho-mqtt3c.a 
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

REMOVE    = rm -f
REMOVEDIR = rm -rf
CREATEDIR = mkdir -p
GET_VERSION = git describe --tags --long

# Object files directory
OBJDIR = obj

#===================================================================================

all: createdirs $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

createdirs:
	@$(CREATEDIR) $(OBJDIR)

clean:
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVE) $(TARGET)

install:
	@$(CREATEDIR) /var/log/$(TARGET)
	@$(CREATEDIR) $(INST_DIR)/share/sofliq-collector
	cp ./$(TARGET) $(INST_DIR)/bin/$(TARGET)
	cp ./$(TARGET).json $(INST_DIR)/etc/$(TARGET).json
	cp ./$(TARGET).service  $(INST_DIR)/bin/$(TARGET).service


