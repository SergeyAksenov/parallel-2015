#ifndef Matrix_h
#define Matrix_h

#include <cstdlib>
#include <random>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

template<typename T>
class AbstractMatrix
{
    
public:
    
    virtual T at(const int& i, const int& j) const = 0;
    
    virtual void set (const int& i, const int& j, const T& newVal) = 0;
};



template<typename T>
class Matrix : public AbstractMatrix<T>
{
    friend class WorkerJob;
    friend class StartCommand;
    
public:
    
    Matrix(const int& width, const int& heigth): _height(heigth), _width(width), _field(vector<vector<T> >(heigth, vector<T>(width))) {}
    
    Matrix(const vector<vector<T> >& v): _field()
    {
        _height = v.size();
        _width = v[0].size();
        for (int i = 0; i < v.size(); ++i)//auto w : v)
        {
            _field.push_back(v[i]);
        }
    }
    
    T at (const int& i, const int& j) const
    {
        return _field[(_height + i) % _height][(_width + j) % _width];
    }
    
    void set (const int& i, const int& j, const T& newVal)
    {
        _field[(_height + i) % _height][(_width + j) % _width] = newVal;
    }
    
    unsigned getHeigth ()
    {
        return _height;
    }
    
    unsigned getWidth ()
    {
        return _width;
    }
    
    void show()
    {
        for (int i = 0; i < _field.size(); ++i)//auto u:_field)
        {
            vector<T> u = _field[i];
            for (int j = 0; j < u.size(); ++i)//auto v : u)
            {
                T v = u[i];
                v == 1 ? cout << "*" << " " : cout << " " << " ";
                //cout << v << " ";
            }
            cout << endl;
        }
    }
private:
    
    unsigned _height;
    
    unsigned _width;
    
    vector<vector<T> > _field;
    
};


template<typename T>
class MatrixView : AbstractMatrix<T>
{
    friend class WorkerJob;
    
public:
    
    
    
    MatrixView (Matrix<T>* M, int ulx, int uly, int drx, int dry): _Matrix(M)
    {
        _upLeft = make_pair(ulx, uly);
        _downRight = make_pair(drx, dry);
    }
    
    T at (const int& i, const int& j) const
    {
        ///////try-catch
        Matrix<unsigned> a = *_Matrix;
        return _Matrix->at(i + _upLeft.first, j + _upLeft.second);
    }
    
    void set (const int& i, const int& j, const T& newVal)
    {
        ///////try-catch
        _Matrix->set(i + _upLeft.first, j + _upLeft.second, newVal);
    }
    
    unsigned getWidth ()
    {
        return _downRight.second - _upLeft.second + 1;
    }
    
    unsigned getHeigth ()
    {
        return _downRight.first - _upLeft.first + 1;
    }
    
    void swap (MatrixView<T>& other)
    {
        //check something with bounds
        Matrix<T>* tmp = _Matrix;
        _Matrix = other._Matrix;
        other._Matrix = tmp;
    }
    
    ////
    void show()
    {
        int heigth = -(_upLeft.first - _downRight.first);
        int width = -(_upLeft.second - _downRight.second);
        for (int i = 0; i < heigth; ++i)
        {
            for (int j = 0; j < width + 1; ++j) //??
            {
                at(i,j) == 1 ? cout << "*" << " " : cout << " " << " ";
            }
            cout << endl;
            fflush(stdout);
        }
    }
    
private:
    
    Matrix<T>* _Matrix;
    
    pair<int,int> _upLeft;
    
    pair<int,int> _downRight;
    
};


Matrix<unsigned>* getRandomMatrix(int width, int heigth)
{
    vector<vector<unsigned> > v(heigth, vector<unsigned>(width));
    for (int i = 0; i < heigth; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            v[i][j] = rand() % 2;
        }
    }
    //return M;//Matrix<int>(v);
    return (new Matrix<unsigned>(v));
}


Matrix<unsigned>* fromCsv(ifstream& in)
{
    vector<pair<int, int> > alive;
    string line;
    int h = 0;
    int w = 0;
    for (; getline(in, line); ++h)
    {
        istringstream iss(line);
        int i = 0;
        string s;
        for (; getline(iss, s, ';'); ++i)
        {
            if (s == "1" || s == "#")
            {
                alive.push_back(make_pair(h, i));
            }
            else if (s != "0" && s != ".")
            {
                cout << "\nOOOH\n";
                fflush(stdout);
                exit(0);
            }
            //throw InputError("CSV incorrect");
        }
        w = max(w, i);
    }
    Matrix<unsigned>* ret = new Matrix<unsigned>(h, w);
    for (size_t i = 0; i < alive.size(); ++i)
    {
        ret->set(alive[i].first, alive[i].second, 1);
    }
    return ret;
}



#endif
