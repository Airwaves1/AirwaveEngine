#pragma once

#include <string>
#include <memory>
#include <optional>
#include <unordered_map>
#include "core/event/event_dispatcher.hpp"
#include "core/event/event_observer.hpp"
#include "utils/uuid.hpp"

namespace Airwave
{
class ResourceEvent : public Event
{
  public:
    enum class Type
    {
        LoadBegin,
        LoadSuccess,
        LoadFailed,
        Reload,
        Dispose
    };
    std::string getType() const override { return "ResourceEvent"; }
};

class Resource
{
  public:
    Resource() : m_uuid(UUID::Generate()) { m_eventDispatcher = std::make_shared<EventDispatcher>(); }
    virtual ~Resource() = default;

    bool load(const std::string &path, const std::any &params = {})
    {
        if (m_loaded)
        {
            return true;
        }

        m_loaded = onLoad(path, params);

        return m_loaded;
    }

  protected:
    virtual bool onLoad(const std::string &path, const std::any &params) = 0;

    std::shared_ptr<EventDispatcher> m_eventDispatcher;
    UUID m_uuid;

    bool m_loaded = false;
};

} // namespace Airwave
