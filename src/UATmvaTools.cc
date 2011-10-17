#include <TString.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <iostream>
#include "../includes/UATmvaConfig.h"

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

  //return hSign; 

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

// ------------------------------------------ limitACLs --------------------------------------------

Double_t limitACLs( int iBin ,TH1D*& hData , vector<TH1D*>& vhSignal , vector<TH1D*>& vhBackground, UATmvaConfig& Cfg ){

  string LimitCardName = "LimitCardTmp.card"   ;
  string LimitOutName  = "LimitResultTmp.ExpLim" ;
  
  Double_t Data   , eStatData   ;
  vector<Double_t>  Signal ;
  vector<Double_t>  eStatSignal ;
  vector<Double_t>  Background ;
  vector<Double_t>  eStatBkgd ;

  vector<string> Proc;

  // Prepare Data inputs

  Data     =  hData->IntegralAndError(iBin,hData->GetNbinsX(),eStatData); 

  // Prepare Signal Inputs

  Proc.clear();
  Signal.clear();
  eStatSignal.clear();
  
  if ( Cfg.GetPlotGroup()->size() == 0 ) {
    for (int iH=0 ; iH < (signed) vhSignal.size() ; ++iH) { 
      Double_t EStat;
      Signal.push_back( (vhSignal.at(iH))->IntegralAndError(iBin,(vhSignal.at(iH))->GetNbinsX(),EStat) );
      eStatSignal.push_back(EStat);
      int nD=0; 
      string cProc ;
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if (iD->SigTest) {
          if ( iH == nD) cProc = iD->NickName ;
          ++nD; 
        }
      }
      Proc.push_back(cProc);
    }
  } else {
    cout << "[limitACLs] PlotGroup not implemented" << endl;
    return 20;
  }

  // Prepare Background Inputs

    
  Background.clear();
  eStatBkgd.clear();
  if ( Cfg.GetPlotGroup()->size() == 0 ) {
    for (int iH=0 ; iH < (signed) vhBackground.size() ; ++iH) {
      Double_t EStat;
      Background.push_back( (vhBackground.at(iH))->IntegralAndError(iBin,(vhBackground.at(iH))->GetNbinsX(),EStat) );
      eStatBkgd.push_back(EStat);
      int nD=0;
      string cProc ;
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if (iD->BkgdData||iD->BkgdTest) {
          if ( iH == nD) cProc = iD->NickName ;
          ++nD;
        }
      }
      Proc.push_back(cProc);
    }
  } else {
    cout << "[limitACLs] PlotGroup not implemented" << endl;
    return 20;
  }


  // Make LimitCard
  FILE *cFile;
  cFile = fopen (LimitCardName.c_str(),"w");
  
  fprintf (cFile,"imax 1 number of channels\n");
  fprintf (cFile,"jmax * number of background\n");
  fprintf (cFile,"kmax * number of nuisance parameters\n");
  fprintf (cFile,"Observation %-9.3f \n",Data);

  fprintf (cFile,"bin ");
  for (int iD=0 ; iD < (signed) Proc.size() ; ++iD ) fprintf (cFile,"1 ") ;

  fprintf (cFile,"\n") ;
  fprintf (cFile,"process ") ;
  for (int iD=0 ; iD < (signed) Proc.size() ; ++iD ) fprintf (cFile,"%s ", (Proc.at(iD)).c_str() );
  fprintf (cFile,"\n") ;
  fprintf (cFile,"process ");
  for (int iD=1 ; iD <= (signed) Signal.size() ; ++iD ) fprintf (cFile,"%i ", - (signed) Signal.size() + iD );
  for (int iD=1 ; iD <= (signed) Background.size() ; ++iD ) fprintf (cFile,"%i ",iD);
  fprintf (cFile,"\n") ;
  
  fprintf (cFile,"rate ") ;
  for (int iD=0 ; iD < (signed) Signal.size() ; ++iD ) fprintf (cFile,"%-9.3f ",Signal.at(iD)) ;
  for (int iD=0 ; iD < (signed) Background.size() ; ++iD ) fprintf (cFile,"%-9.3f ",Background.at(iD)) ;
  fprintf (cFile,"\n") ;

  // ... Syst Errors
  for ( vector<Systematic_t>::iterator itSyst = (Cfg.GetSystematic())->begin() ; itSyst != (Cfg.GetSystematic())->end() ; ++ itSyst ) {
    fprintf (cFile,"%-25s %-5s ",(itSyst->systName).c_str(),(itSyst->systType).c_str());
    for ( vector<string>::iterator itProc =  Proc.begin() ; itProc != Proc.end() ; ++itProc) {
      bool pFound = false ;
      for ( vector<string>::iterator itSM  = (itSyst->systMember).begin() ; itSM != (itSyst->systMember).end() ; ++itSM ) {
        if ( (*itSM) == (*itProc) )  pFound = true ;
      }
      if ( pFound )  fprintf (cFile,"%-5.3f ",itSyst->systVal);
      else           fprintf (cFile,"  -   ");
    }
    fprintf (cFile,"\n") ;
  }

  // ... Stat Errors
  int iProc = 0 ;
  for (int iD=0 ; iD < (signed) Signal.size() ; ++iD ) {
    double eStaRel = 1. ;
    if (Signal.at(iD)>0.) eStaRel += eStatSignal.at(iD) / Signal.at(iD) ;
    string statName = "stat_" + Proc.at(iProc) ;
    string statType = "lnN" ;
    fprintf (cFile,"%-25s %-5s ",statName.c_str() , statType.c_str() ) ;      
    for ( vector<string>::iterator itProc =  Proc.begin() ; itProc != Proc.end() ; ++itProc) {
      if ( (Proc.at(iProc)) == (*itProc) ) fprintf (cFile,"%-5.3f ",eStaRel) ;
      else                                                   fprintf (cFile,"  -   ");
    }
    fprintf (cFile,"\n") ;
    ++iProc;
  }
  for (int iD=0 ; iD < (signed) Background.size() ; ++iD ) {
    double eStaRel = 1. ;
    if (Background.at(iD)>0.) eStaRel += eStatBkgd.at(iD) / Background.at(iD) ;
    string statName = "stat_" + Proc.at(iProc) ;
    string statType = "lnN" ;
    fprintf (cFile,"%-25s %-5s ",statName.c_str() , statType.c_str() ) ;      
    for ( vector<string>::iterator itProc =  Proc.begin() ; itProc != Proc.end() ; ++itProc) {
      if ( (Proc.at(iProc)) == (*itProc) ) fprintf (cFile,"%-5.3f ",eStaRel) ;
      else                                                   fprintf (cFile,"  -   ");
    }
    fprintf (cFile,"\n") ;
    ++iProc;
  }

  fclose(cFile);

  // Compute Limit

  string execBase  = "source $VO_CMS_SW_DIR/cmsset_default.sh ; cd /localgrid/xjanssen/CMSSW_4_2_8/src ; eval `scramv1 runtime -sh` ; cd - ";
  string execLimit = "combine -M Asymptotic --run=expected -m 0 " + LimitCardName + " > " + LimitOutName  ;
  string execGet   = "grep 'Median for expected limits:' " + LimitOutName + " && Limit=$(grep 'Median for expected limits:' " + LimitOutName + " | awk '{print $5}' ) || Limit=40. ; echo $Limit > .explim "  ;
  cout << execLimit << endl;
  string EXEC      = execBase + ";" + execLimit + ";" + execGet  ;
  system( EXEC.c_str() );

  Double_t Limit;  
  ifstream indata;
  indata.open(".explim");
  if(!indata) { // file couldn't be opened
    cerr << "Error: file could not be opened .explim !!!!! " << cout;  
    Limit = 40. ;
  } else { 
    while ( !indata.eof() ) {
      indata >> Limit ;
    }
  }
  return Limit;
}

