#include <iostream>
#include "Ensemble.h"
using namespace std;

Ensemble::Ensemble(unsigned int cardMax){
  pcardMax = cardMax;
  pcardAct = 0;
  pt = new int [cardMax];
}

Ensemble::Ensemble(int t[], unsigned int nbElements){
  pt = new int [nbElements];
  pcardMax = nbElements;
  pcardAct = 0;
  int i;
  int j = 0;
  for (i=0; i<nbElements; i++){
    if (estPresent(pt, pcardAct, t[i]) == false){
      pt[j++] = t[i];
      pcardAct++;
    }
  }
}

Ensemble::~Ensemble(){
  delete[] pt;
}

bool Ensemble::estPresent(const int t[], int taille, int valeur) const{
  int i;
  for (i=0; i<taille; i++){
    if (t[i] == valeur){
      return true;
    }
  }
  return false;
}


void Ensemble::Afficher(void){
  int i;
  cout << pcardAct << "\r\n";
  cout << pcardMax << "\r\n";
  cout << "{";
  trieMoiCa();
  for (i=0; i<pcardAct; i++){
    if (i == pcardAct-1){
      cout << pt[i];
    }
    else{
      cout << pt[i] << ",";
    }
  }
  cout << "}\r\n";
}

void Ensemble::trieMoiCa() const{
  int i;
  int j;
  for (i=0; i<pcardAct; i++){
    for(j=i+1; j<pcardAct; j++){
      if (pt[j] < pt[i]) {
        int tmp = pt[i];
        pt[i] = pt[j];
        pt[j] = tmp;
      }
    }
  }
}

bool Ensemble::EstEgal(const Ensemble& unEnsemble) const{
  int i;
  if (pcardAct == unEnsemble.pcardAct){
    for (i=0; i<pcardAct; i++){
      if (pt[i] != unEnsemble.pt[i]){
        return false;
      }
    }
    return true;
  }
  return false;
}

crduEstInclus Ensemble::EstInclus(const Ensemble & unEnsemble) const{
  if (EstEgal(unEnsemble) == true){
    return INCLUSION_LARGE;
  }
  int i;
  for (i=0; i<pcardAct; i++){
    if (estPresent(unEnsemble.pt, unEnsemble.pcardAct, pt[i]) != true){
      return NON_INCLUSION;
    }
  }
  return INCLUSION_STRICTE;
}

crduAjouter Ensemble::Ajouter(int aAjouter){
  if (estPresent(pt, pcardAct, aAjouter)){
    return DEJA_PRESENT;
  }
  if (pcardAct == pcardMax){
    return PLEIN;
  }
  pt[pcardAct] = aAjouter;
  pcardAct++;
  return AJOUTE;
}

unsigned int Ensemble::Ajuster(int delta){
  if (delta == 0){
    return pcardMax;
  }
  if (pcardMax + delta >= pcardAct){
    pcardMax = pcardMax + delta;
  }
  else {
    pcardMax = pcardAct;
  }
  int *tableau = new int [pcardMax];
  int i;
  for (i=0; i<pcardAct; i++){
    tableau[i] = pt[i];
  }
  
  delete[] pt;
  pt = tableau;
  return pcardMax;
}

int Ensemble::trouveElement(int t[], int taille, int element) const{
  int i;
  for (i=0; i<taille; i++){
    if (t[i] == element){
      return i;
    }
  }
  return -1;
}

bool Ensemble::Retirer (int element){
  int pos = trouveElement(pt, pcardAct, element);
  if (pos != -1){
    int i;
    for (i=pos; i<pcardAct-1; i++){
      pt[i] = pt[i+1];
    }
    pcardAct--;
    Ajuster(-(pcardMax - pcardAct));
    return true;
  }
  Ajuster(-(pcardMax - pcardAct));
  return false;
}

unsigned int Ensemble::Retirer(const Ensemble & unEnsemble){
  if (pt == unEnsemble.pt) {
    int tmp = pcardAct;
    pcardAct = 0;
    return tmp;
  }
  int i;
  int compteur = 0;
  for (i=0; i<unEnsemble.pcardAct; i++){
    int pos = trouveElement(pt, pcardAct, unEnsemble.pt[i]);
    if (pos != -1){
      int j;
      for (j = pos; j<pcardAct-1; j++){
        pt[j] = pt[j+1];
      }
      compteur++;
      pcardAct--;
    }
  }
  return compteur;
}

int Ensemble::Reunir(const Ensemble & unEnsemble){
  int i;
  int tmp = pcardAct;
  int valRen = 0;
  bool signevalRen = false;
  for (i = 0; i<unEnsemble.pcardAct; i++){
    bool pos = estPresent(pt, pcardAct, unEnsemble.pt[i]);
    if (pos == false){
      if (pcardAct < pcardMax){
        Ajouter(unEnsemble.pt[i]);
        valRen++;
        signevalRen = true;
      }
      else {
        Ajuster(1); //réajustement pour contenir la réunion
        Ajouter(unEnsemble.pt[i]);
        valRen++;
        signevalRen = false;
      }
    }
  }
  if (signevalRen == true){
    return valRen;
  }
  return (-valRen);
}

unsigned int Ensemble::Intersection(const Ensemble & unEnsemble){
  int compteur = 0;
  int i;
  for(i = 0; i<pcardAct; i++){
    bool pos = estPresent(unEnsemble.pt, unEnsemble.pcardAct, pt[i]);
    if (pos == false){
      Retirer(pt[i]);
      compteur ++;
      i = i-1;
    }
  }
  Ajuster(pcardAct-pcardMax); //réajustement au plus juste
  return compteur;
}
