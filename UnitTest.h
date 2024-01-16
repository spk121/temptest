#ifndef UNITTEST_H
#define UNITTEST_H

/************************************************
* (c) Copyright TBD
* 
* Name: UnitTest.h
* 
* Description: A simple unit test framework for Omega
* 
* Revision History:
* 
* Target:
* 
*/

// INCLUDE FILES
#include <stdbool.h>
#include <stdint.h>

// DEFINE AND TYPE DECLARATIONS

#define UNIT_TEST_FUNC_NAME_LEN 31
#define UNIT_TEST_MESSAGE_LEN 120

typedef enum UnitTestResult
{
    UNIT_TEST_PASS = 0,
    UNIT_TEST_FAIL,
    UNIT_TEST_N_RESULT_TYPES
} UnitTestResult;

typedef struct UnitTestOutput
{
    UnitTestResult result;
    bool overflow;
    char name[UNIT_TEST_FUNC_NAME_LEN];
    int line;
    char msg[UNIT_TEST_MESSAGE_LEN];
} UnitTestOutput;

#define ASSERT(expr) unitTestAssertTrue(__func__, __LINE__, #expr, (expr))
#define ASSERT_TRUE(expr) unitTestAssertTrue(__func__, __LINE__, #expr, (expr))
#define ASSERT_FALSE(expr) unitTestAssertFalse(__func__, __LINE__, #expr, (expr))

#define ASSERT_S8_EQUAL(a,b) unitTestS8Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S8_NOT_EQUAL(a,b) unitTestS8NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S8_LESS_THAN(a,b) unitTestS8LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S8_GREATER_THAN(a,b) unitTestS8GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S8_LESS_THAN_OR_EQUAL(a,b) unitTestS8LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S8_GREATER_THAN_OR_EQUAL(a,b) unitTestS8GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_EQUAL(a,b) unitTestU8Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_NOT_EQUAL(a,b) unitTestU8NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_LESS_THAN(a,b) unitTestU8LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_GREATER_THAN(a,b) unitTestU8GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_LESS_THAN_OR_EQUAL(a,b) unitTestU8LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U8_GREATER_THAN_OR_EQUAL(a,b) unitTestU8GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_EQUAL(a,b) unitTestS16Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_NOT_EQUAL(a,b) unitTestS16NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_LESS_THAN(a,b) unitTestS16LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_GREATER_THAN(a,b) unitTestS16GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_LESS_THAN_OR_EQUAL(a,b) unitTestS16LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S16_GREATER_THAN_OR_EQUAL(a,b) unitTestS16GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_EQUAL(a,b) unitTestU16Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_NOT_EQUAL(a,b) unitTestU16NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_LESS_THAN(a,b) unitTestU16LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_GREATER_THAN(a,b) unitTestU16GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_LESS_THAN_OR_EQUAL(a,b) unitTestU16LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U16_GREATER_THAN_OR_EQUAL(a,b) unitTestU16GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))

#define ASSERT_S32_EQUAL(a,b) unitTestS32Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S32_NOT_EQUAL(a,b) unitTestS32NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S32_LESS_THAN(a,b) unitTestS32LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S32_GREATER_THAN(a,b) unitTestS32GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S32_LESS_THAN_OR_EQUAL(a,b) unitTestS32LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_S32_GREATER_THAN_OR_EQUAL(a,b) unitTestS32GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_EQUAL(a,b) unitTestU32Equal(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_NOT_EQUAL(a,b) unitTestU32NotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_LESS_THAN(a,b) unitTestU32LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_GREATER_THAN(a,b) unitTestU32GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_LESS_THAN_OR_EQUAL(a,b) unitTestU32LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_U32_GREATER_THAN_OR_EQUAL(a,b) unitTestU32GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))

#define ASSERT_F32_EQUAL(a,b,tol) unitTestF32Equal(__func__, __LINE__, #a, #b, (a), (b), (tol))
#define ASSERT_F32_NOT_EQUAL(a,b,tol) unitTestF32NotEqual(__func__, __LINE__, #a, #b, (a), (b), (tol))
#define ASSERT_F32_LESS_THAN(a,b) unitTestF32LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F32_GREATER_THAN(a,b) unitTestF32GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F32_LESS_THAN_OR_EQUAL(a,b) unitTestF32LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F32_GREATER_THAN_OR_EQUAL(a,b) unitTestF32GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))

