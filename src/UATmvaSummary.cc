
#include "../includes/UATmvaSummary.h"


UATmvaSummary_t::UATmvaSummary_t(){
  TmvaName = "UNDEF";
  Cut      = new TH1D(); 
}


UATmvaSummary_t::UATmvaSummary_t(TString Name){
  TmvaName = Name;
  cout << "[UATmvaSummary_t] Reading File: " << TmvaName << endl;
  TFile*   File     = new TFile("rootfiles/" + TmvaName  + ".root","READ" );
  TH1D* Cut_ = (TH1D*) File->Get("OutputHistograms/Cut");   
  cout << Cut_ << endl;
  gROOT->cd(); // Have to create the new object Curve_ outside of gDirectory from File_

  Cut      = (TH1D*) Cut_->Clone();
  
  delete Cut_;

  File->Close();
  delete File;
  
}


UATmvaSummary_t::~UATmvaSummary_t(){
  delete Cut;    
}



// ---------------------------- UATmvaSummary()

UATmvaSummary::~UATmvaSummary(){
  cout << "UATmvaSummary Destructor" << endl;
  int iC = 1;
  for ( vector<UATmvaSummary_t*>::iterator itC = vUASummary.begin() ; itC != vUASummary.end() ; ++itC ){
     cout << "UATmvaSummary_t Delete # : " << iC++ << endl;
     delete (*itC);
   }
   vUASummary.clear();
}

// ---------------------------- Init()

void UATmvaSummary::Do( UATmvaConfig& Cfg ) {

  // Open All files and Load Histos
  for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove() ; ++nVarRem) {
  Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ;
  for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
  for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {

     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;

     vUASummary.push_back (new UATmvaSummary_t(Name.str()));


/*
     UASummary.TmvaName = Name.str();
     UASummary.TmvaFile = TFile::Open("rootfiles/" + UASummary.TmvaName  + ".root","READ" );
     //gROOT->cd();

     TH1D* Cut   = (TH1D*) (UASummary.TmvaFile)->Get("OutputHistograms/Cut");
     
     gROOT->cd();
     UASummary.Cut = (TH1D*) Cut->Clone(); 

     UASummary.Cut   = (TH1D*) ((UASummary.TmvaFile)->Get("OutputHistograms/Cut"))->Clone();
     UASummary.Sign  = (TH1D*) ((UASummary.TmvaFile)->Get("OutputHistograms/Sign"))->Clone(); 
     UASummary.Limit = (TH1D*) ((UASummary.TmvaFile)->Get("OutputHistograms/Limit"))->Clone();



     int iMethod = 9;
     cout << " " << nHLayers << " " << nHNodes << " " << nVarMax << " "
                 << UASummary.Cut  ->GetBinContent(iMethod) << " " 
                 << UASummary.Sign ->GetBinContent(iMethod) << " "
                 << UASummary.Limit->GetBinContent(iMethod) << endl;  


     //UASummary->Cut  ->Draw();

     //delete UASummary;
*/

  } // Nodes
  } // Layers
  } // Variables



}
