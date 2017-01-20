#include"global.h"
using namespace global;

namespace global
{
    unsigned int UnsignedIntGenerator() { static unsigned int i = 0; return ++i; }
}