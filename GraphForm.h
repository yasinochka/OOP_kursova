#pragma once
#define NOMINMAX
#include <msclr/marshal_cppstd.h>
#include "Student.h"
#include <vector>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::DataVisualization::Charting;
using namespace msclr::interop;

namespace OOPKursova {

    public ref class GraphForm : public System::Windows::Forms::Form
    {
    public:
        // Конструктор — створює форму з графіком успішності для переданого студента
        GraphForm(const Student& selectedStudent);

    protected:
        // Деструктор — звільняє ресурси форми та компонентів
        ~GraphForm();

    private:
        System::Windows::Forms::DataVisualization::Charting::Chart^ chartSuccess;
        System::ComponentModel::Container^ components;

        // Будує графік успішності: додає точки (семестр ? середній бал) і налаштовує вигляд
        void BuildChart(const Student& student);

        // Створює елемент Chart, налаштовує розміри, осі, заголовок і додає його на форму
        void InitializeComponent(void);
    };
}