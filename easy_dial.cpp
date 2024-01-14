#include "easy_dial.hpp"

/* ##IMPLEMENTACIÓ DELS MÈTODES ADICIONALS## */
//---------------------------------------------------------------------------------------------//

// Pre: Cert
// Post: fusiona de forma ordenada els dos intervals de [p,q] i de (q,r] del vector v.
// Cost; O(n) on n es el valor r. 
template< typename T>
void merge(vector<T>& v, int p, int q, int r)
{
  int size1 = q-p+1;
  int size2 = r-q;
  vector<T> L(size1);
  vector<T> R(size2);
   for(int i = 0; i < size1; i++){
    L[i] = v[p+i];
  }
  for(int j = 0; j < size2; j++){
    R[j]=v[q+j+1];
  }

  int i=0,j=0;
  int k;

  for(k = p; k <= r && i < size1 && j < size2; k++){
    if(L[i] < R[j]){
        v[k] = L[i];
        i++;
    }else if(L[i] > R[j]){
        v[k] = R[j];
        j++;
    }
  }
  while(i < size1){
    v[k] = L[i];
    k++;
    i++;
  }
  while(j < size2){
    v[k] = R[j];
    k++;
    j++;
  }
}

// Pre: 0 <= p <= r < v.size
// Post: modifica el vector v ordenant-lo de p fins a r   
// Cost: O(n · log n) on n és la v.size.
template <typename T>
void merge_sort(vector<T>&v,int p,int r) 
{
  if(p<r){
    int q = (p+r)/2;
    merge_sort(v,p,q);
    merge_sort(v,q+1,r);
    merge(v,p,q,r);
  }
}


//---------------------------------------------------------------------------------------------//


/* ##IMPLEMENTACIÓ DELS MÈTODES PRIVATS## */
//---------------------------------------------------------------------------------------------//

easy_dial::node_trie::node_trie(const char &c, const phone &p, node_trie* seg , node_trie* prf) :
  _c(c),_seg(seg), _prf(prf),_p(p) {
}

void easy_dial::delete_nodes(node_trie *n){
    if (n != NULL) {
    delete_nodes(n->_prf);
    delete_nodes(n->_seg);
    delete n;
  }
}

typename easy_dial::node_trie* easy_dial::consulta(const string &s, node_trie *n, nat i) const
{
  node_trie *res = nullptr;
  if (n != nullptr) {
    if (i == s.length()-1 and n->_c==s[s.length()-1]) {
      res = n;
    }
    else if (n->_c == s[i]) {
      res = consulta(s,n->_prf,i+1);
    }
    else {
      res = consulta(s,n->_seg,i);
    }
  }
  return res;
}

typename easy_dial::node_trie* easy_dial::insert(const phone &p, node_trie *n, nat i) 
{
  if (n == nullptr) {
    if (i < p.nom().size())
      n = new node_trie(p.nom()[i],p);
    else
      n = new node_trie('#',p);
  } else {
    if (i >= p.nom().size()) {
      n->_seg = insert(p,n->_seg,i);
    } 
    else if (n->_c == p.nom()[i]){
      n->_prf = insert(p,n->_prf,i+1);
    }
    else {
      n->_seg = insert(p,n->_seg,i);
    }
  }
  return n;
}




typename easy_dial::node_trie* easy_dial::copy_nodes(node_trie *n){
  node_trie *t;
  if(n == nullptr) t = nullptr;
  else{
    t = new node_trie();
    try{
      t->_c = n->_c;
      t->_p = n->_p;
      t->_prf = copy_nodes(n->_prf);
      t->_seg = copy_nodes(n->_seg);
    }catch(...){
      delete t;
      throw;
    }
  }
  return t;
}

void easy_dial::rcomencen(vector<string>& result, node_trie *n) const
{
  if (n != nullptr) {
    result.push_back(n->_p.nom());
    rcomencen(result,n->_prf);
    rcomencen(result,n->_seg);
  }
}

void easy_dial::comencen(const string& pref, vector<string>& result, node_trie *n) const
{
  int i = 0;
  while (n != nullptr and i < pref.size()) {
    if (pref[i] == n->_c) {
      if (n->_p.nom().size() >= pref.size() and pref == n->_p.nom().substr(0,pref.size())) {
        result.push_back(n->_p.nom());
      }
      n = n->_prf;
      ++i;
    } 
    else n = n->_seg;
  }
  if (i == pref.size()) rcomencen(result, n);
}

void easy_dial::recorrer_trie(node_trie *n,nat i,double &frqt,double &sum)const{
  if(n!=nullptr){
    frqt += n->_p.frequencia();
    sum = n->_p.frequencia()*i + sum;
    recorrer_trie(n->_prf,i+1,frqt,sum);
    recorrer_trie(n->_seg,i,frqt,sum);
  }
}

//--------------------------------------------------------------------------------------------//


/* ##IMPLEMENTACIÓ DELS MÉTODES PÚBLICS## */
//--------------------------------------------------------------------------------------------//

/* Tres grans. Constructor per còpia, operador d'assignació i destructor. */
easy_dial::easy_dial(const easy_dial& D) throw(error) : _fav(D._fav),_actual(D._actual),_prefix(D._prefix),_definit(D._definit)
{
  _arrel = copy_nodes(D._arrel);
}

