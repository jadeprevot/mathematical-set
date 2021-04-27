#ifndef ENSEMBLE_H
#define ENSEMBLE_H

const unsigned int CARD_MAX = 5;
enum crduEstInclus{
  NON_INCLUSION,
  INCLUSION_LARGE,
  INCLUSION_STRICTE
};
enum crduAjouter{
  DEJA_PRESENT,
  PLEIN,
  AJOUTE
};

class Ensemble
{
  public :

  Ensemble(unsigned int cardMax = CARD_MAX);
  Ensemble(int t [], unsigned int nbElements);
  virtual ~Ensemble();
  
  void Afficher(void);
  bool EstEgal(const Ensemble & unEnsemble) const;
  crduEstInclus EstInclus(const Ensemble & unEnsemble) const;
  crduAjouter Ajouter(int aAjouter);
  unsigned int Ajuster(int delta);
  bool Retirer (int element);
  unsigned int Retirer(const Ensemble & unEnsemble);
  int Reunir(const Ensemble & unEnsemble);
  unsigned int Intersection(const Ensemble & unEnsemble);

  private :

  bool estPresent(const int t[], int taille, int valeur) const;
  void trieMoiCa() const; //toutes les méthodes ont this en param implicite
  int trouveElement(int t[], int taille, int element) const;

  unsigned int pcardMax;
  unsigned int pcardAct;
  int *pt;
};

#endif