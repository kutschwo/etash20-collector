
//****************************************************************************
// serial.h
//
// (c) Hewell Technology Ltd. 2014
//
//****************************************************************************

#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#pragma once

extern bool serial_open_port(const char *port);
extern bool serial_set_baud_rate(int rate);
extern bool serial_close_port(void);
extern ssize_t serial_write(const void *buf, size_t count);
extern ssize_t serial_read(void *buf, size_t count);
extern const char *serial_get_error(void);
extern void debug_comment(const char *comment, ...);

