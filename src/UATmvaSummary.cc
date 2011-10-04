
#include "../includes/UATmvaSummary.h"
#include <stdio.h>
#include <iostream>

#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TText.h>
#include <TLegend.h>

void SetGoodAxis(TObject* Curve_){

  // For the canvas:
  gStyle->SetOptStat(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);

  // For the Pad:
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadColor(kWhite);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);



  TAxis* xaxis = 0;
  TAxis* yaxis = 0;

  xaxis = ((TH1*) Curve_)->GetXaxis();
  yaxis = ((TH1*) Curve_)->GetYaxis();

  xaxis->SetTitleOffset(0.7);
  yaxis->SetTitleOffset(1.3);
  xaxis->SetTitleSize(0.05);
  yaxis->SetTitleSize(0.05);
  xaxis->SetLabelSize(0.04);
  yaxis->SetLabelSize(0.04);
  //xaxis->SetNdivisions(105);
  yaxis->SetNdivisions(206);
}



UATmvaSummary_t::UATmvaSummary_t(){
  TmvaName = "UNDEF";
  Cut      = new TH1D(); 
}


UATmvaSummary_t::UATmvaSummary_t(TString NameBase, TString MethodName , TString NameExt , UATmvaConfig& Cfg , int iLumi ){
 
  if ( NameExt != "NULL" ) {
    BaseName = NameBase+"_"+MethodName;
    ExtName  = NameExt;
    TmvaName = BaseName+"_"+NameExt;
  } else {
    BaseName = NameBase ;
    TmvaName = BaseName ;
  }

  // Open File $ fetc objects
  cout << "[UATmvaSummary_t] Reading File: " << TmvaName << endl;
  TFile*   File     = new TFile("rootfiles/" + TmvaName  + ".root","READ" );
  ostringstream Directory;
  Directory << "OutputHistograms_" << Cfg.GetTargetLumi()->at(iLumi).Lumi << "pbinv" ;
  TString TSDirectory = Directory.str();
  cout << TSDirectory << endl;

  cout << "[UATmvaSummary_t] Fetching Control Plots" << endl;

  vector< TH1F* >             CPlotsData_ ;
  vector< TH1F* >             CPlotsSign_ ;  
  vector< vector< TH1F* > >  vCPlotsBkgd_ ;
  for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
    CPlotsXAxis.push_back( Cfg.GetCtrlPlot()->at(iP).VarName );
    CPlotsLogY.push_back( Cfg.GetCtrlPlot()->at(iP).kLogY );
    // Data and Signal
    TH1F* CPlotData_ = NULL;
    TH1F* CPlotSign_ = NULL;
    for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
      TString HistName = iD->NickName+"_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
      if (iD->TrueData) {
        if ( CPlotData_ == NULL ) CPlotData_ = (TH1F*) ((TH1F*) File->Get(TSDirectory+"/"+HistName))->Clone() ;
        else                      CPlotData_ -> Add    ((TH1F*) File->Get(TSDirectory+"/"+HistName))          ;
      }
      if (iD->SigTrain) {  
        if ( CPlotSign_ == NULL ) CPlotSign_ = (TH1F*) ((TH1F*) File->Get(TSDirectory+"/"+HistName))->Clone() ;
        else                      CPlotSign_ -> Add    ((TH1F*) File->Get(TSDirectory+"/"+HistName))          ;
      }
    }
    // Backgrounds (need to group if requested)
    vector<TH1F*> CPlotsBkgd_ ;
    if ( Cfg.GetPlotGroup()->size() == 0 ) {
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if (iD->BkgdData||iD->BkgdTrain ) {
          TString HistName = iD->NickName+"_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
          CPlotsBkgd_.push_back( (TH1F*) File->Get(TSDirectory+"/"+HistName) );
        }
      }
    } else {
      for ( vector<PlotGroup_t>::iterator iG = (Cfg.GetPlotGroup())->begin() ; iG != (Cfg.GetPlotGroup())->end() ; ++iG) {
        TH1F* CPlotBkgd_ = NULL; 
        for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
          TString HistName = iD->NickName+"_"+Cfg.GetCtrlPlot()->at(iP).VarName ; 
          for ( int iGM=0 ; iGM < (signed) iG->PlotGroupMember.size() ; ++iGM ) {
            if ( iG->PlotGroupMember.at(iGM) == iD->NickName ) {
              if ( CPlotBkgd_ == NULL ) CPlotBkgd_ = (TH1F*) ((TH1F*) File->Get(TSDirectory+"/"+HistName))->Clone() ;
              else                      CPlotBkgd_ -> Add    ((TH1F*) File->Get(TSDirectory+"/"+HistName))          ;
            }    
          } 
        }
        //if ( CPlotBkgd_ == NULL ) CPlotBkgd_ = new TH1D(
        CPlotsBkgd_.push_back( (TH1F*) CPlotBkgd_ ->Clone() ) ;
        delete CPlotBkgd_;
      }
    }
    // Store CtrlPlots far all sample  
    if ( CPlotData_ == NULL ) CPlotData_ = new TH1F(Cfg.GetCtrlPlot()->at(iP).VarName,Cfg.GetCtrlPlot()->at(iP).VarName,
                                                    Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax);
    CPlotsData_.push_back(CPlotData_ );
    CPlotsSign_.push_back(CPlotSign_ );
    vCPlotsBkgd_.push_back(CPlotsBkgd_); 
  }  
 
  cout << "[UATmvaSummary_t] Fetching CorrelationMatrix" << endl;

  TH2F* CorrMtxS_ ; 
  TH2F* CorrMtxB_ ;
  if ( Cfg.GetTmvaType() != "XML" ) {
    CorrMtxS_ = (TH2F*) File->Get("CorrelationMatrixS");
    CorrMtxB_ = (TH2F*) File->Get("CorrelationMatrixB");  
  } else {
    CorrMtxS_ = new TH2F();
    CorrMtxB_ = new TH2F();
  }

  cout << "[UATmvaSummary_t] Fetching estimatorHist" << endl;
  TH1F* D2Train_  ;
  TH1F* D2Test_   ;
  if ( Cfg.GetTmvaType() == "ANN" ) {
    D2Train_ = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/estimatorHistTrain");
    D2Test_  = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/estimatorHistTest");
  } else {
    D2Train_ = new TH1F();
    D2Test_  = new TH1F();
  }


  cout << "[UATmvaSummary_t] Fetching TMVA Hist" << endl;
  TH1F* STrain_  ;
  TH1F* BTrain_  ;
  TH1F* STest_   ;
  TH1F* BTest_   ;
  if ( Cfg.GetTmvaType() != "XML" ) {
    STrain_   = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_Train_S");
    BTrain_   = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_Train_B");
    STest_    = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_S");
    BTest_    = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_B");
  } else {
    STrain_   = new TH1F();
    BTrain_   = new TH1F();
    STest_    = new TH1F();
    BTest_    = new TH1F();
  }

  cout << "[UATmvaSummary_t] Fetching MVA outputs" << endl;
  TH1D* DCut_     = (TH1D*) File->Get(TSDirectory+"/Data");
  TH1D* SCut_     = (TH1D*) File->Get(TSDirectory+"/Signal");
  TH1D* BCutTr_   = (TH1D*) File->Get(TSDirectory+"/BkgdTrain");
  TH1D* BCutAll_  = (TH1D*) File->Get(TSDirectory+"/BkgdTot");


  vector <TH1D*>  vSCut_ ;
