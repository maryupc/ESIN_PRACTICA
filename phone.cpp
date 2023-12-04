#include "phone.hpp"

bool phone::itsCorrectName(const string &name) throw()
{
    for (unsigned int i = 0; i < name.length(); ++i) {
        if (name[i] == DELETECHAR or name[i] == ENDCHAR or name[i] == ENDPREF)
            return false;
    }
    return true;
}

phone::phone(nat num, const string& name, nat compt) throw(error)
{
    if(itsCorrectName(name)) {
        _num = num;
        _name = name;
        _compt = compt;
    } else
        throw error(ErrNomIncorrecte);
}

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

typename phone::phone& phone::operator=(const phone &T) throw(error)
{
    copyItems(T);
    return *this;
}


phone::~phone() throw()
{
}


nat phone::numero() const throw()
{
    return _num;
}

string phone::nom() const throw()
{
    return _name;
}

nat phone::frequencia() const throw()
{
    return _compt;
}

typename phone::phone& phone::operator++() throw()
{
    _compt++;
    return *this;
}

typename phone::phone phone::operator++(int) throw()
{
    _compt++;
    return *this;
}

bool phone::operator==(const phone& T) const throw()
{
    return _compt == T._compt;
}

bool phone::operator!=(const phone& T) const throw()
{
    return _compt != T._compt;
}

bool phone::operator<(const phone& T) const throw()
{
    return _compt < T._compt;
}

bool phone::operator>(const phone& T) const throw()
{
    return _compt > T._compt;
}

bool phone::operator<=(const phone& T) const throw()
{
    return _compt <= T._compt;
}

bool phone::operator>=(const phone& T) const throw()
{
    return _compt >= T._compt;
}