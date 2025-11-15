// MyForm.cpp
#include "MyForm.h"
#include "StatsForm.h"
#include "AddForm.h"
#include "GraphForm.h"
#include "SortForm.h"
#include "RangeFilterForm.h"
#include <msclr/marshal_cppstd.h>
#include <fstream>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::DataVisualization::Charting;
using namespace msclr::interop;

namespace OOPKursova {

    // Конструктор — ініціалізує форму, створює список студентів і налаштовує таблицю
    MyForm::MyForm(void)
    {
        InitializeComponent();
        this->BackColor = Color::Pink;
        SetupDataGridView();
        studentList = new std::vector<Student>();
        dataGridView1->BackgroundColor = Color::White;
    }

    // Деструктор — звільняє пам'ять списку студентів і компонентів форми
    MyForm::~MyForm()
    {
        if (components) delete components;
        delete studentList;
    }

    // Налаштовує колонки DataGridView: назви, ширину, порядок
    void MyForm::SetupDataGridView()
    {
        dataGridView1->Columns->Clear();
        dataGridView1->Columns->Add("LastName", "Прізвище");
        dataGridView1->Columns->Add("FirstName", "Ім'я");
        dataGridView1->Columns->Add("Department", "Кафедра");
        dataGridView1->Columns->Add("Group", "Група");
        dataGridView1->Columns->Add("AvgScore", "Сер. Бал (Останній Сем.)");
        dataGridView1->Columns->Add("Semester", "Поточний Сем.");
        dataGridView1->Columns->Add("Year", "Рік Вступу");

        dataGridView1->Columns["LastName"]->Width = 120;
        dataGridView1->Columns["FirstName"]->Width = 120;
        dataGridView1->Columns["Department"]->Width = 80;
        dataGridView1->Columns["Group"]->Width = 80;
        dataGridView1->Columns["AvgScore"]->Width = 140;
        dataGridView1->Columns["Semester"]->Width = 120;
        dataGridView1->Columns["Year"]->Width = 100;
    }

    // Оновлює вміст таблиці на основі поточного списку студентів
    void MyForm::UpdateDataGridView()
    {
        dataGridView1->Rows->Clear();
        for (const auto& student : *studentList) {
            dataGridView1->Rows->Add(
                marshal_as<String^>(student.getLastName()),
                marshal_as<String^>(student.getFirstName()),
                marshal_as<String^>(student.getDepartment()),
                marshal_as<String^>(student.getGroup()),
                student.getCurrentAverageScore().ToString("F2"),
                student.getCurrentSemester(),
                student.getStartYear()
            );
        }
    }

    // Сортує список за прізвищем і оновлює таблицю
    void MyForm::SortAndRefresh()
    {
        std::sort(studentList->begin(), studentList->end(), StudentLastNameComparator());
        UpdateDataGridView();
    }

