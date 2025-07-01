#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Content/Scope.h"
#include "Content/Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ScopeTests)
    {
        TEST_MEMCHECK;

        TEST_METHOD(AppendAndFind)
        {
            Scope s;
            Datum& d1 = s.Append("Health");
            Assert::IsNotNull(s.Find(string("Health")));
            Assert::IsNull(s.Find(string("test")));

            d1 = 100;
            Assert::AreEqual(100, s.Find(string("Health"))->GetAsInt());

            Datum& d2 = s.Append("Mana");
            d2 = 80.8f;
            Assert::IsNotNull(s.Find(string("Mana")));
            Assert::AreEqual(80.8f, s.Find(string("Mana"))->GetAsFloat());

            const Scope constScope = s;
            Assert::AreEqual(100, constScope.Find(string("Health"))->GetAsInt());
            Assert::IsNull(constScope.Find("Test"));
        }

        TEST_METHOD(OpSquareBrackets)
        {
            Scope s1;

            Assert::ExpectException<FieaAbortException>([&]() {
                s1[0];
                });

            s1["Health"] = 100;
            s1["Mana"] = 80.8f;

            Assert::AreEqual(100, s1.Find(string("Health"))->GetAsInt());
            Assert::AreEqual(80.8f, s1.Find(string("Mana"))->GetAsFloat());

            Assert::AreEqual(100, s1[0].GetAsInt());
            Assert::AreEqual(80.8f, s1[1].GetAsFloat());
        }

        TEST_METHOD(Equality)
        {
            {
                Scope s1;
                Scope s2;

                s1["Health"] = 100;
                s1["Stamina"] = 60;
                s1["Mana"] = 80.8f;

                s2["Health"] = 100;

                Assert::AreNotEqual(s1, s2);

                s2["Stamina"] = 60;
                s2["Mana"] = 80.8f;

                Assert::AreEqual(s1, s2);
                Assert::IsFalse(s1 != s2);
            }

            {
                Scope s1;
                Scope s2;

                s1["Health"] = 100;
                s1["Stamina"] = 60;
                s1["Mana"] = 80.8f;

                s2["Health"] = 100;
                s2["Stamina"] = 61;
                s2["Mana"] = 80.9f;

                Assert::AreNotEqual(s1, s2);
                Assert::IsTrue(s1 != s2);
            }
        }

        TEST_METHOD(OrderMatters)
        {
            Scope s1;
            Scope s2;

            s1.Append("Heatlh");
            s1.Append("Mana");

            s2.Append("Mana");
            s2.Append("Health");

            Assert::AreNotEqual(s1, s2);
            Assert::IsTrue(s1 != s2);
        }

        TEST_METHOD(SizeAndClear)
        {
            Scope s1;

            s1["Health"] = 100;
            s1["Mana"] = 80.8f;

            Assert::AreEqual(2_z, s1.Size());

            s1.Clear();

            Assert::AreEqual(0_z, s1.Size());
            Assert::IsNull(s1.Find("Health"));
            Assert::IsNull(s1.Find("Mana"));
        }

        TEST_METHOD(TestEmptyScope)
        {
            Scope s;
            Assert::AreEqual(0_z, s.Size());
            Assert::IsNull(s.GetParent());
        }

        TEST_METHOD(Append)
        {
            Scope s;
            Datum& d = s.Append("Health");
            d = 100;
            Datum* found = s.Find("Health");
            Assert::IsNotNull(found);
            Assert::AreEqual(100, found->GetAsInt(0));
        }

        TEST_METHOD(TestAppendScope)
        {
            Scope s;
            Scope& child = s.AppendScope("Children");
            Assert::AreEqual(&s, child.GetParent());
            Datum* d = s.Find("Children");
            Assert::IsNotNull(d);
            Assert::AreEqual(&child, d->GetAsTable(0));
        }

        TEST_METHOD(TestAdopt)
        {
            Scope parent1;
            Scope parent2;

            Scope* child = new Scope();
            parent1.Adopt("Group", *child);

            Assert::IsNull(parent1.GetParent());
            Assert::AreEqual(child->GetParent(), &parent1);

            // Now adopt the same child into parent2.
            parent2.Adopt("Group2", *child);
            Assert::AreNotEqual(child->GetParent(), &parent1);
            Assert::AreEqual(child->GetParent(), &parent2);

            // The contained scope in parent1 should no longer have the child.
            size_t idx = 0;
            Datum* found = parent1.FindContainedScope(*child, idx);
            Assert::IsNull(found);
        }

        TEST_METHOD(TestOrphan)
        {
            Scope parent;
            Scope* child = new Scope();
            parent.Adopt("Group", *child);
            Assert::IsNotNull(child->GetParent());

            Scope* orphanedChild = child->Orphan();
            Assert::IsNull(child->GetParent());
            // Also check that the parent's contained scope no longer includes this child.
            size_t idx = 0;
            Datum* d = parent.FindContainedScope(*child, idx);
            Assert::IsNull(d);

            delete orphanedChild;
        }

        TEST_METHOD(TestFindContainedScope)
        {
            {
                Scope parent;
                Scope* child = new Scope();
                parent.Adopt("Group", *child);

                size_t idx = 0;
                Datum* d = parent.FindContainedScope(*child, idx);
                Assert::IsNotNull(d);
                Assert::AreEqual(child, d->GetAsTable(idx));
            }

            {
                Scope parent;
                const Scope& child = parent.AppendScope("ChildScope");
                const Scope& parentCpy = parent;

                size_t idx = 0;
                const Datum* result = parentCpy.FindContainedScope(child, idx);

                Assert::IsNotNull(result);
                Assert::AreEqual(0_z, idx);
            }
        }

        TEST_METHOD(TestAncestorDescendent)
        {
            Scope root;
            Scope* child = new Scope();
            root.Adopt("Group", *child);
            Scope* grandchild = new Scope();
            child->Adopt("SubGroup", *grandchild);

            Assert::IsTrue(root.IsAncestorOf(*child));
            Assert::IsTrue(root.IsAncestorOf(*grandchild));
            Assert::IsTrue(child->IsAncestorOf(*grandchild));

            Assert::IsTrue(child->IsDescendentOf(root));
            Assert::IsTrue(grandchild->IsDescendentOf(root));
            Assert::IsTrue(grandchild->IsDescendentOf(*child));
        }

        TEST_METHOD(TestCopyConstructor)
        {
            Scope root;
            root.Append("Health") = 100;
            Scope& child = root.AppendScope("Group");
            child.Append("Mana") = 50;

            Scope copyRoot(root);
            Assert::AreEqual(root, copyRoot);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            Scope root;
            root.Append("Health") = 100;
            Scope& child = root.AppendScope("Group");
            child.Append("Mana") = 50;

            Scope moved(std::move(root));

            Assert::AreEqual((size_t)0, root.Size());
            Assert::AreEqual(100, moved["Health"].GetAsInt(0));

            Datum* d = moved.Find("Group");
            Assert::IsNotNull(d);
            Scope* movedChild = d->GetAsTable(0);

            Assert::AreEqual(&moved, movedChild->GetParent());
        }

        TEST_METHOD(TestCopyAssignment)
        {
            {
                Scope root;
                root.Append("Health") = 100;
                Scope& child = root.AppendScope("Group");
                child.Append("Mana") = 50;

                Scope copy;
                copy = root;

                Assert::AreEqual(copy, root);
            }

            {
                Scope root;
                root.Append("Health") = 100;
                Scope& child = root.AppendScope("Group");
                child.Append("Mana") = 50;

                Scope copy;
                copy = root;
                Assert::AreEqual(copy, root);
            }
        }

        TEST_METHOD(TestMoveAssignment)
        {
            Scope root;
            root.Append("Health") = 100;
            Scope& child = root.AppendScope("Group");
            child.Append("Mana") = 50;

            Scope copy;
            copy = root;

            Scope moved;
            moved = std::move(copy);

            Assert::AreEqual(moved, root);
            Assert::AreEqual((size_t)0, copy.Size());
        }
    };
}