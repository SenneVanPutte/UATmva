#include <TROOT.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TFile.h>


#include <RooWorkspace.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooAbsPdf.h>
#include <RooStats/BayesianCalculator.h>


using namespace RooStats;
RooWorkspace *wRoo = 0;


void init() {
    wRoo = new RooWorkspace("w","w");
    wRoo->factory("N[0,100]");
    wRoo->factory("B[1]");
    wRoo->factory("dB[0.35]");
    wRoo->factory("S[1]");
    wRoo->factory("dS[0.1]");
    wRoo->factory("r[1,0,20]");
    wRoo->factory("PROD::nuisancePdf(Gaussian::nuisPdfS(thetaS[-5,5],0,1), Gaussian::nuisPdfB(thetaB[-5,5],0,1))");
    wRoo->factory("expr::expB('B*pow(1+dB,thetaB)', B, dB, thetaB)");
    wRoo->factory("expr::expS('S*pow(1+dS,thetaS)', S, dS, thetaS)");
    wRoo->factory("PROD::model_s(Poisson::modelStat_s(N, sum(prod(r,expS),expB), 1), nuisancePdf)");
    wRoo->factory("PROD::model_b(Poisson::modelStat_b(N,                  expB,  1), nuisancePdf)");
    wRoo->factory("Uniform::prior(r)");
}

double limitBayesian(double B, double dB=0.35, double S=1, double dS=0.1) {
    cout << "[limitBayesian] B = " << B <<" S = "<< S <<endl;

//    return 10.;

    wRoo->var("B")->setVal(B);
    wRoo->var("dB")->setVal(dB);
    wRoo->var("S")->setVal(S);
    wRoo->var("dS")->setVal(dS);
    wRoo->var("N")->setVal(B);
    RooArgSet obs(*wRoo->var("N"));
    RooDataSet data("data","data", obs);
    data.add(obs); // insert entry
    RooArgSet nuisances;
    if (dB) { 
        nuisances.add(*wRoo->var("thetaB")); 
        wRoo->var("thetaB")->setConstant(false); 
    } else {
        wRoo->var("thetaB")->setConstant(true);
    }
    if (dS) { 
        nuisances.add(*wRoo->var("thetaS")); 
        wRoo->var("thetaS")->setConstant(false); 
    } else {
        wRoo->var("thetaS")->setConstant(true);
    }
    RooArgSet poi(*wRoo->var("r"));
    RooAbsPdf *model = wRoo->pdf((dS > 0 || dB > 0) ? "model_s" : "modelStat_s");
    RooAbsPdf *prior = wRoo->pdf("prior");
    RooArgSet *nuisSet = (dS > 0 || dB > 0 ? &nuisances : NULL);
    BayesianCalculator bcalc(data, *model, poi, *prior, nuisSet);
    bcalc.SetLeftSideTailFraction(0);
    bcalc.SetConfidenceLevel(0.95);
    SimpleInterval *interval = bcalc.GetInterval();
    double ret = interval->UpperLimit();
    delete interval;
    return ret;
}
double limitMean(double B, double dB=0.35, double S=1, double dS=0.1, int nToys = 1000) {
    // do 1 limit first to set up everything
    limitBayesian(B,dB,S,dS);
    RooArgSet nuisances;
    if (dB) nuisances.add(*wRoo->var("thetaB")); 
    if (dS) nuisances.add(*wRoo->var("thetaS")); 
    RooDataSet *syst = 0;
    if (dS > 0 || dB > 0) syst = wRoo->pdf("nuisancePdf")->generate(nuisances, nToys);
    //wRoo->tree()->scan("*");
    RooArgSet obs(*wRoo->var("N"));
    RooArgSet poi(*wRoo->var("r"));
    double sum = 0;
    for (int i = 0; i < nToys; ++i) {
        if (syst) nuisances = *syst->get(i);
        RooDataSet *toy = wRoo->pdf("model_b")->generate(obs,1);
        BayesianCalculator bcalc(*toy, *wRoo->pdf("model_s"), poi, *wRoo->pdf("prior"), syst ? &nuisances : NULL);
        bcalc.SetLeftSideTailFraction(0);
        bcalc.SetConfidenceLevel(0.95);
        SimpleInterval *interval = bcalc.GetInterval();
        double ret = interval->UpperLimit();
        //std::cout << "toy " << i << "\t limit " << ret << std::endl;
        sum += ret;
        delete toy;
    }
    return sum / nToys;
}


