#pragma once

#include <stdint.h>
// The EventLog CSU writes debugging output in a structured format to
// an output port.  Unlike the freeform prints of the debug log output,
// the EventLog has one specific format focussed on reporting when
// and which order specific parts of code are executed.
// 
// Since the expected output is a serial port, the events need to be
// kept short to allow for many events to be reported every second. 
//
// Assume we're using a serial port at 115,200 bps for output
// - 115,200 bits per second
// Assume 10 bits per byte and a 50% derating.
// - 115,200 bps -> 5,760 bytes per second.
//
// The event message full format:
// - Word 1
//   - 2-bit version: The version number of this log format. Always = 0
//   - 2-bit level: indicates INFO, WARNING, or ERROR
//   - 4-bit format indicator: an enumerated type describes the length and format of the message
//   - 8-bit source: enumerated type that indicates the module that originates the message
// - Word 2
//   - 16-bit Timestamp MSB: Upper 16 bits of 24-bit number. 1 unit = 1 microsecond.
//            Rolls over a 14,400,000 units = 4 hours
// - Word 3
//   - 8-bit Timestamp LSB: Lower 8 bits of 24-bit number. 1 unit = 1 microsecond
//   - 8-bit event ID: enumerated type that indicates the type of event that occurred
// - Word 4 [Optional, depends on format indicator]
//   - 16-bit payload
// - Word 5 [Optional, depents on format indicator]
//   - 16-bit payload
// 
// For framing, we'll use SLIP (RFC 1055) usually adds one byte to the message.
// 
// Thus message size is usually 7--11 bytes bytes, this allows about 800 messages per second.
//
// Events include
// - state and mode changes
// - every message sent and received
// - CPU usage and Task usage

// Public
typedef enum EventLevel {
	EVENT_INFO,
	EVENT_WARNING,
	EVENT_ERROR
} EventLevel;

// Public
typedef enum EventSource {
	EVENT_SOURCE_UNSPECIFIED,
	EVENT_SOURCE_MAIN,    // The literal "main"
	EVENT_SOURCE_1,
	EVENT_SOURCE_2,
	EVENT_SOURCE_3,
	EVENT_SOURCE_4,
	EVENT_SOURCE_5,
	EVENT_SOURCE_6,
	EVENT_SOURCE_7,
	EVENT_SOURCE_8,
	EVENT_SOURCE_9,
	EVENT_SOURCE_10,
	EVENT_SOURCE_11,
	EVENT_SOURCE_12
} EventSource;

typedef enum EventType {
	EVENT_GENERIC,
	EVENT_VERSION,     // Returns a version number 
	EVENT_INIT,        // For subsystems, this is sent once at launch
	EVENT_FINI,        // For subststems, this is sent once at shutdown
	EVENT_START,       // For semaphore-guarded tasks, this is sent at the beginning of an iteration
	EVENT_STOP,        // For semaphore-guarded taskss, this is sent at the end of an iteration
	EVENT_SEND,        // Sending data. The payload should describe the message or data being sent
	EVENT_RECEIVE,     // Receiving data. The payload should describe the message or data received
	EVENT_NEW_STATE,   // Sent at a state change. The payload should describe the new state
	EVENT_7,
	EVENT_8,
} EventType;

// Functions of this type should take strings and the associated
// lengths and write them to an output sink.
typedef void (*EventOutputFunc)(int len, const char* buf);

// Functions of this type should return the current time in milliseconds
// since the start of the program.
typedef uint32_t(*EventTimeGetterFunc)(void);

void event(EventLevel level, EventSource source, EventType type);
void eventBool(EventLevel level, EventSource source, EventType type, bool val);
void eventU8(EventLevel level, EventSource source, EventType type, uint8_t val);
void eventS8(EventLevel level, EventSource source, EventType type, int8_t val);
void eventU16(EventLevel level, EventSource source, EventType type, uint16_t val);
void eventS16(EventLevel level, EventSource source, EventType type, int16_t val);
void eventU32(EventLevel level, EventSource source, EventType type, uint32_t val);
void eventS32(EventLevel level, EventSource source, EventType type, int32_t val);
void eventFloat(EventLevel level, EventSource source, EventType type, float val);
void eventStr(EventLevel level, EventSource source, EventType type, const char *str);

// For example,
// eventU16(EVENT_INFO, EVENT_SOURCE_177_MANAGER, EVENT_SEND, 7777);
// eventFloat(EVENT_WARNING, EVENT_SOURCE_MAIN_LOOP, EVENT_CPU_TOO_HIGH, cpuPercentage);

// Used to assign the function that EventLog will use to send message data.
void eventSetOutputFunc(EventOutputFunc outputFunc);

// Assigns the function EventLog will use to get the current program time in 1ms units.
void eventSetTimeGetterFunc(EventTimeGetterFunc timeGetterFunc);
