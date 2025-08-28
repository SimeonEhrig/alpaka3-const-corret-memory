#pragma once

#include <type_traits>
#include <memory>

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

template<typename TData>
class ManagedView : public Memory<TData> {
public:
    using value_type = std::remove_reference_t<TData>;
    using plain_type = typename Memory<TData>::plain_type;
    using size_type = typename Memory<TData>::size_type;

    explicit ManagedView(size_type const size) : Memory<TData>(size) {
    }

    template<typename TDataOther>
        requires requires { requires !(std::is_const_v<TDataOther> && !std::is_const_v<TData>); }
    explicit ManagedView(const ManagedView<TDataOther> &other) : Memory<TData>(
        other.size(), std::move(other.handle())) {
    }


    ManagedView &operator=(ManagedView const &other) = default;

    ManagedView(size_type const size, auto &&init_func) : Memory<TData>(size) {
        typename Memory<TData>::plain_type *data = *Memory<TData>::m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    ManagedView(ManagedView &) = default;

    ManagedView(ManagedView &&) = delete;

    ManagedView &operator=(ManagedView &&) = delete;

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
class UniqueManagedView {
public:
    using plain_type = std::decay_t<TData>;
    using value_type = std::remove_reference_t<TData>;
    using size_type = std::size_t;

private:
    size_type m_size = 0;
    std::unique_ptr<plain_type *> m_data;

public:
    explicit UniqueManagedView(size_type const size) : m_size(size),
                                                       m_data(std::make_unique<plain_type *>(new plain_type[size])) {
    }

    UniqueManagedView(size_type const size, auto &&init_func) : m_size(size),
                                                                m_data(std::make_unique<plain_type *>(
                                                                    new plain_type[size])) {
        plain_type *data = *m_data;
        for (size_type i = 0; i < this->size(); ++i) {
            data[i] = init_func(i);
        }
    }

    value_type &operator[](std::size_t const index) {
        return (*(this->m_data))[index];
    }

    plain_type const &operator[](std::size_t const index) const {
        return (*(this->m_data))[index];
    }

    UniqueManagedView(UniqueManagedView &) = delete;

    UniqueManagedView &operator=(UniqueManagedView &) = delete;

    UniqueManagedView(UniqueManagedView &&) = default;

    UniqueManagedView &operator=(UniqueManagedView &&) = default;


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
std::ostream &operator<<(std::ostream &os, UniqueManagedView<TData> const &m) {
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
