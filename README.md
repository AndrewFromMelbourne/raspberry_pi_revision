# raspberry_pi_revision

C function to decode the Revision: field of /proc/cpuinfo on the Raspberry
Pi. 

# Known revisions

The file /proc/cpuinfo contains a line such as:-

Revision    : 0003


## Prior to Raspberry Pi 2

that holds the revision number of the Raspberry Pi.
Known revisions (prior to the Raspberry Pi 2) are:

| Revision |  Model  | PCB Rev | Memory | Manufacturer |
|:--------:|:-------:|:-------:|:------:|:-------------|
|   0000   |         |         |        |              |
|   0001   |         |         |        |              |
|   0002   |    B    |    1    | 256 MB |   Egoman     |
|   0003   |    B    |    1    | 256 MB |   Egoman     |
|   0004   |    B    |    2    | 256 MB |   Sony UK    |
|   0005   |    B    |    2    | 256 MB |   Qisda      |
|   0006   |    B    |    2    | 256 MB |   Egoman     |
|   0007   |    A    |    2    | 256 MB |   Egoman     |
|   0008   |    A    |    2    | 256 MB |   Sony UK    |
|   0009   |    A    |    2    | 256 MB |   Qisda      |
|   000a   |         |         |        |              |
|   000b   |         |         |        |              |
|   000c   |         |         |        |              |
|   000d   |    B    |    2    | 512 MB |   Egoman     |
|   000e   |    B    |    2    | 512 MB |   Sony UK    |
|   000f   |    B    |    2    | 512 MB |   Egoman     |
|   0010   |    B+   |    1    | 512 MB |   Sony UK    |
|   0011   | compute |    1    | 512 MB |   Sony UK    |
|   0012   |    A+   |    1    | 256 MB |   Sony UK    |
|   0013   |    B+   |    1    | 512 MB |   Embest     |
|   0014   | compute |    1    | 512 MB |   Embest     |
|   0015   |    A+   |    1    | 256 MB |   Embest     |
|   0015   |    A+   |    1    | 512 MB |   Embest     |

If the Raspberry Pi has been over-volted (voiding the warranty) the
revision number will have 100 at the front. e.g. 1000002.

## Raspberry Pi 2 / Model Zero

With the release of the Raspberry Pi 2, there is a new encoding of the
Revision field in /proc/cpuinfo. The bit fields are as follows


|``FEDC``|``BA98``|``7654``|``3210``|``FEDC``|``BA98``|``7654``|``3210``|
|---:|---:|---:|---:|---:|---:|---:|---:|
|    |  ``HG``|``FEEE``|``DDDD``|``CCCC``|``BBBB``|``BBBB``|``AAAA``|
|``1098``|``7654``|``3210``|``9876``|``5432``|``1098``|``7654``|``3210``|

| # | bits  |   contains   | values                                                            |
|:-:|:-----:|:-------------|:------------------------------------------------------------------|
| A | 00-03 | PCB Revision | (the pcb revision number)                                         |
| B | 04-11 | Model name   | (see table bellow)                                                |
| C | 12-15 | Processor    | BCM2835, BCM2836, BCM2837, BCM2711, BCM2712                       |
| D | 16-19 | Manufacturer | Sony UK, Egoman, Embest, Sony Japan, Embest, Stadium              |
| E | 20-22 | Memory size  | 256 MB, 512 MB, 1 GB, 2 GB, 4 GB, 8 GB, 16 GB                     |
| F | 23-23 | encoded flag | (if set, revision is a bit field)                                 |
| G | 24-24 | waranty bit  | (if set, warranty void - Pre Pi2)                                 |
| H | 25-25 | waranty bit  | (if set, warranty void - Post Pi2)                                |

Also, due to some early issues the warranty bit has been move from bit
24 to bit 25 of the revision number (i.e. 0x2000000).

| #  | Model Name |
|:--:|:-----------|
| 00 | A          |
| 01 | B          |
| 02 | A+         |
| 03 | B+         |
| 04 | 2B         |
| 05 | Alpha      |
| 06 | CM1        |
| 07 | Unknown    |
| 08 | 3B         |
| 09 | Zero       |
| 0A | CM3        |
| 0B | Unknown    |
| 0C | Zero W     |
| 0D | 3B+        |
| 0E | 3A+        |
| 0F | Unknown    |
| 10 | CM3+       |
| 11 | 4B         |
| 12 | Zero 2 W   |
| 13 | 400        |
| 14 | CM4        |
| 15 | CM4S       |
| 16 | Unknown    |
| 17 | 5          |
| 18 | CM5        |
| 19 | 500        |
| 00 | CM5 Lite   |

Revision    : A01041

    A - PCB Revision - 1 (first revision)
    B - Model Name - 4 (Model B Pi 2)
    C - Processor - 1 (BCM2836)
    D - Manufacturer - 0 (Sony UK)
    E - Memory - 2 (1 GB)
    F - Endcoded flag - 1 (encoded cpu info)

Revision    : A21041

    A - PCB Revision - 1 (first revision)
    B - Model Name - 4 (Model B Pi 2)
    C - Processor - 1 (BCM2836)
    D - Manufacturer - 2 (Embest)
    E - Memory - 2 (1 GB)
    F - Endcoded flag - 1 (encoded cpu info)

Revision    : 900092

    A - PCB Revision - 2 (second revision)
    B - Model Name - 9 (Model Zero)
    C - Processor - 0 (BCM2835)
    D - Manufacturer - 0 (Sony UK)
    E - Memory - 1 (512 MB)
    F - Endcoded flag - 1 (encoded cpu info)

Revision    : A02082

    A - PCB Revision - 2 (second revision)
    B - Model Name - 8 (Model B Pi 3)
    C - Processor - 2 (BCM2837)
    D - Manufacturer - 0 (Sony UK)
    E - Memory - 2 (1 GB)
    F - Endcoded flag - 1 (encoded cpu info)

Revision    : A52082

    A - PCB Revision - 2 (second revision)
    B - Model Name - 8 (Model B Pi 3)
    C - Processor - 2 (BCM2837)
    D - Manufacturer - 5 (Stadium)
    E - Memory - 2 (1 GB)
    F - Endcoded flag - 1 (encoded cpu info)

Revision    : B03111

    A - PCB Revision - 1 (first revision)
    B - Model Name - 17 (Model B Pi 4)
    C - Processor - 3 (BCM2711)
    D - Manufacturer - 0 (Sony UK)
    E - Memory - 3 (2 GB)
    F - Endcoded flag - 1 (encoded cpu info)
