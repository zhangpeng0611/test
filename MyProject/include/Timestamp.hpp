#pragma once

#include "nocopyable.hpp"
#include <iostream>
#include <string>

class Timestamp : nocopyable
{
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch);
    static Timestamp now();
    std::string toString() const;

private:
    int64_t microSecondsSinceEpoch_;
};