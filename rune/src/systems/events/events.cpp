#include "rune/systems/events/events.hpp"

namespace Rune
{
    std::map<size, Owned<ICallbackVector>> EventSystem::s_callbacks;
    u32 EventSystem::s_nextId;
}
