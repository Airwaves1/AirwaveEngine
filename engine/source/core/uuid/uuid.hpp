#pragma once

#include <string>
#include <array>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace Airwave
{

class UUID
{
  public:
    UUID() : m_uuid(GenerateUUID()) {}

    // 生成 UUID
    static UUID Generate() { return UUID(); }

    // 获取无效 UUID（所有字节为零）
    static UUID Invalid()
    {
        UUID id;
        id.m_uuid.fill(0);
        return id;
    }

    // 检查 UUID 是否有效（如果 UUID 的所有字节都为零，则认为无效）
    bool isValid() const
    {
        return std::any_of(m_uuid.begin(), m_uuid.end(), [](uint8_t byte) { return byte != 0; });
    }

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

    // 从字符串转换为 UUID
    void fromString(const std::string &str)
    {
        std::array<uint8_t, 16> uuid{};
        size_t j = 0;

        for (size_t i = 0; i + 1 < str.size() && j < 16; ++i)
        {
            if (str[i] == '-') continue;

            std::string byteStr = str.substr(i, 2);
            uuid[j++]           = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            ++i;
        }

        m_uuid = uuid;
    }

    // 比较两个 UUID 是否相等
    bool operator==(const UUID &other) const { return m_uuid == other.m_uuid; }
    bool operator!=(const UUID &other) const { return m_uuid != other.m_uuid; }
    bool operator<(const UUID &other) const { return m_uuid < other.m_uuid; }
    bool operator>(const UUID &other) const { return m_uuid > other.m_uuid; }

    // 提供哈希支持，用于 unordered_map/unordered_set
    const std::array<uint8_t, 16> &getData() const { return m_uuid; }

    friend std::ostream &operator<<(std::ostream &os, const UUID &uuid) { return os << uuid.toString(); }

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

// 为 UUID 提供哈希支持
namespace std
{
template <> struct hash<Airwave::UUID>
{
    std::size_t operator()(const Airwave::UUID &uuid) const noexcept
    {
        const auto &data = uuid.getData();
        std::size_t hash = 0;
        for (auto b : data)
        {
            hash ^= std::hash<uint8_t>()(b) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};
    

} // namespace std
