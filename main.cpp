#include <QCoreApplication>
#include <ISet.h>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout<<"sas"<<std::endl;
    double* d1 = new double;
    d1[0] = 1.1, d1[1] = 2;
    IVector* vec1 = IVector::createVector(2, d1);
    return a.exec();
}
