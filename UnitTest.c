/*
* @file UnitTest.c
* 
*/

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "UnitTest.h"

#define ENTRIES_COUNT 120

static int32_t itsPassCount;
static int32_t itsFailCount;
static UnitTestOutput itsOutputStore[ENTRIES_COUNT];
static int16_t itsEntryHead;
static int16_t itsEntryTail;
static HANDLE itsMtx;

// When true, add passing entries to table
static bool itsVerbose;

// Initialize the mutex that guards this module
static bool initMutex(void);

static void addEntry(UnitTestResult result, const char* name, int line, const char* msg);

// Creates a mutex.  On success it returns true. Otherwise false.
static bool initMutex(void)
{
	itsMtx = CreateMutex(NULL, FALSE, NULL);  // Initially unowned
	if (!itsMtx)
	{
		printf("CreateMutex error: %d\n", GetLastError());
        return false;
	}
    return true;
}

// If there is room in the output store, this adds an entry to the output store.
// If the output store is full, it toggles the overflow flag in the most recently
// added message.

static void addEntry(UnitTestResult result, const char* name, int line, const char* msg)
{
    DWORD waitResult;

    // Since integers are atomic, this doesn't need a mutex
    if (result == UNIT_TEST_PASS)
    {
        itsPassCount++;
    }
    else if (result == UNIT_TEST_FAIL)
    {
        itsFailCount++;
    }
    else
    {
        ;  // Shouldn't happen
    }

    if (!itsMtx)
    {
        initMutex();
    }

    if (itsMtx)
    {
        waitResult = WaitForSingleObject(itsMtx, INFINITE);
        if (waitResult == WAIT_OBJECT_0)
        {
            if (((itsEntryTail + 1) % ENTRIES_COUNT) == itsEntryHead)
            {
                itsOutputStore[itsEntryTail].overflow = true;
            }
            else
            {
                itsOutputStore[itsEntryTail].overflow = false;
                itsOutputStore[itsEntryTail].result = result;
                itsOutputStore[itsEntryTail].line = line;
                strncpy(itsOutputStore[itsEntryTail].name, name, UNIT_TEST_FUNC_NAME_LEN);
                strncpy(itsOutputStore[itsEntryTail].msg, msg, UNIT_TEST_MESSAGE_LEN);
                itsEntryTail++;
                if (itsEntryTail >= ENTRIES_COUNT)
                {
                    itsEntryTail = 0;
                }
            }

            ReleaseMutex(itsMtx);
        }
    }
}

void unitTestFail(const char* func, int line, const char* msg)
{
    addEntry(UNIT_TEST_FAIL, func, line, msg);
}

void unitTestPass(const char* func, int line, const char* msg)
{
    if (itsVerbose)
    {
        addEntry(UNIT_TEST_PASS, func, line, msg);
    }
}

bool unitTestGetEntry(UnitTestOutput *entryOutput)
{
    bool foundEntry = false;
    DWORD waitResult = WaitForSingleObject(itsMtx, INFINITE);

    if (waitResult == WAIT_OBJECT_0)
    {
        if (itsEntryHead != itsEntryTail)
        {
            entryOutput->overflow = itsOutputStore[itsEntryHead].overflow;
            entryOutput->result = itsOutputStore[itsEntryHead].result;
            entryOutput->line = itsOutputStore[itsEntryHead].line;
            strncpy(entryOutput->name, itsOutputStore[itsEntryHead].name, UNIT_TEST_FUNC_NAME_LEN);
            strncpy(entryOutput->msg, itsOutputStore[itsEntryHead].msg, UNIT_TEST_MESSAGE_LEN);
            itsEntryHead++;
            if (itsEntryHead >= ENTRIES_COUNT)
            {
                itsEntryHead = 0;
            }
            foundEntry = true;
        }
    }
    return foundEntry;
}

