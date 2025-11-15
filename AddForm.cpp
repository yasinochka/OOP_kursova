#include "AddForm.h"
#include <sstream>

using namespace System;
using namespace System::Windows::Forms;
using namespace msclr::interop;

namespace OOPKursova {

    AddForm::AddForm()
    {
        InitializeComponent();
        this->BackColor = System::Drawing::Color::LightYellow;
        this->isEditMode = false;
        this->originalStudent = nullptr;
        this->Text = L"Додавання нового студента";
    }

    AddForm::AddForm(Student* studentToEdit)
    {
        InitializeComponent();
        this->BackColor = System::Drawing::Color::LightYellow;
        this->isEditMode = true;
        this->originalStudent = studentToEdit;

        if (originalStudent) {
            this->Text = L"Редагування студента";
            PopulateFields(originalStudent);
        }
        else {
            this->Text = L"Помилка редагування";
            this->isEditMode = false;
        }
    }

    AddForm::~AddForm()
    {
        if (components)
            delete components;
    }

    void AddForm::PopulateFields(const Student* s)
    {
        txtLastName->Text = marshal_as<String^>(s->getLastName());
        txtFirstName->Text = marshal_as<String^>(s->getFirstName());
        txtDepartment->Text = marshal_as<String^>(s->getDepartment());
        txtGroup->Text = marshal_as<String^>(s->getGroup());
        txtStartYear->Text = s->getStartYear().ToString();

        std::string scoresStr;
        const auto& scores = s->getSemesterScores();
        for (size_t i = 0; i < scores.size(); ++i) {
            scoresStr += std::to_string(scores[i]);
            if (i < scores.size() - 1) scoresStr += ", ";
        }
        txtScores->Text = marshal_as<String^>(scoresStr);
    }

    Student AddForm::getNewStudent()
    {
        std::string ln = marshal_as<std::string>(txtLastName->Text);
        std::string fn = marshal_as<std::string>(txtFirstName->Text);
        std::string dep = marshal_as<std::string>(txtDepartment->Text);
        std::string gr = marshal_as<std::string>(txtGroup->Text);
        int sYear = Convert::ToInt32(txtStartYear->Text);

        std::vector<double> scores;
        std::string scoresStr = marshal_as<std::string>(txtScores->Text);
        std::stringstream ss(scoresStr);
        std::string token;
        int semesterCount = 0;

        while (std::getline(ss, token, ',')) {
            // Видаляємо пробіли
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            if (token.empty()) continue;

            try {
                double score = std::stod(token);
                if (score >= 0.0 && score <= 5.0) {
                    scores.push_back(score);
                    semesterCount++;
                }
            }
            catch (...) {
                // Ігноруємо некоректні значення
            }
        }

        return Student(ln, fn, dep, gr, semesterCount, sYear, scores);
    }