// ------------------------------------------- GetExclusionLimitiACLs ---------------------------------

TH1D* GetExclusionLimitiACLs(TString hName, TH1D*& hData , vector<TH1D*>& vhSignal , vector<TH1D*>& vhBackground, UATmvaConfig& Cfg ) {

  TH1D* hLimit = (TH1D*) hData->Clone(hName);
  hLimit->Reset();
  hLimit->SetTitle(hName);

//  return hLimit; 


  Int_t nBin    = hLimit->GetNbinsX() ;
  Int_t nBinMin = 1 ;
  Int_t nBinMax = nBin ;

  for ( Int_t jBin = nBinMin ; jBin <= nBinMax ; ++jBin ) hLimit->SetBinContent(jBin,20.) ;


  // Adaptative Scan to save CPU time
  // Steps by 40 for >200 bins
  if (nBin>200) {   
    Double_t minLimVal = 19.;
    Int_t    minLimBin = nBinMin  ;
    for ( Int_t iBin = nBinMin ; iBin <= nBinMax ; ) {
      Double_t Limit = limitACLs(iBin+20, hData , vhSignal , vhBackground, Cfg );

      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+39 ; ++jBin ) {
        hLimit->SetBinContent(jBin,Limit); 
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
      Double_t Limit = limitACLs(iBin+5, hData , vhSignal , vhBackground, Cfg );
      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+ 9 ; ++jBin ) {
        hLimit->SetBinContent(jBin,Limit); 
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
      Double_t Limit = limitACLs(iBin+2, hData , vhSignal , vhBackground, Cfg );
      //cout << iBin << " " << S << " " << B << " " << Limit << endl;
      for ( Int_t jBin = iBin ; jBin <= iBin+ 4 ; ++jBin ) {
        hLimit->SetBinContent(jBin,Limit); 
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
   Double_t Limit = limitACLs(iBin  , hData , vhSignal , vhBackground, Cfg ); 
   hLimit->SetBinContent(iBin,Limit);
   //cout << iBin << " " << S << " " << B << " " << Limit << endl; 
  }


//  delete wRoo;


  return hLimit;

}


