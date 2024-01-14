#ifndef _PHONE_HPP
#define _PHONE_HPP

#include <string>
#include <esin/error>
#include <esin/util>   

using namespace std;
using util::nat;

class phone{
public:
  //Pre: Cert
  /*Post: Construeix un telèfon a partir del seu número (num), el seu nom
          (name) i el seu comptador de trucades (compt). 
          Es produeix un error si name no és un identificador legal. */
  //Cost: O(n) on n es la mida de name
  phone(nat num=0, const string& name="", nat compt=0) throw(error);

  /* Tres grans. Constructor per còpia, operador d'assignació i destructor. */

  //Pre: Cert
  //Post: Crea un phone amb els mateixos parametres que T.
  //Cost: O(n) on n es la mida del nom de T
  phone(const phone& T) throw(error);

  //Pre: Cert
  //Post: Retorna una copia de T al phone implicit.
  //Cost: O(n) on n es la mida del nom de T
  phone& operator=(const phone& T) throw(error);

  // Pre: Cert.
  // Post: Elimina el phone implicit
  //Cost: O(1)
  ~phone() throw();

  //Pre: Cert
  /*Post: Retorna el número de telèfon. */
  //Cost: O(1)
  nat numero() const throw();

  //Pre: Cert
  /*Post: Retorna el nom associat al telèfon, eventualment l'string buit. */
  //Cost: O(1)
  string nom() const throw();

  //Pre: Cert
  /*Post: Retorna el número de vegades que s'ha trucat al telèfon. */
  //Cost: O(1)
  nat frequencia() const throw();

  /* Operador de preincrement.
  Pre: Cert
  Post: Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
  retorna una referència a aquest telèfon. */
  //Cost: O(1)
  phone& operator++() throw();

  /* Operador de postincrement. 
  Pre: Cert
  Post: Incrementa en 1 el número de vegades que s'ha trucat al telèfon i
  retorna una còpia d'aquest telèfon sense incrementar. */
  //Cost: O(1)
  phone operator++(int) throw();

  /* Operadors de comparació.  L'operador > retorna cert, si i només si, el
  telèfon sobre el que s'aplica el mètode és més freqüent que el
  telèfon T, o a igual freqüència, el nom associat al telèfon és
  major en ordre lexicogràfic que el nom associat a T. 
  La resta d'operadors es defineixen consistentment respecte a >. */
  //Cost: O(1)
  bool operator==(const phone& T) const throw();
  bool operator!=(const phone& T) const throw();
  bool operator<(const phone& T) const throw();
  bool operator>(const phone& T) const throw();
  bool operator<=(const phone& T) const throw();
  bool operator>=(const phone& T) const throw();

  /* Caràcters especials no permesos en un nom de telèfon. */
  static const char DELETECHAR = '<';
  static const char ENDCHAR = '|';
  static const char ENDPREF = '\0';

  /* Gestió d'errors. */
  static const int  ErrNomIncorrecte = 11;
  
private:
  #include "phone.rep"
};
#endif
