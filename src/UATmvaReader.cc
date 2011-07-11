

#include "../includes/UATmvaReader.h"

#include <iostream>
#include <TH1D.h>
#include <TTreeFormula.h>


// ---------------------------- Init()

void UATmvaReader::Do( UATmvaConfig& Cfg, UATmvaTree& T) {

   if ( Cfg.GetTmvaType() == "ANN" ) DoMLP(Cfg,T);
   if ( Cfg.GetTmvaType() == "BDT" ) DoBDT(Cfg,T);

}

void UATmvaReader::DoMLP( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 
   for (Int_t nHLayers = Cfg.GetANNHiddenLayersMin() ; nHLayers <= Cfg.GetANNHiddenLayersMax() ; ++nHLayers ) {
   for (Int_t nHNodes  = Cfg.GetANNHiddenNodesMin()  ; nHNodes  <= Cfg.GetANNHiddenNodesMax()  ; ++nHNodes  ) {


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_MLP_" << nHLayers << "Layers_" << nHNodes << "Nodes_" << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;
 
   } // Nodes
   } // Layers
   } // Variables
   } // TargetLumi

}


void UATmvaReader::DoBDT( UATmvaConfig& Cfg, UATmvaTree& T) {

   for (Int_t iLumi    = 0 ; iLumi   < (signed) Cfg.GetTargetLumi()->size() ; ++iLumi  ) {
   for (Int_t nVarRem  = 0 ; nVarRem <= Cfg.GetANNVarNumRemove()   ; ++nVarRem) {
   Int_t nVarMax = (Cfg.GetTmvaVar())->size() - nVarRem ; 

   for( int iBDTNTrees         = 0 ; iBDTNTrees         < (signed) (Cfg.GetBDTNTrees())->size()         ; ++iBDTNTrees         ) {
   for( int iBDTBoostType      = 0 ; iBDTBoostType      < (signed) (Cfg.GetBDTBoostType())->size()      ; ++iBDTBoostType      ) {
   for( int iBDTSeparationType = 0 ; iBDTSeparationType < (signed) (Cfg.GetBDTSeparationType())->size() ; ++iBDTSeparationType ) {
   for( int iBDTnCuts          = 0 ; iBDTnCuts          < (signed) (Cfg.GetBDTnCuts())->size()          ; ++iBDTnCuts          ) {
   for( int iBDTPruneMethod    = 0 ; iBDTPruneMethod    < (signed) (Cfg.GetBDTPruneMethod())->size()    ; ++iBDTPruneMethod    ) {
   for( int iBDTPruneStrength  = 0 ; iBDTPruneStrength  < (signed) (Cfg.GetBDTPruneStrength())->size()  ; ++iBDTPruneStrength  ) {


     // Build Name
     ostringstream Name;
     Name << Cfg.GetTmvaName() << "_BDT_" ;
     Name << Cfg.GetBDTNTrees()->at(iBDTNTrees) << "Trees_" ;
     Name << Cfg.GetBDTBoostType()->at(iBDTBoostType) << "_" ;
     Name << Cfg.GetBDTSeparationType()->at(iBDTSeparationType) << "_" ;
     Name << Cfg.GetBDTnCuts()->at(iBDTnCuts) << "Cuts_" ;
     Name << Cfg.GetBDTPruneMethod()->at(iBDTPruneMethod) << "_" ;
     Name << Cfg.GetBDTPruneStrength()->at(iBDTPruneStrength) << "PruneStrength_" ;
     Name << nVarMax << "Var" ;
     NAME =  Name.str();

     Read(Cfg,T,Name.str(),nVarMax,iLumi) ;

   } // BDTNTrees
   } // BDTBoostType
   } // BDTSeparationType
   } // BDTnCuts
   } // BDTPruneMethod
   } // BDTPruneStrength
 
   } // Variables
   } // TargetLumi

}


