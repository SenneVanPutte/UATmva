#ifndef __UATmvaSummary_H__
#define __UATmvaSummary_H__

#include "../includes/UATmvaConfig.h"

#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <string>
#include <vector>
using namespace std;


class UATmvaSummary_t {
  public:
  UATmvaSummary_t() {TmvaFile = NULL ; }
  ~UATmvaSummary_t(){TmvaFile->Close(); delete TmvaFile ; }
   
  TString             TmvaName ;
  TFile*              TmvaFile ;
};


class UATmvaSummary {

  private:

  
  Bool_t                    InitDone  ;
  vector<UATmvaSummary_t*>  vUASummary ;

  public:

  UATmvaSummary(){ InitDone = false ;}
  virtual ~UATmvaSummary(){;}

  Bool_t IsInit() { return InitDone ; }

  void Do ( UATmvaConfig& );



};


#endif

