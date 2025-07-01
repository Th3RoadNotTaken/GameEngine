#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "FIEAGameEngine/GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    class TestGameObject : public GameObject
    {
    public:
        int updateCallCount = 0;
        virtual void Update(float deltaTime) override
        {
            updateCallCount++;
        }
    };

    TEST_CLASS(GameObjectTests)
    {
        //TEST_MEMCHECK;

        TEST_METHOD(DefaultTransform)
        {
            GameObject obj;
            Transform t = obj.GetTransform();

            glm::vec4 zeroVec(0);

            Assert::AreEqual(zeroVec, t.Position);
            Assert::AreEqual(zeroVec, t.Rotation);
            Assert::AreEqual(zeroVec, t.Scale);

            glm::vec4 newPos{1};
            glm::vec4 newRot{2};
            glm::vec4 newScale{3};

            Transform newTransform;
            newTransform.Position = newPos;
            newTransform.Rotation = newRot;
            newTransform.Scale = newScale;

            obj.SetTransform(newTransform);
            Assert::AreEqual(newPos, obj.GetTransform().Position);
            Assert::AreEqual(newRot, obj.GetTransform().Rotation);
            Assert::AreEqual(newScale, obj.GetTransform().Scale);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestAddAndRemoveChild)
        {
            GameObject parent;
            GameObject child1;
            GameObject child2;

            Assert::AreEqual(0_z, parent.GetChildCount());

            parent.AddChild(&child1);
            Assert::AreEqual(1_z, parent.GetChildCount());
            Assert::AreEqual(parent.GetChild(0), &child1);

            parent.AddChild(&child2);
            Assert::AreEqual(2_z, parent.GetChildCount());
            Assert::AreEqual(parent.GetChild(1), &child2);

            bool removed = parent.RemoveChild(&child1);
            Assert::IsTrue(removed);
            Assert::AreEqual(1_z, parent.GetChildCount());
            Assert::AreEqual(parent.GetChild(0), &child2);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestForEachChild)
        {
            GameObject parent;
            GameObject child1;
            GameObject child2;
            parent.AddChild(&child1);
            parent.AddChild(&child2);

            int count = 0;
            parent.ForEachChild([&count](GameObject* child)
                {
                    Assert::IsNotNull(child);
                    ++count;
                });
            Assert::AreEqual(2, count);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestUpdatePropagation)
        {
            GameObject parent;
            TestGameObject child1;
            TestGameObject child2;

            parent.AddChild(&child1);
            parent.AddChild(&child2);

            parent.Update(1.0f);

            Assert::AreEqual(1, child1.updateCallCount);
            Assert::AreEqual(1, child2.updateCallCount);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestCopyConstructor)
        {
            GameObject original;
            Transform custom;
            custom.Position = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
            custom.Rotation = glm::vec4(10.0f, 20.0f, 30.0f, 40.0f);
            custom.Scale = glm::vec4(2.0f, 2.0f, 2.0f, 2.0f);
            original.SetTransform(custom);
            GameObject child;
            original.AddChild(&child);

            GameObject copy(original);

            Transform copyTrans = copy.GetTransform();
            Assert::AreEqual(original.GetTransform().Position, copy.GetTransform().Position);
            Assert::AreEqual(original.GetTransform().Rotation, copy.GetTransform().Rotation);
            Assert::AreEqual(original.GetTransform().Scale, copy.GetTransform().Scale);

            Assert::AreEqual(original.GetChildCount(), copy.GetChildCount());
        
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestCopyAssignment)
        {
            GameObject original;
            Transform custom;
            custom.Position = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
            custom.Rotation = glm::vec4(10.0f, 20.0f, 30.0f, 40.0f);
            custom.Scale = glm::vec4(2.0f, 2.0f, 2.0f, 2.0f);
            original.SetTransform(custom);
            GameObject child;
            original.AddChild(&child);

            GameObject copy;
            copy = original;

            Transform copyTrans = copy.GetTransform();
            Assert::AreNotEqual(&original, &copy);
            Assert::AreEqual(original.GetTransform().Position, copy.GetTransform().Position);
            Assert::AreEqual(original.GetTransform().Rotation, copy.GetTransform().Rotation);
            Assert::AreEqual(original.GetTransform().Scale, copy.GetTransform().Scale);

            Assert::AreEqual(original.GetChildCount(), copy.GetChildCount());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestMoveConstructor)
        {
            GameObject original;
            Transform custom;
            custom.Position = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
            custom.Rotation = glm::vec4(10.0f, 20.0f, 30.0f, 40.0f);
            custom.Scale = glm::vec4(2.0f, 2.0f, 2.0f, 2.0f);
            original.SetTransform(custom);
            GameObject* child = new GameObject();
            original.AddChild(child);

            GameObject copy(std::move(original));

            Assert::AreEqual(custom.Position, copy.GetTransform().Position);
            Assert::AreEqual(custom.Rotation, copy.GetTransform().Rotation);
            Assert::AreEqual(custom.Scale, copy.GetTransform().Scale);

            Assert::AreEqual(1_z, copy.GetChildCount());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestMoveAssignment)
        {
            GameObject original;
            Transform custom;
            custom.Position = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
            custom.Rotation = glm::vec4(10.0f, 20.0f, 30.0f, 40.0f);
            custom.Scale = glm::vec4(2.0f, 2.0f, 2.0f, 2.0f);
            original.SetTransform(custom);
            GameObject* child = new GameObject();
            original.AddChild(child);

            GameObject copy;
            copy = std::move(original);

            Assert::AreEqual(custom.Position, copy.GetTransform().Position);
            Assert::AreEqual(custom.Rotation, copy.GetTransform().Rotation);
            Assert::AreEqual(custom.Scale, copy.GetTransform().Scale);

            Assert::AreEqual(1_z, copy.GetChildCount());

            ServiceManager::ResetServiceMgr();
        }
    };
}