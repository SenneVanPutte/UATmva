import os
import copy
import ROOT
import math
from array import array

def analyze(rootfile):
    test_tree_name = 'TestTree'
    train_tree_name = 'TrainTree'
 
    mva_name = rootfile.split('/')[-1].replace('.root', '')    
    r_file = ROOT.TFile(rootfile)
    te_tree = r_file.Get(test_tree_name)
    tr_tree = r_file.Get(train_tree_name)
    
    n_bins = 100
    #n_bins = 14
    hsw_MVAtest = ROOT.TH1F('hsw_MVAtest', 'MVA weighted signal test sample', n_bins, -1.1, 1.1)
    hbw_MVAtest = ROOT.TH1F('hbw_MVAtest', 'MVA weighted background test sample', n_bins, -1.1, 1.1)

    h_sig  = ROOT.TH1F('h_sig' , 'Significance vs Cut', n_bins, -1.1, 1.1)
    hsw_MVAtest.Sumw2()
    hbw_MVAtest.Sumw2()

    sgn = []
    bck = []
    sgn_w = []
    bck_w = []
    sgn_tr = []
    bck_tr = []

    print('[analyze] getting BDT values')
    n_te = te_tree.GetEntries()
    for i_evt in range(n_te):
        te_tree.GetEntry(i_evt)
        if 'Signal' in te_tree.className:
            hsw_MVAtest.Fill(getattr(te_tree, mva_name), te_tree.weight)
            sgn.append(getattr(te_tree, mva_name))
            sgn_w.append(te_tree.weight) 
        else:
            hbw_MVAtest.Fill(getattr(te_tree, mva_name), te_tree.weight)
            bck.append(getattr(te_tree, mva_name))
            bck_w.append(te_tree.weight) 

    n_tr = tr_tree.GetEntries()
    for i_evt in range(n_tr):
        tr_tree.GetEntry(i_evt)
        if 'Signal' in tr_tree.className:
            sgn_tr.append(getattr(tr_tree, mva_name))
        else:
            bck_tr.append(getattr(tr_tree, mva_name))

    sgn, sgn_w = (list(t) for t in zip(*sorted(zip(sgn, sgn_w)))) 
    bck, bck_w = (list(t) for t in zip(*sorted(zip(bck, bck_w))))
    sgn_tr.sort()
    bck_tr.sort()

    sw = sum(sgn_w)
    sgn_sw = [sw]
    for idx in range(len(sgn_w)-1):
        sw -= sgn_w[idx]
        sgn_sw.append(sw)

    sw = sum(bck_w)
    bck_sw = [sw]
    for idx in range(len(bck_w)-1):
        sw -= bck_w[idx]
        bck_sw.append(sw)
        

    #print('[analyze] preforming KS test')
    #Ps, Pb, Pm = ks(sgn, sgn_tr, bck, bck_tr)
    #print('[analyze] '+str(Pm))
    #print('[analyze] determining maximum significance')
    #significance = maxSigI(hsw_MVAtest, hbw_MVAtest, h_sig, n_bins)
    #print('[analyze] '+str(significance))
    print('[analyze] calculating ROC area and Significance area')
    #roc = ROC_area(rootfile)
    sig, roc, eff, rej = ROC_area2(sgn, bck, sgn_sw, bck_sw)
    print('[analyze] '+str(roc)+', '+str(sig))

    #return Pm, significance, roc, sig


def ROC_area2(sgn, bck, sgn_sw, bck_sw):
    all_evt = sgn + bck
    sgn_cp = copy.deepcopy(sgn)
    bck_cp = copy.deepcopy(bck)
    sgn_sw_cp = copy.deepcopy(sgn_sw)
    bck_sw_cp = copy.deepcopy(bck_sw)
    all_evt.append(-1.)
    all_evt.append(1.)
    all_evt.sort()
    n_sgn = len(sgn)
    n_bck = len(bck)

    bck_rej = []
    sgn_eff = []

    sig = []

    for val in all_evt:
        while len(sgn_cp)>0 and sgn_cp[0] < val: 
            sgn_cp.pop(0)
            sgn_sw_cp.pop(0)
        while len(bck_cp)>0 and bck_cp[0] < val: 
            bck_cp.pop(0)
            bck_sw_cp.pop(0)
        bck_rej_t = 1. - (len(bck_cp) + 0.)/(n_bck + 0.)
        sgn_eff_t = (len(sgn_cp)+0.)/(n_sgn+0.)
        if len(sgn_cp) == 0.: sig_t = 0.
        elif len(bck_cp) == 0: sig_t = (sgn_sw_cp[0] + 0.)/(math.sqrt(abs(sgn_sw_cp[0])) + 0.)
        else: sig_t = (sgn_sw_cp[0] + 0.)/(math.sqrt(abs(sgn_sw_cp[0]+bck_sw_cp[0])) + 0.)
        bck_rej.append(bck_rej_t)
        sgn_eff.append(sgn_eff_t)
        sig.append(sig_t)
    
    sgn_eff_r, bck_rej = (list(t) for t in zip(*sorted(zip(sgn_eff, bck_rej))))   
    sgn_eff_s, sig_s = (list(t) for t in zip(*sorted(zip(sgn_eff, sig))))   

    sig.pop(-1)
    #sig.pop(0)
    all_evt.pop(-1)
    #all_evt.pop(0)

    roc_area = area(sgn_eff_r, bck_rej)
    sig_area = area(all_evt, sig)

    print(area(sgn_eff_s, sig_s))
    rej_a = array('d', bck_rej)
    eff_a = array('d', sgn_eff_r)
    TGroc = ROOT.TGraph(len(eff_a), eff_a, rej_a)
    TGroc.Draw('')
    raw_input('continue')
    eff_s = array('d', sgn_eff_s)
    sig_s = array('d', sig_s)
    TGroc = ROOT.TGraph(len(eff_s), eff_s, sig_s)
    TGroc.Draw('')
    raw_input('continue')
    bdt_a = array('d', all_evt)
    sig_a = array('d', sig)
    TGroc = ROOT.TGraph(len(bdt_a), bdt_a, sig_a)
    TGroc.Draw('')
    raw_input('continue')
    return sig_area, roc_area, sgn_eff_r, bck_rej

def area(x, y):
    area = 0.
    for idx in range(1, len(x)):
        w = x[idx] - x[idx-1]
        h = (y[idx] + y[idx-1])/2.
        area += w*h
    return area

analyze('2HDMa_Ada_18vars/all/rootfiles/2HDMaVWjets_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_19Var.root')
#analyze('2HDMa_Ada_18vars/MHlnjj_MINPTlj_D_Mlmetjj/rootfiles/2HDMaVWjets_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_18Var.root')
