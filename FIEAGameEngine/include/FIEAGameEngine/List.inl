#include "FieaGameEngine/List.h"
#include "List.h"

namespace Fiea::Engine
{
    /**
     * @brief Default constructor for the List class.
     *
     * Initializes an empty list with no nodes.
     */
    template<typename T>
    List<T>::List()
    {
        _Head = nullptr;
        _Tail = nullptr;
        _Size = 0;
    }

    /**
     * @brief Copy constructor for the List class.
     *
     * Creates a deep copy of the provided list.
     *
     * @param listToCopy The list to copy from.
     */
    template<typename T>
    List<T>::List(const List& listToCopy)
    {
        _Head = nullptr;
        _Tail = nullptr;
        _Size = 0;

        if (listToCopy._Head == nullptr)
            return;

        _Node* HeadNode = listToCopy._Head;
        _Node* ParentPrevNode = nullptr;
        _Node* ParentTailNode = nullptr;
        bool FirstNodeConstructed = false;

        while (HeadNode != nullptr)
        {
            _Node* NewNode = new _Node();
            NewNode->_Data = HeadNode->_Data;
            if (!FirstNodeConstructed)
            {
                FirstNodeConstructed = true;
                _Head = NewNode;
                _Head->_Next = nullptr;
                _Head->_Prev = nullptr;
                _Tail = NewNode;
                _Tail->_Next = nullptr;
                _Tail->_Prev = nullptr;
                _Size = 1;
                ParentPrevNode = NewNode;
                ParentTailNode = NewNode;
            }
            else
            {
                ParentPrevNode->_Next = NewNode;
                NewNode->_Prev = ParentPrevNode;
                NewNode->_Next = nullptr;
                ParentPrevNode = NewNode;
                ++_Size;
            }
            
            HeadNode = HeadNode->_Next;
        }
        _Tail = ParentPrevNode;
    }

    /**
     * @brief Destructor for the List class.
     *
     * Deletes all nodes in the list and frees allocated memory.
     */
    template<typename T>
    List<T>::~List()
    {
        if (_Head == nullptr) return;

        _Node* HeadNode = _Head;

        while(HeadNode != nullptr)
        {
            _Node* NextNode = HeadNode->_Next;
            delete HeadNode;

            HeadNode = NextNode;
        }
    }

    /**
     * @brief Equality operator for comparing two lists.
     *
     * Checks if two lists are equal by comparing their sizes and node data.
     *
     * @param rhs The list to compare with.
     * @return True if the lists are equal, otherwise false.
     */
    template<typename T>
    bool List<T>::operator==(const List& rhs) const
    {
        if (_Size != rhs._Size)
            return false;

        if (_Size == 0 && rhs._Size == 0)
            return true;

        _Node* LhsHead = _Head;
        _Node* RhsHead = rhs._Head;

        while (LhsHead != nullptr && RhsHead != nullptr)
        {
            if (LhsHead->_Data != RhsHead->_Data)
                return false;
            LhsHead = LhsHead->_Next;
            RhsHead = RhsHead->_Next;
        }
        return true;
    }

    /**
     * @brief Inequality operator for comparing two lists.
     *
     * Checks if two lists are not equal by comparing their sizes and node data.
     *
     * @param rhs The list to compare with.
     * @return True if the lists are not equal, otherwise false.
     */
    template<typename T>
    bool List<T>::operator!=(const List& rhs) const
    {
        if (_Size != rhs._Size)
            return true;

        if (_Size == 0 && rhs._Size == 0)
            return false;

        _Node* LhsHead = _Head;
        _Node* RhsHead = rhs._Head;

        while (LhsHead != nullptr && RhsHead != nullptr)
        {
            if (LhsHead->_Data != RhsHead->_Data)
                return true;
            LhsHead = LhsHead->_Next;
            RhsHead = RhsHead->_Next;
        }
        return false;
    }

