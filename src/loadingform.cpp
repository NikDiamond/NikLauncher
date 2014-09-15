#include "loadingform.h"
#include "ui_loadingform.h"

loadingForm::loadingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loadingForm)
{
    ui->setupUi(this);
}

loadingForm::~loadingForm()
{
    delete ui;
}

void loadingForm::setUi()
{
    setLayout(ui->loadLayout);
    ui->frame->setMaximumSize(maximumWidth(),maximumHeight());
    ui->frame->setMinimumSize(maximumWidth(),maximumHeight());
}

void loadingForm::setText(QString text)
{
    ui->label->setText(text);
}
