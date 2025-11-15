#pragma once
#define NOMINMAX
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <algorithm>
#include "Student.h"

namespace OOPKursova {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class RangeFilterForm : public System::Windows::Forms::Form
    {
    public:
        // Конструктор — створює форму фільтрації, отримує копію списку студентів
        RangeFilterForm(const std::vector<Student>& initialList);

    protected:
        // Деструктор — звільняє пам'ять списку студентів і компонентів форми
        ~RangeFilterForm();

    private:
        std::vector<Student>* originalList;  // Копія початкового списку для фільтрації

        // === Контроли ===
        System::Windows::Forms::Label^ lblMinScore;
        System::Windows::Forms::TextBox^ txtMinScore;
        System::Windows::Forms::Label^ lblMaxScore;
        System::Windows::Forms::TextBox^ txtMaxScore;
        System::Windows::Forms::Button^ btnApplyFilter;
        System::Windows::Forms::DataGridView^ dataGridView1;
        System::ComponentModel::Container^ components;

        // Налаштовує колонки таблиці (прізвище, ім'я, група, середній бал)
        void SetupDataGridView();

        // Оновлює таблицю відфільтрованими студентами
        void UpdateDataGridView(const std::vector<Student>& list);

        // Фільтрує студентів за введеним діапазоном балів і показує результат
        System::Void btnApplyFilter_Click(System::Object^ sender, System::EventArgs^ e);

        // Створює всі елементи інтерфейсу (поля вводу, кнопку, таблицю)
        void InitializeComponent(void);
    };
}