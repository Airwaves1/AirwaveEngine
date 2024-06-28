#ifndef UUID_H
#define UUID_H

#include "Utils/Common.h"

namespace Airwave
{
    class UUID
    {
    public:
        UUID();
        UUID(uint32_t uuid);
        UUID(const UUID &) = default;

        operator uint32_t() const { return m_UUID; }

        uint64_t m_UUID;
    };

} // namespace Airwave

namespace std
{
    template <>
    struct hash<Airwave::UUID>
    {
        size_t operator()(const Airwave::UUID &uuid) const
        {
            if (!uuid.m_UUID)
            {
                return 0;
            }
            return (uint32_t)uuid.m_UUID;
        }
    };
}

#endif // UUID_H