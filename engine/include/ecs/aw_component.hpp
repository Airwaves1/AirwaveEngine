#pragma once

#include <nlohmann/json.hpp>
#include "utils/uuid.hpp"

namespace Airwave
{
class AwComponent
{
  public:
    AwComponent() : uuid(UUID::Generate()) {}
    virtual ~AwComponent() = default;

    virtual void to_json(nlohmann::json &j) {}
    virtual void from_json(const nlohmann::json &j) {}

    UUID uuid;
};

} // namespace Airwave
