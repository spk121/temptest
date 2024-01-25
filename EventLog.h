#pragma once

#include <stdint.h>
// Smallest log format
// 16-bit timestamp in 1/2000 sec units
// 
// If the 3rd byte has the 0x80 bit set, it indicates that this is a
// short message with no 2nd event byte and no payload.
// The 3 bit eventId1 indicate the 8 short event IDs
// For a short message, the payload field represents
// and integer value between from 0 to 15
// 
// If the 1st byte does not have the 0x80 bit set
// - 4 bits payload is an enumerated type that describes the data field
// - 3 bits are the high bits for the Event ID
// And the 4th byte is 8 low bits for the Event ID
// 
// 0, 8, 16,or 32 bits additional payload

// The 4 bits of payload format enum are
// - no payload
// - boolean false
// - boolean true
// - <noop>
// - 1 char
// - U8
// - S8
// - <noop>
// - 2 chars
// - U16
// - S16
// - <noop>
// - 4 chars
// - U32
// - S32
// - F32

typedef struct EventLogMsg
{
	uint16_t timestamp;
	uint8_t  shortMsgFlag : 1;
	uint8_t  eventId1 : 3;
	uint8_t  payload : 4;
	uint8_t  eventId2;
	union {
		char c;
		uint8_t u8;
		int8_t s8;
		char c2[2];
		uint16_t u16;
		int16_t s16;
		char c4[4];
		uint32_t u32;
		int32_t s32;
		float f32;
	} data;
};
// When sent over Serial, use SLIP RFC 1055



// Base-64 friendly log format - group data into 18-bit blocks
// 3-
// 2-bit protocol
// 4-bit format
// 12-bit event id
//
// 18-bit timestamp
//
// 8-bit payload packed in 12-bits
// 16-bit payload packed in 18-bit
// or 32-bit payload in 36-bits
// CR

// The event log binary format

// Format alpha
// 
// [4-bit protocol version = 2 | 4-bit payload type ]   Starting at 2 to avoid accidentally creating ESC, STX, ETX
// 3-byte timestamp
// 1-byte origin
// 2-byte event
// ETX

// or
// STX
// [4-bit protocol version = 2 | 4-bit payload type ]   Starting at 2 to avoid accidentally creating ESC, STX, ETX
// 4-byte timestamp in NTP format
// 1-byte origin
// 2-byte event
// Payload of 0, 1, 2, or 4 bytes
// ETX

// If any of the body characters equals ESC, send ESC ESC
// If any of the body character equals STX or ETX, send ESC STX or ESC ETX

typedef struct EventPacked
{
	uint16_t  magic : 6;

	// A number from 0 to 0xFFFFF where 1 unit = 0.5 ms. Wraps at 524 seconds
	uint32_t  timestamp : 20;
	uint32_t  origin : 8;    // A 8-bit code indicating the source or the Event
	uint32_t  type : 12;     // A 12-bit unsigned integer indicating the event type
	uint32_t  bodyType : 4;   // a 4-bit enum describing the payload
	union {
		uint8_t  _bool;       // type 1: bool packed as uint8_t
		char     c;           // type 2: one ascii char
		uint8_t  u8;          // type 3: one 8-bit unsigned integer
		int8_t   s8;          // type 4: one 8-bit signed integer
		char     str2[2];     // type 5: two ascii chars
		uint16_t u16;         // type 6: one 16-bit unsigned integer
		int16_t  s16;         // type 7: one 16-bit signed integer
		char     str4[4];     // type 8: four ascii chars
		uint16_t u32;         // type 9: one unsigned 32-bit integer
		int16_t  s32;         // type 10: one signed 32-bit integer
		float    f32;         // type 11: one 32-bit float
	};


};

// 10-bit timestamp

// The line format of an entry is
// STX
// Time: Four bytes Base64 representation of an 18-bit unsigned timestamp
// Origin: 2 character CSC indicator
// Event: 2 character event indicator
// PayloadType: 1 character payload indicator
//  up to 6 bytes of payload
//  ETX

// The unpacked event, for easier access
struct EventEntry {
	// Header
	unt32_t time;        /// 1 unit = 0.5ms. Only 3 significant bytes
	EventOrigin origin;  /// A uint8_t expression of the EventOrigin enum
	EventType type;      /// A uint8_t expression of the EventType enum
	EventPayload payload;
	union 
};



#define EVENT_TX_BPS_MAX 115200     // bits per second on serial line
#define TX_BITS_PER_BYTE 10         // 8 bits + start/stop bit
#define EVENT_TX_FLUSH_INTERVAL 40  // data is flush to serial every 40ms
#define MILLISECONDS_PER_SECOND 1000
#define EVENT_MSG_LEN_MAX 6

// The number of bytes needed to enqueue that maximum
// amount of event messages we can handle at a given output
// rate.
// For example, at 115200 bps, we need ~460 to hold all the
// messages we can push out.
#define EVENT_BUF_SIZE (EVENT_MSG_LEN_MAX \
                        + (EVENT_TX_BPS_MAX * EVENT_TX_FLUSH_INTERVAL) \
                          * (TX_BITS_PER_BYTE * MILLISECONDS_PER_SECOND))


// An event is 13 bits.  The upper 5 bits are for the CSC ID
// and the remaining 8 bits are for the Event type
typedef enum EventCscId
{
	CSC_ANY,
	CSC_ONE
};

#define EVENT_ID(csc,id) ((((uint16_t)(csc))<<8) | ((uint16_t)(id)))
typedef enum EventId {
	GENERIC_TEST_START = EVENT_ID(CSC_ANY, 1),
	GENERIC_TEST_END = EVENT_ID(CSC_ANY, 1),
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
