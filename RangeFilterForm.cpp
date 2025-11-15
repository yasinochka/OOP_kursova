#include "RangeFilterForm.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace msclr::interop;

namespace OOPKursova {

    // Конструктор — ініціалізує форму, зберігає копію списку і налаштовує таблицю
    RangeFilterForm::RangeFilterForm(const std::vector<Student>& initialList)
    {
        InitializeComponent();
        this->BackColor = System::Drawing::Color::LightCoral;

        this->originalList = new std::vector<Student>(initialList);
        SetupDataGridView();
        UpdateDataGridView(*originalList);  // Показуємо всіх студентів одразу
    }

    // Деструктор — звільняє пам'ять списку та компонентів
    RangeFilterForm::~RangeFilterForm()
    {
        if (components)
            delete components;
        delete originalList;
    }

    // Налаштовує колонки таблиці для відображення відфільтрованих студентів
    void RangeFilterForm::SetupDataGridView()
    {
        dataGridView1->Columns->Clear();
        dataGridView1->Columns->Add("LastName", "Прізвище");
        dataGridView1->Columns->Add("FirstName", "Ім'я");
        dataGridView1->Columns->Add("Group", "Група");
        dataGridView1->Columns->Add("AvgScore", "Сер. Бал (Останній Сем.)");

        dataGridView1->Columns["LastName"]->Width = 120;
        dataGridView1->Columns["FirstName"]->Width = 120;
        dataGridView1->Columns["Group"]->Width = 80;
        dataGridView1->Columns["AvgScore"]->Width = 140;
    }

    // Заповнює таблицю списком студентів
    void RangeFilterForm::UpdateDataGridView(const std::vector<Student>& list)
    {
        dataGridView1->Rows->Clear();
        for (const auto& student : list)
        {
            dataGridView1->Rows->Add(
                marshal_as<String^>(student.getLastName()),
                marshal_as<String^>(student.getFirstName()),
                marshal_as<String^>(student.getGroup()),
                student.getCurrentAverageScore().ToString("F2")
            );
        }
    }

    // Обробник кнопки — перевіряє введений діапазон, фільтрує студентів і оновлює таблицю
    System::Void RangeFilterForm::btnApplyFilter_Click(System::Object^ sender, System::EventArgs^ e)
    {
        double minScore, maxScore;

        // Валідація мінімального балу
        if (!Double::TryParse(txtMinScore->Text, minScore) || minScore < 0.0 || minScore > 5.0)
        {
            MessageBox::Show("Введіть коректне мінімальне значення балу (0.0 - 5.0).",
                "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // Валідація максимального балу
        if (!Double::TryParse(txtMaxScore->Text, maxScore) || maxScore < 0.0 || maxScore > 5.0)
        {
            MessageBox::Show("Введіть коректне максимальне значення балу (0.0 - 5.0).",
                "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // Автоматичне виправлення, якщо min > max
        if (minScore > maxScore)
            std::swap(minScore, maxScore);

        std::vector<Student> filteredList;
        for (const auto& s : *originalList)
        {
            double score = s.getCurrentAverageScore();
            if (score >= minScore && score <= maxScore)
                filteredList.push_back(s);
        }

        UpdateDataGridView(filteredList);

        MessageBox::Show(String::Format(
            "Знайдено {0} студентів з середнім балом від {1:F2} до {2:F2}.",
            filteredList.size(), minScore, maxScore),
            "Фільтрація завершена", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    // Створює інтерфейс: поля для діапазону, кнопку "Застосувати" і таблицю результатів
    void RangeFilterForm::InitializeComponent(void)
    {
        this->SuspendLayout();

        this->lblMinScore = (gcnew System::Windows::Forms::Label());
        this->txtMinScore = (gcnew System::Windows::Forms::TextBox());
        this->lblMaxScore = (gcnew System::Windows::Forms::Label());
        this->txtMaxScore = (gcnew System::Windows::Forms::TextBox());
        this->btnApplyFilter = (gcnew System::Windows::Forms::Button());
        this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
        (cli::safe_cast<ISupportInitialize^>(this->dataGridView1))->BeginInit();

        int y = 15;

        // Мінімальний бал
        this->lblMinScore->Text = L"Мінімальний бал (0-5):";
        this->lblMinScore->Location = System::Drawing::Point(10, y);
        this->lblMinScore->AutoSize = true;
        this->txtMinScore->Location = System::Drawing::Point(170, y - 5);
        this->txtMinScore->Size = System::Drawing::Size(60, 25);
        this->txtMinScore->Text = L"4.0";

        // Максимальний бал
        this->lblMaxScore->Text = L"Максимальний бал (0-5):";
        this->lblMaxScore->Location = System::Drawing::Point(250, y);
        this->lblMaxScore->AutoSize = true;
        this->txtMaxScore->Location = System::Drawing::Point(420, y - 5);
        this->txtMaxScore->Size = System::Drawing::Size(60, 25);
        this->txtMaxScore->Text = L"10.0";

        // Кнопка фільтрації
        this->btnApplyFilter->Text = L"Застосувати Фільтр";
        this->btnApplyFilter->Location = System::Drawing::Point(500, y - 5);
        this->btnApplyFilter->Size = System::Drawing::Size(150, 30);
        this->btnApplyFilter->Click += gcnew System::EventHandler(this, &RangeFilterForm::btnApplyFilter_Click);

        // Таблиця результатів
        this->dataGridView1->Location = System::Drawing::Point(10, 50);
        this->dataGridView1->Size = System::Drawing::Size(640, 400);
        this->dataGridView1->ReadOnly = true;
        this->dataGridView1->AllowUserToAddRows = false;
        this->dataGridView1->BackgroundColor = System::Drawing::Color::White;

        // Додаємо все на форму
        this->Controls->Add(this->lblMinScore);
        this->Controls->Add(this->txtMinScore);
        this->Controls->Add(this->lblMaxScore);
        this->Controls->Add(this->txtMaxScore);
        this->Controls->Add(this->btnApplyFilter);
        this->Controls->Add(this->dataGridView1);

        this->ClientSize = System::Drawing::Size(660, 460);
        this->Text = L"Фільтрація студентів за діапазоном середнього балу";

        (cli::safe_cast<ISupportInitialize^>(this->dataGridView1))->EndInit();
        this->ResumeLayout(false);
        this->PerformLayout();
    }
}