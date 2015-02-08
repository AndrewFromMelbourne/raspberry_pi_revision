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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raspberry_pi_info.h"

//-------------------------------------------------------------------------
//
// The file /proc/cpuinfo contains a line such as:-
//
// Revision    : 0003
//
// that holds the revision number of the Raspberry Pi.
// Known revisions (prior to the Raspberry Pi 2) are:
//
//     +----------+---------+---------+--------+-------------+
//     | Revision |  Model  | PCB Rev | Memory | Manufacture |
//     +----------+---------+---------+--------+-------------+
//     |   0000   |         |         |        |             |
//     |   0001   |         |         |        |             |
//     |   0002   |    B    |    1    | 256 MB |             |
//     |   0003   |    B    |    1    | 256 MB |             |
//     |   0004   |    B    |    2    | 256 MB |   Sony      |
//     |   0005   |    B    |    2    | 256 MB |   Qisda     |
//     |   0006   |    B    |    2    | 256 MB |   Egoman    |
//     |   0007   |    A    |    2    | 256 MB |   Egoman    |
//     |   0008   |    A    |    2    | 256 MB |   Sony      |
//     |   0009   |    A    |    2    | 256 MB |   Qisda     |
//     |   000a   |         |         |        |             |
//     |   000b   |         |         |        |             |
//     |   000c   |         |         |        |             |
//     |   000d   |    B    |    2    | 512 MB |   Egoman    |
//     |   000e   |    B    |    2    | 512 MB |   Sony      |
//     |   000f   |    B    |    2    | 512 MB |   Qisda     |
//     |   0010   |    B+   |    1    | 512 MB |   Sony      |
//     |   0011   | compute |    1    | 512 MB |   Sony      |
//     |   0012   |    A+   |    1    | 256 MB |   Sony      |
//     +----------+---------+---------+--------+-------------+
//
// If the Raspberry Pi has been over-volted (voiding the warranty) the
// revision number will have 100 at the front. e.g. 1000002.
//
//-------------------------------------------------------------------------
//
// With the release of the Raspberry Pi 2, there is a new encoding of the
// Revision field in /proc/cpuinfo. The bit fields are as follows
//
//     +----+----+----+----+----+----+----+----+
//     |FEDC|BA98|7654|3210|FEDC|BA98|7654|3210|
//     +----+----+----+----+----+----+----+----+
//     |    |    |    |    |    |    |    |AAAA|
//     |    |    |    |    |    |BBBB|BBBB|    |
//     |    |    |    |    |CCCC|    |    |    |
//     |    |    |    |DDDD|    |    |    |    |
//     |    |    | EEE|    |    |    |    |    |
//     |    |    |F   |    |    |    |    |    |
//     |    |   G|    |    |    |    |    |    |
//     |    |  H |    |    |    |    |    |    |
//     +----+----+----+----+----+----+----+----+
//     |1098|7654|3210|9876|5432|1098|7654|3210|
//     +----+----+----+----+----+----+----+----+
//
// +---+-------+--------------+--------------------------------------------+
// | # | bits  |   contains   | values                                     |
// +---+-------+--------------+--------------------------------------------+
// | A | 00-03 | PCB Revision | (the pcb revision number)                  |
// | B | 04-11 | Model name   | A, B, A+, B+, B Pi2, Alpha, Compute Module |
// | C | 12-15 | Processor    | BCM2835, BCM2836                           |
// | D | 16-19 | Manufacturer | Sony, Egoman, Embest, unknown, Embest      |
// | E | 20-22 | Memory size  | 256 MB, 512 MB, 1024 MB                    |
// | F | 23-23 | encoded flag | (if set, revision is a bit field)          |
// | G | 24-24 | waranty bit  | (if set, warranty void - Pre Pi2)          |
// | H | 25-25 | waranty bit  | (if set, warranty void - Post Pi2)         |
// +---+-------+--------------+--------------------------------------------+
//
// Also, due to some early issues the warranty bit has been move from bit
// 24 to bit 25 of the revision number (i.e. 0x2000000).
//
// e.g.
//
// Revision    : A01041
//
// A - PCB Revison - 1 (first revision)
// B - Model Name - 4 (Model B Pi 2)
// C - Processor - 1 (BCM2836)
// D - Manufacturer - 0 (Sony)
// E - Memory - 2 (1024 MB)
//
//-------------------------------------------------------------------------