//  if ( Cfg.GetPlotGroup()->size() == 0 ) {
    for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
      if (iD->SigTrain) {
        vSName.push_back(iD->NickName);
        vSCut_.push_back( (TH1D*) ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName))->Clone() ) ;
//         (vSCut_.at(0))->Draw();
      } 
    } 
/*
  } else {
    for ( vector<PlotGroup_t>::iterator iG = (Cfg.GetPlotGroup())->begin() ; iG != (Cfg.GetPlotGroup())->end() ; ++iG) {
      bool iSSignal = false ;
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if ( iG->PlotGroupMember.at(0) == iD->NickName ) iSSignal = iD->SigTrain ;
      }
      if ( iSSignal ) {
        vSName.push_back(iG->PlotGroupName) ;
        TH1D* iGHist = NULL ;
        for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
          if ( iG->PlotGroupMember.at(0) == iD->NickName ) iGHist = (TH1D*) ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName))->Clone()  ;
        }
        for ( int iGM=1 ; iGM < (signed) iG->PlotGroupMember.size() ; ++iGM ) {
          for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
            if ( iG->PlotGroupMember.at(iGM) == iD->NickName ) iGHist->Add ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName));
          }
        }
        if ( iGHist != NULL ) {
          vSCut_.push_back( (TH1D*) iGHist->Clone() ) ;
          delete iGHist ;
        }
      }  
    }
  }
*/

  vector <TH1D*>  vBCut_ ;
  if ( Cfg.GetPlotGroup()->size() == 0 ) {
    for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
      if (iD->BkgdData) {
        vBName.push_back(iD->NickName);
        vBCut_.push_back( (TH1D*) ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName))->Clone() ) ;
      } 
    } 
  } else {
    for ( vector<PlotGroup_t>::iterator iG = (Cfg.GetPlotGroup())->begin() ; iG != (Cfg.GetPlotGroup())->end() ; ++iG) {
      bool iSBkgd = false ;
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if ( iG->PlotGroupMember.at(0) == iD->NickName ) iSBkgd = iD->BkgdData ;
      }
      if ( iSBkgd ) {
        vBName.push_back(iG->PlotGroupName) ;
        TH1D* iGHist = NULL ;
        for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
          if ( iG->PlotGroupMember.at(0) == iD->NickName ) iGHist = (TH1D*) ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName))->Clone()  ;
        }
        for ( int iGM=1 ; iGM < (signed) iG->PlotGroupMember.size() ; ++iGM ) {
          for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
            if ( iG->PlotGroupMember.at(iGM) == iD->NickName ) iGHist->Add ((TH1D*) File->Get(TSDirectory+"/"+iD->NickName));
          }
        }
        if ( iGHist != NULL ) {
          vBCut_.push_back( (TH1D*) iGHist->Clone() ) ;
          delete iGHist ;
        } 
      }
    }
  }

  cout << "[UATmvaSummary_t] Fetching S/B & Limit optim hist" << endl;
  TH1D* SignCutTr_   = (TH1D*) File->Get(TSDirectory+"/bgTr_SoverSqrtBPlusDeltaB");
  TH1D* SignCutAll_  = (TH1D*) File->Get(TSDirectory+"/bkgd_SoverSqrtBPlusDeltaB");
  TH1D* LimitCutTr_  = (TH1D*) File->Get(TSDirectory+"/bgTr_BayesLimit");
  TH1D* LimitCutAll_ = (TH1D*) File->Get(TSDirectory+"/bkgd_BayesLimit");

  cout << "[UATmvaSummary_t] Fetching S/B & Limit result hist" << endl;
  TH1D* Bin_      = (TH1D*) File->Get(TSDirectory+"/Bin");   
  TH1D* Cut_      = (TH1D*) File->Get(TSDirectory+"/Cut");   
  TH1D* Sign_     = (TH1D*) File->Get(TSDirectory+"/Sign");
  TH1D* Limit_    = (TH1D*) File->Get(TSDirectory+"/Limit");
  TH1D* DataLimit_= (TH1D*) File->Get(TSDirectory+"/DataLimit");

  cout << "[UATmvaSummary_t] Fetching CutBased hist" << endl;
  TH1D* CutBased_ = (TH1D*) File->Get(TSDirectory+"/CutBased");

  // Have to create the new object outside of gDirectory from File
  gROOT->cd(); 

  cout << "[UATmvaSummary_t] Copying Control Plots" << endl;
  for ( vector< TH1F* >::iterator iCP = CPlotsData_.begin() ; iCP != CPlotsData_.end() ; ++iCP ) CPlotsData.push_back( (TH1F*) (*iCP)->Clone() ) ;
  for ( vector< TH1F* >::iterator iCP = CPlotsSign_.begin() ; iCP != CPlotsSign_.end() ; ++iCP ) CPlotsSign.push_back( (TH1F*) (*iCP)->Clone() ) ;
  for ( vector<vector<TH1F*> >::iterator ivCP  = vCPlotsBkgd_.begin() ; ivCP !=  vCPlotsBkgd_.end() ; ++ivCP ) {
    vector< TH1F* > CPlotsBkgd ;
    for ( vector< TH1F* >::iterator iCP = ivCP->begin() ; iCP != ivCP->end() ; ++iCP )  CPlotsBkgd.push_back( (TH1F*) (*iCP)->Clone() ) ;
    vCPlotsBkgd.push_back(CPlotsBkgd) ;
  }

  cout << "[UATmvaSummary_t] Copying CorrelationMatrix" << endl;
  CorrMtxS = (TH2F*) CorrMtxS_ ->Clone();
  CorrMtxB = (TH2F*) CorrMtxB_ ->Clone();

  cout << "[UATmvaSummary_t] Copying estimatorHist" << endl;
  D2Train  = (TH1F*) D2Train_ ->Clone();
  D2Test   = (TH1F*) D2Test_  ->Clone();

  cout << "[UATmvaSummary_t] Copying TMVA hist" << endl;
  STrain   = (TH1F*) STrain_ ->Clone();
  BTrain   = (TH1F*) BTrain_ ->Clone();
  STest    = (TH1F*) STest_  ->Clone();
  BTest    = (TH1F*) BTest_  ->Clone();

  cout << "[UATmvaSummary_t] Copying MVA outputs" << endl;
  DCut     = (TH1D*) DCut_    ->Clone() ;
  SCut     = (TH1D*) SCut_    ->Clone() ;
  BCutTr   = (TH1D*) BCutTr_  ->Clone() ;
  BCutAll  = (TH1D*) BCutAll_ ->Clone();
  for ( int iD = 0 ; iD < (signed) vSCut_.size() ; ++iD ) vSCut.push_back( (TH1D*) vSCut_.at(iD)->Clone() ) ;
  for ( int iD = 0 ; iD < (signed) vBCut_.size() ; ++iD ) vBCut.push_back( (TH1D*) vBCut_.at(iD)->Clone() ) ;


  cout << "[UATmvaSummary_t] Copying S/B & Limit optim hist" << endl;
  SignCutTr   = (TH1D*) SignCutTr_   ->Clone() ;
  SignCutAll  = (TH1D*) SignCutAll_  ->Clone() ;
  LimitCutTr  = (TH1D*) LimitCutTr_  ->Clone() ;
  LimitCutAll = (TH1D*) LimitCutAll_ ->Clone() ;

  cout << "[UATmvaSummary_t] Copying S/B & Limit result hist" << endl;
  Bin      = (TH1D*) Bin_    ->Clone();
  Cut      = (TH1D*) Cut_    ->Clone();
  Sign     = (TH1D*) Sign_   ->Clone();
  Limit    = (TH1D*) Limit_  ->Clone();
  DataLimit= (TH1D*) DataLimit_  ->Clone();

  cout << "[UATmvaSummary_t] Copying CutBased hist" << endl;
  CutBased = (TH1D*) CutBased_  ->Clone();

  for ( vector< TH1F* >::iterator iCP = CPlotsData.begin() ; iCP != CPlotsData.end() ; ++iCP ) SetGoodAxis(*iCP);
  for ( vector< TH1F* >::iterator iCP = CPlotsSign.begin() ; iCP != CPlotsSign.end() ; ++iCP ) SetGoodAxis(*iCP);
  for ( vector<vector<TH1F*> >::iterator ivCP  = vCPlotsBkgd.begin() ; ivCP !=  vCPlotsBkgd.end() ; ++ivCP ) {
    for ( vector< TH1F* >::iterator iCP = ivCP->begin() ; iCP != ivCP->end() ; ++iCP ) SetGoodAxis(*iCP); 
  }

  cout << "[UATmvaSummary_t] SetGoodAxis" << endl;

  SetGoodAxis(CorrMtxS);
  SetGoodAxis(CorrMtxB);

  SetGoodAxis(D2Train);
  SetGoodAxis(D2Test);

  SetGoodAxis(STrain);  
  SetGoodAxis(BTrain);  
  SetGoodAxis(STest);
  SetGoodAxis(BTest);

  SetGoodAxis(DCut)   ;
  SetGoodAxis(SCut)   ;
  SetGoodAxis(BCutTr) ;
  SetGoodAxis(BCutAll);
  for ( int iD = 0 ; iD < (signed) vSCut.size() ; ++iD ) SetGoodAxis(vSCut.at(iD));
  for ( int iD = 0 ; iD < (signed) vBCut.size() ; ++iD ) SetGoodAxis(vBCut.at(iD));

  SetGoodAxis(SignCutTr)  ;
  SetGoodAxis(SignCutAll) ;
  SetGoodAxis(LimitCutTr) ;
  SetGoodAxis(LimitCutAll); 

  SetGoodAxis(Cut);
  SetGoodAxis(Sign);
  SetGoodAxis(Limit);
  SetGoodAxis(DataLimit);

  SetGoodAxis(CutBased);

  // Delete tmp objects

  cout << "[UATmvaSummary_t] Deleting pointers" << endl;
  for ( vector< TH1F* >::iterator iCP = CPlotsData_.begin() ; iCP != CPlotsData_.end() ; ++iCP ) delete (*iCP);
  for ( vector< TH1F* >::iterator iCP = CPlotsSign_.begin() ; iCP != CPlotsSign_.end() ; ++iCP ) delete (*iCP);
  for ( vector<vector<TH1F*> >::iterator ivCP  = vCPlotsBkgd_.begin() ; ivCP !=  vCPlotsBkgd_.end() ; ++ivCP ) {
    for ( vector< TH1F* >::iterator iCP = ivCP->begin() ; iCP != ivCP->end() ; ++iCP ) delete (*iCP);
    ivCP->clear();
  }
  CPlotsData_.clear();
  CPlotsSign_.clear();
  vCPlotsBkgd_.clear();

  delete CorrMtxS_ ;
  delete CorrMtxB_ ;

  delete D2Train_ ;
  delete D2Test_  ;

  delete STrain_ ;
  delete BTrain_ ;
  delete STest_ ;
  delete BTest_ ;

  delete DCut_    ;
  delete SCut_    ;
  delete BCutTr_  ;
  delete BCutAll_ ;
  for ( int iD = 0 ; iD < (signed) vSCut.size() ; ++iD ) delete vSCut_.at(iD);
  for ( int iD = 0 ; iD < (signed) vBCut.size() ; ++iD ) delete vBCut_.at(iD);
  vSCut_.clear();
  vBCut_.clear();

  delete SignCutTr_   ;
  delete SignCutAll_  ;
  delete LimitCutTr_  ;
  delete LimitCutAll_ ;

  delete Bin_    ;
  delete Cut_    ;
  delete Sign_   ; 
  delete Limit_  ;
  delete DataLimit_  ;

  delete CutBased_ ;

  // Close File
  File->Close();
  delete File;
  
  // (vSCut.at(0))->DrawCopy();
}


