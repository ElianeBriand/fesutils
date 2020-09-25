The .bindump data format is structured as follow:

# Overall structure
* Byte 0-3 (`uint32_t`): magic number `0xFE5D0101`
    * Identifies the format ("**FES** **D**ump binary **0101**)
* Byte 4-7 (`uint32_t`): Byte-offset at which the actual data is located
    * I.e. <end of header+1>
* Byte 8-<end_of_header>: Header
* Byte <end_of_header+1>: Data


# Header stucture

* Byte 8-11: Bindump format version
* Byte 12-15: Size in byte of the original file when this bindump was created (easy check for append, and decent check for
modification).

# Data structure

Structured as N bytes per record, each record contain the value identified in the header in the order
in which they appear. No mark of end of record, no mark of end of file