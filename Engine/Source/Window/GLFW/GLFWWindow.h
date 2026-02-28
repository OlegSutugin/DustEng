#pragma once
#include <string>
#include <functional>

class GLFWwindow;

namespace Dust
{
struct WindowId
{
    unsigned int value{0};
    constexpr explicit WindowId(unsigned int id) : value(id) {};

    constexpr WindowId operator++(int)
    {
        WindowId temp = *this;
        ++value;
        return temp;
    }

    constexpr auto operator<=>(const WindowId&) const = default;
};

struct WindowSettings
{
    std::string title{};
    int width{800};
    int height{800};
    int xPos{50};
    int yPos{50};
};

class GLFWWindow
{
public:
    GLFWWindow(const WindowSettings& settings);
    ~GLFWWindow();

    void setTitle(const std::string& title);
    bool isValid() const;
    bool ShouldClose() const;

private:
    GLFWwindow* m_window{nullptr};
};
}  // namespace Dust

namespace std
{
template <>
struct hash<Dust::WindowId>
{
    size_t operator()(const Dust::WindowId& id) const noexcept { return std::hash<unsigned int>{}(id.value); }
};
}  // namespace std