#pragma once
#define NOMINMAX
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <string>
#include "Student.h"

namespace OOPKursova {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace msclr::interop;

    public ref class AddForm : public System::Windows::Forms::Form
    {
    public:
        // Конструктор для ДОДАВАННЯ нового студента
        AddForm();

        // Конструктор для РЕДАГУВАННЯ існуючого студента
        AddForm(Student* studentToEdit);

        // Публічна властивість: повертає створений/відредагований об'єкт Student
        Student getNewStudent();

    protected:
        ~AddForm();

    private:
        Student* originalStudent;
        bool isEditMode;

        // Controls
        System::Windows::Forms::Label^ lblLastName;
        System::Windows::Forms::TextBox^ txtLastName;
        System::Windows::Forms::Label^ lblFirstName;
        System::Windows::Forms::TextBox^ txtFirstName;
        System::Windows::Forms::Label^ lblDepartment;
        System::Windows::Forms::TextBox^ txtDepartment;
        System::Windows::Forms::Label^ lblGroup;
        System::Windows::Forms::TextBox^ txtGroup;
        System::Windows::Forms::Label^ lblStartYear;
        System::Windows::Forms::TextBox^ txtStartYear;
        System::Windows::Forms::Label^ lblScores;
        System::Windows::Forms::TextBox^ txtScores;
        System::Windows::Forms::Button^ btnSave;

        System::ComponentModel::Container^ components;

        // Методи
        void PopulateFields(const Student* s);
        System::Void btnSave_Click(System::Object^ sender, System::EventArgs^ e);

        // Дизайнер
        void InitializeComponent(void);
    };
}