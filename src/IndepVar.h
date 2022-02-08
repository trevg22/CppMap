#pragma once
#include<QtWidgets/qlabel.h>
#include<QtWidgets/qcombobox.h>
#include<string>
#include<vector>
#include<qboxlayout.h>
class IndepVar {
private:
    QLabel * name=nullptr;
    QComboBox *optionsDrop=nullptr;
    QVBoxLayout *vLayout=nullptr;

public:
    IndepVar(const std::string &name);
    void SetOptions(const std::vector<std::string> &labels);
    QComboBox *GetDrop();
    QVBoxLayout * GetVLayout();
};
