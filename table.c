//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#include <inttypes.h>
#include <stdio.h>

#include "raspberry_pi_revision.h"

//-----------------------------------------------------------------------

typedef struct
{
    int width;
    int padding;
    const char* label;
} FORMAT_T;

//-----------------------------------------------------------------------

void
printTableSeparator(
    FORMAT_T *format,
    int formats)
{
    putchar(' ');

    int f = 0;
    for (f = 0 ; f < formats ; f++)
    {
        putchar('+');

        int d;
        int dashes = format[f].width + format[f].padding + 2;

        for (d = 0 ; d < dashes ; d++)
        {
            putchar('-');
        }
    }
    printf("+\n");
}

//-----------------------------------------------------------------------


void
printTableHeader(
    FORMAT_T *format,
    int formats)
{
    int f = 0;

    for (f = 0 ; f < formats ; f++)
    {
        printf(" | %-*s", format[f].width, format[f].label);

        if (format[f].padding > 0)
        {
            printf("%-*c", format[f].padding, ' ');
        }
    }
    printf(" |\n");
}

//-----------------------------------------------------------------------

void
printRevisionTable(
    int *revisions,
    size_t numberOfRevisions)
{
    static FORMAT_T format[] =
    {
        {  8, 0, "Revision" },
        { 16, 0, "Model Name" },
        {  1, 0, "R" },
        {  7, 0, "Memory" },
        { 12, 0, "Manufacturer" },
        { 16, 0, "Processor" },
        { 10, 0, "I2C Device" },
        {  8, 2, "Base" }
    };

    int formats = sizeof(format) / sizeof(format[0]);

    printTableSeparator(format, formats);
    printTableHeader(format, formats);
    printTableSeparator(format, formats);

    size_t i;
    for (i = 0 ; i < numberOfRevisions ;  i++)
    {
        int revision = revisions[i];
        RASPBERRY_PI_INFO_T info;

        if (getRaspberryPiInformationForRevision(revision, &info) == 0)
        {
            continue;
        }

        int f = 0;

        printf(" | %.*X",
               format[f++].width,
               revision);
        printf(" | %-*s",
               format[f++].width,
               raspberryPiModelToString(info.model));
        printf(" | %.*d",
               format[f++].width,
               info.pcbRevision);
        printf(" | %*s",
               format[f++].width,
               raspberryPiMemoryToString(info.memory));
        printf(" | %-*s",
               format[f++].width,
               raspberryPiManufacturerToString(info.manufacturer));
        printf(" | %*s",
               format[f++].width,
               raspberryPiProcessorToString(info.processor));
        printf(" | %*s",
               format[f++].width,
               raspberryPiI2CDeviceToString(info.i2cDevice));
        printf(" | 0x%.*" PRIX32 "",
               format[f++].width,
               info.peripheralBase);
        printf(" |\n");
    }

    printTableSeparator(format, formats);
    printf("\n");
}

//-----------------------------------------------------------------------

int
main(void)
{
    int
    numericalRevisions[] = 
    {
        0x000000,
        0x000001,
        0x000002,
        0x000003,
        0x000004,
        0x000005,
        0x000006,
        0x000007,
        0x000008,
        0x000009,
        0x00000A,
        0x00000B,
        0x00000C,
        0x00000D,
        0x00000E,
        0x00000F,
        0x000010,
        0x000011,
        0x000012,
        0x000013,
        0x000014,
        0x000015,
    };

    printRevisionTable(
        numericalRevisions,
        sizeof(numericalRevisions) / sizeof(numericalRevisions[0]));

    int
    bitFieldRevisions[] = 
    {
        0x900021,
        0x900092,
        0x900093,
        0x920093,
        0x9000C1,
        0xA01041,
        0xA21041,
        0xA22042,
        0xA02082,
        0xA22082,
        0xA32082,
        0xA020A0,
        0xA220A0,
    };

    printRevisionTable(
        bitFieldRevisions,
        sizeof(bitFieldRevisions) / sizeof(bitFieldRevisions[0]));

    return 0;
}