    System::Void AddForm::btnSave_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (txtLastName->Text->Length == 0 || txtFirstName->Text->Length == 0 ||
            txtGroup->Text->Length == 0 || txtStartYear->Text->Length == 0)
        {
            MessageBox::Show("Будь ласка, заповніть усі обов'язкові поля (крім Балів).",
                "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        int startYear;
        if (!Int32::TryParse(txtStartYear->Text, startYear) || startYear < 1900 || startYear > 2100)
        {
            MessageBox::Show("Рік вступу має бути коректним числом (1900-2100).",
                "Помилка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        // Перевірка балів (з попередженням)
        std::string scoresStr = marshal_as<std::string>(txtScores->Text);
        std::stringstream ss(scoresStr);
        std::string token;
        bool hasWarning = false;

        while (std::getline(ss, token, ',')) {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            if (token.empty()) continue;

            try {
                double score = std::stod(token);
                if (score < 0.0 || score > 5.0) {
                    hasWarning = true;
                }
            }
            catch (...) {
                hasWarning = true;
            }
        }

        if (hasWarning) {
            MessageBox::Show("Увага: Деякі бали некоректні або вийшли за межі 0.0-12.0 і будуть проігноровані.",
                "Попередження", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        }

        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void AddForm::InitializeComponent(void)
    {
        this->lblLastName = (gcnew System::Windows::Forms::Label());
        this->txtLastName = (gcnew System::Windows::Forms::TextBox());
        this->lblFirstName = (gcnew System::Windows::Forms::Label());
        this->txtFirstName = (gcnew System::Windows::Forms::TextBox());
        this->lblDepartment = (gcnew System::Windows::Forms::Label());
        this->txtDepartment = (gcnew System::Windows::Forms::TextBox());
        this->lblGroup = (gcnew System::Windows::Forms::Label());
        this->txtGroup = (gcnew System::Windows::Forms::TextBox());
        this->lblStartYear = (gcnew System::Windows::Forms::Label());
        this->txtStartYear = (gcnew System::Windows::Forms::TextBox());
        this->lblScores = (gcnew System::Windows::Forms::Label());
        this->txtScores = (gcnew System::Windows::Forms::TextBox());
        this->btnSave = (gcnew System::Windows::Forms::Button());

        this->SuspendLayout();

        int lblX = 10, txtX = 120, y = 20, step = 35;

        // Прізвище
        this->lblLastName->Text = L"Прізвище:";
        this->lblLastName->Location = System::Drawing::Point(lblX, y);
        this->lblLastName->AutoSize = true;
        this->txtLastName->Location = System::Drawing::Point(txtX, y - 5);
        this->txtLastName->Size = System::Drawing::Size(200, 25);
        y += step;

        // Ім'я
        this->lblFirstName->Text = L"Ім'я:";
        this->lblFirstName->Location = System::Drawing::Point(lblX, y);
        this->lblFirstName->AutoSize = true;
        this->txtFirstName->Location = System::Drawing::Point(txtX, y - 5);
        this->txtFirstName->Size = System::Drawing::Size(200, 25);
        y += step;

        // Кафедра
        this->lblDepartment->Text = L"Кафедра:";
        this->lblDepartment->Location = System::Drawing::Point(lblX, y);
        this->lblDepartment->AutoSize = true;
        this->txtDepartment->Location = System::Drawing::Point(txtX, y - 5);
        this->txtDepartment->Size = System::Drawing::Size(200, 25);
        y += step;

        // Група
        this->lblGroup->Text = L"Група:";
        this->lblGroup->Location = System::Drawing::Point(lblX, y);
        this->lblGroup->AutoSize = true;
        this->txtGroup->Location = System::Drawing::Point(txtX, y - 5);
        this->txtGroup->Size = System::Drawing::Size(200, 25);
        y += step;

        // Рік вступу
        this->lblStartYear->Text = L"Рік вступу:";
        this->lblStartYear->Location = System::Drawing::Point(lblX, y);
        this->lblStartYear->AutoSize = true;
        this->txtStartYear->Location = System::Drawing::Point(txtX, y - 5);
        this->txtStartYear->Size = System::Drawing::Size(200, 25);
        y += step;

        // Бали
        this->lblScores->Text = L"Бали (через кому):\n(0.0-5.0)";
        this->lblScores->Location = System::Drawing::Point(lblX, y);
        this->lblScores->AutoSize = true;
        this->txtScores->Location = System::Drawing::Point(txtX, y - 5);
        this->txtScores->Multiline = true;
        this->txtScores->Size = System::Drawing::Size(200, 60);
        y += 65;

        // Кнопка
        this->btnSave->Text = L"Зберегти";
        this->btnSave->Location = System::Drawing::Point(txtX, y);
        this->btnSave->Size = System::Drawing::Size(200, 30);
        this->btnSave->Click += gcnew System::EventHandler(this, &AddForm::btnSave_Click);

        // Додавання контролів
        this->Controls->Add(this->lblLastName); this->Controls->Add(this->txtLastName);
        this->Controls->Add(this->lblFirstName); this->Controls->Add(this->txtFirstName);
        this->Controls->Add(this->lblDepartment); this->Controls->Add(this->txtDepartment);
        this->Controls->Add(this->lblGroup); this->Controls->Add(this->txtGroup);
        this->Controls->Add(this->lblStartYear); this->Controls->Add(this->txtStartYear);
        this->Controls->Add(this->lblScores); this->Controls->Add(this->txtScores);
        this->Controls->Add(this->btnSave);

        this->ClientSize = System::Drawing::Size(340, y + 40);
        this->ResumeLayout(false);
        this->PerformLayout();
    }
}