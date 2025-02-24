#pragma once

#include <utility>
#include <stddef.h>

namespace straight_skeleton {

template<typename R, typename B>
class IClosestPair {
public:

    virtual ~IClosestPair() = default;

    virtual void insertFirst(const R &r) = 0;

    virtual void insertSecond(const B &b) = 0;

    virtual void removeFirst(const R &r) = 0;

    virtual void removeSecond(const B &b) = 0;

    virtual size_t firstSetSize() const = 0;

    virtual size_t secondSetSize() const = 0;

    virtual std::pair<R, B> getMinPair() const = 0;

    virtual std::pair<R, B> extractMinPair() = 0;

    virtual bool isEmpty() const = 0;

    virtual bool isFirstSetEmpty() const = 0;

    virtual bool isSecondSetEmpty() const = 0;

    virtual double getMinDistance() const = 0;

};

}