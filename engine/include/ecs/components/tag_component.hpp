#pragma once

#include <string>
#include "ecs/aw_component.hpp"
#include "utils/uuid.hpp"

namespace Airwave
{
class TagComponent : public AwComponent
{
  public:
    std::string tag;
    std::string name;
    UUID uuid;

    TagComponent() = default;
    TagComponent(const std::string &name, const std::string &tag = "")
        : name(name), tag(tag), uuid(UUID::Generate())
    {
    }

    void to_json(nlohmann::json &j) override
    {
        j["name"] = name;
        j["tag"]  = tag;
        j["uuid"] = uuid.toString();
    }

    void from_json(const nlohmann::json &j) override
    {
        name = j.at("name").get<std::string>();
        tag  = j.at("tag").get<std::string>();
        uuid.fromString(j.at("uuid").get<std::string>());
    }
};

} // namespace Airwave