UATmvaSummary_t::~UATmvaSummary_t(){

  cout << "[~UATmvaSummary_t()]" << endl;

  CPlotsXAxis.clear();
  for ( vector< TH1F* >::iterator iCP = CPlotsData.begin() ; iCP != CPlotsData.end() ; ++iCP ) delete (*iCP);
  for ( vector< TH1F* >::iterator iCP = CPlotsSign.begin() ; iCP != CPlotsSign.end() ; ++iCP ) delete (*iCP);
  for ( vector<vector<TH1F*> >::iterator ivCP  = vCPlotsBkgd.begin() ; ivCP !=  vCPlotsBkgd.end() ; ++ivCP ) {
    for ( vector< TH1F* >::iterator iCP = ivCP->begin() ; iCP != ivCP->end() ; ++iCP ) delete (*iCP);
    ivCP->clear();
  }
  CPlotsData.clear();
  CPlotsSign.clear();
  vCPlotsBkgd.clear();

  delete CorrMtxS ;
  delete CorrMtxB ;

  delete D2Train ;
  delete D2Test  ;

  delete STrain ;
  delete BTrain ;
  delete STest ;
  delete BTest ;

  delete DCut   ;
  delete SCut   ;
  delete BCutTr ;
  delete BCutAll;
  for ( int iD = 0 ; iD < (signed) vSCut.size() ; ++iD ) delete vSCut.at(iD);
  for ( int iD = 0 ; iD < (signed) vBCut.size() ; ++iD ) delete vBCut.at(iD);
  vSCut.clear();
  vBCut.clear();
  vSName.clear();
  vBName.clear();


  delete SignCutTr;
  delete SignCutAll;
  delete LimitCutTr;
  delete LimitCutAll;

  delete Bin;    
  delete Cut;    
  delete Sign;
  delete Limit;
  delete DataLimit;
}



// ---------------------------- UATmvaSummary()

UATmvaSummary::~UATmvaSummary(){
  //cout << "UATmvaSummary Destructor" << endl;
  //int iC = 1;
  for ( vector<UATmvaSummary_t*>::iterator itC = vUASummary.begin() ; itC != vUASummary.end() ; ++itC ){
     //cout << "UATmvaSummary_t Delete # : " << iC++ << endl;
     delete (*itC);
   }
   vUASummary.clear();
}

// ---------------------------- Init()

