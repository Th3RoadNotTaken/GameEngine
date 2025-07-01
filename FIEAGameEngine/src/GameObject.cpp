#include "FIEAGameEngine/GameObject.h"
#include <cstddef>

namespace Fiea::Engine
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(GameObject);

    ClassDefinition GameObject::CreateDefinitions()
    {
        return {
        { "Position", offsetof(GameObject, _transform) + offsetof(Transform, Position), Datum::_DatumType::Vec4, 1},
        { "Rotation", offsetof(GameObject, _transform) + offsetof(Transform, Rotation), Datum::_DatumType::Vec4, 1 },
        { "Scale", offsetof(GameObject, _transform) + offsetof(Transform, Scale), Datum::_DatumType::Vec4, 1 },
        { "Children", 0, Datum::_DatumType::Table, 1 },
        { "Actions", 0, Datum::_DatumType::Table, 1 }
        };
    }

    GameObject::GameObject() : Attributed(&CreateDefinitions, TypeIdInstance()), _transform()
    {
    }

    GameObject::GameObject(const GameObject& other) : Attributed(other), _transform(other._transform)
    {
    }

    GameObject::GameObject(GameObject&& other) noexcept : Attributed(std::move(other)), _transform(std::move(other._transform))
    {
    }

    GameObject& GameObject::operator=(const GameObject& other)
    {
        if (this != &other)
        {
            Attributed::operator=(other);
            _transform = other._transform;
        }
        return *this;
    }

    GameObject& GameObject::operator=(GameObject&& other) noexcept
    {
        if (this != &other)
        {
            _transform = std::move(other._transform);
            Attributed::operator=(std::move(other));
        }
        return *this;
    }

    GameObject* GameObject::Clone() const
    {
        return new GameObject(*this);
    }

    void GameObject::Update(float deltaTime)
    {
        Datum* childrenDatum = Find("Children");
        FIEA_ERROR(childrenDatum != nullptr);

        Datum* d = Find("Actions");
        if (d != nullptr)
        {
            for (size_t i = 0; i < d->Size(); ++i)
            {
                Action* a = d->GetAsTable(i)->As<Action>();
                FIEA_ASSERT(a != nullptr);

                if (a->Update())
                {
                    RemoveAction(a);
                }
            }
        }
        
        for (size_t i = 0; i < childrenDatum->Size(); ++i)
        {
            GameObject* child = childrenDatum->GetAsTable(i)->As<GameObject>();
            if (child != nullptr)
            {
                child->Update(deltaTime);
            }
        }
    }

    void GameObject::AddChild(GameObject* child)
    {
        Datum& childrenDatum = Append("Children");
        childrenDatum.Push(child);
    }

    bool GameObject::RemoveChild(GameObject* child)
    {
        Datum* childrenDatum = Find("Children");
        if (childrenDatum != nullptr)
        {
            for (size_t i = 0; i < childrenDatum->Size(); ++i)
            {
                Scope* s = childrenDatum->GetAsTable(i);
                if (s != nullptr)
                {
                    if (s->As<GameObject>() == child)
                    {
                        childrenDatum->RemoveAt(i);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    size_t GameObject::GetChildCount() const
    {
        const Datum* childrenDatum = Find("Children");
        return (childrenDatum != nullptr) ? childrenDatum->Size() : 0;
    }

    GameObject* GameObject::GetChild(size_t index) const
    {
        const Datum* childrenDatum = Find("Children");
        if (childrenDatum != nullptr && index < childrenDatum->Size())
        {
            Scope* s = childrenDatum->GetAsTable(index);
            if (s != nullptr)
            {
                return s->As<GameObject>();
            }
            else
                return nullptr;
        }
        return nullptr;
    }

    void GameObject::ForEachChild(const std::function<void(GameObject*)>& func)
    {
        Datum* childrenDatum = Find("Children");
        if (childrenDatum != nullptr)
        {
            for (size_t i = 0; i < childrenDatum->Size(); ++i)
            {
                Scope* s = childrenDatum->GetAsTable(i);
                if (s != nullptr)
                {
                    GameObject* child = s->As<GameObject>();
                    if (child != nullptr)
                    {
                        func(child);
                    }
                }
            }
        }
    }

    Transform& GameObject::GetTransform()
    {
        return _transform;
    }

    const Transform& GameObject::GetTransform() const
    {
        return _transform;
    }

    void GameObject::SetTransform(const Transform& transform)
    {
        _transform = transform;
    }

    void GameObject::AddAction(Action* action)
    {
        FIEA_ASSERT(action != nullptr);

        Datum* actionDatum = Find("Actions");
        
        if (actionDatum == nullptr)
        {
            Datum& d = Append("Actions");
            d.Push(action);
        }
        else
        {
            actionDatum->Push(action);
        }
    }

    bool GameObject::RemoveAction(Action* action)
    {
        FIEA_ASSERT(action != nullptr);

        Datum* actionDatum = Find("Actions");
        
        if (actionDatum == nullptr)
            return false;

        for (size_t i = 0; i < actionDatum->Size(); ++i)
        {
            if (actionDatum[i].GetAsTable(i)->As<Action>() == action)
            {
                actionDatum->RemoveAt(i);
                return true;
            }
        }

        return false;
    }

    FACTORY_DEFINE(GameObject);
    FACTORY_DEFINE_ABSTRACT(GameObject);
}