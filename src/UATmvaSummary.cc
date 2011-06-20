
#include "../includes/UATmvaSummary.h"
#include <stdio.h>
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


UATmvaSummary_t::UATmvaSummary_t(TString NameBase, TString MethodName , TString NameExt , UATmvaConfig& Cfg ){
 
  BaseName = NameBase+"_"+MethodName;
  ExtName  = NameExt;
  TmvaName = BaseName+"_"+NameExt;

  // Open File $ fetc objects
  // cout << "[UATmvaSummary_t] Reading File: " << TmvaName << endl;
  TFile*   File     = new TFile("rootfiles/" + TmvaName  + ".root","READ" );

  TH2F* CorrMtxS_ = (TH2F*) File->Get("CorrelationMatrixS");
  TH2F* CorrMtxB_ = (TH2F*) File->Get("CorrelationMatrixB");  

  TH1F* D2Train_  = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/estimatorHistTrain");
  TH1F* D2Test_   = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/estimatorHistTest");

  TH1F* STrain_   = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_Train_S");
  TH1F* BTrain_   = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_Train_B");
  TH1F* STest_    = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_S");
  TH1F* BTest_    = (TH1F*) File->Get("Method_"+MethodName+"/"+TmvaName+"/MVA_"+TmvaName+"_B");

  TH1D* DCut_     = (TH1D*) File->Get("OutputHistograms/Data");
  TH1D* SCut_     = (TH1D*) File->Get("OutputHistograms/Signal");
  TH1D* BCutTr_   = (TH1D*) File->Get("OutputHistograms/BkgdTrain");
  TH1D* BCutAll_  = (TH1D*) File->Get("OutputHistograms/BkgdTot");

  vector <TH1D*>  vBCut_ ;
  if ( Cfg.GetPlotGroup()->size() == 0 ) {
    for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
      if (iD->BkgdData) {
        vBName.push_back(iD->NickName);
        vBCut_.push_back( (TH1D*) File->Get("OutputHistograms/"+iD->NickName) ) ;
      } 
    } 
  } else {
    for ( vector<PlotGroup_t>::iterator iG = (Cfg.GetPlotGroup())->begin() ; iG != (Cfg.GetPlotGroup())->end() ; ++iG) {
      vBName.push_back(iG->PlotGroupName) ;
      TH1D* iGHist ;
      for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        if ( iG->PlotGroupMember.at(0) == iD->NickName ) iGHist = (TH1D*) ((TH1D*) File->Get("OutputHistograms/"+iD->NickName))->Clone()  ;
      }
      for ( int iGM=1 ; iGM < iG->PlotGroupMember.size() ; ++iGM ) {
        for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
          if ( iG->PlotGroupMember.at(iGM) == iD->NickName ) iGHist->Add ((TH1D*) File->Get("OutputHistograms/"+iD->NickName));
        }
      }
      vBCut_.push_back( (TH1D*) iGHist->Clone() ) ;
      delete iGHist ; 
    }
  }

  TH1D* SignCutTr_   = (TH1D*) File->Get("OutputHistograms/bgTr_SoverSqrtBPlusDeltaB");
  TH1D* SignCutAll_  = (TH1D*) File->Get("OutputHistograms/bkgd_SoverSqrtBPlusDeltaB");
  TH1D* LimitCutTr_  = (TH1D*) File->Get("OutputHistograms/bgTr_BayesLimit");
  TH1D* LimitCutAll_ = (TH1D*) File->Get("OutputHistograms/bkgd_BayesLimit");

  TH1D* Cut_      = (TH1D*) File->Get("OutputHistograms/Cut");   
  TH1D* Sign_     = (TH1D*) File->Get("OutputHistograms/Sign");
  TH1D* Limit_    = (TH1D*) File->Get("OutputHistograms/Limit");

  TH1D* CutBased_ = (TH1D*) File->Get("OutputHistograms/CutBased");

  // Have to create the new object outside of gDirectory from File
  gROOT->cd(); 

  CorrMtxS = (TH2F*) CorrMtxS_ ->Clone();
  CorrMtxB = (TH2F*) CorrMtxB_ ->Clone();

  D2Train  = (TH1F*) D2Train_ ->Clone();
  D2Test   = (TH1F*) D2Test_  ->Clone();

  STrain   = (TH1F*) STrain_ ->Clone();
  BTrain   = (TH1F*) BTrain_ ->Clone();
  STest    = (TH1F*) STest_  ->Clone();
  BTest    = (TH1F*) BTest_  ->Clone();

  DCut     = (TH1D*) DCut_    ->Clone() ;
  SCut     = (TH1D*) SCut_    ->Clone() ;
  BCutTr   = (TH1D*) BCutTr_  ->Clone() ;
  BCutAll  = (TH1D*) BCutAll_ ->Clone();
  for ( int iD = 0 ; iD < vBCut_.size() ; ++iD ) vBCut.push_back( (TH1D*) vBCut_.at(iD)->Clone() ) ;

  SignCutTr   = (TH1D*) SignCutTr_   ->Clone() ;
  SignCutAll  = (TH1D*) SignCutAll_  ->Clone() ;
  LimitCutTr  = (TH1D*) LimitCutTr_  ->Clone() ;
  LimitCutAll = (TH1D*) LimitCutAll_ ->Clone() ;

  Cut      = (TH1D*) Cut_    ->Clone();
  Sign     = (TH1D*) Sign_   ->Clone();
  Limit    = (TH1D*) Limit_  ->Clone();

  CutBased = (TH1D*) CutBased_  ->Clone();

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
  for ( int iD = 0 ; iD < vBCut.size() ; ++iD ) SetGoodAxis(vBCut.at(iD));

  SetGoodAxis(SignCutTr)  ;
  SetGoodAxis(SignCutAll) ;
  SetGoodAxis(LimitCutTr) ;
  SetGoodAxis(LimitCutAll); 

  SetGoodAxis(Cut);
  SetGoodAxis(Sign);
  SetGoodAxis(Limit);

  SetGoodAxis(CutBased);

  // Delete tmp objects

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
  for ( int iD = 0 ; iD < vBCut.size() ; ++iD ) delete vBCut_.at(iD);
  vBCut_.clear();

  delete SignCutTr_   ;
  delete SignCutAll_  ;
  delete LimitCutTr_  ;
  delete LimitCutAll_ ;

  delete Cut_    ;
  delete Sign_   ; 
  delete Limit_  ;

  delete CutBased_ ;

  // Close File
  File->Close();
  delete File;
  
}


