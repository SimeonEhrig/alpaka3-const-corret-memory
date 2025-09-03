#pragma once

#include "mem/buffer.hpp"


template<typename TData>
class SharedCollection : public SharedBuffer<TData> {
public:
    using size_type = typename SharedBuffer<TData>::size_type;

    explicit SharedCollection(size_type const size) : SharedBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = SharedBuffer<TData>::plain_type();
        }
    }

    SharedCollection(size_type const size, auto &&init_func) : SharedBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    template<typename TDataOther>
        requires requires { requires !(std::is_const_v<TDataOther> && !std::is_const_v<TData>); }
    explicit SharedCollection(const SharedCollection<TDataOther> &other) : SharedBuffer<TData>(
        other.size(), std::move(other.handle())) {
    }

    SharedCollection(SharedCollection &) = default;

    SharedCollection(SharedCollection &&) = delete;

    SharedCollection &operator=(SharedCollection const &other) = default;

    SharedCollection &operator=(SharedCollection &&) = delete;
};


template<typename TData>
class UniqueCollection : public UniqueBuffer<TData> {
public:
    using size_type = typename UniqueBuffer<TData>::size_type;

    explicit UniqueCollection(size_type const size) : UniqueBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = SharedBuffer<TData>::plain_type();
        }
    }

    UniqueCollection(size_type const size, auto &&init_func) : UniqueBuffer<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    UniqueCollection(UniqueCollection &) = delete;

    UniqueCollection(UniqueCollection &&) = default;

    UniqueCollection &operator=(UniqueCollection &) = delete;

    UniqueCollection &operator=(UniqueCollection &&) = default;
};
