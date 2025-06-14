#include "Singletons.h"

namespace Neodot::IOC
{
    Singletons& Sing()
    {
        static Singletons sing;
        return sing;
    }
}