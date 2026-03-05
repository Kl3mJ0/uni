#include "option.h"
#include "ui_option.h"

option::option(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::option)
{
    ui->setupUi(this);
}

option::~option()
{
    delete ui;
}
void option::loadFromPart(ModelPart* part)
{
    if (!part) return;

    ui->lineEditName->setText(part->data(0).toString());
    ui->checkVisible->setChecked(part->data(1).toString() == "true");

    ui->spinR->setValue(part->getColourR());
    ui->spinG->setValue(part->getColourG());
    ui->spinB->setValue(part->getColourB());
}

void option::saveToPart(ModelPart* part)
{
    if (!part) return;

    part->set(0, ui->lineEditName->text());
    part->set(1, ui->checkVisible->isChecked() ? "true" : "false");

    part->setColour(
        static_cast<unsigned char>(ui->spinR->value()),
        static_cast<unsigned char>(ui->spinG->value()),
        static_cast<unsigned char>(ui->spinB->value())
    );
}