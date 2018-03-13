#pragma once

#include "usingopencv.h"
#include "Pipeline.h"
#include "WatchTimer.h"

#pragma message("** Auto linking lprlib.lib")
#ifdef _DEBUG
#pragma comment(lib,"lprlibd.lib")
#else
#pragma comment(lib,"lprlib.lib")
#endif