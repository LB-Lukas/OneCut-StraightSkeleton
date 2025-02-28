#pragma once
#include <cstddef>

namespace straight_skeleton {

class IDataStructure {
   public:
    virtual ~IDataStructure() = default;

    virtual bool isEmpty() const = 0;
    virtual size_t size() const = 0;

};

}