#include <TString.h>
#include <TH1D.h>
#include <iostream>

using namespace std;

Double_t OptimalCutHigh(TH1D hSign,int iVal=0){
  
  Int_t    maxBin  = hSign.GetMaximumBin();  
  Double_t maxSign = hSign.GetMaximum();
  Double_t maxCut  = hSign.GetBinCenter(maxBin);
  
  cout << "[UATMmva::OptimalCut]" << endl;
  cout << "  ---> cut value    = " << maxCut ;
  cout << "  ---> significance = " << maxSign << endl;
  if (iVal==1) return maxSign;
  if (iVal==2) return maxBin ;
  return maxCut;
}

Double_t OptimalCutLow(TH1D hSign,int iVal=0){
  
  Int_t    maxBin  = hSign.GetMinimumBin();  
  Double_t maxSign = hSign.GetMinimum();
  Double_t maxCut  = hSign.GetBinCenter(maxBin);
  
  cout << "[UATMmva::OptimalCut]" << endl;
  cout << "  ---> cut value    = " << maxCut ;
  cout << "  ---> significance = " << maxSign << endl;
  if (iVal==1) return maxSign;
  if (iVal==2) return maxBin ;
  return maxCut;
}


TH1D GetSoverB(TString hName, TH1D* Signal , TH1D* Background) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if(B>0.) hSign->SetBinContent(iBin,S/B);
  }

  return *hSign;

}

TH1D GetSoverSqrtSPlusB(TString hName, TH1D* Signal , TH1D* Background) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if((S+B)>0.) hSign->SetBinContent(iBin,S/sqrt(S+B));
  }

  return *hSign;

}

TH1D GetSoverSqrtBPlusDeltaB(TString hName, TH1D* Signal , TH1D* Background, Double_t FracDeltaB = .35 ) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if(B>0.) hSign->SetBinContent(iBin,S/sqrt(B+pow(FracDeltaB*B,2)));
  }

  return *hSign;

}


TH1D GetExclusionLimit(TString hName, TH1D* Signal , TH1D* Background) {


  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    init();
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    cout << "[CALL limitBayesian] B = " << B <<" S = "<< S <<endl;
    if (S>0) hSign->SetBinContent(iBin,limitBayesian(B,.35,S,.1));
    else     hSign->SetBinContent(iBin,-99.);
    delete wRoo;

  }


  return *hSign;

}




