#pragma once

#include <stdint.h>
// The event log 

#define EVENT_TX_BPS_MAX 115200     // bits per second on serial line
#define TX_BITS_PER_BYTE 10         // 8 bits + start/stop bit
#define EVENT_TX_FLUSH_INTERVAL 40  // data is flush to serial every 40ms
#define MILLISECONDS_PER_SECOND 1000
#define EVENT_MSG_LEN_MAX 6

#define EVENT_BUF_SIZE (EVENT_MSG_LEN_MAX
                        + (EVENT_TX_BPS_MAX * EVENT_TX_FLUSH_INTERVAL)
                          * (TX_BITS_PER_BYTE * MILLISECONDS_PER_SECOND))

// At 115200, a 1K buffer can fill in 80 ms.
// So we won't overflow if we flush in the 40ms task.
#define EVENT_FLUSH_RATE (EVENT_TX_RATE / TX_BITS_PER_BYTE)

typedef enum EventType {

};

typedef enum EventPayloadFormat {
	EVENT_PAYLOAD_NONE,
	EVENT_PAYLOAD_S16,
	EVENT_PAYLOAD_U16,
	EVENT_PAYLOAD_S32,
	EVENT_PAYLOAD_U32,
	EVENT_PAYLOAD_F32,
	EVENT_PAYLOAD_TWO_CHARS,
	EVENT_PAYLOAD_FOUR_CHARS
};

// Timestamp is in 5ms units.
// Timestamp rolls over. Roll over occurs every
// 327.68 seconds = 5 min 27.68 seconds
typedef struct Event {
	uint16_t timestamp;
	uint16_t type:13;
	uint16_t payloadFormat : 3;
	union
	{
		int16_t  i16;
		uint16_t u16;
		int32_t  i32;
		uint32_t u32;
		float    f32;
		char     c16[2];
		char     c32[4];
	} payload;
};

#define EVENT(type) \
  sendEvent(theTimestamp, (type), EVENT_PAYLOAD_NONE)

#define EVENT_S16(type, val) \
  sendEventS16(theTimestamp, (type), (val))

#define EVENT_U16(type, val) \
  sendEventU16(theTimestamp, (type), (val))

#define EVENT_S32(type, val) \
  sendEventS32(theTimestamp, (type), (val))

#define EVENT_U32(type, val) \
  sendEventU32(theTimestamp, (type), (val))

#define EVENT_F32(type, val) \
  sendEventF32(theTimestamp, (type), (val))

#define EVENT_C16(type, val) \
  sendEventC16(theTimestamp, (type), (val))

#define EVENT_C32(type, val) \
  sendEventC32(theTimestamp, (type), (val))