void UATmvaSummary::Init( UATmvaConfig& Cfg ) {

  MVARebinFac = Cfg.GetTmvaRespRebinFac();

  TString MethodName;
  if ( Cfg.GetTmvaType() == "ANN" )  MethodName = "MLP" ;
  if ( Cfg.GetTmvaType() == "BDT" )  MethodName = "BDT" ;
  if ( Cfg.GetTmvaType() == "LH"  )  MethodName = "Likelihood"  ;
  if ( Cfg.GetTmvaType() == "PDERS"  )  MethodName = "PDERS"  ;
  if ( Cfg.GetTmvaType() == "PDEFoam"  )  MethodName = "PDEFoam"  ;
  if ( Cfg.GetTmvaType() == "XML" )  MethodName = "XML" ;

  // Open All files and Load Histos
  for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
  Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;

    if ( Cfg.GetTmvaType() == "XML" ) {
         // Build Name
         ostringstream Name;
         Name << "NULL" ;
         //Name << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
         // Read
         vUASummary.push_back (new UATmvaSummary_t(Cfg.GetTmvaName(),MethodName,Name.str(), Cfg , 0 ));
    }


    if ( Cfg.GetTmvaType() == "LH" ) {
         // Build Name
         ostringstream Name;
         Name << nVarMax << "Var" ;
         // Read
         vUASummary.push_back (new UATmvaSummary_t(Cfg.GetTmvaName(),MethodName,Name.str(), Cfg , 0 ));
    }

    if ( Cfg.GetTmvaType() == "ANN" ) {
      for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
      for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {
         // Build Name
         ostringstream Name;
         Name << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
         if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
         // Read
         vUASummary.push_back (new UATmvaSummary_t(Cfg.GetTmvaName(),MethodName,Name.str(), Cfg , 0 ));
      } // Nodes
      } // Layers
    }

    if ( Cfg.GetTmvaType() == "BDT" ) {
      for( int iBDTNTrees             = 0 ; iBDTNTrees              < (signed) (Cfg.GetBDTNTrees())->size()              ; ++iBDTNTrees              ) {
      for( int iBDTBoostType          = 0 ; iBDTBoostType           < (signed) (Cfg.GetBDTBoostType())->size()           ; ++iBDTBoostType           ) {
//      for( int iBDTAdaBoostR2Loss     = 0 ; iBDTAdaBoostR2Loss      < (signed) (Cfg.GetBDTAdaBoostR2Loss())->size()      ; ++iBDTAdaBoostR2Loss      ) {
      for( int iBDTUseBaggedGrad      = 0 ; iBDTUseBaggedGrad       < (signed) (Cfg.GetBDTUseBaggedGrad())->size()       ; ++iBDTUseBaggedGrad       ) {
      for( int iBDTGradBaggingFraction= 0 ; iBDTGradBaggingFraction < (signed) (Cfg.GetBDTGradBaggingFraction())->size() ; ++iBDTGradBaggingFraction ) {
      for( int iBDTShrinkage          = 0 ; iBDTShrinkage           < (signed) (Cfg.GetBDTShrinkage())->size()           ; ++iBDTShrinkage           ) {
      for( int iBDTSeparationType     = 0 ; iBDTSeparationType      < (signed) (Cfg.GetBDTSeparationType())->size()      ; ++iBDTSeparationType      ) {
      for( int iBDTnCuts              = 0 ; iBDTnCuts               < (signed) (Cfg.GetBDTnCuts())->size()               ; ++iBDTnCuts               ) {
      for( int iBDTPruneMethod        = 0 ; iBDTPruneMethod         < (signed) (Cfg.GetBDTPruneMethod())->size()         ; ++iBDTPruneMethod         ) {
      for( int iBDTPruneStrength      = 0 ; iBDTPruneStrength       < (signed) (Cfg.GetBDTPruneStrength())->size()       ; ++iBDTPruneStrength       ) {
      for( int iBDTNNodesMax          = 0 ; iBDTNNodesMax           < (signed) (Cfg.GetBDTNNodesMax())->size()           ; ++iBDTNNodesMax           ) {



        // Build Name
        ostringstream Name;
        Name << Cfg.GetBDTNTrees()->at(iBDTNTrees) << "Trees_" ;
        Name << Cfg.GetBDTBoostType()->at(iBDTBoostType) << "_" ;
        if ( Cfg.GetBDTBoostType()->at(iBDTBoostType) == "Grad" ) {
         Name << Cfg.GetBDTUseBaggedGrad()->at(iBDTUseBaggedGrad) << "Bagged_" ;
         Name << Cfg.GetBDTGradBaggingFraction()->at(iBDTGradBaggingFraction) << "BagFrac_" ;
         Name << Cfg.GetBDTShrinkage()->at(iBDTShrinkage) << "BagShrink_" ;
        }
        Name << Cfg.GetBDTSeparationType()->at(iBDTSeparationType) << "_" ;
        Name << Cfg.GetBDTnCuts()->at(iBDTnCuts) << "Cuts_" ;
        Name << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod) << "_" ;
        Name << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength) << "PruneStrength_" ;
        Name << Cfg.GetBDTNNodesMax()->at(iBDTNNodesMax) << "NodesMax_" ;
        Name << nVarMax << "Var" ;
        if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
        // Read
        vUASummary.push_back (new UATmvaSummary_t(Cfg.GetTmvaName(),MethodName,Name.str(), Cfg , 0 ));

      } // BDTNTrees
      } // BDTBoostType
//      } // BDTAdaBoostR2Loss
      } // BDTUseBaggedGrad
      } // BDTGradBaggingFraction
      } // BDTShrinkage
      } // BDTSeparationType
      } // BDTnCuts
      } // BDTPruneMethod
      } // BDTPruneStrength
      } // BDTNNodesMax

    }

  } // Variables
} 


void UATmvaSummary::Print( ){

  int iSign = 4 ;
  int iLim  = 7 ;

  cout << endl;
  cout << "  -------> BaseName = " << vUASummary.at(0)->BaseName << endl; 
  cout << "  -------> Cut Based Data         = " << vUASummary.at(0)->CutBased->GetBinContent(1) << endl; 
  cout << "  -------> Cut Based Signal       = " << vUASummary.at(0)->CutBased->GetBinContent(2) << endl; 
  cout << "  -------> Cut Based Bkgd         = " << vUASummary.at(0)->CutBased->GetBinContent(3) << endl; 
  cout << "  -------> Cut Based S/Sqrt(S+B)  = " << vUASummary.at(0)->CutBased->GetBinContent(4) << endl; 
  cout << "  -------> Cut Based S/Sqrt(B+dB) = " << vUASummary.at(0)->CutBased->GetBinContent(5) << endl; 
  cout << "  -------> Cut Based Limit MC     = " << vUASummary.at(0)->CutBased->GetBinContent(6) << endl; 
  cout << "  -------> Cut Based Limit Data   = " << vUASummary.at(0)->CutBased->GetBinContent(7) << endl; 
  cout << "  ------------------------------------------------------------------------------------------------------" << endl ;
  cout << "  | ID | NAME                |" ;
  cout << " S/Sqrt(B+dB):        |";
  cout << " Limit:               |";
  cout << " D^2:        |";
  cout << " Kol-Smirn:  |";
  cout << endl; 
  cout << "  |    |                     |" ;
  cout << " Cut  'WW'  Cut  All  |";
  cout << " Cut  'WW'  Cut  All  |";
  cout << " Train Test  |";
  cout << " Sign  Bkgd  |";
  cout << endl; 
  cout << "  ------------------------------------------------------------------------------------------------------" << endl ;

    
  for ( int iUAS = 0 ; iUAS !=  (signed) vUASummary.size() ; ++iUAS ) {
    
    cout << "  | " << iUAS+1;
    if   (iUAS < 9) cout << "  | " ;
    else             cout << " | " ;
    cout << vUASummary.at(iUAS)->ExtName << " | " ;
    printf ("%3.2f %3.2f  %3.2f %3.2f | %3.2f %3.2f  %3.2f %3.2f | %4.3f %4.3f | %4.3f %4.3f |\n", 
       vUASummary.at(iUAS)->Cut  ->GetBinContent(iSign),
       vUASummary.at(iUAS)->Sign ->GetBinContent(iSign),
       vUASummary.at(iUAS)->Cut  ->GetBinContent(iSign+2),
       vUASummary.at(iUAS)->Sign ->GetBinContent(iSign+2),
       vUASummary.at(iUAS)->Cut  ->GetBinContent(iLim), 
       vUASummary.at(iUAS)->Limit->GetBinContent(iLim),   
       vUASummary.at(iUAS)->Cut  ->GetBinContent(iLim+2), 
       vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2),
       vUASummary.at(iUAS)->D2Train ->GetBinContent( vUASummary.at(iUAS)->D2Train->GetNbinsX() ) ,
       vUASummary.at(iUAS)->D2Test  ->GetBinContent( vUASummary.at(iUAS)->D2Train->GetNbinsX() ) ,
       vUASummary.at(iUAS)->STrain->KolmogorovTest( vUASummary.at(iUAS)->STest ) ,
       vUASummary.at(iUAS)->BTrain->KolmogorovTest( vUASummary.at(iUAS)->BTest ) 
    );
  
  }
 
  cout << "  ------------------------------------------------------------------------------------------------------" << endl ;
  cout << endl;
  BestMVA();
 
}


