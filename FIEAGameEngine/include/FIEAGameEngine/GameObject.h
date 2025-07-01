#pragma once
#include "FIEAGameEngine/Types.h"
#include "Content/Attributed.h"
#include "FIEAGameEngine/FactoryService.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/ActionList.h"
#include "glm/glm.hpp"

using Vec4 = glm::f32vec4;

namespace Fiea::Engine
{
    using namespace Fiea::Engine::Actions;

    struct Transform
    {
        Vec4 Position;
        Vec4 Rotation;
        Vec4 Scale;

        Transform() : Position(glm::f32vec4(0.0f)), Rotation(glm::f32vec4(0.0f)), Scale(glm::f32vec4(0.0f)){}
    };

    class GameObject : public Fiea::Engine::Content::Attributed
    {
        RTTI_DECLARATIONS(GameObject, Attributed);

    public:
        GameObject();
        GameObject(const GameObject& other);
        GameObject(GameObject&& other) noexcept;
        GameObject& operator=(const GameObject& other);
        GameObject& operator=(GameObject&& other) noexcept;
        virtual ~GameObject() = default;

        virtual GameObject* Clone() const override;

        virtual void Update(float deltaTime);

        void AddChild(GameObject* child);
        bool RemoveChild(GameObject* child);
        size_t GetChildCount() const;
        GameObject* GetChild(size_t index) const;
        void ForEachChild(const std::function<void(GameObject*)>& func);

        Transform& GetTransform();
        const Transform& GetTransform() const;
        void SetTransform(const Transform& transform);

        void AddAction(Action* action);
        bool RemoveAction(Action* action);

    protected:
        Transform _transform;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(GameObject, GameObject);
}