void figureOfMerit() {
    init();
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
    // example optimization for a cut on 'x'
    //   eff(S) = 1-x
    //   eff(B) = exp(-5*x)
    // S(X=0) = 4    
    // B(x=0) = 30  
    // dS/S = 15%, dB/B = 35%
    // let's scan X, print values of limit and save them to a tree
    double S0 = 4, B0 = 20, dS = 0.1, dB=0.35;
    double x, S, B, limit, naive;
    TFile *fOut = TFile::Open("figureOfMerit.root", "RECREATE");
    TTree *tOut = new TTree("fom","fom");
    tOut->Branch("x", &x, "X/D");  tOut->Branch("S", &S, "S/D"); tOut->Branch("B", &B, "B/D"); 
    tOut->Branch("limit", &limit, "limit/D"); tOut->Branch("naive", &naive, "naive/D");
    printf(" %4s  %4s  %5s   %5s  %5s\n", " X ", " S ", " B ", "limit", "naive");
    for (x = 0; x <= 1; x += 0.05) {
        S = S0*(1-x);
        B = B0*exp(-5*x);
        limit = limitBayesian(B,dB,S,dS);
        naive = 2*sqrt(B+pow(B*dB,2))/S;
        printf(" %4.2f  %4.2f  %5.2f   %5.2f  %5.2f\n", x, S, B, limit, naive);
        tOut->Fill();     
    }
    tOut->Write(); 
    TCanvas *c1 = new TCanvas("c1","c1");
    tOut->Draw("S/4:B/20","","L");
    ((TGraph*)gROOT->FindObject("htemp"))->SetTitle("ROC curve;Eff(Background);Eff(signal)");
    c1->Print("ROC_curve.png");
    tOut->Draw("S/4:x","","L");  TGraph *geffS = ((TGraph*)gROOT->FindObject("Graph")->Clone("geffS"));
    tOut->Draw("B/20:x","","L"); TGraph *geffB = ((TGraph*)gROOT->FindObject("Graph")->Clone("geffB"));
    geffB->SetLineColor(kRed); geffS->SetLineColor(kBlue);
    geffS->SetTitle("Efficiencies for signal (blue), background (red);x variable;efficiency");
    geffS->Draw("AL"); geffB->Draw("L same");
    c1->Print("Effs.png");
    tOut->Draw("limit:x","","L"); TGraph *geffL = ((TGraph*)gROOT->FindObject("Graph")->Clone("geffL"));
    tOut->Draw("naive:x","","L"); TGraph *geffN = ((TGraph*)gROOT->FindObject("Graph")->Clone("geffN"));
    geffN->SetLineColor(kRed); geffL->SetLineColor(kBlue);
    geffN->SetTitle("True limit (blue) and S/#sqrt{B+dB^{2}} (red);x variable;limit");
    geffN->Draw("AL"); geffL->Draw("L same"); 
    c1->Print("Limit.png");
    fOut->Close();
    /*
    double dS = 0.0, dB=0.0;
    std::cout << "S = 1, B = 0.0: Poisson " << limitBayesian(0.0,dB,1,dS) << "\tMean " << limitMean(0.0,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 0.5: Poisson " << limitBayesian(0.5,dB,1,dS) << "\tMean " << limitMean(0.5,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 1.0: Poisson " << limitBayesian(1.0,dB,1,dS) << "\tMean " << limitMean(1.0,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 1.5: Poisson " << limitBayesian(1.5,dB,1,dS) << "\tMean " << limitMean(1.5,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 2.0: Poisson " << limitBayesian(2.0,dB,1,dS) << "\tMean " << limitMean(2.0,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 2.5: Poisson " << limitBayesian(2.5,dB,1,dS) << "\tMean " << limitMean(2.5,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 3.0: Poisson " << limitBayesian(3.0,dB,1,dS) << "\tMean " << limitMean(3.0,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 3.5: Poisson " << limitBayesian(3.5,dB,1,dS) << "\tMean " << limitMean(3.5,dB,1,dS) << "\n";
    std::cout << "S = 1, B = 4.0: Poisson " << limitBayesian(4.0,dB,1,dS) << "\tMean " << limitMean(4.0,dB,1,dS) << "\n";
    */
}



