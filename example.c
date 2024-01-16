#include <stdint.h>
#include "UnitTest.h"

void testBasics(void)
{
	ASSERT_TRUE(0);
	ASSERT_TRUE(1);
	ASSERT_FALSE(0);
	ASSERT_FALSE(1);
}

void testIntegers(void)
{
	int8_t s8zero = 0;
	int8_t s8one = 1;
	uint8_t u8zero = 0;
	uint8_t u8one = 1;
	int16_t s16zero = 0;
	int16_t s16one = 1;
	uint16_t u16zero = 0;
	uint16_t u16one = 1;
	int32_t s32zero = 0;
	int32_t s32one = 1;
	uint32_t u32zero = 0;
	uint32_t u32one = 1;

	ASSERT_S8_EQUAL(s8zero, s8zero);
	ASSERT_S8_NOT_EQUAL(s8zero, s8zero);
	ASSERT_S8_LESS_THAN(s8zero, s8zero);
	ASSERT_S8_GREATER_THAN(s8zero, s8zero);
	ASSERT_S8_GREATER_THAN_OR_EQUAL(s8zero, s8zero);
	ASSERT_S8_LESS_THAN_OR_EQUAL(s8zero, s8zero);

	ASSERT_S8_EQUAL(s8zero, s8one);
	ASSERT_S8_NOT_EQUAL(s8zero, s8one);
	ASSERT_S8_LESS_THAN(s8zero, s8one);
	ASSERT_S8_GREATER_THAN(s8zero, s8one);
	ASSERT_S8_GREATER_THAN_OR_EQUAL(s8zero, s8one);
	ASSERT_S8_LESS_THAN_OR_EQUAL(s8zero, s8one);

	ASSERT_U8_EQUAL(u8zero, u8zero);
	ASSERT_U8_NOT_EQUAL(u8zero, u8zero);
	ASSERT_U8_LESS_THAN(u8zero, u8zero);
	ASSERT_U8_GREATER_THAN(u8zero, u8zero);
	ASSERT_U8_GREATER_THAN_OR_EQUAL(u8zero, u8zero);
	ASSERT_U8_LESS_THAN_OR_EQUAL(u8zero, u8zero);

	ASSERT_U8_EQUAL(u8zero, u8one);
	ASSERT_U8_NOT_EQUAL(u8zero, u8one);
	ASSERT_U8_LESS_THAN(u8zero, u8one);
	ASSERT_U8_GREATER_THAN(u8zero, u8one);
	ASSERT_U8_GREATER_THAN_OR_EQUAL(u8zero, u8one);
	ASSERT_U8_LESS_THAN_OR_EQUAL(u8zero, u8one);

	ASSERT_S16_EQUAL(s16zero, s16zero);
	ASSERT_S16_NOT_EQUAL(s16zero, s16zero);
	ASSERT_S16_LESS_THAN(s16zero, s16zero);
	ASSERT_S16_GREATER_THAN(s16zero, s16zero);
	ASSERT_S16_GREATER_THAN_OR_EQUAL(s16zero, s16zero);
	ASSERT_S16_LESS_THAN_OR_EQUAL(s16zero, s16zero);

	ASSERT_S16_EQUAL(s16zero, s16one);
	ASSERT_S16_NOT_EQUAL(s16zero, s16one);
	ASSERT_S16_LESS_THAN(s16zero, s16one);
	ASSERT_S16_GREATER_THAN(s16zero, s16one);
	ASSERT_S16_GREATER_THAN_OR_EQUAL(s16zero, s16one);
	ASSERT_S16_LESS_THAN_OR_EQUAL(s16zero, s16one);

	ASSERT_U16_EQUAL(u16zero, u16zero);
	ASSERT_U16_NOT_EQUAL(u16zero, u16zero);
	ASSERT_U16_LESS_THAN(u16zero, u16zero);
	ASSERT_U16_GREATER_THAN(u16zero, u16zero);
	ASSERT_U16_GREATER_THAN_OR_EQUAL(u16zero, u16zero);
	ASSERT_U16_LESS_THAN_OR_EQUAL(u16zero, u16zero);

	ASSERT_U16_EQUAL(u16zero, u16one);
	ASSERT_U16_NOT_EQUAL(u16zero, u16one);
	ASSERT_U16_LESS_THAN(u16zero, u16one);
	ASSERT_U16_GREATER_THAN(u16zero, u16one);
	ASSERT_U16_GREATER_THAN_OR_EQUAL(u16zero, u16one);
	ASSERT_U16_LESS_THAN_OR_EQUAL(u16zero, u16one);

	ASSERT_S32_EQUAL(s32zero, s32zero);
	ASSERT_S32_NOT_EQUAL(s32zero, s32zero);
	ASSERT_S32_LESS_THAN(s32zero, s32zero);
	ASSERT_S32_GREATER_THAN(s32zero, s32zero);
	ASSERT_S32_GREATER_THAN_OR_EQUAL(s32zero, s32zero);
	ASSERT_S32_LESS_THAN_OR_EQUAL(s32zero, s32zero);

	ASSERT_S32_EQUAL(s32zero, s32one);
	ASSERT_S32_NOT_EQUAL(s32zero, s32one);
	ASSERT_S32_LESS_THAN(s32zero, s32one);
	ASSERT_S32_GREATER_THAN(s32zero, s32one);
	ASSERT_S32_GREATER_THAN_OR_EQUAL(s32zero, s32one);
	ASSERT_S32_LESS_THAN_OR_EQUAL(s32zero, s32one);

	ASSERT_U32_EQUAL(u32zero, u32zero);
	ASSERT_U32_NOT_EQUAL(u32zero, u32zero);
	ASSERT_U32_LESS_THAN(u32zero, u32zero);
	ASSERT_U32_GREATER_THAN(u32zero, u32zero);
	ASSERT_U32_GREATER_THAN_OR_EQUAL(u32zero, u32zero);
	ASSERT_U32_LESS_THAN_OR_EQUAL(u32zero, u32zero);

	ASSERT_U32_EQUAL(u32zero, u32one);
	ASSERT_U32_NOT_EQUAL(u32zero, u32one);
	ASSERT_U32_LESS_THAN(u32zero, u32one);
	ASSERT_U32_GREATER_THAN(u32zero, u32one);
	ASSERT_U32_GREATER_THAN_OR_EQUAL(u32zero, u32one);
	ASSERT_U32_LESS_THAN_OR_EQUAL(u32zero, u32one);

}

