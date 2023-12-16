#include "call_registry.hpp"

/* ##IMPLEMENTACIÓ DELS MÈTODES PRIVATS## */
//---------------------------------------------------------------------------------------------//

void call_registry::insert_node(node_hash *p)
{

}

call_registry::call_registry(nat m)
{
    _M = m;
    _taula = new node_hash*[_M];
    for (nat i = 0; i < _M; ++i) {
        _taula[i] = NULL;
    }
}

// Constructor de nodo tipo hash
call_registry::node_hash::node_hash(const phone &k, node_hash *seg) throw() : _k(k), _seg(seg)
{
}

// Calculo de la posicion en la tabla de dispersión
long int call_registry::h(nat k) {
    long i = ((k * k * MULT) << 20) >> 4;
    if (i < 0)
        i = -i;
    return i;
}

// Clàlcul del factor de càrrega de la taula de dispersió
float call_registry::factor_de_carrega()
{
    return _quants/(float)_M;
}

// Redisperció de la taula
void call_registry::redispersio()
{
    nat m = 2*_M + 1;
    call_registry t2(m);
    for(nat i = 0; i < _M; ++i) {
        node_hash *p(_taula[i]);
        while (p != NULL) {
            node_hash *n = new node_hash(p->_k);
            t2.insert_node(n);
        }
    }
    swap(_taula,t2._taula);
    swap(_M,t2._M);
}

// Eliminació de tots els nodes continguts a la taula
void call_registry::delete_nodes(node_hash *p)
{
    if (p == NULL)
        return;
    delete_nodes(p->_seg);
    delete p;
}
//--------------------------------------------------------------------------------------------//


/* ##IMPLEMENTACIÓ DELS MÉTODES PÚBLICS## */

call_registry::call_registry() throw(error) :_quants(0)
{
    _M = 20;
    _taula = new node_hash*[_M];
    for (nat i = 0; i < _M; ++i) {
        _taula[i] = NULL;
    }
}

/* Constructor per còpia, operador d'assignació i destructor. */
call_registry::call_registry(const call_registry& R) throw(error)
{

}

typename call_registry::call_registry& call_registry::operator=(const call_registry& R) throw(error)
{

}

call_registry::~call_registry() throw()
{
    for (nat i = 0; i < _M; ++i) {
        delete_nodes(_taula[i]);
    }
    delete[] _taula;
}

/* Registra que s'ha realitzat una trucada al número donat, 
incrementant en 1 el comptador de trucades associat. Si el número no 
estava prèviament en el call_registry afegeix una nova entrada amb 
el número de telèfon donat, l'string buit com a nom i el comptador a 1. */
void call_registry::registra_trucada(nat num) throw(error)
{
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    node_hash *pant = NULL;
    while (p != NULL and not found and p->_k.numero() <= num) {
        if(p->_k.numero() == num)
            found = true;
        else {
            pant = p;
            p = p->_seg;
        }
    }
    if (not found) {
        phone ph(num,"",1);
        node_hash *n = new node_hash(ph);
        if (pant == NULL) {
            n->_seg = _taula[i];
            _taula[i] = n;
        } else {
            pant->_seg = n;
            n->_seg = p;
        }
        _quants++;
    } 
    else {
        p->_k++;
    }
    if (factor_de_carrega() > 0.8)
        redispersio();
}

/* Assigna el nom indicat al número donat.
Si el número no estava prèviament en el call_registry, s'afegeix
una nova entrada amb el número i nom donats, i el comptador 
de trucades a 0. 
Si el número existia prèviament, se li assigna el nom donat. */
void call_registry::assigna_nom(nat num, const string& name) throw(error)
{

}

/* Elimina l'entrada corresponent al telèfon el número de la qual es dóna.
Es produeix un error si el número no estava present. */
void call_registry::elimina(nat num) throw(error)
{

}

/* Retorna cert si i només si el call_registry conté un 
telèfon amb el número donat. */
bool call_registry::conte(nat num) const throw()
{
    
}

/* Retorna el nom associat al número de telèfon que s'indica.
Aquest nom pot ser l'string buit si el número de telèfon no
té un nom associat. Es produeix un error si el número no està en
el call_registry. */
string call_registry::nom(nat num) const throw(error)
{

}

/* Retorna el comptador de trucades associat al número de telèfon 
indicat. Aquest número pot ser 0 si no s'ha efectuat cap trucada a
aquest número. Es produeix un error si el número no està en el 
call_registry. */
nat call_registry::num_trucades(nat num) const throw(error)
{

}

/* Retorna cert si i només si el call_registry està buit. */
bool call_registry::es_buit() const throw()
{
    return _quants == 0;
}

/* Retorna quants números de telèfon hi ha en el call_registry. */
nat call_registry::num_entrades() const throw()
{
    return _quants;
}

/* Fa un bolcat de totes les entrades que tenen associat un
nom no nul sobre un vector de phone.
Comprova que tots els noms dels telèfons siguin diferents; 
es produeix un error en cas contrari. */
void call_registry::dump(vector<phone>& V) const throw(error)
{

}