    /**
     * @brief Overloaded assignment operator to copy the contents of another list.
     * @param rhs The list to copy from.
     * @return A reference to the current list.
     */
    template<typename T>
    typename List<T>::List& List<T>::operator=(const List& rhs)
    {
        if (this != &rhs)
        {
            // Delete previously created list
            if (_Head != nullptr)
            {
                _Node* HeadNode = _Head;

                while (HeadNode != nullptr)
                {
                    _Node* NextNode = HeadNode->_Next;
                    delete HeadNode;

                    HeadNode = NextNode;
                }
                _Size = 0;
                _Head = nullptr;
                _Tail = nullptr;
            }

            bool FirstNodeConstructed = false;
            _Node* ParentPrevNode = nullptr;
            _Node* ParentTailNode = nullptr;
            _Node* RhsHeadNode = rhs._Head;
            while (RhsHeadNode != nullptr)
            {
                _Node* NewNode = new _Node();
                NewNode->_Data = RhsHeadNode->_Data;
                
                if (!FirstNodeConstructed)
                {
                    FirstNodeConstructed = true;
                    _Head = NewNode;
                    _Head->_Next = nullptr;
                    _Head->_Prev = nullptr;
                    _Tail = NewNode;
                    _Tail->_Next = nullptr;
                    _Tail->_Prev = nullptr;
                    _Size = 1;
                    ParentPrevNode = NewNode;
                    ParentTailNode = NewNode;
                }
                else
                {
                    ParentPrevNode->_Next = NewNode;
                    NewNode->_Prev = ParentPrevNode;
                    NewNode->_Next = nullptr;
                    ParentPrevNode = NewNode;
                    ++_Size;
                }
                RhsHeadNode = RhsHeadNode->_Next;
            }
        }
        return *this;
    }

    /**
     * @brief Gets the number of elements in the list.
     * @return The number of elements in the list.
     */
    template<typename T>
    typename List<T>::size_type List<T>::size() const
    {
        return _Size;
    }

    /**
     * @brief Checks if the list is empty.
     * @return True if the list is empty, false otherwise.
     */
    template<typename T>
    bool List<T>::empty() const
    {
        if (_Head == nullptr)
            return true;
        else 
            return false;
    }

    /**
     * @brief Gets an iterator to the beginning of the list.
     * @return An iterator pointing to the first element in the list.
     */
    template<typename T>
    typename List<T>::Iterator List<T>::begin()
    {
        return Iterator(_Head, this);
    }

    /**
     * @brief Gets an iterator to the end of the list.
     * @return An iterator pointing to the position after the last element.
     */
    template<typename T>
    typename List<T>::Iterator List<T>::end()
    {
        return Iterator(nullptr, this);
    }

    /*template<typename T>
    typename List<T>::Iterator List<T>::insert(Iterator Where, const reference Val)
    {
        if (Where._ParentList != this)
        {
            throw std::runtime_error("Error: Iterator does not belong to this list");
        }

        _Node* NewNode = new _Node();
        NewNode->_Data = Val;

        if (Where._Current == _Head)
        {
            NewNode->_Next = _Head;
            NewNode->_Prev = nullptr;

            if (_Head != nullptr)
            {
                _Head->_Prev = NewNode;
            }
            _Head = NewNode;

            if (_Tail == nullptr)
            {
                _Tail = NewNode;
            }
        }
        else if (Where._Current == nullptr)
        {
            NewNode->_Next = nullptr;
            NewNode->_Prev = _Tail;

            if (_Tail != nullptr)
            {
                _Tail->_Next = NewNode;
            }
            _Tail = NewNode;

            if (_Head == nullptr)
            {
                _Head = NewNode;
            }
        }
        else
        {
            _Node* NextNode = Where._Current;
            _Node* PrevNode = NextNode->_Prev;

            NewNode->_Next = NextNode;
            NewNode->_Prev = PrevNode;

            if (PrevNode != nullptr)
            {
                PrevNode->_Next = NewNode;
            }
            NextNode->_Prev = NewNode;
        }
        ++_Size;
        return Iterator(NewNode, this);
    }

    template<typename T>
    typename List<T>::Iterator List<T>::erase(Iterator Where)
    {
        return Iterator();
    }*/

