// 防止头文件被重复包含
#pragma once

class nocopyable
{
public:
    nocopyable(const nocopyable &other) = delete;
    nocopyable &operator=(const nocopyable &other) = delete;

protected:
    nocopyable() = default;
    ~nocopyable() = default;
};