//-------------------------------- Plots()


void UATmvaSummary::Plots( ){

  Int_t ID = -1 ;
  while ( ID != 0 ) {
    Print();
    cout << "  Enter ID of MVA to plot (0 to exit): ";
    cin  >> ID; 
    if ( ID > 0 && ID <= (signed)  vUASummary.size() ) {
      cout << "  --> Plotting: " << vUASummary.at(ID-1)->TmvaName << endl ;
      //cout << " For Limit optimisation:" << endl;
      //PrintYields(ID-1,9);
      //cout << " For Cutbased optimisation:" << endl;
      //PrintYields(ID,10);

      TCanvas* Canvas = new TCanvas(vUASummary.at(ID-1)->TmvaName,vUASummary.at(ID-1)->TmvaName,950,700);
      
      Canvas->Divide(3,2);
      Canvas->cd(1);
      PlotEpoch(ID-1);
      Canvas->cd(2);
      PlotOvertrain(ID-1);
      Canvas->cd(4);
      PlotCorrMtx(ID-1,1);   
      Canvas->cd(5);
      PlotCorrMtx(ID-1,0);   
      Canvas->cd(3);
      PlotEff(ID-1);
      Canvas->cd(6);
      PlotMVAStack(ID-1 );
      //gPad->WaitPrimitive();
      Canvas->SaveAs("plots/mvasummary_"+vUASummary.at(ID-1)->TmvaName+".eps");
      Canvas->SaveAs("plots/mvasummaty_"+vUASummary.at(ID-1)->TmvaName+".png");

      TCanvas* CanvasStack = new TCanvas(vUASummary.at(ID-1)->TmvaName+"_Stack",vUASummary.at(ID-1)->TmvaName,600,600);
      CanvasStack->cd();
      PlotMVAStack(ID-1);
      CanvasStack->SaveAs("plots/mvastack_"+vUASummary.at(ID-1)->TmvaName+".eps");
      CanvasStack->SaveAs("plots/mvastack_"+vUASummary.at(ID-1)->TmvaName+".png");

    } else if ( ID != 0 ) {
      cout << "  --> Invalid ID !!!!!!!!!!! " << endl ;
    }
  }

}


void UATmvaSummary::CPlots() {

      int nDiv = 2; 
      int nBox = nDiv*nDiv;

      vector<TCanvas*> vCanvasCplot;
      int nCanvas =  vUASummary.at(0)->CPlotsData.size()/nBox ;
      if ( ((float)vUASummary.at(0)->CPlotsData.size()/(float)nBox) > nCanvas ) ++nCanvas;
      for ( int iCanvas = 0 ; iCanvas < nCanvas ; ++iCanvas ) {
        ostringstream CanName; 
        CanName  << "Cplot_" << iCanvas;
        vCanvasCplot.push_back( new TCanvas(CanName.str().c_str(),vUASummary.at(0)->TmvaName,700,700) ) ;
        vCanvasCplot.at(iCanvas)->Divide(nDiv,nDiv);
        TString VarList;
        for(int iPad=1 ; iPad<=nBox ; ++iPad ) { 
          int iVar = iCanvas*nBox+iPad-1 ;
          if (iVar < (signed) vUASummary.at(0)->CPlotsData.size()) { 
            vCanvasCplot.at(iCanvas)->cd(iPad) ; 
            PlotCplotStack(0,iVar);
            VarList += "_" ;
            VarList += vUASummary.at(0)->CPlotsXAxis.at(iVar);
          }
        }
        // gPad->WaitPrimitive();
        vCanvasCplot.at(iCanvas)->SaveAs("plots/cplot"+VarList+"_"+vUASummary.at(0)->TmvaName+".eps");
        vCanvasCplot.at(iCanvas)->SaveAs("plots/cplot"+VarList+"_"+vUASummary.at(0)->TmvaName+".png");

      }


}

//-------------------------------- Yields()
void UATmvaSummary::Yields() {


  for ( int iUAS = 0 ; iUAS !=  (signed) vUASummary.size() ; ++iUAS ) {
    cout << "Yields for: " << vUASummary.at(iUAS)->ExtName << endl;
    cout << "---> Limit optimisation:" << endl;
    PrintYields(iUAS,9);
    cout << "---> CutBased optimisation:" << endl;
    PrintYields(iUAS,10);
  }

}

//-------------------------------- BestMVA()

int UATmvaSummary::GetBestLimitMVAID(){
  Double_t bestlimit = 999. ;
  Int_t    id = 0 ;
  int iLim  = 7 ;
  for ( int iUAS = 0 ; iUAS !=  (signed) vUASummary.size() ; ++iUAS ) {
    if( vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2) < bestlimit) {
       bestlimit = vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2);
       id = iUAS ;
    }
  }
  return id;
}

double UATmvaSummary::GetBestLimitMVAVAL(){
  Double_t bestlimit = 999. ;
  Int_t    id = 0 ;
  int iLim  = 7 ;
  for ( int iUAS = 0 ; iUAS !=  (signed) vUASummary.size() ; ++iUAS ) {
    if( vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2) < bestlimit) {
       bestlimit = vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2);
       id = iUAS ;
    }
  }
  return bestlimit;
}

double UATmvaSummary::GetBestDataLimitMVAVAL(){
  Double_t bestlimit = 999. ;
  Int_t    id = 0 ;
  int iLim  = 7 ;
  for ( int iUAS = 0 ; iUAS !=  (signed) vUASummary.size() ; ++iUAS ) {
    if( vUASummary.at(iUAS)->Limit->GetBinContent(iLim+2) < bestlimit) {
       bestlimit = vUASummary.at(iUAS)->DataLimit->GetBinContent(iLim+2);
       id = iUAS ;
    }
  }
  return bestlimit;
}


void UATmvaSummary::BestMVA() {
  int    iUAS      = GetBestLimitMVAID();
  double bestlimit = GetBestLimitMVAVAL();
  double datalimit = GetBestDataLimitMVAVAL();
  cout << "BEST MVA LIMIT: " << endl; 
  cout << " --> MVA Name      = " << vUASummary.at(iUAS)->ExtName << endl;
  cout << " --> MVA BestLimit = " << bestlimit << endl;
  cout << " --> MVA DataLimit = " << datalimit << endl;
  cout << " --> MVA Yields    : " << endl;
  PrintYields( iUAS , 9 );
}

//-------------------------------- PlottStack()

