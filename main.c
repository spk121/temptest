// UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "UnitTest.h"

extern void runTests(void);

int main()
{
    printf("%s\n", __func__);
    runTests();

    UnitTestOutput output = { 0 };
    while (unitTestGetEntry(&output))
    {
        if (output.result == UNIT_TEST_PASS)
            printf("[PASS]");
        else
            printf("[FAIL]");
        printf(" %s:%d %s\n", output.name, output.line, output.msg);
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
