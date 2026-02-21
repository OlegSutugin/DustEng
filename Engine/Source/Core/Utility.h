#pragma once

namespace Dust
{
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable(const NonCopyable&&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
};
}  // namespace Dust