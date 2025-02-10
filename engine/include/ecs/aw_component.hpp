#pragma once

#include <nlohmann/json.hpp>
#include "utils/uuid.hpp"

namespace Airwave
{
class AwEntity;
class AwComponent
{

  public:
    AwComponent() : uuid(UUID::Generate()) {}
    virtual ~AwComponent() = default;

    virtual void to_json(nlohmann::json &j) {}
    virtual void from_json(const nlohmann::json &j) {}

    UUID uuid;

  protected:
    friend class AwEntity;
    AwEntity *m_owner{nullptr};
};

} // namespace Airwave