static RASPBERRY_PI_MEMORY_T revisionToMemory[] =
{
    RPI_MEMORY_UNKNOWN, //  0
    RPI_MEMORY_UNKNOWN, //  1
    RPI_256MB,          //  2
    RPI_256MB,          //  3
    RPI_256MB,          //  4
    RPI_256MB,          //  5
    RPI_256MB,          //  6
    RPI_256MB,          //  7
    RPI_256MB,          //  8
    RPI_256MB,          //  9
    RPI_MEMORY_UNKNOWN, //  A
    RPI_MEMORY_UNKNOWN, //  B
    RPI_MEMORY_UNKNOWN, //  C
    RPI_512MB,          //  D
    RPI_512MB,          //  E
    RPI_512MB,          //  F
    RPI_512MB,          // 10
    RPI_512MB,          // 11
    RPI_256MB           // 12
};

static RASPBERRY_PI_MEMORY_T bitFieldToMemory[] =
{
    RPI_256MB,
    RPI_512MB,
    RPI_1024MB
};

//-------------------------------------------------------------------------

static RASPBERRY_PI_PROCESSOR_T bitFieldToProcessor[] =
{
    RPI_BROADCOM_2835,
    RPI_BROADCOM_2836
};

//-------------------------------------------------------------------------

static RASPBERRY_PI_I2C_DEVICE_T revisionToI2CDevice[] =
{
    RPI_I2C_DEVICE_UNKNOWN, //  0
    RPI_I2C_DEVICE_UNKNOWN, //  1
    RPI_I2C_0,              //  2
    RPI_I2C_0,              //  3
    RPI_I2C_1,              //  4
    RPI_I2C_1,              //  5
    RPI_I2C_1,              //  6
    RPI_I2C_1,              //  7
    RPI_I2C_1,              //  8
    RPI_I2C_1,              //  9
    RPI_I2C_DEVICE_UNKNOWN, //  A
    RPI_I2C_DEVICE_UNKNOWN, //  B
    RPI_I2C_DEVICE_UNKNOWN, //  C
    RPI_I2C_1,              //  D
    RPI_I2C_1,              //  E
    RPI_I2C_1,              //  F
    RPI_I2C_1,              // 10
    RPI_I2C_1,              // 11
    RPI_I2C_1               // 12
};

//-------------------------------------------------------------------------

static const char *revisionToModelName[] =
{
    "unknown",        //  0
    "unknown",        //  1
    "B",              //  2
    "B",              //  3
    "B",              //  4
    "B",              //  5
    "B",              //  6
    "A",              //  7
    "A",              //  8
    "A",              //  9
    "unknown",        //  A
    "unknown",        //  B
    "unknown",        //  C
    "B",              //  D
    "B",              //  E
    "B",              //  F
    "B+",             // 10
    "Compute Module", // 11
    "A+"              // 12
};

static const char *bitFieldToModelName[] =
{
    "A",
    "B",
    "A+",
    "B+",
    "B Pi 2",
    "Alpha",
    "Compute Module"
};

//-------------------------------------------------------------------------

static const char *bitFieldToManufacturer[] =
{
    "Sony",
    "Egoman",
    "Embest",
    "unknown",
    "Embest"
};

static const char *revisionToManufacturer[] =
{
    "unknown", //  0
    "unknown", //  1
    "unknown", //  2
    "unknown", //  3
    "Sony",    //  4
    "Qisda",   //  5
    "Egoman",  //  6
    "Egoman",  //  7
    "Sony",    //  8
    "Qisda",   //  9
    "unknown", //  A
    "unknown", //  B
    "unknown", //  C
    "Egoman",  //  D
    "Sony",    //  E
    "Qisda",   //  F
    "Sony",    // 10
    "Sony",    // 11
    "Sony"     // 12
};