    /**
     * @brief Adds a new element to the end of the list.
     * @param value The value to add to the list.
     */
    template<typename T>
    void List<T>::push_back(const value_type& value)
    {
        _Node* NewNode = new _Node();
        NewNode->_Data = value;

        if (_Head == nullptr)
        {
            _Head = _Tail = NewNode;
            _Head->_Prev = nullptr;
            _Head->_Next = nullptr;
            _Tail->_Prev = nullptr;
            _Tail->_Next = nullptr;
            _Size = 1;
        }
        else
        {
            _Tail->_Next = NewNode;
            NewNode->_Prev = _Tail;
            NewNode->_Next = nullptr;
            _Tail = NewNode;
            ++_Size;
        }
    }

    /**
     * @brief Adds a new element to the front of the list.
     * @param value The value to add to the list.
     */
    template<typename T>
    void Fiea::Engine::List<T>::push_front(const value_type& value)
    {
        _Node* NewNode = new _Node();
        NewNode->_Data = value;

        if (_Head == nullptr)
        {
            _Head = _Tail = NewNode;
            _Head->_Prev = nullptr;
            _Head->_Next = nullptr;
            _Tail->_Prev = nullptr;
            _Tail->_Next = nullptr;
            _Size = 1;
        }
        else
        {
            NewNode->_Next = _Head;
            NewNode->_Prev = nullptr;
            _Head->_Prev = NewNode;
            _Head = NewNode;
            ++_Size;
        }
    }

    /**
     * @brief Removes the last element from the list.
     */
    template<typename T>
    void List<T>::pop_back()
    {
        if (_Head == nullptr)
            return;

        _Node* SecondToLastNode = _Tail->_Prev;
        if (SecondToLastNode != nullptr)
        {
            SecondToLastNode->_Next = nullptr;
            delete _Tail;
            _Tail = SecondToLastNode;
        }
        else
        {
            delete _Tail;
            _Head = nullptr;
            _Tail = nullptr;
        }
    }

    /**
     * @brief Removes the first element from the list.
     */
    template<typename T>
    void List<T>::pop_front()
    {
        if (_Head == nullptr)
            return;

        _Node* SecondNode = _Head->_Next;
        if (SecondNode != nullptr)
        {
            SecondNode->_Prev = nullptr;
            delete _Head;
            _Head = SecondNode;
        }
        else
        {
            delete _Head;
            _Head = nullptr;
            _Tail = nullptr;
        }
    }

