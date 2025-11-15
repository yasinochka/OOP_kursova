#include "SortForm.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace msclr::interop;

namespace OOPKursova {

    // Конструктор — ініціалізує форму, зберігає копію списку, заповнює комбобокси і налаштовує таблицю
    SortForm::SortForm(const std::vector<Student>& initialList)
    {
        InitializeComponent();
        this->BackColor = System::Drawing::Color::LightGreen;

        this->originalList = new std::vector<Student>(initialList);
        FillYearSemesterComboBoxes();
        SetupDataGridView();
    }

    // Деструктор — звільняє пам'ять списку та компонентів
    SortForm::~SortForm()
    {
        if (components)
            delete components;
        delete originalList;
    }

    // Налаштовує колонки таблиці: прізвище, ім'я, група, бал, семестр, рік
    void SortForm::SetupDataGridView()
    {
        dataGridView1->Columns->Clear();
        dataGridView1->Columns->Add("LastName", "Прізвище");
        dataGridView1->Columns->Add("FirstName", "Ім'я");
        dataGridView1->Columns->Add("Group", "Група");
        dataGridView1->Columns->Add("AvgScore", "Сер. Бал (Останній Сем.)");
        dataGridView1->Columns->Add("Semester", "Поточний Сем.");
        dataGridView1->Columns->Add("Year", "Рік Вступу");

        dataGridView1->Columns["LastName"]->Width = 120;
        dataGridView1->Columns["FirstName"]->Width = 120;
        dataGridView1->Columns["Group"]->Width = 80;
        dataGridView1->Columns["AvgScore"]->Width = 140;
        dataGridView1->Columns["Semester"]->Width = 120;
        dataGridView1->Columns["Year"]->Width = 100;
    }

    // Заповнює таблицю списком студентів
    void SortForm::UpdateDataGridView(const std::vector<Student>& list)
    {
        dataGridView1->Rows->Clear();
        for (const auto& student : list)
        {
            dataGridView1->Rows->Add(
                marshal_as<String^>(student.getLastName()),
                marshal_as<String^>(student.getFirstName()),
                marshal_as<String^>(student.getGroup()),
                student.getCurrentAverageScore().ToString("F2"),
                student.getCurrentSemester(),
                student.getStartYear()
            );
        }
    }

    // Визначає унікальні роки вступу та максимальний семестр, заповнює комбобокси
    void SortForm::FillYearSemesterComboBoxes()
    {
        std::set<int> uniqueYears;
        int maxSemester = 0;
        for (const auto& s : *originalList)
        {
            uniqueYears.insert(s.getStartYear());
            if (s.getCurrentSemester() > maxSemester)
                maxSemester = s.getCurrentSemester();
        }

        cbYear->Items->Clear();
        for (int year : uniqueYears)
            cbYear->Items->Add(year.ToString());

        cbSemester->Items->Clear();
        for (int sem = 1; sem <= maxSemester; ++sem)
            cbSemester->Items->Add(sem.ToString());

        if (cbYear->Items->Count > 0) cbYear->SelectedIndex = 0;
        if (cbSemester->Items->Count > 0) cbSemester->SelectedIndex = cbSemester->Items->Count - 1;
    }

    // Сортування методом Шелла за середнім балом (від більшого до меншого)
    void SortForm::ShellSort(std::vector<Student>& arr)
    {
        int n = static_cast<int>(arr.size());
        for (int gap = n / 2; gap > 0; gap /= 2)
        {
            for (int i = gap; i < n; ++i)
            {
                Student temp = std::move(arr[i]);
                int j;
                for (j = i; j >= gap && arr[j - gap].getCurrentAverageScore() < temp.getCurrentAverageScore(); j -= gap)
                {
                    arr[j] = std::move(arr[j - gap]);
                }
                arr[j] = std::move(temp);
            }
        }
    }

    // Фільтрує студентів за роком і мінімальним семестром, сортує методом Шелла і показує результат
    System::Void SortForm::btnSort_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (cbYear->SelectedItem == nullptr || cbSemester->SelectedItem == nullptr)
        {
            MessageBox::Show("Виберіть **Рік Вступу** та **Мінімальний Семестр** для фільтрації.",
                "Попередження", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        int selectedYear = Convert::ToInt32(cbYear->SelectedItem->ToString());
        int selectedSemester = Convert::ToInt32(cbSemester->SelectedItem->ToString());

        std::vector<Student> filteredList;
        for (const auto& s : *originalList)
        {
            if (s.getStartYear() == selectedYear && s.getCurrentSemester() >= selectedSemester)
            {
                filteredList.push_back(s);
            }
        }

        if (filteredList.empty())
        {
            MessageBox::Show("Не знайдено студентів за вибраними критеріями.", "Результат",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
            dataGridView1->Rows->Clear();
            return;
        }

        ShellSort(filteredList);
        UpdateDataGridView(filteredList);

        MessageBox::Show(String::Format("Знайдено {0} студентів. Результат відсортовано за середнім балом (Шелл).",
            filteredList.size()),
            "Сортування завершено", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    // Створює інтерфейс: вибір року, семестру, кнопку сортування і таблицю результатів
    void SortForm::InitializeComponent(void)
    {
        this->SuspendLayout();

        this->lblYear = (gcnew System::Windows::Forms::Label());
        this->cbYear = (gcnew System::Windows::Forms::ComboBox());
        this->lblSemester = (gcnew System::Windows::Forms::Label());
        this->cbSemester = (gcnew System::Windows::Forms::ComboBox());
        this->btnSort = (gcnew System::Windows::Forms::Button());
        this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();

        // Рік вступу
        this->lblYear->Text = L"Рік Вступу:";
        this->lblYear->Location = System::Drawing::Point(10, 15);
        this->lblYear->AutoSize = true;
        this->cbYear->Location = System::Drawing::Point(100, 10);
        this->cbYear->Size = System::Drawing::Size(120, 25);

        // Мінімальний семестр
        this->lblSemester->Text = L"Мін. Семестр:";
        this->lblSemester->Location = System::Drawing::Point(250, 15);
        this->lblSemester->AutoSize = true;
        this->cbSemester->Location = System::Drawing::Point(350, 10);
        this->cbSemester->Size = System::Drawing::Size(120, 25);

        // Кнопка сортування
        this->btnSort->Text = L"Сортувати та Фільтрувати";
        this->btnSort->Location = System::Drawing::Point(490, 10);
        this->btnSort->Size = System::Drawing::Size(200, 30);
        this->btnSort->Click += gcnew System::EventHandler(this, &SortForm::btnSort_Click);

        // Таблиця результатів
        this->dataGridView1->Location = System::Drawing::Point(10, 50);
        this->dataGridView1->Size = System::Drawing::Size(780, 400);
        this->dataGridView1->ReadOnly = true;
        this->dataGridView1->AllowUserToAddRows = false;
        this->dataGridView1->BackgroundColor = System::Drawing::Color::White;

        // Додаємо все на форму
        this->Controls->Add(this->lblYear);
        this->Controls->Add(this->cbYear);
        this->Controls->Add(this->lblSemester);
        this->Controls->Add(this->cbSemester);
        this->Controls->Add(this->btnSort);
        this->Controls->Add(this->dataGridView1);

        this->ClientSize = System::Drawing::Size(800, 460);
        this->Text = L"Сортування Шелла з фільтрацією";

        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
        this->ResumeLayout(false);
        this->PerformLayout();
    }
}