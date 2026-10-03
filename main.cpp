#include "battery.hpp"
#include "flight-task.hpp"
#include "quadcopter.hpp"


#include <iostream>
#include <windows.h>

using namespace firewatch;

void RunScenario()
{
    std::cout << "\n ОБЛЁТ ЗОНЫ \n";

    {
        std::cout << "\n--- Создание дрона ---\n";
        Battery* battery = new Battery(22, 100, "BAT-001");
        Quadcopter drone("DRN-001",battery);
        std::cout << "Дрон: " << drone.GetNumber() << ", состояние: " << drone.GetStateStr() << "\n";
        std::cout << "Заряд через дрон: " << drone.GetBatteryCharge() << "%\n";
        {
            std::cout << "\n--- Создание задания ---\n";
            FlightTask task("Зона-А", &drone);
            std::cout << "Статус: " << task.GetStatusStr() << "\n";

            // Запуск
            std::cout << "\n--- Корректный запуск ---\n";
            if (task.Start())
            {
                std::cout << "Задание запущено. Статус: " << task.GetStatusStr() << "\n";
            }

            // Сканирование
            std::cout << "\n--- Сканирование точек ---\n";
            drone.Scan();
            drone.Scan();
            drone.Scan();

            std::cout << "Заряд: " << drone.GetBatteryCharge() << "%\n";
            std::cout << "Состояние: " << drone.GetStateStr() << "\n";

            // Завершение
            std::cout << "\n--- Завершение задания ---\n";
            task.Complete();
            std::cout << "Статус: " << task.GetStatusStr() << "\n";

            // Попытка нарушить правило
            std::cout << "\n--- Попытка нарушить правило ---\n";
            std::cout << "Заряд: " << drone.GetBatteryCharge() << "%\n";
            FlightTask task2("Зона-Б", &drone);
            if (!task2.Start())
            {
                std::cout << "Отказ: заряд ниже минимума для взлёта\n";
            } 
            std::cout << "\n--- Внутренний блок закрыт: задание уничтожено ---\n";
        } 
       
        std::cout << "\nдрон жив: " << drone.GetNumber() << ", состояние: " << drone.GetStateStr() << "\n";
        std::cout << "\n--- Конец внешнего блока: дрон умирает с аккумулятором ---\n";
    } 
    
    std::cout << "Дрон и аккумулятор уничтожены\n";

}

void DemoMemory()
{
    std::cout << "\n ПАМЯТЬ \n";
    // Статическая инициализация  
    std::cout << "\n--- Статическая инициализация ---\n";
    {
        Battery battery(80, 100, "BAT-001");
        std::cout << "Статика: " << battery.GetSerial() <<" заряд =" << battery.GetCharge() << "%\n";
    }
 // Динамическая: new / delete, указатель, ссылка
    std::cout << "\n--- Динамическая инициализация ---\n";
    {
        Battery* battery = new Battery(60, 100, "BAT-002");
        std::cout << "Указатель: " << battery->GetSerial() << "\n";
        Battery& ref = *battery;
        std::cout << "Ссылка: " << ref.GetCharge() << "%\n";
        delete battery;
    }
    std::cout << "\n--- Массивы: объектов и динамических объектов ---\n";
    // Динамический массив объектов
    {
        std::cout << "\n- Динамический массив -\n";
        Battery* arr = new Battery[2];
        std::cout << "Массив: " << arr[0].GetSerial()<< ", " << arr[1].GetSerial() << "\n";
        delete[] arr;
    }

    // Массив динамических объектов
    {
        std::cout << "\n- Массив динамических объектов -\n";
        Battery** arr = new Battery * [2];
        arr[0] = new Battery(50, 100, "BAT-10");
        arr[1] = new Battery(60, 100, "BAT-11");
        std::cout << "Указатели: " << arr[0]->GetSerial() << "\n";
        delete arr[0];
        delete arr[1];
        delete[] arr;
    }
}

int main()
{
    SetConsoleOutputCP(65001);  
    SetConsoleCP(65001);
    std::cout << "========== Лабораторная №2 ==========\n";

    RunScenario();     // сценарий + композиция + агрегация + правило + время жизни
    DemoMemory();      // память

    std::cout << "\n========== Конец программы ==========\n";
    return 0;
}
