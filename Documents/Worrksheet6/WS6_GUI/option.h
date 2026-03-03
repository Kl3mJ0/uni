#ifndef OPTION_H
#define OPTION_H

#include <QDialog>
#include "ModelPart.h"

namespace Ui {
class option;
}

class option : public QDialog
{
    Q_OBJECT

public:
    void loadFromPart(ModelPart* part);
    void saveToPart(ModelPart* part);
    explicit option(QWidget *parent = nullptr);
    ~option();

private:
    Ui::option *ui;
};

#endif // OPTION_H
