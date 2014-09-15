#ifndef LOADINGFORM_H
#define LOADINGFORM_H

#include <QWidget>

namespace Ui {
class loadingForm;
}

class loadingForm : public QWidget
{
    Q_OBJECT

public:
    explicit loadingForm(QWidget *parent = 0);
    ~loadingForm();

    void setUi();
    void setText(QString text);
private:
    Ui::loadingForm *ui;
};

#endif // LOADINGFORM_H
