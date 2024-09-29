#pragma once

/*
 * UUID（通用唯一识别码，用于生成和管理唯一的标识符
 *  UUID 通常表示为 16 字节( 128位 )的二进制数据，可以用 32 位十六进制字符串表示，
 *  例如 "123e4567-e89b-12d3-a456-426614174000"
 */

#include <string>
#include <array>
#include <random>
#include <iomanip>
#include <sstream>
namespace Airwave
{

class UUID
{
  public:
    UUID() : m_uuid(GenerateUUID()) {}

    // 生成 UUID
    static UUID Generate() { return UUID(); }

    // 将 UUID 转换为字符串
    std::string toString() const
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < m_uuid.size(); ++i)
        {
            if (i == 4 || i == 6 || i == 8 || i == 10) ss << '-';
            ss << std::setw(2) << static_cast<int>(m_uuid[i]);
        }
        return ss.str();
    }

    // 比较两个 UUID 是否相等
    bool operator==(const UUID &other) const { return m_uuid == other.m_uuid; }
    bool operator!=(const UUID &other) const { return m_uuid != other.m_uuid; }
    bool operator<(const UUID &other) const { return m_uuid < other.m_uuid; }
    bool operator>(const UUID &other) const { return m_uuid > other.m_uuid; }

  private:
    std::array<uint8_t, 16> m_uuid;

    static std::array<uint8_t, 16> GenerateUUID()
    {
        std::array<uint8_t, 16> uuid;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        for (auto &byte : uuid) byte = static_cast<uint8_t>(dis(gen));

        // 设置版本号为 4（随机生成）
        uuid[6] = (uuid[6] & 0x0F) | 0x40;
        // 设置变种号为 1
        uuid[8] = (uuid[8] & 0x3F) | 0x80;

        return uuid;
    }
};

} // namespace Airwave
