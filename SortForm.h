#pragma once
#define NOMINMAX
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <set>
#include <algorithm>
#include "Student.h"

namespace OOPKursova {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class SortForm : public System::Windows::Forms::Form
    {
    public:
        // Конструктор — створює форму сортування, отримує копію списку студентів
        SortForm(const std::vector<Student>& initialList);

    protected:
        // Деструктор — звільняє пам'ять списку студентів і компонентів форми
        ~SortForm();

    private:
        std::vector<Student>* originalList;  // Копія початкового списку для фільтрації та сортування

        // === Контроли ===
        System::Windows::Forms::Label^ lblYear;
        System::Windows::Forms::ComboBox^ cbYear;
        System::Windows::Forms::Label^ lblSemester;
        System::Windows::Forms::ComboBox^ cbSemester;
        System::Windows::Forms::Button^ btnSort;
        System::Windows::Forms::DataGridView^ dataGridView1;
        System::ComponentModel::Container^ components;

        // === Методи ===

        // Налаштовує колонки таблиці для відображення результатів сортування
        void SetupDataGridView();

        // Оновлює таблицю відсортованими та відфільтрованими студентами
        void UpdateDataGridView(const std::vector<Student>& list);

        // Заповнює комбобокси унікальними роками вступу та семестрами
        void FillYearSemesterComboBoxes();

        // Виконує сортування методом Шелла за середнім балом (спадання)
        void ShellSort(std::vector<Student>& arr);

        // Фільтрує студентів за вибраним роком і мінімальним семестром, сортує методом Шелла
        System::Void btnSort_Click(System::Object^ sender, System::EventArgs^ e);

        // Створює інтерфейс: комбобокси, кнопку "Сортувати" і таблицю результатів
        void InitializeComponent(void);
    };
}