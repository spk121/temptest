#include <string.h>
#include <stdbool.h>
#include "EventLog.h"


#define STX 2
#define ETX 3
#define ESC 27
#define OFFSET 32

// bytes in an event packet
#define PACKET_SIZE 10

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
    uint16_t version : 2;
    uint16_t level : 2;
    uint16_t format : 4;
    uint16_t source : 8;

    uint16_t type : 8;
    uint16_t timestampHigh : 8;

    uint16_t timestampLow;

    union {
        uint16_t words[2];
        bool boolean;
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
    p->timestampHigh = (timestamp & 0x00FF0000) >> 16;
    p->timestampLow = (timestamp & 0xFFFF);
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

static bool unpackFrame(const char* frame, int size, Packet* pkt, int *bytesRead)
{
    char buf[PACKET_SIZE] = { 0 };
    bool startFound = false;
    bool endFound = false;
    bool escFound = false;
    bool errorFound = false;
    int packetPos = 0;
    int framePos = 0;

    while (framePos < size && packetPos < PACKET_SIZE && !errorFound)
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
                errorFound = true;;
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

    *bytesRead = framePos;

    if (packetPos == PACKET_SIZE && startFound && endFound)
    {
        // May need to byteswap here.
        memcpy((void*)pkt, buf, PACKET_SIZE);
        return true;
    }

    return false;
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

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventBool(EventLevel level, EventSource source, EventType type, bool val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.boolean = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventU8(EventLevel level, EventSource source, EventType type, uint8_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.u8 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventS8(EventLevel level, EventSource source, EventType type, int8_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.s8 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventU16(EventLevel level, EventSource source, EventType type, uint16_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.u16 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventS16(EventLevel level, EventSource source, EventType type, int16_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.s16 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventU32(EventLevel level, EventSource source, EventType type, uint32_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.u32 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventS32(EventLevel level, EventSource source, EventType type, int32_t val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.s32 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventFloat(EventLevel level, EventSource source, EventType type, float val)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        p.f32 = val;
        itsOutputFunc(&p, sizeof(p));
    }
}

void eventStr(EventLevel level, EventSource source, EventType type, const char *str)
{
    Packet p = { 0 };

    if (itsOutputFunc)
    {
        setHeader(&p, level, source, type);
        if (str)
        {
            strncpy(p.str, str, 4);
        }
        itsOutputFunc(&p, sizeof(p));
    }
}
