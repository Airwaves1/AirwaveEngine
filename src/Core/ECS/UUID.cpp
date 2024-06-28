#include "UUID.h"
#include <random>

namespace Airwave
{

    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_RandomEngine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_Distribution;

    UUID::UUID()
        : m_UUID(s_Distribution(s_RandomEngine))
    {
    }

    UUID::UUID(uint32_t uuid)
        : m_UUID(uuid)
    {
    }

}