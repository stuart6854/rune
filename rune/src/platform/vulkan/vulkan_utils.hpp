// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

namespace Rune
{
    bool vulkanIsLayerSupported(const char* layerName);

    bool vulkanIsExtensionSupported(const char* extensionName);
}
