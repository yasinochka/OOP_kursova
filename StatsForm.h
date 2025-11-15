#pragma once
#define NOMINMAX
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <set>
#include "Student.h"

namespace OOPKursova {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class StatsForm : public System::Windows::Forms::Form
    {
    public:
        // Режими роботи форми
        enum class Mode {
            BestWorstYears,  // Найкращі та найгірші роки за загальним середнім балом
            BestGroup        // Найкраща група за вибраний рік вступу
        };

        // Конструктор — створює форму статистики, отримує копію списку студентів
        StatsForm(const std::vector<Student>& initialList);

        // Змінює режим роботи форми (викликається з MyForm)
        void SetMode(Mode mode);

    protected:
        // Деструктор — звільняє пам'ять списку та компонентів
        ~StatsForm();

    private:
        std::vector<Student>* studentListCopy;  // Копія списку студентів для аналізу
        Mode currentMode;                       // Поточний режим відображення

        // === Контроли ===
        System::Windows::Forms::Label^ lblResult;
        System::Windows::Forms::Label^ lblYearFilter;
        System::Windows::Forms::ComboBox^ cbYearFilter;
        System::Windows::Forms::Button^ btnApplyGroupFilter;
        System::ComponentModel::Container^ components;

        // === Методи ===

        // Заповнює комбобокс унікальними роками вступу
        void FillYearFilterComboBox();

        // Розраховує статистику залежно від режиму і виводить у lblResult
        void CalculateAndDisplayStats(Mode mode);

        // Знаходить найкращу групу за вибраний рік і виводить результат
        System::Void btnApplyGroupFilter_Click(System::Object^ sender, System::EventArgs^ e);

        // Створює інтерфейс: мітку результату, фільтр по року (для режиму груп)
        void InitializeComponent(void);
    };
}