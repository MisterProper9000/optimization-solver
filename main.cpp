#include <QCoreApplication>
#include <ISet.h>
#include <ICompact.h>
#include <ILog.h>
#include <iostream>
const int N = 10;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ILog::init("out.txt");
    std::cout<<"sas"<<std::endl;
    double* d1 = new double;
    d1[0] = 1.1, d1[1] = 2;
    IVector* vec1 = IVector::createVector(2, d1);
    double* arr1 = new double[N];
    double* arr2 = new double[N];
    double* arr3 = new double[N];
    for(int i = 0; i< N; i++)
    {
        arr3[i] = 1;
    }
    if(!arr1 || !arr2 || !arr3)
        std::cout<<"blya"<<std::endl;

    /*--------vector memory  test-------------*/
    /*
    else
        while(true)
        {
            IVector* vev = IVector::createVector(N, arr1);
            delete vev;
            std::cout<<"sas"<<std::endl;
        }*/

    /*--------compact memory  test-------------*/
    else
        while(true)
        {
           IVector* v1 = IVector::createVector(N,arr1);
           IVector* v2 = IVector::createVector(N,arr2);
           IVector* v3 = IVector::createVector(N,arr3);//in case of error all factory-stuff must be deleted inside of factory
           ICompact* com = ICompact::createCompact(v1,v2,v3);
           ICompact::IIterator** cit = new ICompact::IIterator*[100000];
           for(int i = 0; i < 100000; i++)
           {
               cit[i] = com->begin();
           }
           delete com;
           delete[] cit;
           delete v1;
           delete v2;
           delete v3;
           //std::cout<<"sas"<<std::endl;
        }

    /*--------set memory  test-------------*/
    /*else while(true)
    {
        IVector* v4 = IVector::createVector(N,arr1);
        ISet* set = ISet::createSet(N);
        set->put(v4);

        ISet::IIterator** sit = new ISet::IIterator*[100000];
        for(int i = 0; i < 100000; i++)
        {
            sit[i] = set->begin();
        }
        delete set;
        delete[] sit;
        delete v4;

    }*/
    ILog::destroy();
    return a.exec();
}
