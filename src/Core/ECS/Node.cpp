#include "Node.h"

namespace Airwave
{


    UUID Node::GetID() const
    {
        return m_ID;
    }

    void Node::SetID(const UUID &nodeID)
    {
        m_ID = nodeID;
    }

    const std::string &Node::GetName() const
    {
        return m_name;
    }

    void Node::SetName(const std::string &name)
    {
        m_name = name;
    }

    const std::vector<Node *> &Node::GetChildren() const
    {
        return m_childrenArray;
    }

    bool Node::HasParent() const
    {
        return m_parent != nullptr;
    }

    bool Node::HasChildren() const
    {
        return !m_childrenArray.empty();
    }

    void Node::SetParent(Node *parent)
    {
        parent->AddChild(this);
    }

    Node *Node::GetParent() const
    {
        return m_parent;
    }

    void Node::AddChild(Node *child)
    {
        if(child->HasParent()){
            child->GetParent()->RemoveChild(child);
        }
        child->m_parent = this;
        m_childrenArray.push_back(child);
    }

    void Node::RemoveChild(Node *child)
    {
        if(!HasChildren()){
            return;
        }
        auto it = std::find(m_childrenArray.begin(), m_childrenArray.end(), child);
        if(it != m_childrenArray.end()){
            m_childrenArray.erase(it);
            child->m_parent = nullptr;
        }
    }

} // namespace Airwave
