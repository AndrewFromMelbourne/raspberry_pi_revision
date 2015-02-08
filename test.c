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

#include <stdio.h>

#include "raspberry_pi_info.h"

//-----------------------------------------------------------------------

int
main(void)
{
    RASPBERRY_PI_INFO_T info;
    
    if (getRaspberryPiInformation(&info) > 0)
    {
        printf("memory: %d MB\n", info.memory);
        printf("processor: Broadcom BCM%d\n", info.processor);
        printf("i2cDevice: /dev/i2c-%d\n", info.i2cDevice);
        printf("model name: Raspberry Pi Model %s\n", info.modelName);
        printf("manufacturer: %s\n", info.manufacturer);
        printf("pcb revision: %d\n", info.pcbRevision);
        printf("warranty void: %s\n", (info.warrantyBit) ? "yes" : "no");
        printf("revision: %04X\n", info.revisionNumber);
    }

    return 0;
}
