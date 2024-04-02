#pragma once

#include <iostream>
#include <string>

class TimeStamp
{

private:
    time_t secSinceEpoh_; // 1970->至今
public:
    TimeStamp();
    TimeStamp(int64_t secSinceEpoch);
    ~TimeStamp();
    static TimeStamp now();
    time_t toInt() const;         // 返回整数表示时间
    std::string toString() const; // 返回字符串时间 yyyy-MM-dd hh:mi::ss
};