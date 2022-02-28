#include<QtWidgets/qwidget.h>
#include<map>
#include<QtWidgets/qlabel.h>
#include<QtWidgets/qboxlayout.h>
class DisplayPanel: public QWidget
{

    private:
    struct Entry{
    QLabel * label=nullptr;;
    QLabel* value=nullptr;
    };
    
    QVBoxLayout *rows=nullptr;
    std::vector<Entry> respEntries;
    public:
    DisplayPanel();
    void AddDataEntry(const std::string &label,double data);
    
    void Clear();

};
