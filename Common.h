#ifndef _COMMON_
#define _COMMON_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

namespace SNY
{
#define TOSTRING(src) #src

#define REAL float

#ifndef _WIN32
#define UCHAR unsigned char
#define UINT unsigned int
#define ULONG unsigned long
#endif

const int XAxis = 0;
const int YAxis = 1;
const int ZAxis = 2;

} // namespace SNY

#endif
