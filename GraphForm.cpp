#include "GraphForm.h"
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms::DataVisualization::Charting;
using namespace msclr::interop;

namespace OOPKursova {

    // Конструктор — створює форму, встановлює заголовок з ПІБ студента і будує графік
    GraphForm::GraphForm(const Student& selectedStudent)
    {
        InitializeComponent();
        this->BackColor = System::Drawing::Color::LightCyan;

        String^ title = marshal_as<String^>(selectedStudent.getLastName()) + " " +
            marshal_as<String^>(selectedStudent.getFirstName()) +
            L" - Графік Успішності";
        this->Text = title;

        BuildChart(selectedStudent);
    }

    // Деструктор — звільняє компоненти форми
    GraphForm::~GraphForm()
    {
        if (components)
            delete components;
    }

    // Будує графік успішності: очищає попередні дані, створює область, серію, додає точки і налаштовує осі
    void GraphForm::BuildChart(const Student& student)
    {
        chartSuccess->Series->Clear();
        chartSuccess->Titles->Clear();
        chartSuccess->ChartAreas->Clear();

        ChartArea^ chartArea1 = gcnew ChartArea();
        chartSuccess->ChartAreas->Add(chartArea1);

        // Додає заголовок графіка
        Title^ mainTitle = gcnew Title(L"Динаміка середнього балу по семестрах");
        chartSuccess->Titles->Add(mainTitle);

        // Створює лінію з маркерами для відображення балів
        Series^ seriesScore = gcnew Series(L"Середній Бал");
        seriesScore->ChartType = SeriesChartType::Line;
        seriesScore->BorderWidth = 3;
        seriesScore->Color = Color::Red;
        seriesScore->MarkerStyle = MarkerStyle::Circle;
        seriesScore->MarkerSize = 8;
        seriesScore->IsValueShownAsLabel = true;
        seriesScore->Label = "#VALY{F2}";

        // Додає точки (семестр ? бал) з мітками
        const std::vector<double>& scores = student.getSemesterScores();
        for (size_t i = 0; i < scores.size(); ++i)
        {
            int semesterNumber = static_cast<int>(i) + 1;
            String^ label = L"Сем " + semesterNumber;
            seriesScore->Points->AddXY(label, scores[i]);
        }
        chartSuccess->Series->Add(seriesScore);

        // Налаштовує підписи та діапазон осей
        chartSuccess->ChartAreas[0]->AxisX->Title = L"Семестр";
        chartSuccess->ChartAreas[0]->AxisX->Interval = 1;
        chartSuccess->ChartAreas[0]->AxisY->Title = L"Бал (5-бальна шкала)";
        chartSuccess->ChartAreas[0]->AxisY->Minimum = 2.0;
        chartSuccess->ChartAreas[0]->AxisY->Maximum = 5.0;
        chartSuccess->ChartAreas[0]->AxisY->Interval = 0.5;
    }

    // Створює елемент Chart, розміщує його на формі та налаштовує розміри вікна
    void GraphForm::InitializeComponent(void)
    {
        this->SuspendLayout();

        this->chartSuccess = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartSuccess))->BeginInit();

        // Налаштовує розміщення та розмір графіка
        this->chartSuccess->Location = System::Drawing::Point(10, 10);
        this->chartSuccess->Size = System::Drawing::Size(780, 580);
        this->chartSuccess->Name = L"chartSuccess";

        // Додає графік на форму та встановлює розмір вікна
        this->Controls->Add(this->chartSuccess);
        this->ClientSize = System::Drawing::Size(800, 600);
        this->Text = L"Графік успішності студента";

        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartSuccess))->EndInit();
        this->ResumeLayout(false);
    }
}