#include "Buffer.h"

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char *data, size_t size)
{
    buf_.append(data, size);
}

/**
 * @将数据追加到buf、附加报文头部
 */
void Buffer::appendWithHead(const char *data, size_t size)
{
    buf_.append((char *)&size, 4);
    buf_.append(data, size);
}

size_t Buffer::size()
{
    return buf_.size();
}

void Buffer::eraseDate(size_t pos, size_t nn)
{
    buf_.erase(pos, nn);
}

const char *Buffer::data()
{
    return buf_.data();
    // 返回一个指向字符串内容的非空字符数组的指针，类型为 const char *。这个字符数组不包含空字符终止符（\0）
}

void Buffer::clear()
{
    buf_.clear();
}
