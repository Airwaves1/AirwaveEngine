#ifndef NODE_H
#define NODE_H

#include "UUID.h"

namespace Airwave
{

    class Node
    {
    public:
        Node() = default;
        virtual ~Node() = default;

        UUID GetID() const;
        void SetID(const UUID &nodeID);
        const std::string &GetName() const;
        void SetName(const std::string &name);

        const std::vector<Node *> &GetChildren() const;
        bool HasParent() const;
        bool HasChildren() const;
        void SetParent(Node *parent);
        Node *GetParent() const;
        void AddChild(Node *child);
        void RemoveChild(Node *child);

    private:
        UUID m_ID;
        std::string m_name;

        Node *m_parent = nullptr;
        std::vector<Node *> m_childrenArray;
    };
}

#endif // NODE_H