void UATmvaSummary::PlotStack( TH1F* hData , TH1F* hSign , vector<TH1F*> vBkgd , TString XAxisTitle , TString GlobalTitle , int iUAS , bool kLogY ){

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);
   if (kLogY) gPad->SetLogy(1);

   vector<TH1D*> vStack;
   for (int iD=0 ; iD < (signed) vBkgd.size() ; ++iD ) {
     TH1D* iStack = (TH1D*) vBkgd.at(iD)->Clone();
     for (int iD2Sum=iD+1 ; iD2Sum < (signed)  vBkgd.size() ; ++iD2Sum ) {
       iStack->Add(vBkgd.at(iD2Sum));
     }
     iStack->SetLineColor(iD+2);
     iStack->SetFillColor(iD+2);
     vStack.push_back( (TH1D*) iStack->Clone() );
     delete iStack; 
   }
  
   hSign->SetLineColor(kBlack);
   hSign->SetLineWidth(2);
   hData->SetMarkerColor(kBlack);
   hData->SetMarkerStyle(20);

   Double_t hMax = TMath::Max( vStack.at(0)->GetMaximum() , hSign->GetMaximum() );
   hMax = TMath::Max ( hMax , hData->GetMaximum() ); 

   if (kLogY) vStack.at(0)->GetYaxis()->SetRangeUser( 0.01 , 10*hMax);
   else       vStack.at(0)->GetYaxis()->SetRangeUser( 0.   , 1.2*hMax); 
   vStack.at(0)->SetTitle(GlobalTitle);
   vStack.at(0)->GetXaxis()->SetTitle(XAxisTitle);
   vStack.at(0)->GetYaxis()->SetTitle("Events");

   vStack.at(0)->DrawCopy("hist"); 
   for (int iD=1 ; iD < (signed) vStack.size()  ; ++iD ) vStack.at(iD)->DrawCopy("histsame");
   hData->DrawCopy("esame");
   hSign->DrawCopy("histsame");

   int nLegEntry = 2 + (signed) vStack.size();
   TLegend* Legend = new TLegend (.18,.85-nLegEntry*.035,.5,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetFillStyle(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( hData , "Data  " , "p");
   Legend->AddEntry( hSign , "Signal" , "l");
   for (int iD=0 ; iD < (signed) vStack.size()  ; ++iD ) Legend->AddEntry( vStack.at(iD) , (vUASummary.at(iUAS)->vBName.at(iD)).c_str() , "f");
   Legend->Draw("same");

}

//-------------------------------- PlotCplotStack()

void UATmvaSummary::PlotCplotStack(int iUAS , int iVar ){

   PlotStack(vUASummary.at(iUAS)->CPlotsData.at(iVar),
             vUASummary.at(iUAS)->CPlotsSign.at(iVar),
             vUASummary.at(iUAS)->vCPlotsBkgd.at(iVar),
             vUASummary.at(iUAS)->CPlotsXAxis.at(iVar),
             " ",
             iUAS, 
             vUASummary.at(iUAS)->CPlotsLogY.at(iVar)
            );

    cout << vUASummary.at(iUAS)->CPlotsLogY.at(iVar) << endl;

}

//-------------------------------- PlotMVAStack()

void UATmvaSummary::PlotMVAStack(int iUAS ){


/*
   PlotStack(vUASummary.at(iUAS)->DCut,
             vUASummary.at(iUAS)->SCut,
             vUASummary.at(iUAS)->vBCut);
             TString("MVA Output"),
             TString(vUASummary.at(iUAS)->TmvaName),
             iUAS,
             1
            );
*/


   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);
   gPad->SetLogy(1);

   vector<TH1D*> vStack;
   for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
     cout << vUASummary.at(iUAS)->vBName.at(iD) << " = " ;
     TH1D* iStack = (TH1D*) vUASummary.at(iUAS)->vBCut.at(iD)->Clone(("s"+vUASummary.at(iUAS)->vBName.at(iD)).c_str());
     for (int iD2Sum=iD+1 ; iD2Sum < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD2Sum ) {
        cout << vUASummary.at(iUAS)->vBName.at(iD2Sum) << " + " ;
       iStack->Add(vUASummary.at(iUAS)->vBCut.at(iD2Sum));
     }
     cout << endl;
     iStack->SetLineColor(iD+2);
     iStack->SetFillColor(iD+2);
     iStack->Rebin(MVARebinFac);
     vStack.push_back( (TH1D*) iStack->Clone() );
     delete iStack;
   }

   vUASummary.at(iUAS)->SCut->SetLineColor(kBlack);
   vUASummary.at(iUAS)->SCut->SetLineWidth(2);
   vUASummary.at(iUAS)->DCut->SetMarkerColor(kBlack);
   vUASummary.at(iUAS)->DCut->SetMarkerStyle(20);
    
   Double_t hMax = TMath::Max( vStack.at(0)->GetMaximum() , vUASummary.at(iUAS)->DCut->GetMaximum() );
   hMax = TMath::Max ( hMax , vUASummary.at(iUAS)->BCutAll->GetMaximum() );

   vStack.at(0)->GetYaxis()->SetRangeUser( 0.01 , 10*hMax);
   vStack.at(0)->SetTitle(vUASummary.at(iUAS)->TmvaName);
   vStack.at(0)->GetXaxis()->SetTitle("MVA Output");
   vStack.at(0)->GetYaxis()->SetTitle("Events");

   vStack.at(0)->DrawCopy("histe"); 
   for (int iD=1 ; iD < (signed) vStack.size()  ; ++iD ) vStack.at(iD)->DrawCopy("histsame");
   //vUASummary.at(iUAS)->SCut->DrawCopy("histsame");
   //vUASummary.at(iUAS)->DCut->DrawCopy("esame");
   TH1D* SCut = (TH1D*) (vUASummary.at(iUAS)->SCut)->Clone() ;
   TH1D* DCut = (TH1D*) (vUASummary.at(iUAS)->DCut)->Clone() ;
   SCut->Rebin(MVARebinFac);
   DCut->Rebin(MVARebinFac);
   SCut->DrawCopy("histsame");
   DCut->DrawCopy("esame");
   //TH1D* BCut = (TH1D*) (vUASummary.at(iUAS)->BCutAll)->Clone() ;
   //BCut->Rebin(MVARebinFac);
   //BCut->DrawCopy("histsame");
   delete SCut;
   delete DCut;

   int nLegEntry = 2 + (signed) vStack.size();
   TLegend* Legend = new TLegend (.18,.85-nLegEntry*.035,.5,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetFillStyle(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->DCut   , "Data  " , "p");
   Legend->AddEntry( vUASummary.at(iUAS)->SCut   , "Signal" , "l");
   for (int iD=0 ; iD < (signed) vStack.size()  ; ++iD ) Legend->AddEntry( vStack.at(iD) , (vUASummary.at(iUAS)->vBName.at(iD)).c_str() , "f");
   Legend->Draw("same");

   //gPad->WaitPrimitive();


}



//-------------------------------- PlotCorrMtx()

void UATmvaSummary::PlotCorrMtx(int iUAS, bool Signal ){

   gStyle->SetPalette( 1, 0 );

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);

   TH2F* CorrMtx;
   if (Signal) CorrMtx = vUASummary.at(iUAS)->CorrMtxS ;
   else        CorrMtx = vUASummary.at(iUAS)->CorrMtxB ;

   CorrMtx->SetMarkerColor( kBlack );
   CorrMtx->DrawCopy("col");
   CorrMtx->DrawCopy("textsame"); 
}


//-------------------------------- PlotEpoch()

