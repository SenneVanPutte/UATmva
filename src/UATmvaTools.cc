#include <TString.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <iostream>

using namespace std;

Double_t OptimalCutHigh(TH1D* hSign,int iVal=0){
  
  Int_t    maxBin  = hSign->GetMaximumBin();  
  Double_t maxSign = hSign->GetMaximum();
  Double_t maxCut  = hSign->GetBinCenter(maxBin);
  
  cout << "[UATMmva::OptimalCut]" << endl;
  cout << "  ---> cut value    = " << maxCut ;
  cout << "  ---> significance = " << maxSign << endl;
  if (iVal==1) return maxSign;
  if (iVal==2) return maxBin ;
  return maxCut;
}

Double_t OptimalCutLow(TH1D* hSign,int iVal=0){
  
  Int_t    maxBin  = hSign->GetMinimumBin();  
  Double_t maxSign = hSign->GetMinimum();
  Double_t maxCut  = hSign->GetBinCenter(maxBin);
  
  cout << "[UATMmva::OptimalCut]" << endl;
  cout << "  ---> cut value    = " << maxCut ;
  cout << "  ---> significance = " << maxSign << endl;
  if (iVal==1) return maxSign;
  if (iVal==2) return maxBin ;
  return maxCut;
}


Double_t FindCutBased(TH1D* Signal, Double_t CutBSign , int iVal=0){

  if ( ! CutBSign > 0 ) return 999. ;

  TH1D* iSign = (TH1D*) Signal->Clone();
  iSign->Reset();
  for ( Int_t iBin = 1 ; iBin <= Signal->GetNbinsX() ; ++iBin ) {
    Double_t vbin = Signal->Integral(iBin,Signal->GetNbinsX()); 
    iSign->SetBinContent(iBin,vbin) ; 
  }

  TH1D* hDiv  = (TH1D*) iSign ->Clone();
  hDiv->Scale(1./CutBSign);
  for ( Int_t iBin = 1 ; iBin <= Signal->GetNbinsX() ; ++iBin ) {
    Double_t vbin = hDiv->GetBinContent(iBin);
    if (vbin == 0. ) vbin = 10. ;
    if (vbin <  1. ) vbin = 1./vbin ;
    if (vbin > 10. ) vbin = 10. ;
    hDiv->SetBinContent(iBin,vbin) ;
  }

  Int_t    maxBin  = hDiv->GetMinimumBin();
  Double_t maxSign = hDiv->GetMinimum();
  Double_t Sign    = iSign->GetBinContent(maxBin);
  Double_t maxCut  = hDiv->GetBinCenter(maxBin);

  cout << "[UATMmva::FindCutBased]" << endl;
  cout << "  ---> cut value    = " << maxCut ;
  cout << "  ---> CutBase Sign = " << CutBSign ;
  cout << "  ---> TMVA re Sign = " << Sign << endl ;

/*
  // DEBUG
  TCanvas *c1 = new TCanvas();
  iSign->Draw();
  TCanvas *c2 = new TCanvas();
  hDiv->Draw();
  gPad->WaitPrimitive();
*/

  if (iVal==1) return maxSign;
  if (iVal==2) return maxBin ;
  return maxCut;



  return 0;
}

TH1D* GetSoverB(TString hName, TH1D* Signal , TH1D* Background) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return new TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if(B>0.) hSign->SetBinContent(iBin,S/B);
  }

  return hSign;

}

TH1D* GetSoverSqrtSPlusB(TString hName, TH1D* Signal , TH1D* Background) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return new TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if((S+B)>0.) hSign->SetBinContent(iBin,S/sqrt(S+B));
  }

  return hSign;

}

TH1D* GetSoverSqrtBPlusDeltaB(TString hName, TH1D* Signal , TH1D* Background, Double_t FracDeltaB = .35 ) {

  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return new TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    if(B>0.) hSign->SetBinContent(iBin,S/sqrt(B+pow(FracDeltaB*B,2)));
  }

  return hSign;

}


