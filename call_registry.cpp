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

void call_registry::insert(node_hash *q)
{
    if (q == NULL)
        return;
    node_hash *n = new node_hash(q->_k);
    int i = h(n->_k.numero()) % _M;
    node_hash *p = _taula[i];
    node_hash *pant = NULL;
    while (p != NULL and p->_k.numero() <= n->_k.numero()) {
        pant = p;
        p = p->_seg;
    }
    if (pant == NULL) {
        n->_seg = _taula[i];
        _taula[i] = n;
    } else {
        pant->_seg = n;
        n->_seg = p;
    }
    delete q;
}

// Redisperció de la taula
void call_registry::redispersio()
{
    nat nM = 2 * _M + 1;
    node_hash **ntaula = new node_hash*[nM];
    for (nat i = 0; i < nM; ++i) ntaula[i] = NULL;
    swap(_taula,ntaula);
    swap(_M,nM);
    for (nat i = 0; i < nM; ++i) {
        node_hash *m = ntaula[i];
        while (m != NULL) {
            node_hash *seg = m->_seg;
            insert(m);
            m = seg;
        }
    }
    delete[] ntaula;
}

// Eliminació de tots els nodes continguts a la taula
void call_registry::delete_nodes(node_hash *p)
{
    if (p == NULL)
        return;
    delete_nodes(p->_seg);
    delete p;
}

void call_registry::merge_sort(vector<phone>&v,int p,int r,bool &repetidos) const
{
    if(p<r){
        int q = (p+r)/2;
        merge_sort(v,p,q,repetidos);
        merge_sort(v,q+1,r,repetidos);
        if(not repetidos) merge(v,p,q,r,repetidos);
    }
}

void call_registry::merge(vector<phone>& v, int p, int q, int r,bool &repetidos) const
{
    int size1 = q-p+1;
    int size2 = r-q;
    vector<phone> L(size1);
    vector<phone> R(size2);

    for(int i = 0; i < size1; i++){
        L[i] = v[p+i];
    }
    for(int j = 0; j < size2; j++){
        R[j]=v[q+j+1];
    }

    int i=0,j=0;
    int k;

        for(k = p; k <= r && i < size1 && j < size2; k++){
            if(L[i].nom() < R[j].nom()){
                v[k] = L[i];
                i++;
            }else if(L[i].nom() > R[j].nom()){
                v[k] = R[j];
                j++;
            }else {
                repetidos = true;
            }
        }
        while(i < size1 and not repetidos){
            if(k >= 1 && v[k-1].nom() == L[i].nom()){
                repetidos = true;
            }else{
                v[k] = L[i];
                k++;
                i++;
            } 
        }
        while(j < size2 and not repetidos){
            if(k>=1 && v[k-1].nom() == R[j].nom()){
                repetidos = true;
            }else{
                v[k] = R[j];
                k++;
                j++;
            }

        }
}

typename call_registry::node_hash* call_registry::copy_nodes(node_hash *p)
{
    node_hash *n;
    if (p == NULL) n = NULL;
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
        _taula[i] = NULL;
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
    node_hash *pant = NULL;
    while (p != NULL and not found and p->_k.numero() <= num) {
        if(p->_k.numero() == num){
            found = true;
            p->_k++;
        }
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
    node_hash *pant = NULL;
    while (p != NULL and not found and p->_k.numero() <= num) {
        if(p->_k.numero() == num){
            found = true;
            phone aux(num,name,p->_k.frequencia());
            p->_k = aux;
        }
        else {
            pant = p;
            p = p->_seg;
        }
    }
    if(not found){
        phone aux(num,name,0);
        node_hash *n = new node_hash(aux);
        if (pant == NULL) {
            n->_seg = _taula[i];
            _taula[i] = n;
        } else {
            pant->_seg = n;
            n->_seg = p;
        }
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
    node_hash *pant = NULL;
    while (p != NULL and not found and p->_k.numero() <= num) {
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
        if(pant == NULL){
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
    while (p != NULL and not found and p->_k.numero() <= num) {
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
    while (p != NULL and not found and p->_k.numero() <= num) {
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
    while (p != NULL and not found and p->_k.numero() <= num) {
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
        while(p != NULL) {
            if(p->_k.nom() != "")
                V.push_back(p->_k);
            p = p->_seg;
        }
    }
    bool repetidos = false;
    merge_sort(V,0,V.size()-1,repetidos);
    if(repetidos){
        throw error(ErrNomRepetit);
    }
}

/* void call_registry::prin() const 
{
    print();
}

void call_registry::print() const {
  for (nat i=0; i < _M; ++i) {
    cout << i << ": ";
    node_hash *p = _taula[i];
    while (p != NULL) {
      cout << p->_k.numero() << " ";
      p = p->_seg;
    }
    cout << endl;
  }
  cout << "-----------\n";
} */