void UATmvaSummary::PlotEpoch(int iUAS){

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);
 
   vUASummary.at(iUAS)->D2Train->SetLineColor(kRed);
   vUASummary.at(iUAS)->D2Test->SetLineColor(kBlue);

   Double_t hMin = TMath::Min( vUASummary.at(iUAS)->D2Train->GetMinimum() , vUASummary.at(iUAS)->D2Test->GetMinimum() );
   Double_t hMax = TMath::Max( vUASummary.at(iUAS)->D2Train->GetMaximum() , vUASummary.at(iUAS)->D2Test->GetMaximum() );

   vUASummary.at(iUAS)->D2Train->GetYaxis()->SetRangeUser(.98*hMin , 1.02*hMax);
   vUASummary.at(iUAS)->D2Train->SetTitle("Convergence Test");
   vUASummary.at(iUAS)->D2Train->GetXaxis()->SetTitle("Epochs");
   vUASummary.at(iUAS)->D2Train->GetYaxis()->SetTitle("D^2");

   vUASummary.at(iUAS)->D2Train->DrawCopy();
   vUASummary.at(iUAS)->D2Test->DrawCopy("same");

   TLegend* Legend = new TLegend (.5,.75,.8,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetFillStyle(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->D2Train , "Train Sample" , "l" );
   Legend->AddEntry( vUASummary.at(iUAS)->D2Test  , "Test  Sample" , "l" );

   Legend->Draw("same");  
 

}

//-------------------------------- PlotOvertrain()

void UATmvaSummary::PlotOvertrain(int iUAS) {

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);

   vUASummary.at(iUAS)->STrain->SetLineColor(kBlue);
   vUASummary.at(iUAS)->BTrain->SetLineColor(kRed);
   vUASummary.at(iUAS)->STest ->SetLineColor(kBlue);
   vUASummary.at(iUAS)->BTest ->SetLineColor(kRed);

   vUASummary.at(iUAS)->STrain->SetMarkerColor(kBlue);
   vUASummary.at(iUAS)->BTrain->SetMarkerColor(kRed);
   vUASummary.at(iUAS)->STrain->SetMarkerStyle(20);
   vUASummary.at(iUAS)->BTrain->SetMarkerStyle(20);

   Double_t hMax = TMath::Max( vUASummary.at(iUAS)->STrain->GetMaximum() , vUASummary.at(iUAS)->BTrain->GetMaximum() );
   hMax = TMath::Max ( hMax , vUASummary.at(iUAS)->STest->GetMaximum() );
   hMax = TMath::Max ( hMax , vUASummary.at(iUAS)->BTest->GetMaximum() );

   vUASummary.at(iUAS)->STest ->GetYaxis()->SetRangeUser( 0. , 1.3*hMax);
   vUASummary.at(iUAS)->STest ->SetTitle("Overtraining Test");
   vUASummary.at(iUAS)->STest ->GetXaxis()->SetTitle("MVA Output");
   vUASummary.at(iUAS)->STest ->GetYaxis()->SetTitle("(1/N) dN/dx");

   vUASummary.at(iUAS)->STest ->DrawCopy("hist");      
   vUASummary.at(iUAS)->BTest ->DrawCopy("histsame"); 
   vUASummary.at(iUAS)->STrain->DrawCopy("esame");      
   vUASummary.at(iUAS)->BTrain->DrawCopy("esame");

   TLegend* Legend = new TLegend (.5,.65,.8,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetFillStyle(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->STest  , "Signal (Test Sample)" , "l");
   Legend->AddEntry( vUASummary.at(iUAS)->BTest  , "Bkgd   (Test Sample)" , "l");
   Legend->AddEntry( vUASummary.at(iUAS)->STrain , "Signal (Train Sample)" , "p");
   Legend->AddEntry( vUASummary.at(iUAS)->BTrain , "Bkgd   (Train Sample)" , "p");
   Legend->Draw("same");

   Double_t kolS = vUASummary.at(iUAS)->STrain->KolmogorovTest( vUASummary.at(iUAS)->STest );
   Double_t kolB = vUASummary.at(iUAS)->BTrain->KolmogorovTest( vUASummary.at(iUAS)->BTest );

   cout << "  ---> Goodness of signal (background) consistency: " << kolS << " (" << kolB << ")" << endl;

   char sKSS[50] ;
   char sKSB[50] ;
   sprintf (sKSS,"K-S Signal: %5.3g",kolS);
   sprintf (sKSB,"K-S Bkgd  : %5.3g",kolB);

   TText* KSS = new TText( .6 , .6  , TString(sKSS) );KSS->SetTextSize(0.04);KSS->SetNDC();KSS->AppendPad();
   TText* KSB = new TText( .6 , .55 , TString(sKSB) );KSB->SetTextSize(0.04);KSB->SetNDC();KSB->AppendPad();


}

// --------------------------- Ploteff()

void UATmvaSummary::PlotEff ( int iUAS ) {

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);
 
   vUASummary.at(iUAS)->SignCutAll ->SetLineColor(kRed);
   vUASummary.at(iUAS)->LimitCutAll->SetLineColor(kBlue);

   vUASummary.at(iUAS)->SignCutAll ->GetYaxis()->SetRangeUser(0.,10.);
   vUASummary.at(iUAS)->SignCutAll ->SetTitle("MVA Efficiency");
   vUASummary.at(iUAS)->SignCutAll ->GetXaxis()->SetTitle("MVA Output");
   vUASummary.at(iUAS)->SignCutAll ->GetYaxis()->SetTitle("Sigma");

   vUASummary.at(iUAS)->SignCutAll ->DrawCopy(); 
   vUASummary.at(iUAS)->LimitCutAll->DrawCopy("same");

   TLegend* Legend = new TLegend (.2,.75,.5,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetFillStyle(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->SignCutAll  , "Significance" , "l" );
   Legend->AddEntry( vUASummary.at(iUAS)->LimitCutAll , "Limit" , "l" );

   Legend->Draw("same");  
 

    
}


// --------------------------- PrintYields ()

void UATmvaSummary::PrintYields ( int iUAS , int iOptim ) {

  Double_t          Data   ;  
  Double_t          Signal ;
  Double_t          BkgdTot;
  Double_t          EStatData  ;
  Double_t          EStatSignal;
  Double_t          EStatBkgdTot; 
  vector<Double_t>  Background ;
  vector<Double_t>  EStatBkgd ;

  int iBin = (int) vUASummary.at(iUAS)->Bin->GetBinContent(iOptim)  ;
  Data    =  vUASummary.at(iUAS)->DCut->IntegralAndError(iBin,vUASummary.at(iUAS)->DCut->GetNbinsX(),EStatData);
  Signal  =  vUASummary.at(iUAS)->SCut->IntegralAndError(iBin,vUASummary.at(iUAS)->SCut->GetNbinsX(),EStatSignal);
  BkgdTot =  vUASummary.at(iUAS)->BCutAll->IntegralAndError(iBin,vUASummary.at(iUAS)->BCutAll->GetNbinsX(),EStatBkgdTot);

  cout << "Yields: Data    = " << Data    << " +- " << EStatData    << endl;
  cout << "Yields: Signal  = " << Signal  << " +- " << EStatSignal  << endl;
  cout << "Yields: BkgdTot = " << BkgdTot << " +- " << EStatBkgdTot << endl;

  for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
    Double_t EStat;
    Background.push_back( vUASummary.at(iUAS)->vBCut.at(iD)->IntegralAndError(iBin,vUASummary.at(iUAS)->vBCut.at(iD)->GetNbinsX(),EStat) );
    EStatBkgd.push_back(EStat);
    cout << "Yields: Bkgd " <<  vUASummary.at(iUAS)->vBName.at(iD) << " = " << Background.at(iD) << " +- " << EStat << endl;
  }

}

