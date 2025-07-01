#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ContentService.h"
#include <stack>

namespace Fiea::Engine::Content
{
    class Scope;
    class ScopeWriter : public ParseWriter
    {
    private:
        std::stack<Scope*> _CurrentScopes;
        Scope* _RootScope;
        std::string _LastKey;
        size_t _ArrIdx;

    public:
        ScopeWriter();
        ~ScopeWriter();
        void AddScope(const std::string& key, const std::string& type);
        void RemoveObject();
        void AddGameObject(const std::string& key);
        void AddActionObject(const std::string& key, const std::string& type);
        void AddActionToPredecessor(const std::string& key, const std::string& type);
        void AddActionListObject(const std::string& key, const std::string& type);
        void AddWhileActionParamObject(const std::string& key, const std::string& type);
        void AddPrimitiveType(const std::string& key, const Json::Value& value);
        void AddVectorType(const std::string& key, const Json::Value& value);
        void AddMatrixType(const std::string& key, const Json::Value& value);

        Scope* GetTopScope();
    };
}