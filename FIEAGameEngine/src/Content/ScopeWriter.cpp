#include "ScopeWriter.h"
#include "FIEAGameEngine/FactoryService.h"
#include "FIEAGameEngine/GameObject.h"
#include "FIEAGameEngine/ActionList.h"

namespace Fiea::Engine::Content
{
    ScopeWriter::ScopeWriter()
    {
        _RootScope = new Scope();
        _CurrentScopes.push(_RootScope);
        _ArrIdx = 0;
    }

    ScopeWriter::~ScopeWriter()
    {
        delete _RootScope;
    }

    void ScopeWriter::AddScope(const std::string& key, const std::string& type)
    {
        Scope* parentScope = _CurrentScopes.top();
        FIEA_ERROR(parentScope != nullptr);

        if (type == "GameObject")
        {
            AddGameObject(key);
            return;
        }

        if (type.empty())
        {
            Scope& newScope = parentScope->AppendScope(key.c_str());
            _CurrentScopes.push(&newScope);
            return;
        }
        else
        {
            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
            Scope* s = fs->Create<Scope>(type);

            parentScope->AppendScope(key.c_str(), s);
            _CurrentScopes.push(s);
        }
    }

    void ScopeWriter::RemoveObject()
    {
        FIEA_ERROR(_CurrentScopes.size() > 0);

        _CurrentScopes.pop();

        _LastKey = "";
        _ArrIdx = 0;
    }

    void ScopeWriter::AddGameObject(const std::string& key)
    {
        IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
        GameObject* createdGO = fs->Create<GameObject>("GameObject");

        FIEA_ERROR(createdGO != nullptr);

        Scope* parentScope = _CurrentScopes.top();
        GameObject* parentGameObject = parentScope->As<GameObject>();

        if (parentGameObject != nullptr)
            parentGameObject->AddChild(createdGO);
        
        parentScope->AppendScope(key.c_str(), createdGO);

        _CurrentScopes.push(createdGO);
    }

    void ScopeWriter::AddActionObject(const std::string& key, const std::string& type)
    {
        Scope* parentScope = _CurrentScopes.top();
        FIEA_ERROR(parentScope != nullptr);
        GameObject* parentGO = parentScope->As<GameObject>();
        ActionList* parentAL = parentScope->As<ActionList>();
        if (parentAL != nullptr)
        {
            AddActionListObject(key, type);
            return;
        }
        if (parentGO == nullptr)
        {
            AddActionToPredecessor(key, type);
            return;
        }

        if (type.empty())
        {
            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
            Action* newAction = fs->Create<Action>("Action");

            parentGO->AddAction(newAction);
            parentScope->AppendScope(key.c_str(), newAction);
            _CurrentScopes.push(newAction);
        }
        else
        {
            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
            Action* newAction = fs->Create<Action>(type);

            parentGO->AddAction(newAction);
            parentScope->AppendScope(key.c_str(), newAction);
            _CurrentScopes.push(newAction);
        }
    }

    void ScopeWriter::AddActionToPredecessor(const std::string& key, const std::string& type)
    {
        Scope* parentScope = _CurrentScopes.top();

        while (parentScope != nullptr)
        {
            GameObject* parentGO = parentScope->As<GameObject>();
            ActionList* parentAL = parentScope->As<ActionList>();

            if (parentGO != nullptr)
            {
                if (type.empty())
                {
                    IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
                    Action* newAction = fs->Create<Action>("Action");

                    parentGO->AddAction(newAction);
                    parentScope->AppendScope(key.c_str(), newAction);
                    _CurrentScopes.push(newAction);
                }
                else
                {
                    IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
                    Action* newAction = fs->Create<Action>(type);

                    parentGO->AddAction(newAction);
                    parentScope->AppendScope(key.c_str(), newAction);
                    _CurrentScopes.push(newAction);
                }
                return;
            }
            else if (parentAL != nullptr)
            {
                IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
                std::string actionListType = (type.empty()) ? "ActionList" : type;
                Action* newAction = fs->Create<Action>(actionListType);
                FIEA_ERROR(newAction != nullptr);

                ActionList* newActionList = newAction->As<ActionList>();
                if (newActionList != nullptr)
                {
                    parentAL->AddAction(newActionList);
                    parentScope->AppendScope(key.c_str(), newActionList);
                    _CurrentScopes.push(newActionList);
                }
                else
                {
                    parentAL->AddAction(newAction);
                    parentScope->AppendScope(key.c_str(), newAction);
                    _CurrentScopes.push(newAction);
                }
                return;
            }
            else
            {
                parentScope = parentScope->GetParent();
            }
        }
        FIEA_ERROR(parentScope != nullptr);
    }