    /**
     * @brief Returns a reference to the first element of the list.
     * @return A reference to the first element.
     * @throws std::runtime_error If the list is empty.
     */
    template<typename T>
    typename List<T>::value_type& List<T>::front()
    {
        if (_Head == nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");

        else
            return _Head->_Data;
    }

    /**
     * @brief Returns a const reference to the first element of the list.
     * @return A const reference to the first element.
     * @throws std::runtime_error If the list is empty.
     */
    template<typename T>
    typename const List<T>::value_type& List<T>::front() const
    {
        if (_Head == nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");

        else
            return _Head->_Data;
    }

    /**
     * @brief Returns a reference to the last element of the list.
     * @return A reference to the last element.
     * @throws std::runtime_error If the list is empty.
     */
    template<typename T>
    typename List<T>::value_type& List<T>::back()
    {
        if (_Tail == nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");

        else
            return _Tail->_Data;
    }

    /**
     * @brief Returns a const reference to the last element of the list.
     * @return A const reference to the last element.
     * @throws std::runtime_error If the list is empty.
     */
    template<typename T>
    typename const List<T>::value_type& List<T>::back() const
    {
        if (_Tail == nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");

        else
            return _Tail->_Data;
    }

    /**
     * @brief Constructs an iterator for the list.
     * @param ItrNode The node to start the iteration from.
     * @param ParentList The parent list to which this iterator belongs.
     */
    template<typename T>
    List<T>::Iterator::Iterator(_Node* ItrNode, List<T>* ParentList)
    {
        _Current = ItrNode;
        _ParentList = ParentList;
    }

    /**
     * @brief Destructor for the iterator.
     */
    template<typename T>
    List<T>::Iterator::~Iterator()
    {
        
    }

    /**
     * @brief Prefix increment operator to move to the next node.
     * @return A reference to the updated iterator.
     * @throws std::runtime_error if the iterator is at the end of the list.
     */
    template<typename T>
    typename List<T>::Iterator& List<T>::Iterator::operator++()
    {
        if (_Current == nullptr)
        {
            throw std::runtime_error("Cannot increment end() of a list");
        }
        _Current = _Current->_Next;
        return *this;
    }

    /**
     * @brief Postfix increment operator to move to the next node.
     * @return A copy of the iterator before incrementing.
     * @throws std::runtime_error if the iterator is at the end of the list.
     */
    template<typename T>
    typename List<T>::Iterator List<T>::Iterator::operator++(int)
    {
        if (_Current == nullptr)
        {
            throw std::runtime_error("Cannot increment end() of a list");
        }
        Iterator Temp = *this;
        _Current = _Current->_Next;

        return Temp;
    }

    /**
     * @brief Prefix decrement operator to move to the previous node.
     * @return A reference to the updated iterator.
     * @throws std::runtime_error if the iterator is at the beginning of the list or the list is empty.
     */
    template<typename T>
    typename List<T>::Iterator& List<T>::Iterator::operator--()
    {
        if (_Current == nullptr)
        {
            if (_ParentList == nullptr || _ParentList->_Tail == nullptr)
            {
                throw std::runtime_error("Cannot decrement end() of an empty list.");
            }

            _Current = _ParentList->_Tail;
        }
        else
        {
            _Current = _Current->_Prev;
        }
        return *this;
    }

    /**
     * @brief Postfix decrement operator to move to the previous node.
     * @return A copy of the iterator before decrementing.
     * @throws std::runtime_error if the iterator is at the beginning of the list or the list is empty.
     */
    template<typename T>
    typename List<T>::Iterator List<T>::Iterator::operator--(int)
    {
        if (_Current == nullptr)
        {
            if (_ParentList == nullptr || _ParentList->_Tail == nullptr)
            {
                throw std::runtime_error("Cannot decrement end() of an empty list.");
            }

            _Current = _ParentList->_Tail;
            return *this;
        }
        else
        {
            Iterator Temp = *this;
            _Current = _Current->_Next;

            return Temp;
        }
    }

    /**
     * @brief Assignment operator to copy another iterator.
     * @param Rhs The iterator to copy from.
     * @return A reference to the current iterator.
     */
    template<typename T>
    typename List<T>::Iterator& List<T>::Iterator::operator=(const Iterator& Rhs)
    {
        if (this != &Rhs)
        {
            _Current = Rhs._Current;
            _ParentList = Rhs._ParentList;
        }
        return *this;
    }

    /**
     * @brief Dereference operator to access the data of the current node.
     * @return A reference to the data of the current node.
     * @throws std::runtime_error if the iterator is pointing to an empty list.
     */
    template<typename T>
    typename List<T>::reference List<T>::Iterator::operator*() const
    {
        if(_Current==nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");
        
        return _Current->_Data;
    }

    /**
     * @brief Arrow operator to access the data of the current node.
     * @return A pointer to the data of the current node.
     * @throws std::runtime_error if the iterator is pointing to an empty list.
     */
    template<typename T>
    typename List<T>::pointer List<T>::Iterator::operator->() const
    {
        if (_Current == nullptr)
            throw std::runtime_error("Error: Attempted to access the front of an empty list.");

        return &(_Current->_Data);
    }
}