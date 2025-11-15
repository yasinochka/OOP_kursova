#include "StatsForm.h"
#include <set>
#include <map>
using namespace System;
using namespace System::Windows::Forms;
using namespace msclr::interop;
using namespace System::Drawing;

namespace OOPKursova {

    // Допоміжний компаратор для пошуку max/min за середнім балом
    struct CompareAvgScore {
        bool operator()(const std::pair<const int, double>& a, const std::pair<const int, double>& b) const {
            return a.second < b.second;
        }
    };

    // === Конструктор ===
    StatsForm::StatsForm(const std::vector<Student>& initialList)
    {
        InitializeComponent();
        this->BackColor = Color::LightBlue;
        this->studentListCopy = new std::vector<Student>(initialList);
        this->currentMode = Mode::BestWorstYears;

        try {
            CalculateAndDisplayStats(currentMode);
        }
        catch (Exception^ ex) {
            MessageBox::Show(ex->Message, L"Помилка статистики", MessageBoxButtons::OK, MessageBoxIcon::Error);
            lblResult->Text = L"Виникла помилка при обчисленні.";
        }
    }

    // Деструктор — звільняє пам'ять
    StatsForm::~StatsForm()
    {
        if (components)
            delete components;
        delete studentListCopy;
    }

    // Змінює режим і оновлює інтерфейс та статистику
    void StatsForm::SetMode(Mode mode)
    {
        this->currentMode = mode;

        if (mode == Mode::BestGroup) {
            this->Text = L"Визначення Найкращої Групи за Рік Вступу";
            lblYearFilter->Visible = true;
            cbYearFilter->Visible = true;
            btnApplyGroupFilter->Visible = true;
            lblResult->Text = L"Виберіть Рік Вступу для аналізу:";

            try {
                FillYearFilterComboBox();
                if (cbYearFilter->Items->Count > 0)
                    btnApplyGroupFilter_Click(nullptr, nullptr);
            }
            catch (Exception^ ex) {
                MessageBox::Show(ex->Message, L"Помилка фільтра", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
        else {
            this->Text = L"Статистика Успішності по Рокам Вступу";
            lblYearFilter->Visible = false;
            cbYearFilter->Visible = false;
            btnApplyGroupFilter->Visible = false;

            try {
                CalculateAndDisplayStats(mode);
            }
            catch (Exception^ ex) {
                MessageBox::Show(ex->Message, L"Помилка статистики", MessageBoxButtons::OK, MessageBoxIcon::Error);
                lblResult->Text = L"Не вдалося розрахувати статистику.";
            }
        }
    }

    // Заповнює комбобокс унікальними роками вступу
    void StatsForm::FillYearFilterComboBox()
    {
        std::set<int> uniqueYears;
        for (const auto& s : *studentListCopy)
            uniqueYears.insert(s.getStartYear());

        cbYearFilter->Items->Clear();
        for (int year : uniqueYears)
            cbYearFilter->Items->Add(year.ToString());

        if (cbYearFilter->Items->Count > 0)
            cbYearFilter->SelectedIndex = 0;
    }

    // Розраховує і виводить статистику по рокам (найкращий/найгірший рік)
    void StatsForm::CalculateAndDisplayStats(Mode mode)
    {
        if (studentListCopy->empty())
        {
            lblResult->Text = L"Дані відсутні.";
            return;
        }

        if (mode == Mode::BestWorstYears)
        {
            std::map<int, std::pair<double, int>> yearStats; // {сума балів, кількість}

            for (const auto& s : *studentListCopy)
            {
                double avgScore = s.getOverallAverageScore();
                if (avgScore > 0.0)
                {
                    yearStats[s.getStartYear()].first += avgScore;
                    yearStats[s.getStartYear()].second++;
                }
            }

            if (yearStats.empty())
            {
                lblResult->Text = L"Немає даних з балами для розрахунку.";
                return;
            }

            std::map<int, double> finalYearAverages;
            for (const auto& p : yearStats)
                finalYearAverages[p.first] = p.second.first / p.second.second;

            auto bestYear = std::max_element(finalYearAverages.begin(), finalYearAverages.end(), CompareAvgScore());
            auto worstYear = std::min_element(finalYearAverages.begin(), finalYearAverages.end(), CompareAvgScore());

            String^ result = L"Статистика успішності (Загальний середній бал):\n\n";
            for (const auto& p : finalYearAverages)
                result += String::Format(L"Рік вступу **{0}**: Сер. бал = {1:F3}\n", p.first, p.second);

            result += L"\n---\n";
            result += String::Format(L"Найуспішніший рік: {0} (Сер. бал: {1:F3})\n", bestYear->first, bestYear->second);
            result += String::Format(L"Найгірший рік: {0} (Сер. бал: {1:F3})", worstYear->first, worstYear->second);

            lblResult->Text = result;
        }
    }

    // Знаходить найкращу групу серед студентів вибраного року вступу
    System::Void StatsForm::btnApplyGroupFilter_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (cbYearFilter->SelectedItem == nullptr)
        {
            lblResult->Text = L"Виберіть Рік Вступу.";
            return;
        }

        int selectedYear = Convert::ToInt32(cbYearFilter->SelectedItem->ToString());

        std::map<std::string, std::pair<double, int>> groupStats; // {сума балів, кількість}

        for (const auto& s : *studentListCopy)
        {
            if (s.getStartYear() == selectedYear)
            {
                double currentScore = s.getCurrentAverageScore();
                if (currentScore > 0.0)
                {
                    groupStats[s.getGroup()].first += currentScore;
                    groupStats[s.getGroup()].second++;
                }
            }
        }

        if (groupStats.empty())
        {
            lblResult->Text = String::Format(L"Не знайдено студентів, які вступили у {0} році.", selectedYear);
            return;
        }

        std::map<std::string, double> finalGroupAverages;
        for (const auto& p : groupStats)
            finalGroupAverages[p.first] = p.second.first / p.second.second;

        // Пошук найкращої групи
        auto bestGroup = finalGroupAverages.begin();
        for (auto it = finalGroupAverages.begin(); it != finalGroupAverages.end(); ++it)
            if (it->second > bestGroup->second)
                bestGroup = it;

        String^ result = String::Format(L"Результати для студентів, які вступили у {0} році:\n\n", selectedYear);
        for (const auto& p : finalGroupAverages)
        {
            int count = groupStats[p.first].second;
            result += String::Format(L"Група {0}: Сер. бал = {1:F3} ({2} студ.)\n",
                marshal_as<String^>(p.first), p.second, count);
        }

        result += L"\n---\n";
        result += String::Format(L"Найкраща група: {0} (Сер. бал: {1:F3})",
            marshal_as<String^>(bestGroup->first), bestGroup->second);

        lblResult->Text = result;
    }

    // Створює інтерфейс: великий текстовий результат + фільтр по року (прихований за замовчуванням)
    void StatsForm::InitializeComponent(void)
    {
        this->SuspendLayout();

        this->lblResult = (gcnew Label());
        this->lblYearFilter = (gcnew Label());
        this->cbYearFilter = (gcnew ComboBox());
        this->btnApplyGroupFilter = (gcnew Button());

        // Результат
        this->lblResult->Location = System::Drawing::Point(10, 10);
        this->lblResult->AutoSize = true;
        this->lblResult->MaximumSize = System::Drawing::Size(480, 0);
        this->lblResult->Text = L"Результати розрахунків...";

        // Фільтр по року (використовується тільки в режимі BestGroup)
        this->lblYearFilter->Text = L"Виберіть Рік Вступу:";
        this->lblYearFilter->Location = System::Drawing::Point(10, 300);
        this->lblYearFilter->AutoSize = true;
        this->lblYearFilter->Visible = false;

        this->cbYearFilter->Location = System::Drawing::Point(150, 295);
        this->cbYearFilter->Size = System::Drawing::Size(120, 25);
        this->cbYearFilter->Visible = false;

        this->btnApplyGroupFilter->Text = L"Знайти Найкращу Групу";
        this->btnApplyGroupFilter->Location = System::Drawing::Point(280, 295);
        this->btnApplyGroupFilter->Size = System::Drawing::Size(200, 30);
        this->btnApplyGroupFilter->Click += gcnew EventHandler(this, &StatsForm::btnApplyGroupFilter_Click);
        this->btnApplyGroupFilter->Visible = false;

        // Додаємо на форму
        this->Controls->Add(this->lblResult);
        this->Controls->Add(this->lblYearFilter);
        this->Controls->Add(this->cbYearFilter);
        this->Controls->Add(this->btnApplyGroupFilter);

        this->ClientSize = System::Drawing::Size(500, 340);
        this->Text = L"Статистика";

        this->ResumeLayout(false);
        this->PerformLayout();
    }
}