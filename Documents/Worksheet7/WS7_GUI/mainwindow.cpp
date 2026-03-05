#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QFileInfo>
#include "option.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.0, 0.0, 0.0);
    renderWindow->AddRenderer(renderer);
    renderWindow->Render();
    ui->treeView->addAction(ui->actionItem_Options);

   
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    // Create/allocate the ModelPartList
    this->partList = new ModelPartList("Parts List");

    // Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // Build a demo model tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top-level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        // Add 5 sub-items under each top-level item
        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* childChildItem = new ModelPart({ childName, childVisible });
            childItem->appendChild(childChildItem);
        }
    }
    connect(ui->pushButton_2, &QPushButton::released,
        this, &MainWindow::handleAddButton);

    connect(ui->pushButton, &QPushButton::released,
        this, &MainWindow::handleOptionsButton);

    connect(ui->treeView, &QTreeView::clicked,
        this, &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleAddButton() {
    emit statusUpdateMessage("Add button was clicked", 0);
}

void MainWindow::handleOptionsButton()
{
    // 1. Get selected tree item
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage("Select an item first", 0);
        return;
    }

    // 2. Get underlying ModelPart
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    // 3. Open dialog
    option dialog(this);
    dialog.loadFromPart(part);

    if (dialog.exec() == QDialog::Accepted) {

        // 4. Save values back to ModelPart
        dialog.saveToPart(part);

        // 5. Refresh tree view display
        QModelIndex left = index.sibling(index.row(), 0);
        QModelIndex right = index.sibling(index.row(), 1);
        emit partList->dataChanged(left, right);

        emit statusUpdateMessage("Options saved", 0);
    }
}
void MainWindow::handleTreeClicked() {

    // Get selected index
    QModelIndex index = ui->treeView->currentIndex();

    // Get pointer to selected item
    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
        return;

    // Get the name (column 0)
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(
        "The selected item is: " + text, 0);
}
void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open STL"),
        "C:\\",
        tr("STL Files (*.stl)")
    );

    if (fileName.isEmpty())
        return;

    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Select a tree item first", 0);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    // 1) Load STL into that part
    part->loadSTL(fileName);

    // 2) Rename part in tree
    QString justName = QFileInfo(fileName).fileName();
    part->set(0, justName);

    // 3) Refresh tree view row
    QModelIndex left = index.sibling(index.row(), 0);
    QModelIndex right = index.sibling(index.row(), 1);
    emit partList->dataChanged(left, right);

    // 4) Re-render from the tree
    updateRender();

    emit statusUpdateMessage("Loaded STL: " + justName, 0);
}
void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("Select an item first", 0);
        return;
    }

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    option dialog(this);
    dialog.loadFromPart(part);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToPart(part);

        QModelIndex left = index.sibling(index.row(), 0);
        QModelIndex right = index.sibling(index.row(), 1);
        emit partList->dataChanged(left, right);

        emit statusUpdateMessage("Options saved", 0);
    }
}
void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->ResetCamera();
    renderWindow->Render();
}
void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if (part && part->visible() && part->getActor())
    {
        renderer->AddActor(part->getActor());
    }

    int rows = partList->rowCount(index);

    for (int i = 0; i < rows; i++)
    {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}