// --------------------------- LimitCard ()

void UATmvaSummary::LimitCard ( UATmvaConfig& Cfg ) {


  string LimitCardName ; 
  string LimitRootName ; 

  Double_t Data   , eStatData   ;  
  vector<Double_t>  Signal ;
  vector<Double_t>  eStatSignal ;
  vector<Double_t>  Background ;
  vector<Double_t>  eStatBkgd ;

  vector<string> Proc;

  // Select Best MVA 

  int iOptim = 9 ;
  int iUAS   = GetBestLimitMVAID() ;

  // Build CardName

  string CardName ;  
  if ( Cfg.GetTmvaType() == "ANN" ) {
    ostringstream Name;
    Name << Cfg.GetTmvaName() << "_MLP_"   
                              << Cfg.GetANNHiddenLayersMin() << "_" << Cfg.GetANNHiddenLayersMax() << "Layers_" 
                              << Cfg.GetANNHiddenNodesMin()  << "_" << Cfg.GetANNHiddenNodesMax()  << "Nodes_" 
                              << (Cfg.GetTmvaVar())->size()  << "Var" ;
    if (Cfg.GetTmvaOptim()) Name << "_Optim" ;
    CardName = Name.str();
  } else {
    CardName = vUASummary.at(iUAS)->TmvaName ;
  } 

  // 0: Cut Based MVA / 1: Shape Based MVA 

  for (int iMethod = 0 ; iMethod < 2 ; ++iMethod ) {

    int iBin = 1 ;
    if ( iMethod == 0 ) iBin = (int) vUASummary.at(iUAS)->Bin->GetBinContent(iOptim)  ;
    Data     =  vUASummary.at(iUAS)->DCut->IntegralAndError(iBin,vUASummary.at(iUAS)->DCut->GetNbinsX(),eStatData); 

    Proc.clear();

    Signal.clear();
    eStatSignal.clear();
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) {
      Proc.push_back((vUASummary.at(iUAS)->vSName.at(iD)).c_str());
      Double_t EStat;
      Signal.push_back( vUASummary.at(iUAS)->vSCut.at(iD)->IntegralAndError(iBin,vUASummary.at(iUAS)->vSCut.at(iD)->GetNbinsX(),EStat) );
      eStatSignal.push_back(EStat);
    }          

    Background.clear(); 
    eStatBkgd.clear();
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
      Proc.push_back((vUASummary.at(iUAS)->vBName.at(iD)).c_str());
      Double_t EStat;
      Background.push_back( vUASummary.at(iUAS)->vBCut.at(iD)->IntegralAndError(iBin,vUASummary.at(iUAS)->vBCut.at(iD)->GetNbinsX(),EStat) );
      eStatBkgd.push_back(EStat);
    }          
  
    if ( iMethod == 0 ) LimitCardName = "LimitCards/" + CardName + "__MVACut.card" ;  
    if ( iMethod == 1 ) { 
       LimitCardName = "LimitCards/" + CardName + "__MVAShape.card" ;  
       LimitRootName = "LimitCards/" + CardName + "__MVAShape.root" ;  
    }
  
    FILE *cFile; 
    cFile = fopen (LimitCardName.c_str(),"w");
  
    fprintf (cFile,"imax 1 number of channels\n"); 
    fprintf (cFile,"jmax * number of background\n");
    fprintf (cFile,"kmax * number of nuisance parameters\n");
    fprintf (cFile,"Observation %-9.3f \n",Data);
    if ( iMethod == 1 ) {
      fprintf (cFile,"shapes *         * %s histo_$PROCESS \n",LimitRootName.c_str());
      fprintf (cFile,"shapes data_obs  * %s histo_Data\n",LimitRootName.c_str());
    }
    fprintf (cFile,"bin ");
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) fprintf (cFile,"1 ") ; 
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) fprintf (cFile,"1 ") ; 
    fprintf (cFile,"\n") ; 
    fprintf (cFile,"process ") ; 
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) fprintf (cFile,"%s ", (vUASummary.at(iUAS)->vSName.at(iD)).c_str() ) ;
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) fprintf (cFile,"%s ", (vUASummary.at(iUAS)->vBName.at(iD)).c_str() ) ;
    fprintf (cFile,"\n") ;
    fprintf (cFile,"process ");
    for (int iD=1 ; iD <= (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) fprintf (cFile,"%i ", - (signed) vUASummary.at(iUAS)->vSCut.size() + iD );
    for (int iD=1 ; iD <= (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) fprintf (cFile,"%i ",iD);
    fprintf (cFile,"\n") ;
    fprintf (cFile,"rate ") ; 
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) fprintf (cFile,"%-9.3f ",Signal.at(iD)) ;
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) fprintf (cFile,"%-9.3f ",Background.at(iD)) ;
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
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) {
      double eStaRel = 1. ;
      if (Signal.at(iD)>0.) eStaRel += eStatSignal.at(iD) / Signal.at(iD) ;
      string statName = "stat_" + vUASummary.at(iUAS)->vSName.at(iD);  
      string statType = "lnN" ;   
      fprintf (cFile,"%-25s %-5s ",statName.c_str() , statType.c_str() ) ; 
      for ( vector<string>::iterator itProc =  Proc.begin() ; itProc != Proc.end() ; ++itProc) {
        if ( (vUASummary.at(iUAS)->vSName.at(iD)) == (*itProc) ) fprintf (cFile,"%-5.3f ",eStaRel) ;
        else                                                   fprintf (cFile,"  -   "); 
      }
      fprintf (cFile,"\n") ;
    }
  
    for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
      double eStaRel = 1. ;
      if (Background.at(iD)>0.) eStaRel += eStatBkgd.at(iD) / Background.at(iD) ;
      string statName = "stat_" + vUASummary.at(iUAS)->vBName.at(iD);  
      string statType = "lnN" ;   
      fprintf (cFile,"%-25s %-5s ",statName.c_str() , statType.c_str() ) ; 
      for ( vector<string>::iterator itProc =  Proc.begin() ; itProc != Proc.end() ; ++itProc) {
        if ( (vUASummary.at(iUAS)->vBName.at(iD)) == (*itProc) ) fprintf (cFile,"%-5.3f ",eStaRel) ;
        else                                                   fprintf (cFile,"  -   "); 
      }
      fprintf (cFile,"\n") ;
    }
  
    fclose(cFile);
    

    // Prepare shape rootfile
    if ( iMethod == 1 ) {
      MVARebinFac = Cfg.GetTmvaRespRebinFac();
      TFile* fShape = TFile::Open(LimitRootName.c_str(),"RECREATE");
      fShape->cd();
      TH1F* data = (TH1F*) vUASummary.at(iUAS)->DCut->Clone("histo_Data");
      data->Rebin(MVARebinFac);
      data->Write();
      for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vSCut.size() ; ++iD ) {
        string hName = "histo_" + (vUASummary.at(iUAS)->vSName.at(iD));
        TH1F* h = (TH1F*) vUASummary.at(iUAS)->vSCut.at(iD)->Clone(hName.c_str());
        h->Rebin(MVARebinFac);  
        h->Write();
      }
      for (int iD=0 ; iD < (signed) vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
        string hName = "histo_" + (vUASummary.at(iUAS)->vBName.at(iD));
        TH1F* h = (TH1F*) vUASummary.at(iUAS)->vBCut.at(iD)->Clone(hName.c_str());
        h->Rebin(MVARebinFac);  
        h->Write();
      }
      fShape->Close();
      delete fShape;
    }

  }

  return ;
}


