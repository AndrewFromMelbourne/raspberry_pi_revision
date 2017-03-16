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

void
printRevisionTable(
    int *revisions,
    size_t numberOfRevisions)
{
    size_t i;
    for (i = 0 ; i < numberOfRevisions ;  i++)
    {
        int revision = revisions[i];
        RASPBERRY_PI_INFO_T info;

        if (getRaspberryPiInformationForRevision(revision, &info) == 0)
        {
            continue;
        }

        printf(" | %08X", revision);
        printf(" | %16s", raspberryPiModelToString(info.model));
        printf(" | %d", info.pcbRevision);
        printf(" | %7s", raspberryPiMemoryToString(info.memory));
        printf(" | %10s", raspberryPiManufacturerToString(info.manufacturer));
        printf(" | %16s", raspberryPiProcessorToString(info.processor));
        printf(" | %10s", raspberryPiI2CDeviceToString(info.i2cDevice));
        printf(" | 0x%08" PRIX32 "", info.peripheralBase);
        printf(" |\n");
    }
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
        0x920093,
        0x900093,
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