void unitTestAssertTrue(const char* func, int line, const char* exprName, bool exprVal)
{
    UnitTestResult result = (exprVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == true", exprName);
    addEntry(result, func, line, buf);

}

void unitTestAssertFalse(const char* func, int line, const char* exprName, bool exprVal)
{
    UnitTestResult result = (!exprVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == false", exprName);
    addEntry(result, func, line, buf);

}
void unitTestAssertTrue(const char* func, int line, const char* exprName, bool exprVal);
void unitTestAssertFalse(const char* func, int line, const char* exprName, bool exprVal);

void unitTestS8Equal(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%hhd == %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS8NotEqual(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%hhd != %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS8LessThan(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%hhd < %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS8GreaterThan(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%hhd > %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS8LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%hhd <= %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS8GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, int8_t aVal, int8_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%hhd >= %hhd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8Equal(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%hhu == %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8NotEqual(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%hhu != %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8LessThan(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%hhu < %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8GreaterThan(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%hhu > %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%hhu <= %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU8GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%hhu >= %hhu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

////////////////

void unitTestS16Equal(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%hd == %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS16NotEqual(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%hd != %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS16LessThan(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%hd < %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS16GreaterThan(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%hd > %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS16LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%hd <= %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS16GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, int16_t aVal, int16_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%hd >= %hd)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16Equal(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%hu == %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16NotEqual(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%hu != %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16LessThan(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%hu < %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16GreaterThan(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%hu > %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%hu <= %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU16GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%hu >= %hu)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

////////////////

void unitTestS32Equal(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%d == %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS32NotEqual(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%d != %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS32LessThan(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%d < %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS32GreaterThan(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%d > %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS32LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%d <= %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestS32GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, int32_t aVal, int32_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%d >= %d)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32Equal(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal == bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%u == %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32NotEqual(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal != bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%u != %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32LessThan(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%u < %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32GreaterThan(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%u > %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%u <= %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestU32GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%u >= %u)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

////////////////

void unitTestF32Equal(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal, float tolerance)
{
    float absA = fabsf(aVal);
    float absB = fabsf(bVal);
    float diff = fabsf(aVal - bVal);
    float largest = (absA > absB) ? absA : absB;
    UnitTestResult result = (diff <= tolerance * largest) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%f == %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF32NotEqual(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal, float tolerance)
{
    float absA = fabsf(aVal);
    float absB = fabsf(bVal);
    float diff = fabsf(aVal - bVal);
    float largest = (absA > absB) ? absA : absB;
    UnitTestResult result = (diff > tolerance * largest) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%f != %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF32LessThan(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%f < %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF32GreaterThan(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%f > %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF32LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%f <= %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF32GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, float aVal, float bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%f >= %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

////////////////

void unitTestF64Equal(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal, double tolerance)
{
    double absA = fabs(aVal);
    double absB = fabs(bVal);
    double diff = fabs(aVal - bVal);
    double largest = (absA > absB) ? absA : absB;
    UnitTestResult result = (diff <= tolerance * largest) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%f == %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF64NotEqual(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal, double tolerance)
{
    double absA = fabs(aVal);
    double absB = fabs(bVal);
    double diff = fabs(aVal - bVal);
    double largest = (absA > absB) ? absA : absB;
    UnitTestResult result = (diff > tolerance * largest) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%f != %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF64LessThan(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal)
{
    UnitTestResult result = (aVal < bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s < %s (%f < %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF64GreaterThan(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal)
{
    UnitTestResult result = (aVal > bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s > %s (%f > %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF64LessThanOrEqual(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal)
{
    UnitTestResult result = (aVal <= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s <= %s (%f <= %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestF64GreaterThanOrEqual(const char* func, int line, const char* aName, const char* bName, double aVal, double bVal)
{
    UnitTestResult result = (aVal >= bVal) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s >= %s (%f >= %f)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

//////////

void unitTestStrEqual(const char* func, int line, const char* aName, const char* bName, const char* aVal, const char* bVal)
{
    UnitTestResult result = (strcmp(aVal, bVal) == 0) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s (%s == %s)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

void unitTestStrNotEqual(const char* func, int line, const char* aName, const char* bName, const char* aVal, const char* bVal)
{
    UnitTestResult result = (strcmp(aVal, bVal) != 0) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s (%s != %s)", aName, bName, aVal, bVal);
    addEntry(result, func, line, buf);
}

//////////

void unitTestMemEqual(const char* func, int line, const char* aName, const char* bName, const void* aVal, const void* bVal, int siz)
{
    UnitTestResult result = (memcmp(aVal, bVal, siz) == 0) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s == %s", aName, bName);
    addEntry(result, func, line, buf);
}

void unitTestMemNotEqual(const char* func, int line, const char* aName, const char* bName, const void* aVal, const void* bVal, int siz)
{
    UnitTestResult result = (memcmp(aVal, bVal, siz) != 0) ? UNIT_TEST_PASS : UNIT_TEST_FAIL;
    char buf[UNIT_TEST_MESSAGE_LEN];
    snprintf(buf, UNIT_TEST_MESSAGE_LEN, "%s != %s", aName, bName);
    addEntry(result, func, line, buf);
}
