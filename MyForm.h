#pragma once
#define NOMINMAX
#include <vector>
#include <msclr/marshal_cppstd.h>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>
#include "Student.h"
#include "AddForm.h"
#include "SortForm.h"
#include "StatsForm.h"
#include "GraphForm.h"
#include "RangeFilterForm.h"

namespace OOPKursova {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    // Компаратор для сортування студентів за середнім балом (спадання)
    struct StudentScoreComparator {
        bool operator()(const Student& a, const Student& b) const {
            return a.getCurrentAverageScore() > b.getCurrentAverageScore();
        }
    };

    // Компаратор для сортування студентів за прізвищем (за зростанням)
    struct StudentLastNameComparator {
        bool operator()(const Student& a, const Student& b) const {
            return a.getLastName() < b.getLastName();
        }
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        // Конструктор — створює головну форму, ініціалізує список студентів і налаштовує інтерфейс
        MyForm(void);

    protected:
        // Деструктор — звільняє пам'ять списку студентів і компонентів форми
        ~MyForm();

    private:
        std::vector<Student>* studentList;  // Динамічний список усіх студентів

        // === Елементи інтерфейсу ===
        System::Windows::Forms::MenuStrip^ menuStrip1;
        System::Windows::Forms::ToolStripMenuItem^ файлToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ завантажитиToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ зберегтиToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ вихідToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ студентToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ додатиToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ видалитиToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ функціїToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ сортуванняШеллToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ графікУспішностіToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ фільтрПоДіапазонуToolStripMenuItem;
        System::Windows::Forms::ToolStrip^ toolStrip1;
        System::Windows::Forms::ToolStripButton^ toolStripButtonLoad;
        System::Windows::Forms::ToolStripButton^ toolStripButtonSave;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
        System::Windows::Forms::ToolStripButton^ toolStripButtonAdd;
        System::Windows::Forms::ToolStripButton^ toolStripButtonDelete;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator2;
        System::Windows::Forms::ToolStripButton^ toolStripButtonSort;
        System::Windows::Forms::ToolStripButton^ toolStripButtonFilter;
        System::Windows::Forms::DataGridView^ dataGridView1;
        System::Windows::Forms::Button^ btnLoadFile;
        System::Windows::Forms::Button^ btnSaveFile;
        System::Windows::Forms::Button^ btnAddStudent;
        System::Windows::Forms::Button^ btnDeleteStudent;
        System::Windows::Forms::Button^ btnShellSort;
        System::Windows::Forms::Button^ btnBestWorstYears;
        System::Windows::Forms::Button^ btnTop3;
        System::Windows::Forms::Button^ btnBestGroup;
        System::Windows::Forms::Button^ btnShowGraph;
        System::Windows::Forms::Button^ btnRangeFilter;
        System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;
        System::Windows::Forms::ToolStripMenuItem^ MenuItemDelete;
        System::Windows::Forms::ToolStripMenuItem^ MenuItemShowGraph;
        System::ComponentModel::Container^ components;

        // Налаштовує колонки таблиці DataGridView (назви, ширину)
        void SetupDataGridView();

        // Оновлює вміст таблиці на основі поточного списку студентів
        void UpdateDataGridView();

        // Сортує студентів за прізвищем і оновлює відображення
        void SortAndRefresh();


        // Закриває програму
        System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        // Завантажує студентів з файлу students.txt
        System::Void btnLoadFile_Click(System::Object^ sender, System::EventArgs^ e);

        // Зберігає студентів у файл input.txt
        System::Void btnSaveFile_Click(System::Object^ sender, System::EventArgs^ e);

        // Відкриває форму додавання нового студента
        System::Void btnAddStudent_Click(System::Object^ sender, System::EventArgs^ e);

        // Видаляє вибраного студента з таблиці
        System::Void btnDeleteStudent_Click(System::Object^ sender, System::EventArgs^ e);

        // Викликає видалення через контекстне меню
        System::Void MenuItemDelete_Click(System::Object^ sender, System::EventArgs^ e);

        // Відкриває форму сортування методом Шелла
        System::Void btnShellSort_Click(System::Object^ sender, System::EventArgs^ e);

        // Показує найкращі та найгірші роки вступу
        System::Void btnBestWorstYears_Click(System::Object^ sender, System::EventArgs^ e);

        // Показує ТОП-3 студентів за середнім балом
        System::Void btnTop3_Click(System::Object^ sender, System::EventArgs^ e);

        // Показує найкращу групу за середнім балом
        System::Void btnBestGroup_Click(System::Object^ sender, System::EventArgs^ e);

        // Показує графік успішності вибраного студента
        System::Void btnShowGraph_Click(System::Object^ sender, System::EventArgs^ e);

        // Викликає графік через контекстне меню
        System::Void MenuItemShowGraph_Click(System::Object^ sender, System::EventArgs^ e);

        // Відкриває форму фільтрації за діапазоном
        System::Void btnRangeFilter_Click(System::Object^ sender, System::EventArgs^ e);

        // Створює всі елементи інтерфейсу (меню, кнопки, таблицю)
        void InitializeComponent(void);
    };
}