#pragma once

namespace Fiea::Engine
{
    template<typename T>
    class List
    {
    private:
        struct _Node
        {
            T _Data;
            _Node* _Prev;
            _Node* _Next;
        };

        _Node* _Head;
        _Node* _Tail;
        size_t _Size;

    public:
        // Type aliases
        using size_type = size_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        class Iterator
        {
        private:
            _Node* _Current;
            List<T>* _ParentList;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(_Node* ItrNode, List<T>* ParentList);
            ~Iterator();

            Iterator& operator++();
            Iterator operator++(int);
            Iterator& operator--();
            Iterator operator--(int);
            Iterator& operator=(const Iterator& Rhs);

            reference operator*() const;
            pointer operator->() const;

            friend bool operator==(const Iterator& Itr1, const Iterator& Itr2)
            {
                return Itr1._Current == Itr2._Current;
            }

            friend bool operator!=(const Iterator& Itr1, const Iterator& Itr2)
            {
                return Itr1._Current != Itr2._Current;
            }
        };

        List();
        List(const List& listToCopy);
        ~List();
        bool operator==(const List& rhs) const;
        bool operator!=(const List& rhs) const;
        List& operator=(const List& rhs);
        void push_back(const value_type& value);
        void push_front(const value_type& value);
        void pop_back();
        void pop_front();
        value_type& front();
        const value_type& front() const;
        value_type& back();
        const value_type& back() const;
        size_type size() const;
        bool empty() const;
        Iterator begin();
        Iterator end();
        //Iterator insert(Iterator Where, const reference Val);
        //Iterator erase(Iterator Where);
    };
}

#include "FIEAGameEngine/List.inl"