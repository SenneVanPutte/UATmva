

#include "../includes/UATmvaTree.h"

#include <TFile.h>
#include <TROOT.h>

// ---------------------------- Constructor

UATmvaTree::UATmvaTree(UATmvaConfig& Cfg) { Open (Cfg) ;}

// ---------------------------- Open

void UATmvaTree::Open (UATmvaConfig& Cfg){
 
 // FlatFile Input
 if (Cfg.InputType == 1 ) { 
   cout << "[UATmvaTree::OpenTree] FlatFile Input not available !!!! " << endl;
   InitDone = false ;
 }
 // TTree Input
 else if (Cfg.InputType == 2 ) {  
    for (vector<InputData_t>::iterator iD = (Cfg.GetInputData())->begin() ; iD != (Cfg.GetInputData())->end() ; ++iD) {
      TString NickName = iD->NickName; 
      TString FileName = iD->FileName;
      cout << "Opening TTree: " << NickName << " " << FileName << " " << Cfg.GetTreeName() << endl;
      TFile* File = new TFile(FileName,"READ");
      File->ls();
      TTree* Tree = (TTree*) File->Get(Cfg.GetTreeName());
      gROOT->cd(); // Have to create the new object outside of gDirectory from File
      InputTree_t InTree;
      InTree.NickName = iD->NickName ;
      // Tree->Print();
      InTree.Tree     = (TTree*) Tree->CloneTree();
      InTree.Tree    -> SetName( NickName ) ;
      InputTree.push_back(InTree); 
      delete Tree ;
      File->Close();
      delete File;
    }
    InitDone = true;
 } 
 // Error
 else {
   cout << "[UATmvaTree::OpenTree] Unknown InputType !!!! " << endl;
   InitDone = false ;
 }  

}

// ---------------------------- Close

void UATmvaTree::Close(){
  for (vector<InputTree_t>::iterator iT = InputTree.begin() ; iT != InputTree.end() ; ++iT ) {
    cout << "---------------- Deleting Tree: " << iT->NickName << endl;
    delete iT->Tree;
  }
  InputTree.clear();
  InitDone = false ;
} 

// ---------------------------- Print

void UATmvaTree::Print(){
  for (vector<InputTree_t>::iterator iT = InputTree.begin() ; iT != InputTree.end() ; ++iT ) {
    cout << "---------------- Printing Tree: " << iT->NickName << endl;
    iT->Tree->Print();
  }
} 

// ---------------------------- Write

void UATmvaTree::Write(){

  for (vector<InputTree_t>::iterator iT = InputTree.begin() ; iT != InputTree.end() ; ++iT ) {
    cout << "---------------- Writing Tree: " << iT->NickName << endl;
    iT->Tree->Write();
  }


}


TTree* UATmvaTree::GetTree(TString TreeName) {

  for (vector<InputTree_t>::iterator iT = InputTree.begin() ; iT != InputTree.end() ; ++iT ) {
    if ( iT->NickName == TreeName ) return (iT->Tree) ;
  }
  return NULL ;
}


