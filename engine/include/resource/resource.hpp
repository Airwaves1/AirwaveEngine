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
class Resource;

enum class ResourceType
{
    Texture,
    Mesh,
    Material,
    Shader,
    Sound,
    Font,
    Unknown
};

class ResourceEvent : public Event
{
  public:
    ResourceEvent(uint32_t handle, UUID uuid) : handle(handle), m_uuid(uuid) {}
    std::string getType() const override { return "ResourceEvent"; }
    uint32_t handle;
    UUID m_uuid;
};

class Resource
{
  public:
    Resource() : m_uuid(UUID::Generate()) { m_eventDispatcher = std::make_shared<EventDispatcher>(); }
    virtual ~Resource() = default;

    virtual ResourceType getType() const = 0;

    virtual void onDispose() {}

    bool isLoaded() const { return m_loaded; }

    uint32_t getHandle() const { return m_handle; }

  protected:
    friend class ResourceManager;

    UUID m_uuid; // 资源唯一标识符

    uint32_t m_handle = 0; // GPU 资源句柄

    bool m_loaded = false;

    std::shared_ptr<EventDispatcher> m_eventDispatcher;
};

} // namespace Airwave
