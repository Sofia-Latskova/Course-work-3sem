#include "battery.hpp"
#include "flight-task.hpp"
#include "quadcopter.hpp"


#include <iostream>
#include <windows.h>
#include <memory>

using namespace firewatch;

void RunScenario()
{
    std::cout << "\n ОБЛЁТ ЗОНЫ \n";

    {
        std::cout << "\n--- Создание дрона ---\n";
        auto drone = std::make_shared<Quadcopter>("DRN-001", 22, 100, "BAT-001");
        std::cout << "Дрон: " << drone->GetNumber() << ", состояние: " << drone->GetStateStr() << "\n";
        std::cout << "Заряд через дрон: " << drone->GetBatteryCharge() << "%\n";
        {
            std::cout << "\n--- Создание задания ---\n";
            FlightTask task("Зона-А", drone);
            std::cout << "Статус: " << task.GetStatusStr() << "\n";

// Запуск
            std::cout << "\n--- Корректный запуск ---\n";
            if (task.Start())
            {
                std::cout << "Задание запущено. Статус: " << task.GetStatusStr() << "\n";
            }

// Сканирование
            std::cout << "\n--- Сканирование точек ---\n";
            drone->Scan();
            drone->Scan();
            drone->Scan();

            std::cout << "Заряд: " << drone->GetBatteryCharge() << "%\n";
            std::cout << "Состояние: " << drone->GetStateStr() << "\n";

// Завершение
            std::cout << "\n--- Завершение задания ---\n";
            task.Complete();
            std::cout << "Статус: " << task.GetStatusStr() << "\n";

// Попытка нарушить правило
            std::cout << "\n--- Попытка нарушить правило ---\n";
            auto weakDrone = std::make_shared<Quadcopter>("DRN-002", 5, 100, "BAT-002");
            FlightTask task2("Зона-Б", weakDrone); 
            if (!task2.Start())
            {
                std::cout << "Отказ: заряд " << weakDrone->GetBatteryCharge() << "% ниже минимума для взлёта\n";
            }

            std::cout << "\n--- Внутренний блок закрыт: задание уничтожено ---\n";
        }

        std::cout << "\nдрон жив: " << drone->GetNumber() << ", состояние: " << drone->GetStateStr() << "\n";
        std::cout << "\n--- Конец внешнего блока: дрон умирает с аккумулятором ---\n";
        
          
    }

    std::cout << "\n--- weak_ptr:(lock) ---\n";
    {
       auto drone = std::make_shared<Quadcopter>("DRN-003", 80, 100, "BAT-003");
       FlightTask task("Зона-W", drone);

       std::cout << "Попытка запустить задание (дрон жив):\n";
       if (task.Start()) {
           std::cout << "Успешно! Задание запущено.\n";
       }

       std::cout << "\nУничтожаем дрон снаружи\n";
       drone.reset();
       std::cout << "Попытка запустить задание (дрон мертв):\n";
       if (!task.Start()) {
           std::cout << "Отказ! Задание корректно обработало пустой lock().\n";
       }         
    }
}

void DemoMemory()
{
    std::cout << "\n ПАМЯТЬ \n";
    std::cout << "\n--- unique_ptr: один владелец ---\n";
    {
        auto battery = std::make_unique<Battery>(80, 100, "BAT-001");
        std::cout << "Заряд: " << battery->GetCharge() << "%\n";
    }

    std::cout << "\n--- Передача владения unique_ptr ---\n";
    {
        auto owner1 = std::make_unique<Battery>(60, 100, "BAT-MOVE");
        auto owner2 = std::move(owner1); // Передаем владение
        if (!owner1) 
        {
            std::cout << "owner1 пуст: владение передано\n";
        }
    }

    std::cout << "\n--- shared_ptr: счетчик владельцев ---\n";
    {
        auto drone = std::make_shared<Quadcopter>("DRN-003", 70, 100, "BAT-003");
        std::cout << "Владельцев drone: " << drone.use_count() << "\n"; // 1
        {
            auto droneCopy = drone;
            std::cout << "Владельцев после копии: " << drone.use_count() << "\n"; // 2
        } // копия уничтожена
        std::cout << "Владельцев после выхода: " << drone.use_count() << "\n"; // 1
    }
 
    std::cout << "\n--- Массивы: объектов и динамических объектов ---\n";
    // Динамический массив объектов
    {
        std::cout << "\n- Динамический массив -\n";
        auto arr = std::make_unique<Battery[]>(2);
        std::cout << "Массив: " << arr[0].GetSerial() << ", " << arr[1].GetSerial() << "\n";
    }

    // Массив динамических объектов
    {
        std::cout << "\n- Массив динамических объектов -\n";
        auto arr = std::make_unique<std::unique_ptr<Battery>[]>(2);
        arr[0] = std::make_unique<Battery>(50, 100, "BAT-10");
        arr[1] = std::make_unique<Battery>(60, 100, "BAT-11");
        std::cout << "Указатели: " << arr[0]->GetSerial() << ", " << arr[1]->GetSerial() << "\n";
    }
}

int main()
{
    SetConsoleOutputCP(65001);  
    SetConsoleCP(65001);
    std::cout << "========== Лабораторная №3 ==========\n";

    RunScenario();     
    DemoMemory();      

    std::cout << "\n========== Конец программы ==========\n";
    return 0;
}
