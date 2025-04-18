#pragma once

#include <string>
#include <atomic>

namespace Airwave
{
enum class ResourceState
{
    Loading,
    Loaded,
    Unloaded,
    Error,
    Ready,
    Releasing,
    Released
};

class IResource
{
  public:
    IResource(const std::string &resource_path) : m_resource_path(resource_path) {}
    virtual ~IResource() = default;
    virtual void load() {}
    virtual void reload() {};
    virtual void release() = 0;

    const std::string &getResourcePath() const { return m_resource_path; }
    ResourceState getState() const { return m_state.load(); }

  protected:
    std::string m_resource_path;
    std::atomic<ResourceState> m_state{ResourceState::Unloaded};
};

} // namespace Airwave
