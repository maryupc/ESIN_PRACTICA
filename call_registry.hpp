#ifndef _CALL_REGISTRY_HPP
#define _CALL_REGISTRY_HPP

#include "phone.hpp"
#include <esin/error>
#include <esin/util>   
#include <string>
#include <vector>

using namespace std;
using util::nat;

class call_registry {
public:

  
  call_registry() throw(error);
  // Pre: Cert
  // Post: Construeix un call_registry buit.
  // Cost: O(k) on k = 100

  call_registry(const call_registry& R) throw(error);
  // Pre: Cert
  // Post: Crea un call registry amb els mateixos parametres que R.
  // Const: O(n) on n es la mida de R

  call_registry& operator=(const call_registry& R) throw(error);
  // Pre: Cert.
  // Post: Retorna una copia de R al call registry implicit.
  // Cost: O(n) on n es la mida del call registry mes gran.

  ~call_registry() throw();
  // Pre: Cert.
  // Post: Elimina el call regsitry implicit
  // Cost: O(n) on en es la mida del call registry

  void registra_trucada(nat num) throw(error);
  // Pre: Cert.
  // Post: Registra que s'ha realitzat una trucada al número donat, 
  // incrementant en 1 el comptador de trucades associat. Si el número no 
  // estava prèviament en el call_registry afegeix una nova entrada amb 
  // el número de telèfon donat, l'string buit com a nom i el comptador a 1. 
  // Cost: O(1)

  void assigna_nom(nat num, const string& name) throw(error);
  // Pre: Cert.
  // Post: Assigna el nom indicat al número donat.
  // Si el número no estava prèviament en el call_registry, s'afegeix
  // una nova entrada amb el número i nom donats, i el comptador 
  // de trucades a 0. 
  // Si el número existia prèviament, se li assigna el nom donat.
  // Cost: O(1)
  
  void elimina(nat num) throw(error);
  // Pre: Cert.
  // Post: Elimina l'entrada corresponent al telèfon el número de la qual es dóna.
  // Es produeix un error si el número no estava present.
  // Cost: O(1)

  bool conte(nat num) const throw();
  // Pre: Cert.
  // Post: Retorna cert si i només si el call_registry conté un 
  // telèfon amb el número donat.
  // Cost: (1)

  string nom(nat num) const throw(error);
  // Pre: Cert
  // Post: Retorna el nom associat al número de telèfon que s'indica.
  // Aquest nom pot ser l'string buit si el número de telèfon no
  // té un nom associat. Es produeix un error si el número no està en
  // el call_registry.
  // Cost: O(1)

  nat num_trucades(nat num) const throw(error);
  // Pre: Cert
  // Post: Retorna el comptador de trucades associat al número de telèfon 
  // indicat. Aquest número pot ser 0 si no s'ha efectuat cap trucada a
  // aquest número. Es produeix un error si el número no està en el 
  // call_registry.
  // Cost: O(1)

  bool es_buit() const throw();
  // Pre: Cert
  // Post: Retorna true si el call registry implicit es buit.
  // En cas contrari retorna false.
  // Cost: O(1)

  nat num_entrades() const throw();
  // Pre: Cert.
  // Post: Retorna quants números de telèfon hi ha en el call_registry.
  // Cost: O(1)

  void dump(vector<phone>& V) const throw(error);
  // Pre: Cert.
  // Post: Fa un bolcat de totes les entrades que tenen associat un
  // nom no nul sobre un vector de phone.
  // Comprova que tots els noms dels telèfons siguin diferents; 
  // es produeix un error en cas contrari.
  // Cost: O(n²) on n es el numero de phones al call registry

  /* Gestió d'errors. */
  static const int  ErrNumeroInexistent = 21;
  static const int  ErrNomRepetit       = 22;

private:
  #include "call_registry.rep"
};
#endif
