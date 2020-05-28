#include "logic.h"

vector<vector<string>> matrix;
vector<vector<string>> regionMatrix;
int loadFlag = 0;

vector<string> readCSV(string filepath)
{
    ifstream file(filepath);
    string tmp;
    vector<string> sArray;
    if (!file.bad())
    {
        for (int i = 0; getline(file, tmp); i++)
        {
            sArray.push_back(tmp);
        }
        for (int i = 0; i  < sArray.size(); i++)
        {
            qDebug() << QString::fromStdString(sArray[i]);
        }
    }
    return sArray;
}

vector<string> splitString(string str, string sep)
{
    vector<string> arr;
    size_t prev = 0;
    size_t next;
    size_t delta = 1;
    while ((next = str.find(sep, prev)) != string::npos)
    {
        arr.push_back(str.substr(prev, next - prev));
        prev = next + delta;
    }
    arr.push_back(str.substr(prev));
    return arr;
}

void getStringMatrix(vector<string>& array)
{
    vector<vector<string>> tempMatrix;
    vector<string> tempArray;
    for (int i = 0; i < array.size(); i++)
    {
        tempArray = splitString(array[i], ",");
        tempMatrix.push_back(tempArray);
    }
    matrix = tempMatrix;
}

vector<vector<string>> getRegionMatrix(string region, int year)
{
    vector<vector<string>> tempMatrix;
    if (!matrix.empty() && !region.empty())
    for (int i = 0; i < matrix.size()-1; i++)
    {
        if (matrix[i][1] == region && stod(matrix[i][0]) >= year)
            tempMatrix.push_back(matrix[i]);
    }
    regionMatrix = tempMatrix;
    loadFlag = 1;
    return tempMatrix;
}

void getMinMaxValueFromMatrix(double *min, double *max, int column)
{
    if (!regionMatrix.empty() && loadFlag==1)
    {
        *min = stod(regionMatrix[0][column]);
        *max = stod(regionMatrix[0][column]);
        double value;
        for (int i = 0; i < regionMatrix.size(); i++)
        {
            value = stod(regionMatrix[i][column]);
            if (value > *min)
                *min = value;
            else if (value < *max)
                *max = value;
        }
    }
}

void getMedianFromMatrix(double *med, int column)
{
    if (!regionMatrix.empty() && loadFlag==1)
    {
        vector<double> array;
        for (int i = 0; i < regionMatrix.size(); i++)
            array.push_back(stod(regionMatrix[i][column]));
        sort(array.begin(), array.end());
        *med = array[ round((regionMatrix.size()-1)/2) ];
    }
}

string checkFilepath(string filepath)
{
    if (!filepath.empty() && filepath.substr(filepath.length()-3, 3) == "csv")
        return filepath;
    else
        return ERROR;
}

void getAverageFromMatrix(double *avg, int column)
{
    if (!regionMatrix.empty() && loadFlag==1)
    {
        double counter = 0;
        for (int i = 0; i < regionMatrix.size(); i++)
            counter += stod(regionMatrix[i][column]);
        *avg = counter / regionMatrix.size();
    }
}

void getDispFromMatrix(double *disp, double *avg, int column)
{
    if (!regionMatrix.empty())
    {
        double n = regionMatrix.size();
        for (int i = 0; i < regionMatrix.size()-1; i++)
            *disp += pow((stod(regionMatrix[i][column]) - *avg), 2)/n;
    }
}

vector<vector<string>> getMatrixByRegion(string *region)
{
    vector<vector<string>> tempMatrix;
    if (!matrix.empty() && !(*region).empty())
    for (int i = 0; i < matrix.size()-1; i++)
    {
        if (matrix[i][1] == *region)
            tempMatrix.push_back(matrix[i]);
    }
    return tempMatrix;
}

void getGraphMinMax(double *min, double *max, int column, string *region)
{
    vector<vector<string>> matrixByRegion = getMatrixByRegion(region);
    *min = stod(matrixByRegion[0][column]);
    *max = stod(matrixByRegion[0][column]);
    double value = NULL;
    for (int i = 0; i < matrixByRegion.size(); i++)
    {
        value = stod(matrixByRegion[i][column]);
        if (value > *min)
            *min = value;
        else if (value < *max)
            *max = value;
    }
}

void getGraphMedian(double *med, int column, string *region)
{
    vector<vector<string>> matrixByRegion = getMatrixByRegion(region);
    vector<double> array;
    for (int i = 0; i < matrixByRegion.size(); i++)
        array.push_back(stod(matrixByRegion[i][column]));
    sort(array.begin(), array.end());
    *med = array[ round((matrixByRegion.size()-1)/2) ];
}
void getAxisData(vector<double> &x_axis, vector<double> &y_axis, string *region, int column)
{
    vector<vector<string>> matrixByRegion = getMatrixByRegion(region);
    for (int i = 0; i < matrixByRegion.size(); i++)
    {
        x_axis.push_back(stoi(matrixByRegion[i][0]));
        y_axis.push_back(stod(matrixByRegion[i][column]));
    }
}

void getXByY(vector<double> &x_array, vector<double> &y_array, string *region, int column)
{
    vector<vector<string>> matrixByRegion = getMatrixByRegion(region);
    for (int i = 0; i < matrixByRegion.size(); i++)
    {
        if (stod(matrixByRegion[i][column]) == y_array[0])
            x_array[0] = stod(matrixByRegion[i][0]);
        else if (stod(matrixByRegion[i][column]) == y_array[1])
            x_array[1] = stod(matrixByRegion[i][0]);
        else if (stod(matrixByRegion[i][column]) == y_array[2])
            x_array[2] = stod(matrixByRegion[i][0]);
    }
     qDebug() << x_array;
}