void testFloats(void)
{
	float f32zero = 0.0;
	float f32two = 2.0;
	double f64zero = 0.0;
	double f64two = 2.0;

	ASSERT_F32_EQUAL(f32zero, f32zero, 1e-5);
	ASSERT_F32_NOT_EQUAL(f32zero, f32zero, 1e-5);
	ASSERT_F32_LESS_THAN(f32zero, f32zero);
	ASSERT_F32_GREATER_THAN(f32zero, f32zero);
	ASSERT_F32_GREATER_THAN_OR_EQUAL(f32zero, f32zero);
	ASSERT_F32_LESS_THAN_OR_EQUAL(f32zero, f32zero);

	ASSERT_F32_EQUAL(f32zero, f32two, 1e-5);
	ASSERT_F32_NOT_EQUAL(f32zero, f32two, 1e-5);
	ASSERT_F32_LESS_THAN(f32zero, f32two);
	ASSERT_F32_GREATER_THAN(f32zero, f32two);
	ASSERT_F32_GREATER_THAN_OR_EQUAL(f32zero, f32two);
	ASSERT_F32_LESS_THAN_OR_EQUAL(f32zero, f32two);

	ASSERT_F64_EQUAL(f64zero, f64zero, 1e-5);
	ASSERT_F64_NOT_EQUAL(f64zero, f64zero, 1e-5);
	ASSERT_F64_LESS_THAN(f64zero, f64zero);
	ASSERT_F64_GREATER_THAN(f64zero, f64zero);
	ASSERT_F64_GREATER_THAN_OR_EQUAL(f64zero, f64zero);
	ASSERT_F64_LESS_THAN_OR_EQUAL(f64zero, f64zero);

	ASSERT_F64_EQUAL(f64zero, f64two, 1e-5);
	ASSERT_F64_NOT_EQUAL(f64zero, f64two, 1e-5);
	ASSERT_F64_LESS_THAN(f64zero, f64two);
	ASSERT_F64_GREATER_THAN(f64zero, f64two);
	ASSERT_F64_GREATER_THAN_OR_EQUAL(f64zero, f64two);
	ASSERT_F64_LESS_THAN_OR_EQUAL(f64zero, f64two);

}

void testStrings(void)
{
	char strFoo[] = "FOO";
	char strBar[] = "BAR";

	ASSERT_STR_EQUAL(strFoo, strFoo);
	ASSERT_STR_EQUAL(strFoo, strBar);
	ASSERT_STR_NOT_EQUAL(strFoo, strFoo);
	ASSERT_STR_NOT_EQUAL(strFoo, strBar);
}

void testMemory(void)
{
	uint16_t bufFoo[] = {0, 1, 2};
	uint16_t bufBar[] = {4, 5, 6};

	ASSERT_MEM_EQUAL(6, bufFoo, bufFoo);
	ASSERT_MEM_EQUAL(6, bufFoo, bufBar);
	ASSERT_MEM_NOT_EQUAL(6, bufFoo, bufFoo);
	ASSERT_MEM_NOT_EQUAL(6, bufFoo, bufBar);
}

#if 0
void testPointers(void)
{
	int tmpFoo;
	int tmpBar;
	void* ptrNull = 0;
	void* ptrFoo = &tmpFoo;
	void* ptrBar = &tmpBar;

	ASSERT_PTR_NULL(ptrNull);
	ASSERT_PTR_NULL(ptrFoo);
	ASSERT_PTR_EQ(ptrFoo, ptrFoo);
	ASSERT_PTR_EQ(ptrFoo, ptrBar);
	ASSERT_PTR_NEQ(ptrFoo, ptrFoo);
	ASSERT_PTR_NEQ(ptrFoo, ptrBar);
}
#endif

typedef void(*TestFunc)(void);

TestFunc testList[] = {
	testBasics,
	testIntegers,
	testFloats,
	testStrings,
	testMemory,
//	testPointers
};

#define N_TESTS (sizeof(testList)/sizeof(testList[0]))

void runTests(void)
{
	for (int i = 0; i < N_TESTS; i++)
		(testList[i])();
}