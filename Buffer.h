#pragma once

#include <string>
#include <iostream>

class Buffer
{
private:
    std::string buf_;

public:
    Buffer();
    ~Buffer();

    void append(const char *data, size_t size);
    void appendWithHead(const char *data, size_t size);
    size_t size();
    void eraseDate(size_t pos, size_t nn);
    const char *data();
    void clear();
};