//-------------------------------------------------------------------------

static int revisionToPcbRevision[] =
{
    0, //  0
    0, //  1
    1, //  2
    1, //  3
    2, //  4
    2, //  5
    2, //  6
    2, //  7
    2, //  8
    2, //  9
    2, //  A
    0, //  B
    0, //  C
    2, //  D
    2, //  E
    2, //  F
    1, // 10
    1, // 11
    1  // 12
};

//-------------------------------------------------------------------------
//
// Remove leading and trailing whitespace from a string.

static char *
trimWhiteSpace(
    char *string)
{
    if (string == NULL)
    {
        return NULL;
    }

    while (isspace(*string))
    {
        string++;
    }

    if (*string == '\0')
    {
        return string;
    }

    char *end = string;

    while (*end)
    {
        ++end;
    }
    --end;

    while ((end > string) && isspace(*end))
    {
        end--;
    }

    *(end + 1) = 0;
    return string;
}

//-------------------------------------------------------------------------

static int
getRaspberryPiRevision()
{
    int raspberryPiRevision = 0;

    FILE *fp = fopen("/proc/cpuinfo", "r");

    if (fp == NULL)
    {
        perror("/proc/cpuinfo");
        return raspberryPiRevision;
    }

    char entry[80];

    while (fgets(entry, sizeof(entry), fp) != NULL)
    {
        char* saveptr = NULL;

        char *key = trimWhiteSpace(strtok_r(entry, ":", &saveptr));
        char *value = trimWhiteSpace(strtok_r(NULL, ":", &saveptr));

        if (strcasecmp("Revision", key) == 0)
        {
            raspberryPiRevision = strtol(value, NULL, 16);
        }
    }

    fclose(fp);

    return raspberryPiRevision;
}

//-------------------------------------------------------------------------

int
getRaspberryPiInformation(
    RASPBERRY_PI_INFO_T *info)
{
    int result = 0;

    if (info != NULL)
    {
        int revision = getRaspberryPiRevision();

        info->memory = RPI_MEMORY_UNKNOWN;
        info->processor = RPI_PROCESSOR_UNKNOWN;
        info->i2cDevice = RPI_I2C_DEVICE_UNKNOWN;
        info->modelName = "unknown";
        info->manufacturer = "unknown";
        info->pcbRevision = 0;
        info->warrantyBit = 0;
        info->revisionNumber = revision;

        if (revision != 0)
        {
            // remove warranty bit

            revision &= ~0x3000000;

            if (revision & 0x800000)
            {
                // Raspberry Pi2 style revision encoding

                result = 2;

                if (info->revisionNumber & 0x2000000)
                {
                    info->warrantyBit = 1;
                }

                int memoryIndex = (revision & 0x700000) >> 20;
                info->memory = bitFieldToMemory[memoryIndex];

                int processorIndex = (revision & 0xF000) >> 12;
                info->processor = bitFieldToProcessor[processorIndex];

                // If some future firmware changes the Rev number of
                // older Raspberry Pis, then need to work out the i2c
                // device.

                info->i2cDevice = RPI_I2C_1;

                int modelIndex = (revision & 0xFF0) >> 4;
                info->modelName = bitFieldToModelName[modelIndex];

                int madeByIndex = (revision & 0xF0000) >> 16;
                info->manufacturer = bitFieldToManufacturer[madeByIndex];

                info->pcbRevision = revision & 0xF;
            }
            else if (revision <= 0x12)
            {
                // Original revision encoding

                result = 1;

                if (info->revisionNumber & 0x1000000)
                {
                    info->warrantyBit = 1;
                }

                info->memory = revisionToMemory[revision];
                info->processor = RPI_BROADCOM_2835;
                info->i2cDevice = revisionToI2CDevice[revision];
                info->modelName = revisionToModelName[revision];
                info->manufacturer = revisionToManufacturer[revision];
                info->pcbRevision = revisionToPcbRevision[revision];
            }
        }
    }

    return result;
}