TH1D* GetExclusionLimit(TString hName, TH1D* Signal , TH1D* Background) {


  if ( ! Signal->CheckConsistency(Signal,Background) ) { 
    cout << "[GetSignificance] histograms with different definitions" << endl; 
    return new TH1D() ;
  } 

  TH1D* hSign = (TH1D*) Signal->Clone(hName);
  hSign->Reset();
  hSign->SetTitle(hName);

//  return hSign; 

//  init();
//  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

  Int_t nBin    = hSign->GetNbinsX() ;
  Int_t nBinMin = 1 ;
  Int_t nBinMax = nBin ;

  for ( Int_t jBin = nBinMin ; jBin <= nBinMax ; ++jBin ) hSign->SetBinContent(jBin,20.) ;

  // Adaptative Scan to save CPU time
  // Steps by 40 for >200 bins
  if (nBin>200) {   
    Double_t minLimVal = 19.;
    Int_t    minLimBin = nBinMin  ;
    for ( Int_t iBin = nBinMin ; iBin <= nBinMax ; ) {
      Double_t S = Signal->Integral(iBin+20,hSign->GetNbinsX());
      Double_t B = Background->Integral(iBin+20,hSign->GetNbinsX());
      init(); RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
      Double_t Limit = limitBayesian(B,.35,S,.2);
      delete wRoo;
      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+39 ; ++jBin ) {
        hSign->SetBinContent(jBin,Limit); 
      }
      if ( Limit < minLimVal ) {
        minLimVal = Limit ;
        minLimBin = iBin  ;
      }
      iBin+=40; 
    }
    if (minLimBin > 1 ) nBinMin = minLimBin - 40 ;
    nBinMax = minLimBin + 80 ;
    if (nBinMax > nBin ) nBinMax = nBin ;
  }


  // Steps by 10 for >80  bins
  if (nBin>80 ) {   
    Double_t minLimVal = 19.;
    Int_t    minLimBin = nBinMin  ;
    for ( Int_t iBin = nBinMin ; iBin <= nBinMax ; ) {
      Double_t S = Signal->Integral(iBin+5 ,hSign->GetNbinsX());
      Double_t B = Background->Integral(iBin+5 ,hSign->GetNbinsX());
      init(); RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
      Double_t Limit = limitBayesian(B,.35,S,.2);
      delete wRoo;
      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+ 9 ; ++jBin ) {
        hSign->SetBinContent(jBin,Limit); 
      }
      if ( Limit < minLimVal ) {
        minLimVal = Limit ;
        minLimBin = iBin  ;
      }
      iBin+=10; 
    }
    if (minLimBin > 1 ) nBinMin = minLimBin - 10 ;
    nBinMax = minLimBin + 20 ;
    if (nBinMax > nBin ) nBinMax = nBin ;
  }


  // Steps by 5  for >20  bins
  if (nBin>20 ) {   
    Double_t minLimVal = 19.;
    Int_t    minLimBin = nBinMin  ;
    for ( Int_t iBin = nBinMin ; iBin <= nBinMax ; ) {
      Double_t S = Signal->Integral(iBin+2 ,hSign->GetNbinsX());
      Double_t B = Background->Integral(iBin+2 ,hSign->GetNbinsX());
      init(); RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
      Double_t Limit = limitBayesian(B,.35,S,.2);
      delete wRoo;
      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+ 4 ; ++jBin ) {
        hSign->SetBinContent(jBin,Limit); 
      }
      if ( Limit < minLimVal ) {
        minLimVal = Limit ;
        minLimBin = iBin  ;
      }
      iBin+=5; 
    }
    if (minLimBin > 1 ) nBinMin = minLimBin - 5  ;
    nBinMax = minLimBin + 10 ;
    if (nBinMax > nBin ) nBinMax = nBin ;
  }

  // Last Step of 1 Bin

  for ( Int_t iBin = nBinMin ; iBin <= nBinMax ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    init(); RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
    hSign->SetBinContent(iBin,limitBayesian(B,.35,S,.1));
    delete wRoo;
    //cout << iBin << " " << S << " " << B << " " << Limit << endl; 
  }

/*
  for ( Int_t iBin = 1 ; iBin <= hSign->GetNbinsX() ; ++iBin ) {
    Double_t S = Signal->Integral(iBin,hSign->GetNbinsX());
    Double_t B = Background->Integral(iBin,hSign->GetNbinsX());
    cout << "[CALL limitBayesian] B = " << B <<" S = "<< S <<endl;
    hSign->SetBinContent(iBin,limitBayesian(B,.35,S,.1));
  }
*/

//  delete wRoo;
  return hSign;

}




