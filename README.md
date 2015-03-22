# raspberry_pi_revision

C function to decode the Revision: field of /proc/cpuinfo on the Raspberry
Pi. 

The file /proc/cpuinfo contains a line such as:-

Revision    : 0003


## Prior to Raspberry Pi 2

that holds the revision number of the Raspberry Pi.
Known revisions (prior to the Raspberry Pi 2) are:

| Revision |  Model  | PCB Rev | Memory | Manufacture |
|:--------:|:-------:|:-------:|:------:|:------------|
|   0000   |         |         |        |             |
|   0001   |         |         |        |             |
|   0002   |    B    |    1    | 256 MB |             |
|   0003   |    B    |    1    | 256 MB |             |
|   0004   |    B    |    2    | 256 MB |   Sony      |
|   0005   |    B    |    2    | 256 MB |   Qisda     |
|   0006   |    B    |    2    | 256 MB |   Egoman    |
|   0007   |    A    |    2    | 256 MB |   Egoman    |
|   0008   |    A    |    2    | 256 MB |   Sony      |
|   0009   |    A    |    2    | 256 MB |   Qisda     |
|   000a   |         |         |        |             |
|   000b   |         |         |        |             |
|   000c   |         |         |        |             |
|   000d   |    B    |    2    | 512 MB |   Egoman    |
|   000e   |    B    |    2    | 512 MB |   Sony      |
|   000f   |    B    |    2    | 512 MB |   Qisda     |
|   0010   |    B+   |    1    | 512 MB |   Sony      |
|   0011   | compute |    1    | 512 MB |   Sony      |
|   0012   |    A+   |    1    | 256 MB |   Sony      |
|   0013   |    B+   |    1    | 512 MB |   Embest    |
|   0014   | compute |    1    | 512 MB |   Sony      |

If the Raspberry Pi has been over-volted (voiding the warranty) the
revision number will have 100 at the front. e.g. 1000002.

## Raspberry Pi 2

With the release of the Raspberry Pi 2, there is a new encoding of the
Revision field in /proc/cpuinfo. The bit fields are as follows


|FEDC|BA98|7654|3210|FEDC|BA98|7654|3210|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|    |    |    |    |    |    |    |AAAA|
|    |    |    |    |    |BBBB|BBBB|    |
|    |    |    |    |CCCC|    |    |    |
|    |    |    |DDDD|    |    |    |    |
|    |    | EEE|    |    |    |    |    |
|    |    |F   |    |    |    |    |    |
|    |   G|    |    |    |    |    |    |
|    |  H |    |    |    |    |    |    |
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|1098|7654|3210|9876|5432|1098|7654|3210|

| # | bits  |   contains   | values                                     |
|:-:|:-----:|:-------------|:-------------------------------------------|
| A | 00-03 | PCB Revision | (the pcb revision number)                  |
| B | 04-11 | Model name   | A, B, A+, B+, B Pi2, Alpha, Compute Module |
| C | 12-15 | Processor    | BCM2835, BCM2836                           |
| D | 16-19 | Manufacturer | Sony, Egoman, Embest, unknown, Embest      |
| E | 20-22 | Memory size  | 256 MB, 512 MB, 1024 MB                    |
| F | 23-23 | encoded flag | (if set, revision is a bit field)          |
| G | 24-24 | waranty bit  | (if set, warranty void - Pre Pi2)          |
| H | 25-25 | waranty bit  | (if set, warranty void - Post Pi2)         |

Also, due to some early issues the warranty bit has been move from bit
24 to bit 25 of the revision number (i.e. 0x2000000).

e.g.

Revision    : A01041

A - PCB Revison - 1 (first revision)
B - Model Name - 4 (Model B Pi 2)
C - Processor - 1 (BCM2836)
D - Manufacturer - 0 (Sony)
E - Memory - 2 (1024 MB)

