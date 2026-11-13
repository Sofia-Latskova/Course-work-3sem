#pragma once

#include "quadcopter.hpp"
#include <string>
#include <string_view>
#include <memory>


namespace firewatch {

    class BaseSensor {
        //можно наследникам, но наружу не попадает
    protected:
       std::string m_serial;
       std::string GetHeader() const; //метод только для наследников
       std::shared_ptr<Quadcopter> m_quadcopter;

    private:
        static inline int m_lastId{ 0 }; // Статический счётчик 
        int m_id; //айди датчика 
        mutable bool m_isBroken{ false };
        mutable int m_durability;

        //доступно снаружи
    public:
        //конструктор с параметрами
        BaseSensor(std::string_view serial, std::shared_ptr<Quadcopter> quadcopter);

        //виртуальный деструктор virtual - наследники могут менять и в своем классе использовать не от базового, а от своего
        virtual ~BaseSensor();

        //копирование
       BaseSensor(const BaseSensor& other) = default;
       BaseSensor& operator=(const BaseSensor& other);
//перемещение
       BaseSensor(BaseSensor&& other) noexcept = default;
       BaseSensor& operator=(BaseSensor&& other) noexcept = default;
        //чисто виртуальный метод - наследник пишет сам, делает класс абстрактным
        virtual double ReadValue() const = 0;

        // Виртуальный метод с реализацией (наследник может вызвать базовый метод и добавить чтото свое)
        virtual std::string Describe() const;

        // Виртуальный метод для клонироавния
        virtual std::unique_ptr<BaseSensor> Clone() const = 0;

        //управление состоянием (износ и поломка)
        void Break() const { m_isBroken = true; }
        [[nodiscard]] bool IsBroken() const { return m_isBroken; }

        bool WearDown() const; //вернет true если датчик сломался

        // Геттеры
        [[nodiscard]] int GetId() const { return m_id; }
        [[nodiscard]] const std::string& GetSerial() const { return m_serial; }
        [[nodiscard]] int GetDurability() const { return m_durability; }
        [[nodiscard]] const std::shared_ptr<Quadcopter>& GetQuadcopter() const { return m_quadcopter; }

        // Статический метод 
        static int NextId() { return ++m_lastId; }

    };
}