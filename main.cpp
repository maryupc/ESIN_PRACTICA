#include "phone.hpp"
#include "iostream"
using namespace std;

int main()
{
    phone telefon(691747243,"Joel", 132);
    cout << telefon.numero() << " " << telefon.nom() << " " << telefon.frequencia() << endl;
    phone telefon1(69324151, "Jo|el",123);
    cout << telefon1.numero() << " " << telefon1.nom() << " " << telefon1.frequencia() << endl;
}