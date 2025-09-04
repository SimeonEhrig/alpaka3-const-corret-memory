#pragma once

#include "mem/buffer.hpp"


template<typename TData>
class SharedContainer : public SharedBuffer<TData> {
public:
    using size_type = typename SharedBuffer<TData>::size_type;

    explicit SharedContainer(size_type const size) : SharedBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = SharedBuffer<TData>::plain_type();
        }
    }

    SharedContainer(size_type const size, auto &&init_func) : SharedBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    template<typename TDataOther>
        requires requires { requires !(std::is_const_v<TDataOther> && !std::is_const_v<TData>); }
    explicit SharedContainer(const SharedContainer<TDataOther> &other) : SharedBuffer<TData>(
        other.size(), std::move(other.handle())) {
    }

    SharedContainer(SharedContainer &) = default;

    SharedContainer(SharedContainer &&) = delete;

    SharedContainer &operator=(SharedContainer const &other) = default;

    SharedContainer &operator=(SharedContainer &&) = delete;
};


template<typename TData>
class UniqueContainer : public UniqueBuffer<TData> {
public:
    using size_type = typename UniqueBuffer<TData>::size_type;

    explicit UniqueContainer(size_type const size) : UniqueBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = SharedBuffer<TData>::plain_type();
        }
    }

    UniqueContainer(size_type const size, auto &&init_func) : UniqueBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    UniqueContainer(UniqueContainer &) = delete;

    UniqueContainer(UniqueContainer &&) = default;

    UniqueContainer &operator=(UniqueContainer &) = delete;

    UniqueContainer &operator=(UniqueContainer &&) = default;
};
