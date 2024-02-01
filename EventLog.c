#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdbool.h>
#include "EventLog.h"


#define STX '['
#define ETX ']'
#define ESC 27
#define OFFSET 32

// In the worst case, every byte in the packet requires an escape
// character plus starting and ending framing characters. 
#define FRAME_SIZE_MAX (PACKET_SIZE * 2 + 2)

EventOutputFunc itsOutputFunc;
EventTimeGetterFunc itsTimeGetterFunc;

// Private
typedef enum EventPayload {
    PAYLOAD_NONE,
    PAYLOAD_BOOLEAN,
    PAYLOAD_INT8,
    PAYLOAD_UINT8,
    PAYLOAD_INT16,
    PAYLOAD_UINT16,
    PAYLOAD_INT32,
    PAYLOAD_UINT32,
    PAYLOAD_FLOAT,
    PAYLOAD_STRING
} EventPayload;

typedef struct Packet
{
    uint8_t version : 4;
    uint8_t level : 4;
    uint8_t format;
    uint8_t source;
    uint8_t type;

    uint32_t timestamp;

    union {
        uint16_t words[2];
        int8_t boolean;
        int8_t s8;
        uint8_t u8;
        int16_t s16;
        uint16_t u16;
        int32_t s32;
        uint32_t u32;
        float f32;
        char str[4];
    };
} Packet;

// bytes in an event packet
#define PACKET_SIZE sizeof(Packet)

static void setHeader(Packet *p, EventLevel level, EventSource source, EventType type)
{
    uint32_t timestamp = 0;

    if (itsTimeGetterFunc)
    {
        timestamp = itsTimeGetterFunc();
    }

    p->level = level;
    p->source = source;
    p->type = type;
    p->format = PAYLOAD_NONE;
    p->timestamp = timestamp;
}

static void sendPacket(const char* buf, int len)
{
    char frame[FRAME_SIZE_MAX] = { 0 };
    int n = 0;

    if (!itsOutputFunc)
    {
        return;
    }

    frame[0] = STX;
    n++;

    for (int i = 0; i < len; i ++)
    {
        // Escape certain control character that have special meaning
        // by replacing them with two-character sequences.
        switch(buf[i])
        {
        case ESC:
        case STX:
        case ETX:
            if (n + 2 < FRAME_SIZE_MAX - 1)
            {
                frame[n] = ESC;
                n++;
                frame[n] = buf[i] + OFFSET;
                n++;
            }
            break;
        default:
            if (n + 1 < FRAME_SIZE_MAX - 1)
            {
                frame[n] = buf[i];
                n++;
            }
            break;
        }
    }
    frame[n] = ETX;
    n++;

    itsOutputFunc(frame, n);
}

static EventData convertPacketToEvent(Packet* pkt)
{
    EventData event = { 0 };
    event.level = pkt->level;
    event.sourceID = pkt->source;
    event.eventID = pkt->type;
    event.timestamp = pkt->timestamp;
    switch (pkt->format)
    {
    case PAYLOAD_BOOLEAN:
        event.dataType = EVENT_DATA_BOOL;
        event.data.boolean = pkt->boolean;
        break;
    case PAYLOAD_INT8:
        event.dataType = EVENT_DATA_INT8;
        event.data.s8 = pkt->s8;
        break;
    case PAYLOAD_UINT8:
        event.dataType = EVENT_DATA_UINT8;
        event.data.u8 = pkt->u8;
        break;
    case PAYLOAD_INT16:
        event.dataType = EVENT_DATA_INT16;
        event.data.s16 = pkt->s16;
        break;
    case PAYLOAD_UINT16:
        event.dataType = EVENT_DATA_UINT16;
        event.data.u16 = pkt->u16;
        break;
    case PAYLOAD_INT32:
        event.dataType = EVENT_DATA_INT32;
        event.data.s32 = pkt->s32;
        break;
    case PAYLOAD_UINT32:
        event.dataType = EVENT_DATA_UINT32;
        event.data.u16 = pkt->u32;
        break;
    case PAYLOAD_FLOAT:
        event.dataType = EVENT_DATA_FLOAT;
        event.data.f32 = pkt->f32;
        break;
    case PAYLOAD_STRING:
        event.dataType = EVENT_DATA_STRING;
        event.data.str[0] = pkt->str[0];
        event.data.str[1] = pkt->str[1];
        event.data.str[2] = pkt->str[2];
        event.data.str[3] = pkt->str[3];
        break;
    case PAYLOAD_NONE:
    default:
        event.dataType = EVENT_DATA_NONE;
        event.data.u32 = 0;
        break;
    }
    return event;
}