typename easy_dial::easy_dial& easy_dial::operator=(const easy_dial& D) throw(error)
{
  if(this != &D){
    delete_nodes(_arrel);
    _fav = D._fav;
    _actual = D._actual;
    _prefix = D._prefix;
    _definit = D._definit;
    _arrel = copy_nodes(D._arrel);
  }
  return(*this);
}

easy_dial::~easy_dial() throw()
{
  delete_nodes(_arrel);
  _actual = phone();
  _definit = false;
}

/* Construeix un easy_dial a partir de la 
informació continguda en el call_registry donat. El
prefix en curs queda indefinit. */
easy_dial::easy_dial(const call_registry& R) throw(error) : _arrel(nullptr), _definit(false)
{
  vector<phone> arr;
  R.dump(arr);
  nat size = arr.size();
  if(size == 1){
    _fav = arr[0];
  }else{
    merge_sort(arr,0,size-1);
    for (nat i = 0; i < size; ++i) {
      if(i == 0) {
        _fav = arr[size-1-i];
      }else _arrel = insert(arr[size-1-i],_arrel,0);
    }
  }
}

/* Inicialitza el prefix en curs a buit. Retorna el nom de F(S, '');
si F (S, '') no existeix llavors retorna l'string buit. */
string easy_dial::inici() throw()
{
  _prefix = "";
  string nom = "";
  _definit = true;
  if(!_fav.nom().empty()){
    nom = _fav.nom();
    _actual = _fav;
  }
  return nom;
}

/* Retorna el nom de F(S, p') on p' és el prefix resultant d'afegir
el caràcter c al final del prefix en curs p i
fa que el nou prefix en curs sigui p'.
Si F(S, p) existeix però F(S, p') no existeix llavors retorna 
l'string buit. 
Si no existeix F(S, p) (i per tant tampoc pot existir F(S, p'))
llavors es produeix un error i el prefix en curs queda indefinit. 
Naturalment, es produeix un error si el prefix en curs inicial p 
fos indefinit. */
string easy_dial::seguent(char c) throw(error)
{
  string nom = "";
  if(_definit == false) throw error(ErrPrefixIndef);
  if(_actual.nom().empty()){
    _definit = false;
    throw error(ErrPrefixIndef);
  }
 
  if(c == '\0')_prefix+='#';
  else  _prefix += c;
  node_trie *p = consulta(_prefix, _arrel, 0);
  if(p != nullptr){
    nom = p->_p.nom();
    _actual = p->_p;
  }else
    _actual = phone();
  
  return nom;
}

/* Elimina l'últim caràcter del prefix en curs p = p' · a
(a és el caràcter eliminat). Retorna el nom F(S, p') 
i fa que el nou prefix en curs sigui p'. 
Es produeix un error si p fos buida i si es fa que el prefix en curs
quedi indefinit. Òbviament, també es produeix un error 
si p fos indefinit. */
string easy_dial::anterior() throw(error)
{
  string nom;
  if(_definit == false) throw error(ErrPrefixIndef);
  else if(_prefix.empty()){
    _definit = false;
    throw error(ErrNoHiHaAnterior);
  }
  else{
    _prefix.pop_back();
    if(_prefix.empty()){
      nom = _fav.nom();
      _actual = _fav;
    }else{
      node_trie *p = consulta(_prefix,_arrel, 0);
      if(p!=nullptr) {
        nom = p->_p.nom();
        _actual = p->_p;
      } else
        _actual = phone();
    }
  }
  return nom;
}
/* Retorna el número de telèfon de F(S, p), sent p
el prefix en curs. Es produeix un error si p és indefinit o si
no existeix F(S, p). */
nat easy_dial::num_telf() const throw(error)
{
  if(_definit == false) throw error(ErrPrefixIndef);
  if( _actual.nom().empty()){
    throw error(ErrNoExisteixTelefon);
  } 
  return _actual.numero();
}
  
/* Retorna en el vector result tots els noms dels contactes de 
telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent. */
void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error)
{
  if (!_fav.nom().empty() and _fav.nom().size() >= pref.size() 
                and pref == _fav.nom().substr(0,pref.size())) {
    result.push_back(_fav.nom());
  }
  comencen(pref,result,_arrel);
  merge_sort(result,0,result.size()-1);
}

/* Retorna el número mitjà de pulsacions necessàries para obtenir un
telèfon. Formalment, si X és el conjunt de noms emmagatzemats en
el easy_dial i t(s) és el número de pulsacions mínimes
necessàries (= número de crides a l'operació seguent) per
obtenir el telèfon el nom del qual és s. La funció retorna la suma
    Pr(s) · t(s)
per tots els telèfons s del conjunt X, sent Pr(s) la probabilitat de
telefonar a s. La probabilitat s'obté dividint la freqüència de s per
la suma de totes les freqüències. */
double easy_dial::longitud_mitjana() const throw()
{
  double frqt = 0;
  frqt+=_fav.frequencia();
  double sum = 0;
  recorrer_trie(_arrel,1,frqt,sum);
  if (frqt != 0) frqt= sum /frqt;
  return frqt;
}