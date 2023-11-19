#include "phone.hpp"

phone::phone(nat num=0, const string& name="", nat compt=0) throw(error) : _num(num), _name(name), _compt(compt)
{    
}

phone::phone(const phone& T) throw(error) : _num(T._num), _name(T._name), _compt(T._compt)
{
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