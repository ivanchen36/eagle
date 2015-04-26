#include "Singleton.h"

pthread_mutex_t SingletonInitializer::s_lock;
namespace
{
    SingletonInitializer s_initializer;
}