void UATmvaReader::Read( UATmvaConfig& Cfg, UATmvaTree& T, string Name, int nVarMax, int iLumi ) {

     TH1::SetDefaultSumw2(1);

     // Basic Reader Settings -> MVA Out Binning

     nbins  = Cfg.GetTmvaRespNBins() ; 
     minBin = Cfg.GetTmvaRespXMin() ;
     maxBin = Cfg.GetTmvaRespXMax() ;

     // 

     UAReader = new UATmvaReader_t() ; 
 
     cout << "[UATmvaReader] Reading: " << Name << endl;
     UAReader->TmvaName = Name;

     // Load InputTrees branches
     Double_t treeWeight=1.;
     Int_t    IVar[nVarMax];
     Float_t  FVar[nVarMax];
     Double_t DVar[nVarMax];

     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
        (T.GetTree(iD->NickName))->SetBranchStatus("*",0);
        (T.GetTree(iD->NickName))->SetBranchStatus("*",1); // Enable all branch for now (helps for definint Presel cut anyway)
        if ( Cfg.GetTmvaWeight() != "" ) {
          (T.GetTree(iD->NickName))->SetBranchStatus(Cfg.GetTmvaWeight(),1);
          (T.GetTree(iD->NickName))->SetBranchAddress(Cfg.GetTmvaWeight(),&treeWeight);
        }
        Int_t nVar = 0;
        for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
          if (++nVar <= nVarMax) {
            (T.GetTree(iD->NickName))->SetBranchStatus(iVar->VarName,1);
            if ( iVar->VarType == 'I' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&IVar[nVar]);
            if ( iVar->VarType == 'F' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&FVar[nVar]);
            if ( iVar->VarType == 'D' ) (T.GetTree(iD->NickName))->SetBranchAddress(iVar->VarName,&DVar[nVar]);
          } 
        } 
     }



     // Create TMVA Reader
     cout << "[UATmvaReader::DoMLP()] Create: " << UAReader->TmvaName << endl;
     UAReader->TmvaReader = new Reader( "!Color:!Silent" );

     // Add VariablesCfg.GetTmvaVar())
     Int_t nVar = 0;     
     for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
       if (++nVar <= nVarMax) {
         if ( iVar->VarType == 'I' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'F' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
         if ( iVar->VarType == 'D' )  UAReader->TmvaReader->AddVariable(iVar->VarName,&FVar[nVar]);
       }
     }

     // Fetch TMVA weights
     TString TmvaWeightFile = "weights/"+UAReader->TmvaName+"_"+UAReader->TmvaName+".weights.xml" ;
     
     // BookMethod
     UAReader->TmvaReader->BookMVA( UAReader->TmvaName , TmvaWeightFile ) ;

     // Re-Open TMVA output file
     UAReader->TmvaFile  = TFile::Open("rootfiles/" + UAReader->TmvaName  + ".root","UPDATE" );
     //string Directory("OutputHistograms");
     ostringstream Directory;
     Directory << "OutputHistograms_" << Cfg.GetTargetLumi()->at(iLumi).Lumi << "pbinv" ;
     UAReader->TmvaFile->mkdir(Directory.str().c_str()); 
     UAReader->TmvaFile->cd(Directory.str().c_str());    


     // Fill Output for each tree
     cout << "[UATmvaReader::DoMLP()] Fill: " << UAReader->TmvaName << endl;

     TH1D* hMVA_data = new TH1D ("Data"     ,"Data"     ,nbins,minBin,maxBin) ;
     TH1D* hMVA_sig  = new TH1D ("Signal"   ,"Signal"   ,nbins,minBin,maxBin) ;
     TH1D* hMVA_bkgd = new TH1D ("BkgdTot"  ,"BkgdTot"  ,nbins,minBin,maxBin) ;
     TH1D* hMVA_bgTr = new TH1D ("BkgdTrain","BkgdTrain",nbins,minBin,maxBin) ;
     TH1D* hMVA_bgSp = new TH1D ("BkgdSpect","BkgdSpect",nbins,minBin,maxBin) ;

     for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
       Double_t DaWeight=1.0; 
       Double_t SgWeight=1.0;
       Double_t BgWeight=1.0;
       Double_t Weight=1.0;
       // Set TargetLumi
       Double_t LumiScale = 1.;
       LumiScale = Cfg.GetTargetLumi()->at(iLumi).Lumi / iD->Lumi ;
       cout << iD->NickName << " : SampleLumi= " << iD->Lumi<< " TargetLumi= " << Cfg.GetTargetLumi()->at(iLumi).Lumi << " --> LumiScale = " << LumiScale << endl;
       if ( Cfg.GetTargetLumi()->at(iLumi).useData ) {  
         if ( iD->TrueData &&  LumiScale != 1. ) cout << "[UATmvaReade::DoMLP] WARNING: Rescaling Data Luminosity" << endl; 
         DaWeight *= LumiScale;
       }
       else DaWeight = 0;
       SgWeight *= LumiScale;
       BgWeight *= LumiScale;
       cout << "------> DaWeight= " << DaWeight << " SgWeight= " << SgWeight << " BgWeight= " << BgWeight << endl;
       // File MVA Response
       TH1D* hMVA = new TH1D (iD->NickName,iD->NickName,nbins,minBin,maxBin) ;
       vector<TH1F*> hCtrl ;
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) {
         TString HistName = iD->NickName+"_"+Cfg.GetCtrlPlot()->at(iP).VarName ;
         hCtrl.push_back( new TH1F (HistName,HistName,Cfg.GetCtrlPlot()->at(iP).nBins,Cfg.GetCtrlPlot()->at(iP).xMin,Cfg.GetCtrlPlot()->at(iP).xMax) );
       } 
       Int_t nEntries = (T.GetTree(iD->NickName))->GetEntries(); 
       // Recreate Tmva Preselection Cut
       TTreeFormula* Presel = 0 ; 
       if ( Cfg.GetTmvaPreCut() != "" ) {
         // define cut
         Presel = new TTreeFormula("Presel",(Cfg.GetTmvaPreCut()).c_str(),T.GetTree(iD->NickName));
         // Enable needed branches 
         for (Int_t bi = 0; bi<Presel->GetNcodes(); bi++) {
            T.GetTree(iD->NickName)->SetBranchStatus( Presel->GetLeaf(bi)->GetBranch()->GetName(), 1 );
         }
       }
       // Tree Loop
       for (Int_t jEntry = 0 ;  jEntry < nEntries ; ++jEntry) {
         (T.GetTree(iD->NickName))->GetEntry(jEntry);
         // Apply Tmva Preselection Cut
         if ( Presel && !(Presel->EvalInstance()) ) continue; 
         // ... convert everything to Float_t
         nVar = 0;     
         for (vector<InputVar_t>::iterator iVar = (Cfg.GetTmvaVar())->begin() ; iVar != (Cfg.GetTmvaVar())->end() ; ++iVar){
           if (++nVar <= nVarMax) {
             if ( iVar->VarType == 'I' ) FVar[nVar] = IVar[nVar] ; 
             if ( iVar->VarType == 'D' ) FVar[nVar] = DVar[nVar] ;  
           }
         }
         // ... Evaluate
         Weight = treeWeight ; 
         if (iD->SigTrain               ) Weight *= SgWeight;
         if (iD->BkgdData||iD->BkgdTrain) Weight *= BgWeight;
         if (iD->TrueData               ) Weight *= DaWeight;
         Double_t result = UAReader->TmvaReader->EvaluateMVA(UAReader->TmvaName); 
//         cout << result << " " << Weight << endl;
         hMVA -> Fill( result , Weight);
         if (iD->TrueData                ) hMVA_data -> Fill( result , Weight);
         if (iD->SigTrain                ) hMVA_sig  -> Fill( result , Weight);
         if (iD->BkgdData||iD->BkgdTrain ) hMVA_bkgd -> Fill( result , Weight);
         if (iD->BkgdData&&!iD->BkgdTrain) hMVA_bgSp -> Fill( result , Weight);
         if (iD->BkgdTrain               ) hMVA_bgTr -> Fill( result , Weight);
         // Var Control Plots 
         for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) hCtrl.at(iP)->Fill( FVar[((Cfg.GetCtrlPlot())->at(iP)).iVarPos+1] , Weight);
       }
       UAReader->TmvaFile->cd(Directory.str().c_str());
       hMVA->Write();
       for ( int iP = 0 ; iP < (signed) Cfg.GetCtrlPlot()->size() ; ++iP ) { hCtrl.at(iP)->Write(); delete hCtrl.at(iP) ; } // hCtrl.clear() ; }
       delete hMVA; 
     }     

     //UAReader->TmvaFile->cd(Directory.str().c_str());
     hMVA_data ->Write();
     hMVA_sig  ->Write();
     hMVA_bkgd ->Write(); 
     hMVA_bgSp ->Write(); 
     hMVA_bgTr ->Write(); 


     // --------------- Cut Based Reference --------------------------

     Double_t CutBased_Data                 = 0.;
     Double_t CutBased_Signal               = 0.;
     Double_t CutBased_Bkgd                 = 0.;
     Double_t CutBased_Limit                = 100. ;
     Double_t CutBased_SoverSqrtSPlusB      = 0. ;
     Double_t CutBased_SoverSqrtBPlusDeltaB = 0. ;


     if ( Cfg.GetCutBasedHistName() != "NULL" ) {
       // Retrive yields
       for ( vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {

         Double_t DaWeight=1.0; 
         Double_t SgWeight=1.0;
         Double_t BgWeight=1.0;
         // Set TargetLumi
         Double_t LumiScale = 1.;
         LumiScale = Cfg.GetTargetLumi()->at(iLumi).Lumi / iD->Lumi ;
         cout << iD->NickName << " : SampleLumi= " << iD->Lumi<< " TargetLumi= " << Cfg.GetTargetLumi()->at(iLumi).Lumi << " --> LumiScale = " << LumiScale << endl;
         if ( Cfg.GetTargetLumi()->at(iLumi).useData ) {  
           if ( iD->TrueData &&  LumiScale != 1. ) cout << "[UATmvaReade::DoMLP] WARNING: Rescaling Data Luminosity" << endl; 
           DaWeight *= LumiScale;
         }
         else DaWeight = 0;
         SgWeight *= LumiScale;
         BgWeight *= LumiScale;
         cout << "------> DaWeight= " << DaWeight << " SgWeight= " << SgWeight << " BgWeight= " << BgWeight << endl;

         TFile* File = new TFile(iD->FileName,"READ" );
         if (iD->TrueData                ) CutBased_Data   += DaWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         if (iD->SigTrain                ) CutBased_Signal += SgWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         if (iD->BkgdData||iD->BkgdTrain ) CutBased_Bkgd   += BgWeight * ( (TH1*) File->Get(Cfg.GetCutBasedHistName()) ) -> GetBinContent(Cfg.GetCutBasedHistBin()) ;
         File->Close();
         delete File;
       }
       // Compute Significance and Limit
       CutBased_SoverSqrtSPlusB       = CutBased_Signal/sqrt(CutBased_Signal+CutBased_Bkgd);
       CutBased_SoverSqrtBPlusDeltaB  = CutBased_Signal/sqrt(CutBased_Bkgd+pow(0.35*CutBased_Bkgd,2)) ;
       init();
       CutBased_Limit = limitBayesian(CutBased_Bkgd,.35,CutBased_Signal,.2);
       delete wRoo;
     }

     TH1D* CutBased = new TH1D("CutBased","CutBased",6,0.5,6.5);
     CutBased->SetBinContent(1,CutBased_Data );
     CutBased->SetBinContent(2,CutBased_Signal );
     CutBased->SetBinContent(3,CutBased_Bkgd );
     CutBased->SetBinContent(4,CutBased_SoverSqrtSPlusB  );
     CutBased->SetBinContent(5,CutBased_SoverSqrtBPlusDeltaB );
     CutBased->SetBinContent(6,CutBased_Limit );
     UAReader->TmvaFile->cd();
     UAReader->TmvaFile->cd(Directory.str().c_str());
     CutBased->Write();


     // Build Significance
     cout << "[UATmvaReader::DoMLP()] Evaluate: " << UAReader->TmvaName << endl;

     TH1D* bgTr_SoverB = GetSoverB("bgTr_SoverB",hMVA_sig ,hMVA_bgTr); bgTr_SoverB->Write();
     TH1D* bgSp_SoverB = GetSoverB("bgSp_SoverB",hMVA_sig ,hMVA_bgSp); bgSp_SoverB->Write();
     TH1D* bkgd_SoverB = GetSoverB("bkgd_SoverB",hMVA_sig ,hMVA_bkgd); bkgd_SoverB->Write();
 
     TH1D* bgTr_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bgTr_SoverSqrtSPlusB",hMVA_sig ,hMVA_bgTr); bgTr_SoverSqrtSPlusB->Write();
     TH1D* bgSp_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bgSp_SoverSqrtSPlusB",hMVA_sig ,hMVA_bgSp); bgSp_SoverSqrtSPlusB->Write();
     TH1D* bkgd_SoverSqrtSPlusB = GetSoverSqrtSPlusB("bkgd_SoverSqrtSPlusB",hMVA_sig ,hMVA_bkgd); bkgd_SoverSqrtSPlusB->Write();
     
     TH1D* bgTr_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bgTr_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bgTr); bgTr_SoverSqrtBPlusDeltaB->Write();
     TH1D* bgSp_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bgSp_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bgSp); bgSp_SoverSqrtBPlusDeltaB->Write();
     TH1D* bkgd_SoverSqrtBPlusDeltaB = GetSoverSqrtBPlusDeltaB("bkgd_SoverSqrtBPlusDeltaB",hMVA_sig ,hMVA_bkgd); bkgd_SoverSqrtBPlusDeltaB->Write();

     // Find Optimal Cut for Significance
     Double_t Cut_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB);
     Double_t Cut_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB);
     Double_t Cut_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB);

     Double_t Cut_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB);
     Double_t Cut_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB);
     Double_t Cut_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB);

     Double_t Sign_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB,1);
     Double_t Sign_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB,1);
     Double_t Sign_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB,1);

     Double_t Sign_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB,1);
     Double_t Sign_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB,1);
     Double_t Sign_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB,1);

     Double_t Bin_bgTr_SoverSqrtSPlusB = OptimalCutHigh(bgTr_SoverSqrtSPlusB,2);
     Double_t Bin_bgSp_SoverSqrtSPlusB = OptimalCutHigh(bgSp_SoverSqrtSPlusB,2);
     Double_t Bin_bkgd_SoverSqrtSPlusB = OptimalCutHigh(bkgd_SoverSqrtSPlusB,2);

     Double_t Bin_bgTr_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgTr_SoverSqrtBPlusDeltaB,2);
     Double_t Bin_bgSp_SoverSqrtBPlusDeltaB = OptimalCutHigh(bgSp_SoverSqrtBPlusDeltaB,2);
     Double_t Bin_bkgd_SoverSqrtBPlusDeltaB = OptimalCutHigh(bkgd_SoverSqrtBPlusDeltaB,2);

     // Get Limits
     TH1D* bgTr_BayesLimit = GetExclusionLimit("bgTr_BayesLimit",hMVA_sig ,hMVA_bgTr); bgTr_BayesLimit->Write();
     TH1D* bgSp_BayesLimit = GetExclusionLimit("bgSp_BayesLimit",hMVA_sig ,hMVA_bgSp); bgSp_BayesLimit->Write();
     TH1D* bkgd_BayesLimit = GetExclusionLimit("bkgd_BayesLimit",hMVA_sig ,hMVA_bkgd); bkgd_BayesLimit->Write();
     //TH1D* bgTr_BayesLimit = (TH1D*) bkgd_BayesLimit->Clone("bgTr_BayesLimit");
     //TH1D* bgSp_BayesLimit = (TH1D*) bkgd_BayesLimit->Clone("bgSp_BayesLimit");

     // Find Optimal Cut for Limit

     Double_t Cut_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit);
     Double_t Cut_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit);
     Double_t Cut_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit);

     Double_t Sign_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit,1);
     Double_t Sign_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit,1);
     Double_t Sign_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit,1);

     Double_t Bin_bgTr_BayesLimit = OptimalCutLow(bgTr_BayesLimit,2);
     Double_t Bin_bgSp_BayesLimit = OptimalCutLow(bgSp_BayesLimit,2);
     Double_t Bin_bkgd_BayesLimit = OptimalCutLow(bkgd_BayesLimit,2);

     // Find Cut matching cut base analysis

     Double_t Cut_CutBased  = FindCutBased(hMVA_sig,  CutBased_Signal );
     Double_t Sign_CutBased = FindCutBased(hMVA_sig,  CutBased_Signal,1 );
     Double_t Bin_CutBased  = FindCutBased(hMVA_sig,  CutBased_Signal,2 );

     // Keep Cuts

     TH1D* Cut = new TH1D("Cut","Cut",10,0.5,10.5);
     Cut->SetBinContent(1,Cut_bgTr_SoverSqrtSPlusB);
     Cut->SetBinContent(2,Cut_bgSp_SoverSqrtSPlusB);
     Cut->SetBinContent(3,Cut_bkgd_SoverSqrtSPlusB);
     Cut->SetBinContent(4,Cut_bgTr_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(5,Cut_bgSp_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(6,Cut_bkgd_SoverSqrtBPlusDeltaB);
     Cut->SetBinContent(7,Cut_bgTr_BayesLimit);
     Cut->SetBinContent(8,Cut_bgSp_BayesLimit);
     Cut->SetBinContent(9,Cut_bkgd_BayesLimit);
     Cut->SetBinContent(10,Cut_CutBased);
     Cut->Write();

     TH1D* Bin = new TH1D("Bin","Bin",10,0.5,10.5);
     Bin->SetBinContent(1,Bin_bgTr_SoverSqrtSPlusB);
     Bin->SetBinContent(2,Bin_bgSp_SoverSqrtSPlusB);
     Bin->SetBinContent(3,Bin_bkgd_SoverSqrtSPlusB);
     Bin->SetBinContent(4,Bin_bgTr_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(5,Bin_bgSp_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(6,Bin_bkgd_SoverSqrtBPlusDeltaB);
     Bin->SetBinContent(7,Bin_bgTr_BayesLimit);
     Bin->SetBinContent(8,Bin_bgSp_BayesLimit);
     Bin->SetBinContent(9,Bin_bkgd_BayesLimit);
     Bin->SetBinContent(10,Bin_CutBased);
     Bin->Write();

     // Keep Cut Discri values
     TH1D* Sign = new TH1D("Sign","Sign",9,0.5,9.5);
     Sign->SetBinContent(1,Sign_bgTr_SoverSqrtSPlusB);
     Sign->SetBinContent(2,Sign_bgSp_SoverSqrtSPlusB);
     Sign->SetBinContent(3,Sign_bkgd_SoverSqrtSPlusB);
     Sign->SetBinContent(4,Sign_bgTr_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(5,Sign_bgSp_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(6,Sign_bkgd_SoverSqrtBPlusDeltaB);
     Sign->SetBinContent(7,Sign_bgTr_BayesLimit);
     Sign->SetBinContent(8,Sign_bgSp_BayesLimit);
     Sign->SetBinContent(9,Sign_bkgd_BayesLimit);
     Sign->SetBinContent(10,Sign_CutBased);
     Sign->Write();

     // Keep Cut Limit values
     
     TH1D* Limit = new TH1D("Limit","Limit",10,0.5,10.5);
     for (Int_t iBin = 1 ; iBin < 9 ; ) {
      // Training Only
      Double_t S = hMVA_sig->Integral(Bin->GetBinContent(iBin),hMVA_sig->GetNbinsX());
      Double_t B = hMVA_bgTr->Integral(Bin->GetBinContent(iBin),hMVA_bgTr->GetNbinsX());
      init();
      Limit->SetBinContent(iBin,limitBayesian(B,.35,S,.2));
      delete wRoo;
      // Spectator Only
      S = hMVA_sig->Integral(Bin->GetBinContent(iBin+1),hMVA_sig->GetNbinsX());
      B = hMVA_bgSp->Integral(Bin->GetBinContent(iBin+1),hMVA_bgSp->GetNbinsX());
      init();
      Limit->SetBinContent(iBin+1,limitBayesian(B,.35,S,.2));
      delete wRoo;
      // All
      S = hMVA_sig->Integral(Bin->GetBinContent(iBin+2),hMVA_sig->GetNbinsX());
      B = hMVA_bkgd->Integral(Bin->GetBinContent(iBin+2),hMVA_bkgd->GetNbinsX());
      init();
      Limit->SetBinContent(iBin+2,limitBayesian(B,.35,S,.2));
      delete wRoo;
      iBin+=3;
     }
     Double_t S = hMVA_sig->Integral(Bin->GetBinContent(10),hMVA_sig->GetNbinsX());
     Double_t B = hMVA_bkgd->Integral(Bin->GetBinContent(10),hMVA_bkgd->GetNbinsX());
     init();
     Limit->SetBinContent(10,limitBayesian(B,.35,S,.2));
     delete wRoo;

     Limit->Write();


     // clean histo's 
     delete hMVA_data;
     delete hMVA_sig;
     delete hMVA_bkgd;
     delete hMVA_bgSp;
     delete hMVA_bgTr;

     delete bgTr_SoverB ;
     delete bgSp_SoverB ;
     delete bkgd_SoverB ;

     delete bgTr_SoverSqrtSPlusB ;
     delete bgSp_SoverSqrtSPlusB ;
     delete bkgd_SoverSqrtSPlusB ;

     delete bgTr_SoverSqrtBPlusDeltaB ;
     delete bgSp_SoverSqrtBPlusDeltaB ;
     delete bkgd_SoverSqrtBPlusDeltaB ;

     delete bgTr_BayesLimit ;
     delete bgSp_BayesLimit ;
     delete bkgd_BayesLimit ;

     delete Cut ;
     delete Bin ;
     delete Sign ;
     delete Limit ;

     // Clean UAReader
     delete UAReader ; 
   

}

