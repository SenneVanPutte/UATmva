#ifndef __UATmvaSummary_H__
#define __UATmvaSummary_H__

#include "../includes/UATmvaConfig.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TMath.h>
#include <TString.h>
#include <string>
#include <vector>
using namespace std;


class UATmvaSummary_t {
  public:
  UATmvaSummary_t()  ;
  UATmvaSummary_t(TString,TString,TString,UATmvaConfig&,int)  ;
  ~UATmvaSummary_t() ; 

  TString             BaseName;
  TString             ExtName;  
  TString             TmvaName;
  vector<TString>     TmvaNameDim;
 
  vector< Bool_t >            CPlotsLogY ;   
  vector< TString >           CPlotsXAxis;
  vector< TH1F* >             CPlotsData ;
  vector< TH1F* >             CPlotsSign ;  
  vector< vector< TH1F* > >  vCPlotsBkgd ;

  TH2F*               CorrMtxS;
  TH2F*               CorrMtxB;

  TH1F*               D2Train;
  TH1F*               D2Test ;

  TH1F*               STrain ;
  TH1F*               BTrain ;
  TH1F*               STest  ;
  TH1F*               BTest  ;

  TH1D*               DCut   ;
  TH1D*               SCut   ;
  TH1D*               BCutTr ;
  TH1D*               BCutAll;
  vector<TH1D*>       vSCut  ;
  vector<string>      vSName ;
  vector<TH1D*>       vBCut  ;
  vector<string>      vBName ;

  TH1D*               SignCutTr;
  TH1D*               SignCutAll;
  TH1D*               LimitCutTr;
  TH1D*               LimitCutAll;

  TH1D*               Bin; 
  TH1D*               Cut; 
  TH1D*               Sign; 
  TH1D*               Limit; 
  TH1D*               DataLimit; 

  TH1D*               CutBased;

};


class UATmvaSummary {

  private:

  
  Bool_t                    InitDone  ;
  vector<UATmvaSummary_t*>  vUASummary ;
  void PlotStack( TH1F* , TH1F* , vector<TH1F*> , TString ="" , TString ="" , int = 0 , bool = false );

  Int_t MVARebinFac ;

  public:

  UATmvaSummary(){ InitDone = false ;}
  virtual ~UATmvaSummary();

  Bool_t IsInit() { return InitDone ; }

  void Init ( UATmvaConfig& );
  void Print( );
  void Plots( UATmvaConfig& , bool = true );
  void CPlots();
  void Yields();
  void BestMVA();
  void LimitCard(  UATmvaConfig&  );

  void PlotCorrMtx    ( int , bool = true);
  void PlotEpoch      ( int );
  void PlotOvertrain  ( int );
  void PlotEff        ( int );
  void PlotMVAStack   ( UATmvaConfig& , int );
  void PlotDimMVA     ( UATmvaConfig& , int , int );
  void PlotCplotStack ( int , int );
  void PrintYields ( int , int );
  int    GetBestLimitMVAID ();
  double GetBestLimitMVAVAL ();
  double GetBestDataLimitMVAVAL ();


};


#endif

