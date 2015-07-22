#include "Singleton.h"

namespace eagle
{

MutexLock g_singletonLock(1, 1);

}