    // Закриває програму
    System::Void MyForm::exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->Close();
    }

    // Завантажує студентів з файлу students.txt, очищає список, сортує і оновлює таблицю
    System::Void MyForm::btnLoadFile_Click(System::Object^ sender, System::EventArgs^ e)
    {
        std::string filePath = "students.txt";
        std::ifstream inFile(filePath);
        if (inFile.is_open()) {
            studentList->clear();
            Student s;
            while (inFile >> s) {
                studentList->push_back(std::move(s));
            }
            if (studentList->empty() && !inFile.eof()) {
                MessageBox::Show("Помилка зчитування students.txt. Перевірте формат.", "Помилка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            else {
                SortAndRefresh();
                MessageBox::Show("Завантажено студентів: " + studentList->size(),
                    "Успіх", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            inFile.close();
        }
        else {
            MessageBox::Show("Не вдалося відкрити students.txt", "Помилка",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    // Зберігає всіх студентів у файл input.txt
    System::Void MyForm::btnSaveFile_Click(System::Object^ sender, System::EventArgs^ e)
    {
        std::ofstream outFile("input.txt");
        if (outFile.is_open()) {
            for (const auto& s : *studentList) outFile << s;
            outFile.close();
            MessageBox::Show("Дані збережено у input.txt", "Успіх",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        else {
            MessageBox::Show("Не вдалося зберегти файл input.txt", "Помилка",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    // Відкриває форму додавання студента і додає його до списку з сортуванням
    System::Void MyForm::btnAddStudent_Click(System::Object^ sender, System::EventArgs^ e)
    {
        AddForm^ addForm = gcnew AddForm();
        addForm->Owner = this;
        if (addForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            studentList->push_back(addForm->getNewStudent());
            SortAndRefresh();
            MessageBox::Show("Студент успішно додано!", "Додавання",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    // Видаляє вибраного студента з таблиці та списку
    System::Void MyForm::btnDeleteStudent_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (dataGridView1->SelectedRows->Count > 0) {
            int idx = dataGridView1->SelectedRows[0]->Index;
            if (idx >= 0 && idx < (int)studentList->size()) {
                studentList->erase(studentList->begin() + idx);
                UpdateDataGridView();
            }
        }
        else {
            MessageBox::Show("Виберіть рядок для видалення", "Попередження",
                MessageBoxButtons::OK, MessageBoxIcon::Warning);
        }
    }

    // Викликає видалення через контекстне меню
    System::Void MyForm::MenuItemDelete_Click(System::Object^ sender, System::EventArgs^ e)
    {
        btnDeleteStudent_Click(sender, e);
    }

    // Відкриває форму сортування методом Шелла
    System::Void MyForm::btnShellSort_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (studentList->empty()) {
            MessageBox::Show("Список порожній", "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        SortForm^ form = gcnew SortForm(*studentList);
        form->ShowDialog();
    }

    // Показує найкращі та найгірші роки за середнім балом
    System::Void MyForm::btnBestWorstYears_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (studentList->empty()) return;
        StatsForm^ f = gcnew StatsForm(*studentList);
        f->SetMode(StatsForm::Mode::BestWorstYears);
        f->ShowDialog();
    }

    // Показує ТОП-3 студентів за середнім балом
    System::Void MyForm::btnTop3_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (studentList->size() < 3) {
            MessageBox::Show("Недостатньо студентів для Топ-3", "Попередження",
                MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        std::vector<Student> temp = *studentList;
        std::sort(temp.begin(), temp.end(), StudentScoreComparator());
        String^ msg = "ТОП-3 студентів:\n\n";
        for (int i = 0; i < 3; ++i) {
            msg += String::Format("{0}. {1} {2} — {3:F2}\n",
                i + 1,
                marshal_as<String^>(temp[i].getLastName()),
                marshal_as<String^>(temp[i].getFirstName()),
                temp[i].getCurrentAverageScore());
        }
        MessageBox::Show(msg, "Топ-3", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    // Показує найкращу групу за середнім балом
    System::Void MyForm::btnBestGroup_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (studentList->empty()) return;
        StatsForm^ f = gcnew StatsForm(*studentList);
        f->SetMode(StatsForm::Mode::BestGroup);
        f->ShowDialog();
    }

    // Показує графік успішності вибраного студента
    System::Void MyForm::btnShowGraph_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (dataGridView1->SelectedRows->Count == 0) {
            MessageBox::Show("Виберіть студента", "Попередження",
                MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int idx = dataGridView1->SelectedRows[0]->Index;
        if (idx >= 0 && idx < (int)studentList->size()) {
            GraphForm^ gf = gcnew GraphForm((*studentList)[idx]);
            gf->ShowDialog();
        }
    }

    // Викликає графік через контекстне меню
    System::Void MyForm::MenuItemShowGraph_Click(System::Object^ sender, System::EventArgs^ e)
    {
        btnShowGraph_Click(sender, e);
    }

    // Відкриває форму фільтрації студентів за діапазоном балів, семестрів і років
    System::Void MyForm::btnRangeFilter_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (studentList->empty()) return;
        RangeFilterForm^ f = gcnew RangeFilterForm(*studentList);
        f->ShowDialog();
    }

    // Створює всі елементи інтерфейсу: меню, кнопки, таблицю, панелі інструментів
    void MyForm::InitializeComponent(void)
    {
        this->components = (gcnew System::ComponentModel::Container());
        this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
        this->MenuItemDelete = (gcnew ToolStripMenuItem());
        this->MenuItemShowGraph = (gcnew ToolStripMenuItem());
        this->menuStrip1 = (gcnew MenuStrip());
        this->файлToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->завантажитиToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->зберегтиToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->вихідToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->студентToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->додатиToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->видалитиToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->функціїToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->сортуванняШеллToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->графікУспішностіToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->фільтрПоДіапазонуToolStripMenuItem = (gcnew ToolStripMenuItem());
        this->toolStrip1 = (gcnew ToolStrip());
        this->toolStripButtonLoad = (gcnew ToolStripButton());
        this->toolStripButtonSave = (gcnew ToolStripButton());
        this->toolStripSeparator1 = (gcnew ToolStripSeparator());
        this->toolStripButtonAdd = (gcnew ToolStripButton());
        this->toolStripButtonDelete = (gcnew ToolStripButton());
        this->toolStripSeparator2 = (gcnew ToolStripSeparator());
        this->toolStripButtonSort = (gcnew ToolStripButton());
        this->toolStripButtonFilter = (gcnew ToolStripButton());
        this->dataGridView1 = (gcnew DataGridView());
        this->btnLoadFile = (gcnew Button());
        this->btnSaveFile = (gcnew Button());
        this->btnAddStudent = (gcnew Button());
        this->btnDeleteStudent = (gcnew Button());
        this->btnShellSort = (gcnew Button());
        this->btnBestWorstYears = (gcnew Button());
        this->btnTop3 = (gcnew Button());
        this->btnBestGroup = (gcnew Button());
        this->btnShowGraph = (gcnew Button());
        this->btnRangeFilter = (gcnew Button());
        this->SuspendLayout();

        int btnW = 200, btnH = 30, btnX = 980, y = 90, step = 40;
        int botX = 10, botY = 540;

        // === Контекстне меню ===
        this->MenuItemDelete->Text = L"Видалити студента";
        this->MenuItemDelete->Click += gcnew EventHandler(this, &MyForm::MenuItemDelete_Click);
        this->MenuItemShowGraph->Text = L"Графік успішності";
        this->MenuItemShowGraph->Click += gcnew EventHandler(this, &MyForm::MenuItemShowGraph_Click);
        this->contextMenuStrip1->Items->AddRange(gcnew array<ToolStripItem^>{ this->MenuItemDelete, this->MenuItemShowGraph });

        // === Головне меню ===
        this->menuStrip1->Location = Drawing::Point(0, 0);
        this->menuStrip1->Size = Drawing::Size(1200, 24);
        this->menuStrip1->Items->AddRange(gcnew array<ToolStripItem^>{
            this->файлToolStripMenuItem, this->студентToolStripMenuItem, this->функціїToolStripMenuItem
        });

        this->файлToolStripMenuItem->Text = L"Файл";
        this->файлToolStripMenuItem->DropDownItems->AddRange(gcnew array<ToolStripItem^>{
            this->завантажитиToolStripMenuItem, this->зберегтиToolStripMenuItem, this->вихідToolStripMenuItem
        });
        this->завантажитиToolStripMenuItem->Text = L"Завантажити";
        this->завантажитиToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnLoadFile_Click);
        this->зберегтиToolStripMenuItem->Text = L"Зберегти";
        this->зберегтиToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnSaveFile_Click);
        this->вихідToolStripMenuItem->Text = L"Вихід";
        this->вихідToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::exitToolStripMenuItem_Click);

        this->студентToolStripMenuItem->Text = L"Студент";
        this->студентToolStripMenuItem->DropDownItems->AddRange(gcnew array<ToolStripItem^>{
            this->додатиToolStripMenuItem, this->видалитиToolStripMenuItem
        });
        this->додатиToolStripMenuItem->Text = L"Додати";
        this->додатиToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnAddStudent_Click);
        this->видалитиToolStripMenuItem->Text = L"Видалити";
        this->видалитиToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnDeleteStudent_Click);

        this->функціїToolStripMenuItem->Text = L"Функції";
        this->функціїToolStripMenuItem->DropDownItems->AddRange(gcnew array<ToolStripItem^>{
            this->сортуванняШеллToolStripMenuItem, this->графікУспішностіToolStripMenuItem, this->фільтрПоДіапазонуToolStripMenuItem
        });
        this->сортуванняШеллToolStripMenuItem->Text = L"Сортування (Шелл)";
        this->сортуванняШеллToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnShellSort_Click);
        this->графікУспішностіToolStripMenuItem->Text = L"Графік";
        this->графікУспішностіToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnShowGraph_Click);
        this->фільтрПоДіапазонуToolStripMenuItem->Text = L"Фільтр";
        this->фільтрПоДіапазонуToolStripMenuItem->Click += gcnew EventHandler(this, &MyForm::btnRangeFilter_Click);

        // === Панель інструментів ===
        this->toolStrip1->Location = Drawing::Point(0, 24);
        this->toolStrip1->Size = Drawing::Size(1200, 25);
        this->toolStrip1->Items->AddRange(gcnew array<ToolStripItem^>{
            this->toolStripButtonLoad, this->toolStripButtonSave, this->toolStripSeparator1,
                this->toolStripButtonAdd, this->toolStripButtonDelete, this->toolStripSeparator2,
                this->toolStripButtonSort, this->toolStripButtonFilter
        });
        this->toolStripButtonLoad->Text = L"Завантажити"; this->toolStripButtonLoad->Click += gcnew EventHandler(this, &MyForm::btnLoadFile_Click);
        this->toolStripButtonSave->Text = L"Зберегти"; this->toolStripButtonSave->Click += gcnew EventHandler(this, &MyForm::btnSaveFile_Click);
        this->toolStripButtonAdd->Text = L"Додати"; this->toolStripButtonAdd->Click += gcnew EventHandler(this, &MyForm::btnAddStudent_Click);
        this->toolStripButtonDelete->Text = L"Видалити"; this->toolStripButtonDelete->Click += gcnew EventHandler(this, &MyForm::btnDeleteStudent_Click);
        this->toolStripButtonSort->Text = L"Сортувати"; this->toolStripButtonSort->Click += gcnew EventHandler(this, &MyForm::btnShellSort_Click);
        this->toolStripButtonFilter->Text = L"Фільтр"; this->toolStripButtonFilter->Click += gcnew EventHandler(this, &MyForm::btnRangeFilter_Click);

        // === Таблиця ===
        this->dataGridView1->Location = Drawing::Point(10, 55);
        this->dataGridView1->Size = Drawing::Size(950, 475);
        this->dataGridView1->ReadOnly = true;
        this->dataGridView1->AllowUserToAddRows = false;
        this->dataGridView1->ContextMenuStrip = this->contextMenuStrip1;

        // === Кнопки праворуч ===
        this->btnLoadFile->Text = L"Зчитати з файлу";
        this->btnLoadFile->Location = Drawing::Point(btnX, y); y += step;
        this->btnLoadFile->Size = Drawing::Size(btnW, btnH);
        this->btnLoadFile->BackColor = Color::LightYellow;
        this->btnLoadFile->Click += gcnew EventHandler(this, &MyForm::btnLoadFile_Click);

        this->btnSaveFile->Text = L"Записати у файл";
        this->btnSaveFile->Location = Drawing::Point(btnX, y); y += step;
        this->btnSaveFile->Size = Drawing::Size(btnW, btnH);
        this->btnSaveFile->BackColor = Color::LightYellow;
        this->btnSaveFile->Click += gcnew EventHandler(this, &MyForm::btnSaveFile_Click);

        this->btnAddStudent->Text = L"Додати студента";
        this->btnAddStudent->Location = Drawing::Point(btnX, y); y += step;
        this->btnAddStudent->Size = Drawing::Size(btnW, btnH);
        this->btnAddStudent->BackColor = Color::LightYellow;
        this->btnAddStudent->Click += gcnew EventHandler(this, &MyForm::btnAddStudent_Click);

        this->btnDeleteStudent->Text = L"Видалити студента";
        this->btnDeleteStudent->Location = Drawing::Point(btnX, y); y += step;
        this->btnDeleteStudent->Size = Drawing::Size(btnW, btnH);
        this->btnDeleteStudent->BackColor = Color::LightYellow;
        this->btnDeleteStudent->Click += gcnew EventHandler(this, &MyForm::btnDeleteStudent_Click);

        // === Кнопки знизу ===
        this->btnShellSort->Text = L"Сортування (Шелл)";
        this->btnShellSort->Location = Drawing::Point(botX, botY); botX += btnW + 10;
        this->btnShellSort->Size = Drawing::Size(btnW, btnH);
        this->btnShellSort->BackColor = Color::LightYellow;
        this->btnShellSort->Click += gcnew EventHandler(this, &MyForm::btnShellSort_Click);

        this->btnBestWorstYears->Text = L"Найкращі роки";
        this->btnBestWorstYears->Location = Drawing::Point(botX, botY); botX += btnW + 10;
        this->btnBestWorstYears->Size = Drawing::Size(btnW, btnH);
        this->btnBestWorstYears->BackColor = Color::LightYellow;
        this->btnBestWorstYears->Click += gcnew EventHandler(this, &MyForm::btnBestWorstYears_Click);

        this->btnTop3->Text = L"Топ-3";
        this->btnTop3->Location = Drawing::Point(botX, botY); botX += btnW + 10;
        this->btnTop3->Size = Drawing::Size(btnW, btnH);
        this->btnTop3->BackColor = Color::LightYellow;
        this->btnTop3->Click += gcnew EventHandler(this, &MyForm::btnTop3_Click);

        this->btnRangeFilter->Text = L"Фільтр";
        this->btnRangeFilter->Location = Drawing::Point(botX, botY); botX += btnW + 10;
        this->btnRangeFilter->Size = Drawing::Size(btnW, btnH);
        this->btnRangeFilter->BackColor = Color::LightYellow;
        this->btnRangeFilter->Click += gcnew EventHandler(this, &MyForm::btnRangeFilter_Click);

        botX = 10; botY += step;
        this->btnShowGraph->Text = L"Графік";
        this->btnShowGraph->Location = Drawing::Point(botX, botY); botX += btnW + 10;
        this->btnShowGraph->Size = Drawing::Size(btnW, btnH);
        this->btnShowGraph->BackColor = Color::LightYellow;
        this->btnShowGraph->Click += gcnew EventHandler(this, &MyForm::btnShowGraph_Click);

        this->btnBestGroup->Text = L"Найкраща група";
        this->btnBestGroup->Location = Drawing::Point(botX, botY);
        this->btnBestGroup->Size = Drawing::Size(btnW, btnH);
        this->btnBestGroup->BackColor = Color::LightYellow;
        this->btnBestGroup->Click += gcnew EventHandler(this, &MyForm::btnBestGroup_Click);

        // === Додавання всіх елементів на форму ===
        this->Controls->Add(this->menuStrip1);
        this->Controls->Add(this->toolStrip1);
        this->Controls->Add(this->dataGridView1);
        this->Controls->Add(this->btnLoadFile);
        this->Controls->Add(this->btnSaveFile);
        this->Controls->Add(this->btnAddStudent);
        this->Controls->Add(this->btnDeleteStudent);
        this->Controls->Add(this->btnShellSort);
        this->Controls->Add(this->btnBestWorstYears);
        this->Controls->Add(this->btnTop3);
        this->Controls->Add(this->btnBestGroup);
        this->Controls->Add(this->btnShowGraph);
        this->Controls->Add(this->btnRangeFilter);

        this->ClientSize = Drawing::Size(1200, botY + btnH + 40);
        this->Text = L"Ясінська Єлизавета курсова робота варіант 20";
        this->MainMenuStrip = this->menuStrip1;
        this->ResumeLayout(false);
        this->PerformLayout();
    }
}