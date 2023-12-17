
#include "phone.hpp"
#include "call_registry.hpp"
#include "iostream"
using namespace std;

int main()
{
    nat n;
    cin >> n;
    call_registry c1;
    for(nat i = 0; i < n; ++i)
        c1.registra_trucada(i+1);
    c1.prin();
    for(nat i = n +1; i < 2)
}