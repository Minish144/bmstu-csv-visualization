#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <qcustomplot.h>
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_selectButton_clicked();

    void on_loadButton_clicked();

    void on_calcButton_clicked();

    void fillTable(std::vector<std::vector<std::string>> regionMatrix);

    void errorMessage();

    void on_drawButton_clicked();

    void buildGraph(std::string graphRegion, int graphNumber);

    void graphSettings(std::string graphRegion, int graphNumber);

    Qt::GlobalColor getColor(int graphNumber);

    void graphSetRangeY(int graphNumber, double &min_y, double &max_y);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