    void ScopeWriter::AddActionListObject(const std::string& key, const std::string& type)
    {
        Scope* parentScope = _CurrentScopes.top();
        FIEA_ERROR(parentScope != nullptr);

        GameObject* parentGO = parentScope->As<GameObject>();
        ActionList* parentAL = parentScope->As<ActionList>();

        IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
        std::string actionListType = (type.empty()) ? "ActionList" : type;
        Action* newAction = fs->Create<Action>(actionListType);
        FIEA_ERROR(newAction != nullptr);

        if (parentGO != nullptr)
        {
            ActionList* newActionList = newAction->As<ActionList>();
            if (newActionList != nullptr)
            {
                parentGO->AddAction(newActionList);
                parentScope->AppendScope(key.c_str(), newActionList);
                _CurrentScopes.push(newActionList);
            } 
            else
            {
                parentGO->AddAction(newAction);
                parentScope->AppendScope(key.c_str(), newAction);
                _CurrentScopes.push(newAction);
            }
        }
        else if (parentAL != nullptr)
        {
            ActionList* newActionList = newAction->As<ActionList>();
            if (newActionList != nullptr)
            {
                parentAL->AddAction(newActionList);
                parentScope->AppendScope(key.c_str(), newActionList);
                _CurrentScopes.push(newActionList);
            }
            else
            {
                parentAL->AddAction(newAction);
                parentScope->AppendScope(key.c_str(), newAction);
                _CurrentScopes.push(newAction);
            }
        }
        else
        {
            FIEA_ERROR(parentGO != nullptr || parentAL != nullptr);
        }
    }

    void ScopeWriter::AddWhileActionParamObject(const std::string& key, const std::string& type)
    {
        Scope* parentScope = _CurrentScopes.top();
        FIEA_ERROR(parentScope != nullptr && !type.empty());

        IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();
        Action* a = fs->Create<Action>(type);

        parentScope->AppendScope(key.c_str(), a);
        _CurrentScopes.push(a);
    }

    Scope* ScopeWriter::GetTopScope()
    {
        return _CurrentScopes.top();
    }

    void ScopeWriter::AddPrimitiveType(const std::string& key, const Json::Value& value)
    {
        FIEA_ERROR(value.type() == Json::ValueType::intValue ||
        value.type() == Json::ValueType::realValue ||
        value.type() == Json::ValueType::stringValue);

        Scope* topScope = GetTopScope();

        FIEA_ERROR(topScope != nullptr);

        Datum& d = topScope->Append(key.c_str());

        if (_LastKey == key)
        {
            ++_ArrIdx;
        }
        else
        {
            _ArrIdx = 0;
        }
        _LastKey = key;

        bool useSet = d.Size() > _ArrIdx;

        if (value.type() == Json::ValueType::intValue)
        {
            d.SetType(Datum::_DatumType::Int);
            if (useSet)
            {
                d.Set(value.as<int32_t>(), _ArrIdx);
            }
            else
            {
                d.Push(value.as<int32_t>());
            }
        }
        else if (value.type() == Json::ValueType::realValue)
        {
            d.SetType(Datum::_DatumType::Float);
            if (useSet)
            {
                d.Set(value.as<float>(), _ArrIdx);
            }
            else
            {
                d.Push(value.as<float>());
            }
        }
        else
        {
            d.SetType(Datum::_DatumType::String);
            if (useSet)
            {
                d.Set<string>(value.as<std::string>().c_str(), _ArrIdx);
            }
            else
            {
                d.Push<string>(value.as<std::string>().c_str());
            }
        }
    }

    void ScopeWriter::AddVectorType(const std::string& key, const Json::Value& value)
    {
        FIEA_ERROR(value.type() == Json::ValueType::stringValue);

        Scope* topScope = GetTopScope();

        FIEA_ERROR(topScope != nullptr);

        Datum& d = topScope->Append(key.c_str());

        if (_LastKey == key)
        {
            ++_ArrIdx;
        }
        else
        {
            _ArrIdx = 0;
        }
        _LastKey = key;

        bool useSet = d.Size() > _ArrIdx;

        d.SetType(Datum::_DatumType::Vec4);
        if (useSet)
        {
            d.SetFromString(value.asString().c_str(), _ArrIdx);
        }
        else
        {
            d.PushFromString(value.asString().c_str());
        }
    }

    void ScopeWriter::AddMatrixType(const std::string& key, const Json::Value& value)
    {
        FIEA_ERROR(value.type() == Json::ValueType::stringValue);

        Scope* topScope = GetTopScope();

        FIEA_ERROR(topScope != nullptr);

        Datum& d = topScope->Append(key.c_str());

        if (_LastKey == key)
        {
            ++_ArrIdx;
        }
        else
        {
            _ArrIdx = 0;
        }
        _LastKey = key;

        bool useSet = d.Size() > _ArrIdx;

        d.SetType(Datum::_DatumType::Mat4x4);
        if (useSet)
        {
            d.SetFromString(value.asString().c_str(), _ArrIdx);
        }
        else
        {
            d.PushFromString(value.asString().c_str());
        }
    }
}