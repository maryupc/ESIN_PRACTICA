#include "easy_dial.hpp"

easy_dial::node_trie::node_trie(const char &c, phone p, node_trie* seg , node_trie* prf) :
  _c(c),_p(p),_seg(seg), _prf(prf) {
}


void easy_dial::delete_nodes(node_trie *n){
    if (n != NULL) {
    delete_nodes(n->_prf);
    delete_nodes(n->_seg);
    delete n;
  }
}

void easy_dial::merge_sort(vector<phone>&v,int p,int r) 
{
  if(p<r){
    int q = (p+r)/2;
    merge_sort(v,p,q);
    merge_sort(v,q+1,r);
    merge(v,p,q,r);
  }
}

void easy_dial::merge(vector<phone>& v, int p, int q, int r) 
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
    if(L[i].frequencia() < R[j].frequencia()){
        v[k] = L[i];
        i++;
    }else if(L[i].frequencia() > R[j].frequencia()){
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

typename easy_dial::node_trie* easy_dial::consulta(const string &s, node_trie *n, nat i)
{
  node_trie *res = nullptr;
  if (n != nullptr) {
    if (i == s.length()-1) {
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



  /* Construeix un easy_dial a partir de la 
  informació continguda en el call_registry donat. El
  prefix en curs queda indefinit. */
  easy_dial::easy_dial(const call_registry& R) throw(error) : _arrel(nullptr), _definit(false)
  {
    vector<phone> arr;
    R.dump(arr);
    merge_sort(arr,0,arr.size()-1);
    for (nat i = 0; i < arr.size(); ++i) {
      if(i == 0) _fav = arr[i];
      else _arrel = insert(arr[i],_arrel,0);
    }

  }

  /* Tres grans. Constructor per còpia, operador d'assignació i destructor. */
  easy_dial::easy_dial(const easy_dial& D) throw(error)
  {

  }

  typename easy_dial::easy_dial& easy_dial::operator=(const easy_dial& D) throw(error)
  {
    
  }

  easy_dial::~easy_dial() throw()
  {
    delete_nodes(_arrel);
    _definit = false;
  }



  /* Inicialitza el prefix en curs a buit. Retorna el nom de F(S, '');
  si F (S, '') no existeix llavors retorna l'string buit. */
  string easy_dial::inici() throw()
  {
    _prefix = "";
    _definit = true;
    string nom = "";
    if(_M != 0){
      nom = _fav.nom();
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
  string  easy_dial::seguent(char c) throw(error)
  {
    string nom = "";
    if(_definit == false) throw error(ErrPrefixIndef);
    if(_actual.nom()==""){
      _definit = false;
      throw error(ErrNoExisteixTelefon);
    }
    _prefix += c;
    node_trie *p = consulta(_prefix, _arrel, 0);
    if(p!=nullptr){
      nom = p->_p.nom();
      _actual = p->_p;
    }
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
    else if(_prefix == ""){
      _definit == false;
      throw error(ErrNoHiHaAnterior);
    }else{
      _prefix[_prefix.size()-1]='\0';
      node_trie *p = consulta(_prefix,_arrel, 0);
      if(p!=nullptr) {
        nom = p->_p.nom();
        _actual = p->_p;
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
    if(_actual.numero()==0) throw error(ErrNoExisteixTelefon);
    return _actual.numero();
  }

  /* Retorna en el vector result tots els noms dels contactes de 
  telèfon que comencen amb el prefix pref, en ordre lexicogràfic creixent. */
  void easy_dial::comencen(const string& pref, vector<string>& result) const throw(error)
  {
    
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
    
  }