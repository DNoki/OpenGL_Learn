#pragma once

#include <vector>
#include <algorithm>

namespace OpenGL_Learn
{
    using namespace std;

    template <typename T>
    class List : public vector<T>
    {
    public:
        size_t Count() { return this->size(); }

        bool Exists(const T& item)
        {
            for (auto& i : *this)
                if (item == i) return true;
            return false;
        }
        T* Find(const T& item)
        {
            for (auto& i : *this)
                if (item == i) return &i;
            return nullptr;
        }
        template <typename _Pr>
        T* Find(_Pr _Pred) // bool (*_Pred)(const T&)
        {
            for (auto& i : *this)
                if (_Pred(i))
                    return &i;
            return nullptr;
        }

        void Remove(const T& item)
        {
            for (size_t i = 0; i < this->size(); i++)
                if (item == (*this)[i])
                {
                    this->erase(this->begin() + i);
                    return;
                }
        }

        template <typename _Pr>
        void Sort(_Pr _Pred) // bool (*_Pred)(const T&, const T&)
        {
            sort(this->begin(), this->end(), _Pred);
        }

        List() :vector<T>() {}
        List(const allocator<T>& _Al) :vector<T>(_Al) {}
        List(vector<T>&& _Right) :vector<T>(_Right) {}
        List(const vector<T>& _Right) :vector<T>(_Right) {}
        List(vector<T>&& _Right, const allocator<T>& _Al) :vector<T>(_Right, _Al) {}
        List(const vector<T>& _Right, const allocator<T>& _Al) :vector<T>(_Right, _Al) {}
        List(initializer_list<T>& _Ilist, const allocator<T>& _Al) :vector<T>(_Ilist, _Al) {}
        List(const int _Count, const allocator<T>& _Al) :vector<T>(_Count, _Al) {}
        List(const int _Count, const T& _Val, const allocator<T>& _Al) :vector<T>(_Count, _Val, _Al) {}
    };

    class IList : public vector<int>
    {
    public:
        void Add(const int& item) { this->push_back(item); }
        size_t GetCount() { return this->size(); }

        bool Exists(const int& item)
        {
            for (auto& i : *this)
                if (item == i) return true;
            return false;
        }
        int* Find(const int& item)
        {
            for (auto& i : *this)
                if (item == i) return &i;
            return nullptr;
        }

        IList() :vector<int>() {}
        IList(const allocator<int>& _Al) :vector<int>(_Al) {}
        IList(vector<int>&& _Right) :vector<int>(_Right) {}
        IList(const vector<int>& _Right) :vector<int>(_Right) {}
        IList(vector<int>&& _Right, const allocator<int>& _Al) :vector<int>(_Right, _Al) {}
        IList(const vector<int>& _Right, const allocator<int>& _Al) :vector<int>(_Right, _Al) {}
        IList(initializer_list<int>& _Ilist, const allocator<int>& _Al) :vector<int>(_Ilist, _Al) {}
        IList(const size_type _Count, const allocator<int>& _Al) :vector<int>(_Count, _Al) {}
        IList(const size_type _Count, const int& _Val, const allocator<int>& _Al) :vector<int>(_Count, _Val, _Al) {}
    };


}
