#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

#define ERROR "ERROR"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->minValueLineEdit->setReadOnly(true);
    ui->maxValueLineEdit->setReadOnly(true);
    ui->medValueLineEdit->setReadOnly(true);
    ui->averageLineEdit->setReadOnly(true);
    ui->dispValueLineEdit->setReadOnly(true);
    ui->textEdit->setReadOnly(true);
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, QColor(0,0,0,0));
    ui->textEdit->setPalette(p);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectButton_clicked()
{
    string filepath = QFileDialog::getOpenFileName().toStdString();
    if (checkFilepath(filepath) != ERROR)
    {
        vector<string> matrixTmp = readCSV(filepath);
        getStringMatrix(matrixTmp);
    }
    else
        errorMessage();
}

void MainWindow::on_loadButton_clicked()
{
    string region = ui->regionLineEdit->text().toStdString();
    int year = ui->yearLineEdit->text().toInt();
    vector<vector<string>> regionMatrix = getRegionMatrix(region, year);
    if (!regionMatrix.empty())
    {
        ui->tableWidget->setRowCount(regionMatrix.size());
        fillTable(regionMatrix);
    }
    else
        errorMessage();
}


void MainWindow::on_calcButton_clicked()
{
    double min, max, med, avg;
    double disp = 0;
    if (ui->columnNumberLineEdit->text().isEmpty())
        errorMessage();
    else
    {
        int column = ui->columnNumberLineEdit->text().toInt();
        if (column > 0 && column <= 7 && column != 2)
        {
            getMinMaxValueFromMatrix(&min, &max, column-1);
            getMedianFromMatrix(&med, column-1);
            getAverageFromMatrix(&avg, column-1);
            getDispFromMatrix(&disp, &avg, column-1);
            ui->maxValueLineEdit->setText(QString::number(min));
            ui->minValueLineEdit->setText(QString::number(max));
            ui->medValueLineEdit->setText(QString::number(med));
            ui->averageLineEdit->setText(QString::number(avg));
            ui->dispValueLineEdit->setText(QString::number(disp));
        }
        else
        {
            errorMessage();
        }
    }
}

void MainWindow::fillTable(vector<vector<string>> regionMatrix)
{
    if (!regionMatrix.empty())
        for (int i = 0; i < regionMatrix.size(); i++)
            for (int j = 0; j < regionMatrix[i].size(); j++)
            {
                QString text = QString::fromStdString(regionMatrix[i][j]);
                QTableWidgetItem *Item = new QTableWidgetItem(text);
                Item -> setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ui->tableWidget->setItem(i, j, Item);
            }
    else
        errorMessage();
}

void MainWindow::errorMessage()
{
    QMessageBox msgBox;
    msgBox.setText("Ошибка!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText("Попробуйте еще раз");
    msgBox.exec();
}


void MainWindow::on_drawButton_clicked()
{
    ui->graph->clearGraphs();
    if (ui->graphColumnLineEdit_1->text().toUInt() > 2 && ui->graphColumnLineEdit_1->text().toUInt() < 8)
    {
        string regionListString = ui->graphRegionLineEdit_1->toPlainText().toStdString();
        vector<string> regionList = splitString(regionListString, "\n");
        QCustomPlot *graph = ui->graph;
        graph->legend->setVisible(true);
        graph->legend->setBrush(QColor(255, 255, 255, 150));
        graph->setInteraction(QCP::iRangeZoom, QCP::iRangeDrag);
        for (int i = 0, k = 0; i < regionList.size(); i++, k+=2)
        {
            if (!getMatrixByRegion(&regionList[i]).empty())
                buildGraph(regionList[i], k);
            else
            {
                errorMessage();
                break;
            }

        }
    }
    else
        errorMessage();
}
void MainWindow::buildGraph(string graphRegion, int graphNumber)
{
    QCustomPlot *graph = ui->graph;
    vector<double> x_axis, y_axis;
    double min_y, max_y, med_y;
    getAxisData(x_axis, y_axis, &graphRegion, ui->graphColumnLineEdit_1->text().toInt()-1);
    if (!x_axis.empty() && !y_axis.empty())
    {
        graph->addGraph();
        QVector<double> x_axis_qvector = QVector<double>::fromStdVector(x_axis);
        QVector<double> y_axis_qvector = QVector<double>::fromStdVector(y_axis);
        getGraphMinMax(&max_y, &min_y, ui->graphColumnLineEdit_1->text().toInt()-1, &graphRegion);
        getGraphMedian(&med_y, ui->graphColumnLineEdit_1->text().toInt()-1, &graphRegion);
        graph->graph(graphNumber)->setData(x_axis_qvector, y_axis_qvector);
        graph->xAxis->setRange(x_axis_qvector[0]-2, x_axis_qvector[x_axis_qvector.size()-1]+2);
        graphSetRangeY(graphNumber, min_y, max_y);
        graph->addGraph(ui->graph->xAxis, ui->graph->yAxis);
        vector<double> x(3), y(3);
        y[0]=min_y; y[1]=max_y; y[2]=med_y;
        getXByY(x, y, &graphRegion, ui->graphColumnLineEdit_1->text().toInt()-1);
        QVector<double> extremum_x = QVector<double>::fromStdVector(x);
        QVector<double> extremum_y = QVector<double>::fromStdVector(y);
        graph->graph(graphNumber+1)->addData(extremum_x, extremum_y);
        graphSettings(graphRegion, graphNumber);
        graph->replot();
    }
    else
        errorMessage();
}

Qt::GlobalColor MainWindow::getColor(int graphNumber)
{
    return Qt::GlobalColor((7+graphNumber)%19);
}

void MainWindow::graphSettings(string graphRegion, int graphNumber)
{
    QCustomPlot *graph = ui->graph;
    Qt::GlobalColor color = getColor(graphNumber);
    graph->graph(graphNumber)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    graph->graph(graphNumber)->setPen(QPen(color));
    graph->graph(graphNumber)->removeFromLegend();
    graph->graph(graphNumber+1)->setLineStyle(QCPGraph::lsNone);
    graph->graph(graphNumber+1)->setName(QString::fromStdString(graphRegion));
    graph->graph(graphNumber+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, color, color, 7));

}

void MainWindow::graphSetRangeY(int graphNumber, double &min_y, double &max_y)
{
    QCustomPlot *graph = ui->graph;
    if (graphNumber > 0)
    {
        double min_y_prev = graph->yAxis->range().lower;
        double max_y_prev = graph->yAxis->range().upper;
        if (min_y_prev <= min_y)
            min_y = min_y_prev;
        if (max_y_prev >= max_y)
            max_y = max_y_prev;
    }
    graph->yAxis->setRange(min_y-0.15, max_y+0.15);
}
