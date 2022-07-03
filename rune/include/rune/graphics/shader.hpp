#pragma once

#include "shader_reflection.hpp"
#include "rune/assets/asset.hpp"

#include <vector>

namespace Rune
{
    class Shader : public Asset
    {
    public:
        class Observer
        {
        public:
            virtual ~Observer() = default;

            virtual void destroying(const Shader* shader) = 0;
            virtual void changed(const Shader* shader) = 0;
        };

    public:
        void init(const std::vector<u8>& code);

        void attachObserver(Observer* observer);
        void detachObserver(Observer* observer);

        auto getCode() const -> const std::vector<u8>&;
        auto getReflectionData() const -> const ReflectionData&;

        void setReflectionData(const ReflectionData& reflectionData);

    protected:
        void onDestroying() const;
        void onChanged() const;

    private:
        std::vector<Observer*> m_observers;

        std::vector<u8> m_code;

        ReflectionData m_reflectionData;
    };
}
