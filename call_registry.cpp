#include "call_registry.hpp"

/* ##IMPLEMENTACIÓ DELS MÈTODES PRIVATS## */
//---------------------------------------------------------------------------------------------//

// Constructor de nodo tipo hash
call_registry::node_hash::node_hash(const phone &k, node_hash *seg) throw() : _k(k), _seg(seg)
{
}

// Calculo de la posicion en la tabla de dispersión
long int call_registry::h(nat k) 
{
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
    nat nM = 2 * _M + 1;
    node_hash **ntaula = new node_hash*[nM];
    for (nat i = 0; i < nM; ++i) ntaula[i] = nullptr;
    swap(_taula,ntaula);
    swap(_M,nM);
    for (nat i = 0; i < nM; ++i) {
        node_hash *m = ntaula[i];
        while (m != nullptr) {
            node_hash *seg = m->_seg;
            int i = h(m->_k.numero()) % _M;
            m->_seg = _taula[i];
            _taula[i] = m;
            m = seg;
        }
    }
    delete[] ntaula;
}

// Eliminació de tots els nodes continguts a la taula
void call_registry::delete_nodes(node_hash *p)
{
    if (p == nullptr)
        return;
    delete_nodes(p->_seg);
    delete p;
}

typename call_registry::node_hash* call_registry::copy_nodes(node_hash *p)
{
    node_hash *n;
    if (p == nullptr) n = nullptr;
    else {
        n = new node_hash();
        try {
            n->_k = p->_k;
            n->_seg = copy_nodes(p->_seg);
        } catch (...){
            delete n;
            throw;
        }
    }
    return n;
}

//--------------------------------------------------------------------------------------------//


/* ##IMPLEMENTACIÓ DELS MÉTODES PÚBLICS## */

call_registry::call_registry() throw(error) : _M(100), _quants(0)
{
    _taula = new node_hash*[_M];
    for (nat i = 0; i < _M; ++i) {
        _taula[i] = nullptr;
    }
}

/* Constructor per còpia, operador d'assignació i destructor. */
call_registry::call_registry(const call_registry& R) throw(error) : _M(R._M), _quants(R._quants)
{
    _taula = new node_hash*[_M];
    for (nat i = 0; i < _M; ++i) {
        _taula[i] = copy_nodes(R._taula[i]);
    }
    
}

typename call_registry::call_registry& call_registry::operator=(const call_registry& R) throw(error)
{
    if (this != &R) {
        for (nat i = 0; i < _M; ++i) {
            delete_nodes(_taula[i]);
        }
        node_hash **aux(_taula);
        _M = R._M;
        _quants = R._quants;
        _taula = new node_hash*[_M];
        delete[] aux;
        for (nat i = 0; i < _M; ++i) {
            _taula[i] = copy_nodes(R._taula[i]);
        }
    }
    return (*this);
}

call_registry::~call_registry() throw()
{
    for (nat i = 0; i < _M; ++i) {
        delete_nodes(_taula[i]);
    }
    delete _taula;
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
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
            p->_k++;
        }
        else {
            p = p->_seg;
        }
    }
    if (not found) {
        phone ph(num,"",1);
        _taula [i] = new node_hash (ph , _taula [i]);
        _quants++;
        if (factor_de_carrega() > 0.8)
            redispersio();
    }
}

/* Assigna el nom indicat al número donat.
Si el número no estava prèviament en el call_registry, s'afegeix
una nova entrada amb el número i nom donats, i el comptador 
de trucades a 0. 
Si el número existia prèviament, se li assigna el nom donat. */
void call_registry::assigna_nom(nat num, const string& name) throw(error)
{
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
            phone aux(num,name,p->_k.frequencia());
            p->_k = aux;
        }
        else {
            p = p->_seg;
        }
    }
    if(not found){
        phone ph(num,name,0);
         _taula [i] = new node_hash (ph , _taula [i]);
        _quants++;
        if (factor_de_carrega() > 0.8)
            redispersio(); 
    }
}

/* Elimina l'entrada corresponent al telèfon el número de la qual es dóna.
Es produeix un error si el número no estava present. */
void call_registry::elimina(nat num) throw(error)
{    
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    node_hash *pant = nullptr;
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
        }
        else {
            pant = p;
            p = p->_seg;
        }
    }
    if(not found){
        throw error(ErrNumeroInexistent);
    }else{
        if(pant == nullptr){
            _taula[i] = p->_seg;
        }else{
            pant->_seg = p->_seg;
        }
        delete(p);
        --_quants;
    }
}

/* Retorna cert si i només si el call_registry conté un 
telèfon amb el número donat. */
bool call_registry::conte(nat num) const throw()
{
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
        }
        else {
            p = p->_seg;
        }
    }
    return found;
}

/* Retorna el nom associat al número de telèfon que s'indica.
Aquest nom pot ser l'string buit si el número de telèfon no
té un nom associat. Es produeix un error si el número no està en
el call_registry. */
string call_registry::nom(nat num) const throw(error)
{
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    string name = "";
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
            name = p->_k.nom();
        }
        else {
            p = p->_seg;
        }
    }
    if(not found){
        throw error(ErrNumeroInexistent);
    }
    return name;
}

/* Retorna el comptador de trucades associat al número de telèfon 
indicat. Aquest número pot ser 0 si no s'ha efectuat cap trucada a
aquest número. Es produeix un error si el número no està en el 
call_registry. */
nat call_registry::num_trucades(nat num) const throw(error)
{
    int i = h(num) % _M;
    bool found = false;
    node_hash *p = _taula[i];
    nat trucades_n = 0;
    while (p != nullptr and not found) {
        if(p->_k.numero() == num){
            found = true;
            trucades_n = p->_k.frequencia();
        }
        else {
            p = p->_seg;
        }
    }
    if(not found){
        throw error(ErrNumeroInexistent);
    }
    return trucades_n;
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
    for(nat i = 0; i < _M; ++i){
        node_hash *p(_taula[i]);
        while(p != nullptr) {
            if(p->_k.nom() != ""){
                if(V.size()==0)
                    V.push_back(p->_k);
                else{
                    for(int i = 0; i < V.size(); i++){
                        if(p->_k.nom()==V[i].nom()){
                            throw error(ErrNomRepetit);
                        } 
                    }
                    V.push_back(p->_k);
                }
            }
            p = p->_seg;
        }
    }
}
