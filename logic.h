#ifndef LOGIC_H
#define LOGIC_H

#define ERROR "ERROR"
#define OK "OK"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDebug>

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<string> readCSV(string filepath);

vector<string> splitString(string str, string sep);

void getStringMatrix(vector<string>& array);

vector<vector<string>> getRegionMatrix(string region, int year);

void getMinMaxValueFromMatrix(double *min, double *max, int column);

void getMedianFromMatrix(double *med, int column);

void getAverageFromMatrix(double *avg, int column);

void getDispFromMatrix(double *disp, double *avg, int column);

string checkFilepath(string filepath);

vector<vector<string>> getMatrixByRegion(string *region);

void getGraphMinMax(double *min, double *max, int column, string *region);

void getGraphMedian(double *med, int column, string *region);

void getAxisData(vector<double> &x_axis, vector<double> &y_axis, string *region, int column);

void getXByY(vector<double> &x_array, vector<double> &y_array, string *region, int column);

#endif // LOGIC_H
