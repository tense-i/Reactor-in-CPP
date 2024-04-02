#include "TimeStamp.h"

TimeStamp::TimeStamp()
{
    secSinceEpoh_ = time(0);
}

TimeStamp::TimeStamp(int64_t secSinceEpoch) : secSinceEpoh_(secSinceEpoch)
{
}

TimeStamp::~TimeStamp()
{
}

// 当前时间
TimeStamp TimeStamp::now()
{
    return TimeStamp(); // 返回当前时间
}

time_t TimeStamp::toInt() const
{
    return secSinceEpoh_;
}

std::string TimeStamp::toString() const
{
    char buf[128] = "";
    tm *tm_time = localtime(&secSinceEpoh_);
    snprintf(buf, 128, "%4d-%02d-%02d %02d:%02d:%02d", tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
    return buf;
}
