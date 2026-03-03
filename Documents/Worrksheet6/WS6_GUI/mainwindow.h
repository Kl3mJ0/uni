#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ModelPart.h"
#include "ModelPartList.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

signals:
    void statusUpdateMessage(const QString& message, int timeout);

private slots:
    void handleAddButton();
    void handleOptionsButton();
    void handleTreeClicked();
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();
};
#endif // MAINWINDOW_H
