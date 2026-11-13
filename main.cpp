#include "TempSensor.hpp"
#include "SmokeSensor.hpp"
#include "FlameSensor.hpp"
#include "Quadcopter.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace firewatch;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "===========Lab-04 Наследование======\n\n";
    std::cout << "--- 0. Создание квадрокоптера-носителя ---\n";
    auto quad = std::make_shared<Quadcopter>("DRN-001", 100, 100, "BAT-001");
    std::cout << "\n";
//Массив наследников
    std::cout << "--- 1. Создание массива наследников ---\n";
    std::vector<std::unique_ptr<BaseSensor>> sensors;
    sensors.push_back(std::make_unique<TempSensor>("TEMP-1", quad, 25.5));
    sensors.push_back(std::make_unique<SmokeSensor>("SMOKE-1", quad, 0.1));
    sensors.push_back(std::make_unique<FlameSensor>("FLAME-1", quad, 15));
//полиморфизм
    std::cout << "\n--- 2. Полиморфизм (опрос датчиков) ---\n";
    for (const auto& sensor : sensors)
    {
        sensor->ReadValue();
        std::cout << sensor->Describe() << "\n";
        std::cout << "-------------------------\n";
    }
//деструктор
    std::cout << "\n--- 3. Уничтожение через указатель на базовый класс ---\n";
    std::cout << "Создаём датчик во вложенном блоке...\n";
    {
        std::unique_ptr<BaseSensor> temp =
            std::make_unique<TempSensor>("TEMP-TEMP", quad, 30.0);
        std::cout << "Выходим из блока...\n";
    }
//копирование
    std::cout << "--- 4. Копирование ---\n";
    auto original = std::make_shared<TempSensor>("TEMP-ORIG", quad, 20.0);
    original->GetQuadcopter()->TakeOff();   //в полете
    original->ReadValue();
    std::cout << "\nПОВЕРХНОСТНАЯ КОПИЯ\n";
    std::cout << "Оригинал до изменений:\n" << original->Describe() << "\n";
    TempSensor copy = *original;              
    copy.GetQuadcopter()->Land();          
    copy.ReadValue();
    std::cout << "\nОригинал после изменения дрона через копию:\n"
        << original->Describe() << "\n";
    std::cout << "Копия:\n" << copy.Describe() << "\n";
    original->GetQuadcopter()->TakeOff();     // в полете
    std::cout << "\nГЛУБОКАЯ КОПИЯ:\n";
    auto deep = original->Clone();             // свой дрон
    std::cout << "Оригинал до изменений:\n" << original->Describe() << "\n";
    deep->GetQuadcopter()->Land();            
    deep->ReadValue();
    std::cout << "\nОригинал после изменения deep-копии:\n" << original->Describe() << "\n";
    std::cout << "Deep-копия:\n" << deep->Describe() << "\n";
//самоприсваивание
    std::cout << "\n--- 5. Проверка this ---\n";
    TempSensor assignTest("TEMP-ASSIGN", quad, 25.0);
    assignTest.ReadValue();
    std::cout << "До самоприсваивания:\n" << assignTest.Describe() << "\n";
    std::cout << "Выполняем: assignTest = assignTest\n";
    assignTest = assignTest;                       
    std::cout << "\nПосле самоприсваивания:\n" << assignTest.Describe() << "\n";
//стат счетчик
    std::cout << "\n--- 6. Статический счётчик ---\n";
    for (const auto& sensor : sensors)
    {
        std::cout << " ID: " << sensor->GetId();
    }
    std::cout << "\nПоследний выданный ID: " << BaseSensor::NextId() - 1 << "\n\n";
//строки
    std::cout << "--- 7. Работа со строками ---\n";
    std::string prefix = "Датчик: ";
    std::string fullName = prefix + sensors[0]->GetSerial();
    std::cout << "Конкатенация: " << fullName << "\n";
    if (fullName.find("TEMP") != std::string::npos)
    {
        std::cout << "Поиск 'TEMP': найдено\n";
    }
    std::cout << "Подстрока: " << fullName.substr(8, 4) << "\n";
    if (prefix == "Датчик: ")
    {
        std::cout << "Сравнение: строки равны\n";
    }
    std::cout << "\n";
    std::cout << "\n=== КОНЕЦ ===\n";
}










