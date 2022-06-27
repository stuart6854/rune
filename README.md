# Rune

C++ game engine

## Platforms
- Windows
- Linux *(not implemented)*
- Mac *(not implemented)*

<br>

- OpenGL
- DirectX *(not implemented)*
- Vulkan *(not implemented)*

## Features

*Todo*

## Style Guide

```C++
// 'CamelCase' namespace
namespace Rune
{
    // 'CamelCase' types/classes
    class SomeClass
    {
    public:
        // 'camelCase' functions/methods
        // Use trailing return types
        auto doSomething() -> int
        {
            // 'camelCase' local variables
            int localVar = 0;
            return localVar;
        }

    private:
        // 'camelCase' member variables. prefix with 'm_'
        int m_privateMember;
    }
}
```

## Building

```
$ git clone --recursive git@github.com:stuart6854/rune.git
$ cd rune && mkdir build && cd build
$ cmake ..
```