# Console application example

## Dependencies
Gcc, G++

## Compiling
`g++ -I"../../include" -o rqr.exe main.cpp`

## Usage
`rqr [--stats] [--details] [--ecc={L|M|Q|H}] [--mode={N|A|B}] [--stress] {...valuesToEncode}`

### Options:
- `--stats` Displays memory statistics for each subsequent code
- `--details` Displays version, mask, ECC level, and pixel size for each subsequent code
- `--ecc=X` where `X` is `L`, `M`, `Q`, or `H` sets the error correction code level
- `--mode=X` where `X` is `N`, `A`, or `B` forces encoding modes to _Numeric_, _Alphanumeric_, and _Byte_ respectively 
- `--stress` stress test, creating 1,000,000 codes to see if memory is leaking

`...valuesToEncode`: Any string that will be encoded. Each string will get its own QR code. Use quotes to include spaces in strings