UATmvaSummary_t::~UATmvaSummary_t(){

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
  for ( int iD = 0 ; iD < vBCut.size() ; ++iD ) delete vBCut.at(iD);
  vBCut.clear();
  vBName.clear();


  delete SignCutTr;
  delete SignCutAll;
  delete LimitCutTr;
  delete LimitCutAll;

  delete Cut;    
  delete Sign;
  delete Limit;
}



// ---------------------------- UATmvaSummary()

UATmvaSummary::~UATmvaSummary(){
  //cout << "UATmvaSummary Destructor" << endl;
  int iC = 1;
  for ( vector<UATmvaSummary_t*>::iterator itC = vUASummary.begin() ; itC != vUASummary.end() ; ++itC ){
     //cout << "UATmvaSummary_t Delete # : " << iC++ << endl;
     delete (*itC);
   }
   vUASummary.clear();
}

// ---------------------------- Init()

void UATmvaSummary::Init( UATmvaConfig& Cfg ) {

  TString MethodName("MLP");

  // Open All files and Load Histos
  for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
  Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;
  for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
  for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {

     // Build Name
     ostringstream Name;
     Name << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;

     vUASummary.push_back (new UATmvaSummary_t(Cfg.GetTmvaName(),MethodName,Name.str(), Cfg ));

  } // Nodes
  } // Layers
  } // Variables
} 


