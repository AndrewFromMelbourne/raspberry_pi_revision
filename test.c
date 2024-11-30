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

#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>

#include "raspberry_pi_revision.h"

//-----------------------------------------------------------------------

typedef unsigned (*bhgpa)(void);

//-----------------------------------------------------------------------

uint32_t
getPeripheralBaseFromBcmHost(void)
{
    uint32_t base = 0;

    void *dl = dlopen("/opt/vc/lib/libbcm_host.so", RTLD_NOW);

    if (dl == NULL)
    {
        dl = dlopen("/usr/lib/arm-linux-gnueabihf/libbcm_host.so", RTLD_NOW);
    }

    if (dl == NULL)
    {
        dl = dlopen("/usr/lib/aarch64-linux-gnu/libbcm_host.so", RTLD_NOW);
    }

    if (dl != NULL)
    {
        bhgpa func = dlsym(dl, "bcm_host_get_peripheral_address");

        if (func != NULL)
        {
            base = func();
        }

        dlclose(dl);
    }

    return base;
}


//-----------------------------------------------------------------------

int
main(void)
{
    RASPBERRY_PI_INFO_T info;

    if (getRaspberryPiInformation(&info) > 0)
    {
        printf("memory: %s\n", raspberryPiMemoryToString(info.memory));

        printf("processor: %s\n",
               raspberryPiProcessorToString(info.processor));

        printf("i2cDevice: %s\n",
               raspberryPiI2CDeviceToString(info.i2cDevice));

        printf("model: %s\n",
               raspberryPiModelToString(info.model));

        printf("manufacturer: %s\n",
               raspberryPiManufacturerToString(info.manufacturer));

        printf("pcb revision: %d\n", info.pcbRevision);

        printf("warranty void: %s\n", (info.warrantyBit) ? "yes" : "no");

        printf("revision: %04x\n", info.revisionNumber);

        printf("extended revision: 0x%08"PRIX32"\n", info.extendedRevisionNumber);

        printf("emmc fitted (CM4 only): %s\n", (info.emmcFittedBit) ? "yes" : "no");

        printf("wifi fitted (CM4 only): %s\n", (info.wifiFittedBit) ? "yes" : "no");

        printf("country code (400 only): %d\n", info.countryCode);

        printf("peripheral base: 0x%"PRIX32"\n", info.peripheralBase);

        printf("\n");
        printf("checking peripheral base against bcm_host library\n");

        uint32_t base = getPeripheralBaseFromBcmHost();

        if (base == 0)
        {
            printf("... function not found in bcm_host library\n");
        }
        else if (base != info.peripheralBase)
        {
            printf("... error peripheral base does not match base from bcm_host\n");
        }
        else
        {
            printf("... peripheral base matches base from bcm_host\n");
        }
    }
    else
    {
        printf("getRaspberryPiInformation() failed ...\n");
    }

    return 0;
}