#define ASSERT_F64_EQUAL(a,b,tol) unitTestF64Equal(__func__, __LINE__, #a, #b, (a), (b), (tol))
#define ASSERT_F64_NOT_EQUAL(a,b,tol) unitTestF64NotEqual(__func__, __LINE__, #a, #b, (a), (b), (tol))
#define ASSERT_F64_LESS_THAN(a,b) unitTestF64LessThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F64_GREATER_THAN(a,b) unitTestF64GreaterThan(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F64_LESS_THAN_OR_EQUAL(a,b) unitTestF64LessThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_F64_GREATER_THAN_OR_EQUAL(a,b) unitTestF64GreaterThanOrEqual(__func__, __LINE__, #a, #b, (a), (b))

#define ASSERT_STR_EQUAL(a,b) unitTestStrEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_STR_NOT_EQUAL(a,b) unitTestStrNotEqual(__func__, __LINE__, #a, #b, (a), (b))
#define ASSERT_MEM_EQUAL(siz,a,b) unitTestMemEqual(__func__, __LINE__, #a, #b, (const void *)(a), (const void *)(b), (siz))
#define ASSERT_MEM_NOT_EQUAL(siz,a,b) unitTestMemNotEqual(__func__, __LINE__, #a, #b, (const void *)(a), (const void *)(b), (siz))

// PUBLIC FUNCTION PROTOTYPES

void unitTestAssertTrue(const char* func, int line, const char* exprName, bool exprVal);
void unitTestAssertFalse(const char* func, int line, const char* exprName, bool exprVal);

void unitTestS8Equal(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);
void unitTestS8NotEqual(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);
void unitTestS8LessThan(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);
void unitTestS8GreaterThan(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);
void unitTestS8LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);
void unitTestS8GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int8_t aVal, int8_t bVal);

void unitTestU8Equal(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);
void unitTestU8NotEqual(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);
void unitTestU8LessThan(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);
void unitTestU8GreaterThan(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);
void unitTestU8LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);
void unitTestU8GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint8_t aVal, uint8_t bVal);

void unitTestS16Equal(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);
void unitTestS16NotEqual(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);
void unitTestS16LessThan(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);
void unitTestS16GreaterThan(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);
void unitTestS16LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);
void unitTestS16GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int16_t aVal, int16_t bVal);

void unitTestU16Equal(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);
void unitTestU16NotEqual(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);
void unitTestU16LessThan(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);
void unitTestU16GreaterThan(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);
void unitTestU16LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);
void unitTestU16GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint16_t aVal, uint16_t bVal);

void unitTestS32Equal(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);
void unitTestS32NotEqual(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);
void unitTestS32LessThan(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);
void unitTestS32GreaterThan(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);
void unitTestS32LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);
void unitTestS32GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, int32_t aVal, int32_t bVal);

void unitTestU32Equal(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);
void unitTestU32NotEqual(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);
void unitTestU32LessThan(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);
void unitTestU32GreaterThan(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);
void unitTestU32LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);
void unitTestU32GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, uint32_t aVal, uint32_t bVal);

void unitTestF32Equal(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal, float tolerance);
void unitTestF32NotEqual(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal, float tolernace);
void unitTestF32LessThan(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal);
void unitTestF32GreaterThan(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal);
void unitTestF32LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal);
void unitTestF32GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, float aVal, float bVal);

void unitTestF64Equal(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal, double tolerance);
void unitTestF64NotEqual(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal, double tolernace);
void unitTestF64LessThan(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal);
void unitTestF64GreaterThan(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal);
void unitTestF64LessThanOrEqual(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal);
void unitTestF64GreaterThanOrEqual(const char* func, int lint, const char* aName, const char* bName, double aVal, double bVal);

void unitTestStrEqual(const char* func, int lint, const char* aName, const char* bName, const char * aVal, const char* bVal);
void unitTestStrNotEqual(const char* func, int lint, const char* aName, const char* bName, const char* aVal, const char* bVal);
void unitTestMemEqual(const char* func, int lint, const char* aName, const char* bName, const void* aVal, const void* bVal, int siz);
void unitTestMemNotEqual(const char* func, int lint, const char* aName, const char* bName, const void* aVal, const void* bVal, int siz);

bool unitTestGetEntry(UnitTestOutput* entryOutput);

#endif