void UATmvaSummary::Print( ){

  int iSign = 4 ;
  int iLim  = 7 ;

  cout << endl;
  cout << "  -------> BaseName = " << vUASummary.at(0)->BaseName << endl; 
  cout << "  -------> Cut Based S/Sqrt(S+B)  = " << vUASummary.at(0)->CutBased->GetBinContent(4) << endl; 
  cout << "  -------> Cut Based S/Sqrt(B+dB) = " << vUASummary.at(0)->CutBased->GetBinContent(5) << endl; 
  cout << "  -------> Cut Based Limit        = " << vUASummary.at(0)->CutBased->GetBinContent(6) << endl; 
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

  
  for ( int iUAS = 0 ; iUAS !=  vUASummary.size() ; ++iUAS ) {

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

 
}


//-------------------------------- Plots()


void UATmvaSummary::Plots( ){

  Int_t ID = -1 ;
  while ( ID != 0 ) {
    Print();
    cout << "  Enter ID of MVA to plot (0 to exit): ";
    cin  >> ID; 
    if ( ID > 0 && ID <=  vUASummary.size() ) {
      cout << "  --> Plotting: " << vUASummary.at(ID-1)->TmvaName << endl ;
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
      PlotStack(ID-1);
      //gPad->WaitPrimitive();
      Canvas->SaveAs("plots/mvasummary_"+vUASummary.at(ID-1)->TmvaName+".eps");
      Canvas->SaveAs("plots/mvasummaty_"+vUASummary.at(ID-1)->TmvaName+".png");

      TCanvas* CanvasStack = new TCanvas(vUASummary.at(ID-1)->TmvaName+"_Stack",vUASummary.at(ID-1)->TmvaName,500,500);
      CanvasStack->cd();
      PlotStack(ID-1);
      CanvasStack->SaveAs("plots/mvastack_"+vUASummary.at(ID-1)->TmvaName+".eps");
      CanvasStack->SaveAs("plots/mvastack_"+vUASummary.at(ID-1)->TmvaName+".png");


    } else if ( ID != 0 ) {
      cout << "  --> Invalid ID !!!!!!!!!!! " << endl ;
    }
  }

}



//-------------------------------- PlotStack()

void UATmvaSummary::PlotStack(int iUAS ){

   gPad->SetRightMargin(0.02);
   gPad->SetLeftMargin(0.15);
   gPad->SetLogy(1);

   vector<TH1D*> vStack;
   for (int iD=0 ; iD < vUASummary.at(iUAS)->vBCut.size() ; ++iD ) {
     cout << vUASummary.at(iUAS)->vBName.at(iD) << " = " ;
     TH1D* iStack = (TH1D*) vUASummary.at(iUAS)->vBCut.at(iD)->Clone("s"+vUASummary.at(iUAS)->vBName.at(iD));
     for (int iD2Sum=iD+1 ; iD2Sum <  vUASummary.at(iUAS)->vBCut.size() ; ++iD2Sum ) {
        cout << vUASummary.at(iUAS)->vBName.at(iD2Sum) << " + " ;
       iStack->Add(vUASummary.at(iUAS)->vBCut.at(iD2Sum));
     }
     cout << endl;
     iStack->SetLineColor(iD+2);
     iStack->SetFillColor(iD+2);
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

   vStack.at(0)->Draw("hist"); 
   for (int iD=1 ; iD < vStack.size()  ; ++iD ) vStack.at(iD)->Draw("histsame");
   vUASummary.at(iUAS)->SCut->Draw("histsame");
   vUASummary.at(iUAS)->DCut->Draw("esame");

   TLegend* Legend = new TLegend (.18,.45,.5,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->DCut   , "Data  " , "p");
   Legend->AddEntry( vUASummary.at(iUAS)->SCut   , "Signal" , "l");
   for (int iD=0 ; iD < vStack.size()  ; ++iD ) Legend->AddEntry( vStack.at(iD) , vUASummary.at(iUAS)->vBName.at(iD) , "f");
   Legend->Draw("same");



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
   CorrMtx->Draw("col");
   CorrMtx->Draw("textsame"); 
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

   vUASummary.at(iUAS)->D2Train->Draw();
   vUASummary.at(iUAS)->D2Test->Draw("same");

   TLegend* Legend = new TLegend (.5,.75,.8,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
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

   vUASummary.at(iUAS)->STest ->Draw("hist");      
   vUASummary.at(iUAS)->BTest ->Draw("histsame"); 
   vUASummary.at(iUAS)->STrain->Draw("esame");      
   vUASummary.at(iUAS)->BTrain->Draw("esame");

   TLegend* Legend = new TLegend (.5,.65,.8,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
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

   vUASummary.at(iUAS)->SignCutAll ->Draw(); 
   vUASummary.at(iUAS)->LimitCutAll->Draw("same");

   TLegend* Legend = new TLegend (.2,.75,.5,.85);
   Legend->SetBorderSize(0);
   Legend->SetFillColor(0);
   Legend->SetTextSize(0.04);
   Legend->AddEntry( vUASummary.at(iUAS)->SignCutAll  , "Significance" , "l" );
   Legend->AddEntry( vUASummary.at(iUAS)->LimitCutAll , "Limit" , "l" );

   Legend->Draw("same");  
 

    
}

