#pragma once

#include <type_traits>
#include <memory>
#include <ostream>
#include "mem/memory.hpp"


template<typename TData>
class SharedBuffer : public Memory<TData> {
public:
    using value_type = std::remove_reference_t<TData>;
    using plain_type = typename Memory<TData>::plain_type;
    using size_type = typename Memory<TData>::size_type;

    explicit SharedBuffer(size_type const size) : Memory<TData>(size) {
    }

    template<typename TDataOther>
        requires requires { requires !(std::is_const_v<TDataOther> && !std::is_const_v<TData>); }
    explicit SharedBuffer(const SharedBuffer<TDataOther> &other) : Memory<TData>(
        other.size(), std::move(other.handle())) {
    }

    SharedBuffer(SharedBuffer &) = default;

    SharedBuffer(SharedBuffer &&) = delete;

    SharedBuffer &operator=(SharedBuffer const &other) = default;

    SharedBuffer &operator=(SharedBuffer &&) = delete;

    value_type &operator[](std::size_t const index) {
        return (*(this->m_data))[index];
    }

    plain_type const &operator[](std::size_t const index) const {
        return (*(this->m_data))[index];
    }

    std::shared_ptr<plain_type *> handle() const {
        return this->m_data;
    }

    // only for testing
    auto use_count() const {
        return this->m_data.use_count();
    }
};


template<typename TData>
class UniqueBuffer {
public:
    using plain_type = std::decay_t<TData>;
    using value_type = std::remove_reference_t<TData>;
    using size_type = std::size_t;

private:
    size_type m_size = 0;
    std::unique_ptr<plain_type *> m_data;

public:
    explicit UniqueBuffer(size_type const size) : m_size(size),
                                                  m_data(std::make_unique<plain_type *>(new plain_type[size])) {
    }

    UniqueBuffer(size_type const size, auto &&init_func) : m_size(size),
                                                           m_data(std::make_unique<plain_type *>(
                                                               new plain_type[size])) {
        plain_type *data = *m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    UniqueBuffer(UniqueBuffer &) = delete;

    UniqueBuffer(UniqueBuffer &&) = default;

    UniqueBuffer &operator=(UniqueBuffer &) = delete;

    UniqueBuffer &operator=(UniqueBuffer &&) = default;

    value_type &operator[](std::size_t const index) {
        return (*(this->m_data))[index];
    }

    plain_type const &operator[](std::size_t const index) const {
        return (*(this->m_data))[index];
    }

    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] bool is_valid() const {
        return static_cast<bool>(m_data);
    }

    // only for debugging
    [[nodiscard]] plain_type *data() const {
        if (this->is_valid()) {
            return *m_data;
        }
        return nullptr;
    }
};

template<typename TData>
std::ostream &operator<<(std::ostream &os, UniqueBuffer<TData> const &m) {
    os << "[";
    if (m.size() > 0) {
        for (std::size_t i = 0; i < m.size() - 1; ++i) {
            os << m[i] << ", ";
        }
        if (m.size() > 0) {
            os << m[m.size() - 1];
        }
    }
    os << "]";
    return os;
}
