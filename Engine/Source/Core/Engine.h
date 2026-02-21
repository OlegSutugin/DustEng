#pragma once
#include "EngineConfig.h"
#include <string_view>

namespace Dust
{
class Engine final
{
public:
    Engine();
    static constexpr std::string_view version() { return Engine_VERSION_STRING; };
};
}  // namespace Dust