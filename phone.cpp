#include "phone.hpp"

bool phone::itsCorrectName(const string &name) throw()
{
    for (unsigned int i = 0; i < name.length(); ++i) {
        if (name[i] == DELETECHAR or name[i] == ENDCHAR or name[i] == ENDPREF)
            return false;
    }
    return true;
}

//Pre: Cert
/*Post: Construeix un telèfon a partir del seu número (num), el seu nom
          (name) i el seu comptador de trucades (compt). 
          Es produeix un error si name no és un identificador legal. */
//Cost: O(n) on n es la mida de name
phone::phone(nat num, const string& name, nat compt) throw(error)
{
    if(itsCorrectName(name)) {
        _num = num;
        _name = name;
        _compt = compt;
    } else
        throw error(ErrNomIncorrecte);
}

//Pre: Cert
//Post: Crea un phone amb els mateixos parametres que T.
//Cost: O(n) on n es la mida del nom de 
phone::phone(const phone& T) throw(error)
{
    if (itsCorrectName(T._name)) {
        copyItems(T);
    } else
        throw error(ErrNomIncorrecte);
}

void phone::copyItems(const phone& T) throw()
{
    _num = T._num;
    _name = T._name;
    _compt = T._compt;
}

//Pre: Cert
//Post: Retorna una copia de T al phone implicit.
//Cost: O(n) on n es la mida del nom de T
typename phone::phone& phone::operator=(const phone &T) throw(error)
{
    copyItems(T);
    return *this;
}

// Pre: Cert.
// Post: Elimina el phone implicit
//Cost: O(1)
phone::~phone() throw()
{
}

//Pre: Cert
/*Post: Retorna el número de telèfon. */
//Cost: O(1)
nat phone::numero() const throw()
{
    return _num;
}

//Pre: Cert
/*Post: Retorna el nom associat al telèfon, eventualment l'string buit. */
//Cost: O(1)
string phone::nom() const throw()
{
    return _name;
}

//Pre: Cert
/*Post: Retorna el número de vegades que s'ha trucat al telèfon. */
//Cost: O(1)
nat phone::frequencia() const throw()
{
    return _compt;
}

/* Operador de preincrement.
Pre: Cert
Post: Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
retorna una referència a aquest telèfon. */
//Cost: O(1)
typename phone::phone& phone::operator++() throw()
{
    ++_compt;
    return *this;
}

/* Operador de postincrement. 
Pre: Cert
Post: Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
retorna una còpia d'aquest telèfon sense incrementar. */
//Cost: O(1)
typename phone::phone phone::operator++(int) throw()
{
    _compt++;
    return phone(_num,_name,_compt-1);
}

/* Operadors de comparació.  L'operador > retorna cert, si i només si, el
telèfon sobre el que s'aplica el mètode és més freqüent que el
telèfon T, o a igual freqüència, el nom associat al telèfon és
major en ordre lexicogràfic que el nom associat a T. 
La resta d'operadors es defineixen consistentment respecte a >. */
//Cost: O(1)
bool phone::operator==(const phone& T) const throw()
{
    if(_compt == T._compt)
        return _name == T._name;
    return _compt == T._compt;
}

bool phone::operator!=(const phone& T) const throw()
{
    return _compt != T._compt;
}

bool phone::operator<(const phone& T) const throw()
{
    if (_compt == T._compt)
        return _name < T._name;
    return _compt < T._compt;
}

bool phone::operator>(const phone& T) const throw()
{
    if (_compt == T._compt)
        return _name > T._name;
    return _compt > T._compt;
}

bool phone::operator<=(const phone& T) const throw()
{
    if (_compt == T._compt)
        return _name <= T._name;
    return _compt <= T._compt;
}

bool phone::operator>=(const phone& T) const throw()
{
    if (_compt == T._compt)
        return _name >= T._name;
    return _compt >= T._compt;
}