EventData eventUnpackFrame(const char* frame, int size)
{
    char buf[sizeof(Packet)] = {0};
    bool startFound = false;
    bool endFound = false;
    bool escFound = false;
    bool errorFound = false;
    int packetPos = 0;
    int framePos = 0;

    while (framePos < size && !errorFound)
    {
        if (!startFound && frame[framePos] == STX)
        {
            startFound = true;
            framePos++;
        }
        else if (escFound)
        {
            escFound = false;
            if ((frame[framePos] != STX + OFFSET) &&
                (frame[framePos] != ETX + OFFSET) &&
                (frame[framePos] != ESC + OFFSET))
            {
                errorFound = true;
            }
            buf[packetPos] = frame[framePos] - OFFSET;
            packetPos++;
            framePos++;
        }
        else if (frame[framePos] == ETX)
        {
            endFound = true;
            framePos++;
            break;
        }
        else if (frame[framePos] == ESC)
        {
            escFound = true;
            framePos++;
        }
        else
        {
            buf[packetPos] = frame[framePos];
            packetPos++;
            framePos++;
        }
    }

    if (packetPos == sizeof(Packet) && startFound && endFound)
    {
        Packet pkt;
        EventData eventGood;
        // May need to byteswap here.
        memcpy((void*)&pkt, buf, sizeof(Packet));
        eventGood = convertPacketToEvent(&pkt);
        eventGood.frameSize = framePos;
        eventGood.valid = true;
        return eventGood;
    }
    else
    {
        EventData eventBad = { 0 };
        eventBad.frameSize = framePos;
        eventBad.valid = false;
        return eventBad;
    }
}

void eventSetOutputFunc(EventOutputFunc outputFunc)
{
    itsOutputFunc = outputFunc;
}

void eventSetTimeGetterFunc(EventTimeGetterFunc timeGetterFunc)
{
    itsTimeGetterFunc = timeGetterFunc;
}

void event(EventLevel level, EventSource source, EventType type)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    sendPacket((const char *) & p, sizeof(p));
}

void eventBool(EventLevel level, EventSource source, EventType type, bool val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_BOOLEAN;
    p.boolean = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventU8(EventLevel level, EventSource source, EventType type, uint8_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_UINT8;
    p.u8 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventS8(EventLevel level, EventSource source, EventType type, int8_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_INT8;
    p.s8 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventU16(EventLevel level, EventSource source, EventType type, uint16_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_UINT16;
    p.u16 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventS16(EventLevel level, EventSource source, EventType type, int16_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_INT16;
    p.s16 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventU32(EventLevel level, EventSource source, EventType type, uint32_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_UINT32;
    p.u32 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventS32(EventLevel level, EventSource source, EventType type, int32_t val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_INT32;
    p.s32 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventFloat(EventLevel level, EventSource source, EventType type, float val)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_FLOAT;
    p.f32 = val;
    sendPacket((const char*)&p, sizeof(p));
}

void eventStr(EventLevel level, EventSource source, EventType type, const char *str)
{
    Packet p = { 0 };

    setHeader(&p, level, source, type);
    p.format = PAYLOAD_STRING;
    if (str)
    {
        strncpy(p.str, str, 4);
    }
    else
    {
        p.str[0] = 0;
        p.str[1] = 0;
        p.str[2] = 0;
        p.str[3] = 0;
    }
    sendPacket((const char *) & p, sizeof(p));
}

