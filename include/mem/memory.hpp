#pragma once

#include <ostream>
#include <cassert>

template<typename TData>
class Memory {
public:
    using plain_type = std::decay_t<TData>;
    using size_type = std::size_t;

protected:
    size_type m_size = 0;
    std::shared_ptr<plain_type *> m_data;

    explicit Memory(size_type const size) : m_size(size),
                                            m_data(std::make_shared<plain_type *>(new plain_type[size])) {
        assert(size > 0);
    }

    Memory(size_type const size, std::shared_ptr<plain_type *> &&data) : m_size(size),
                                                                         m_data(std::move(data)) {
    }


    template<typename T>
    friend std::ostream &operator<<(std::ostream &os, Memory<T> const &memory);

public:
    [[nodiscard]] size_type size() const {
        return m_size;
    }
};

template<typename TData>
std::ostream &operator<<(std::ostream &os, Memory<TData> const &m) {
    os << "[";
    auto data = *(m.m_data);
    if (m.size() > 0) {
        for (std::size_t i = 0; i < m.m_size - 1; ++i) {
            os << data[i] << ", ";
        }
        if (m.m_size > 0) {
            os << data[m.m_size - 1];
        }
    }
    os << "]";
    return os;
}
