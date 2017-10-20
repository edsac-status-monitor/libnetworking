/*
 * Copyright 2017
 * MIT Licence
 * representation.h
 * Functions relating to the wire representation of data
 */

#ifndef REPRES_H
#define REPRES_H

// link properly with C++
#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

// includes
#include<stdbool.h>
#include<sys/types.h> // ssize_t size_t 

// declarations

// the type of message sent
typedef enum {
    HARD_ERROR, // the monitor found a hardware error
    SOFT_ERROR, // the monitor encountered a software error
    INVALID,    // invalid message type
} MessageType;

// hardware error message
typedef struct {
    int valve_no; // the number of the valve expected to be broken
    int test_point_no; // the number of the test point which was wrong
    bool test_point_high; // the logic value of the test point
} HardErrorData;

// software error message
#define MAX_MSG_LEN 200 // maximum allowable length for SoftErrorData.message
typedef struct {
    char *message; // message to be reported to the UI
} SoftErrorData;

// combined representation of the data sections
typedef union {
    HardErrorData hardware;
    SoftErrorData software;
} MessageData;

// representation of the full message
typedef struct {
    MessageType type;
    MessageData data;
} ErrorMessage;

// function to initialise a hardware error structure
void hardware_error(ErrorMessage *message, int valve_no, int test_point_no, bool test_point_high);

// function to initialise a software error structure
void software_error(ErrorMessage *message, const char *string);

// function to encode a message. Dynamically allocates storage
// returns the size of the encoded message or -1 on error
ssize_t encode_message(const ErrorMessage *message, char **encoded_message);

// function to decode a message. 
// Returns success or failure
bool decode_message(const char* encoded_message, ErrorMessage *message);

// internals
#define DATA_FORMAT_VERSION 1.0
#define MAX_ENCODED_LEN ((MAX_MSG_LEN) + 100) // approximate

#ifdef _cplusplus
}
#endif // _cplusplus
#endif // REPRES_H