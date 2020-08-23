#include "opengarage.h"
#include "ui_opengarage.h"
#include <QDir>
#include <QFileDialog>

OpenGarage::OpenGarage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenGarage)
{
    ui->setupUi(this);
    ui->location->setText(QDir::homePath());
}

OpenGarage::~OpenGarage()
{
    delete ui;
}

void OpenGarage::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Open)) {


    }
}

void OpenGarage::on_Browse_clicked()
{
    QString location = QFileDialog::getExistingDirectory(this, "Open", QDir::homePath());
    ui->location->setText(location);
}
