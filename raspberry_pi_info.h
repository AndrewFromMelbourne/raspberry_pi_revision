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

#ifndef RASPBERRY_PI_INFO_H
#define RASPBERRY_PI_INFO_H

//-------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------

typedef enum
{
    RPI_MEMORY_UNKNOWN,
    RPI_256MB = 256,
    RPI_512MB = 512,
    RPI_1024MB = 1024,
}
RASPBERRY_PI_MEMORY_T;

typedef enum
{
    RPI_PROCESSOR_UNKNOWN = 0,
    RPI_BROADCOM_2835 = 2835,
    RPI_BROADCOM_2836 = 2836
}
RASPBERRY_PI_PROCESSOR_T;

typedef enum
{
    RPI_I2C_DEVICE_UNKNOWN = -1,
    RPI_I2C_0 = 0,
    RPI_I2C_1 = 1
}
RASPBERRY_PI_I2C_DEVICE_T;

//-------------------------------------------------------------------------

typedef struct
{
    RASPBERRY_PI_MEMORY_T memory;
    RASPBERRY_PI_PROCESSOR_T processor;
    RASPBERRY_PI_I2C_DEVICE_T i2cDevice;
    const char *modelName;
    const char *manufacturer;
    int pcbRevision;
    int warrantyBit;
    int revisionNumber;
}
RASPBERRY_PI_INFO_T;

//-------------------------------------------------------------------------

int
getRaspberryPiInformation(
    RASPBERRY_PI_INFO_T *info);

//-------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------

#endif
