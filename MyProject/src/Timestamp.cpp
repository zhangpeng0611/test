
#include "Timestamp.hpp"
#include <time.h>

Timestamp::Timestamp():microSecondsSinceEpoch_(0)
{

}
Timestamp::Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch)
{

}
Timestamp Timestamp::now()
{
}
std::string Timestamp::toString() const
{

}