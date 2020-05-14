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
    print('[analyze] preforming KS test')
    Ps, Pb, Pm = ks(sgn, sgn_tr, bck, bck_tr)
    print('[analyze] '+str(Pm))
    print('[analyze] determining maximum significance')
    significance = maxSigI(hsw_MVAtest, hbw_MVAtest, h_sig, n_bins)
    print('[analyze] '+str(significance))
    print('[analyze] calculating ROC area and Significance area')
    #roc = ROC_area(rootfile)
    sig, roc, eff, rej, sig_lis, bdt_lis = ROC_area2(sgn, bck)
    print('[analyze] '+str(roc)+', '+str(sig))

    return Pm, significance, roc, sig

def analyze_ks(rootfile):
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
    print('[analyze] preforming KS test')
    Ps, Pb, Pm = ks(sgn, sgn_tr, bck, bck_tr)
    print('[analyze] '+str(Pm))
    print('[analyze] determining maximum significance')
    significance = maxSigI(hsw_MVAtest, hbw_MVAtest, h_sig, n_bins)
    print('[analyze] '+str(significance))
    print('[analyze] calculating ROC area and Significance area')
    #roc = ROC_area(rootfile)
    sig, roc, eff, rej, sig_lis, bdt_lis = ROC_area2(sgn, bck)
    print('[analyze] '+str(roc)+', '+str(sig))

    return Ps, Pb, Pm, significance, roc, sig

def ks(sgn, sgn_tr, bck, bck_tr):
    a_sgn = array('d', sgn)
    a_bck = array('d', bck)
    a_sgn_tr = array('d', sgn_tr)
    a_bck_tr = array('d', bck_tr)

    Ds = ROOT.TMath.KolmogorovTest(len(a_sgn), a_sgn, len(a_sgn_tr), a_sgn_tr, '')
    Db = ROOT.TMath.KolmogorovTest(len(a_bck), a_bck, len(a_bck_tr), a_bck_tr, '')

    return Ds, Db, min(Db, Ds)

def maxSigI(hsw_MVAtest, hbw_MVAtest, h_sig, n_bins):
    sig = []
    cut = []
    for i_bin in range(1, n_bins+1):
        s = hsw_MVAtest.Integral(i_bin, n_bins+1)
        b = hbw_MVAtest.Integral(i_bin, n_bins+1)
        if s <= 0.: sig_y = 0.
        else: sig_y = (s+0.)/math.sqrt(s+abs(b)+0.)
        sig.append(sig_y)
        cut.append(hsw_MVAtest.GetBinLowEdge(i_bin))
        h_sig.SetBinContent(i_bin, sig_y)
    return max(sig) 

def ROC_area(file_name):
    mva_name = file_name.split('/')[-1].replace('.root', '')
    or_file = ROOT.TFile(file_name)
    hist = or_file.Get('Method_BDT/'+mva_name+'/MVA_'+mva_name+'_rejBvsS')
    roc_i = hist.Integral(0, hist.GetNbinsX(), "width")
    or_file.Close()
    return roc_i

def ROC_area2(sgn, bck):
    all_evt = sgn + bck
    sgn_cp = copy.deepcopy(sgn)
    bck_cp = copy.deepcopy(bck)
    all_evt.append(-1.)
    all_evt.append(1.)
    all_evt.sort()
    n_sgn = len(sgn)
    n_bck = len(bck)

    bck_rej = []
    sgn_eff = []

    sig = []

    for val in all_evt:
        while len(sgn_cp)>0 and sgn_cp[0] < val: sgn_cp.pop(0)
        while len(bck_cp)>0 and bck_cp[0] < val: bck_cp.pop(0)
        bck_rej_t = 1. - (len(bck_cp) + 0.)/(n_bck + 0.)
        sgn_eff_t = (len(sgn_cp)+0.)/(n_sgn+0.)
        if len(sgn_cp) == 0.: sig_t = 0.
        else: sig_t = (len(sgn_cp) + 0.)/(math.sqrt(len(sgn_cp) + len(bck_cp) + 0.) + 0.)
        bck_rej.append(bck_rej_t)
        sgn_eff.append(sgn_eff_t)
        sig.append(sig_t)
    
    sgn_eff, bck_rej = (list(t) for t in zip(*sorted(zip(sgn_eff, bck_rej))))   

    sig.pop(-1)
    #sig.pop(0)
    all_evt.pop(-1)
    #all_evt.pop(0)

    roc_area = area(sgn_eff, bck_rej)
    sig_area = area(all_evt, sig)
    #for idx in range(1, len(sgn_eff)):
    #    w = sgn_eff[idx] - sgn_eff[idx-1]
    #    h = (bck_rej[idx] + bck_rej[idx-1])/2.
    #    area += w*h
    #print(roc_area)
    #print(sig_area)

    #rej_a = array('d', bck_rej)
    #eff_a = array('d', sgn_eff)
    #TGroc = ROOT.TGraph(len(eff_a), eff_a, rej_a)
    #TGroc.Draw('')
    #raw_input('continue')
    #bdt_a = array('d', all_evt)
    #sig_a = array('d', sig)
    #TGroc = ROOT.TGraph(len(bdt_a), bdt_a, sig_a)
    #TGroc.Draw('')
    #raw_input('continue')
    return sig_area, roc_area, sgn_eff, bck_rej, sig, all_evt
        

def area(x, y):
    area = 0.
    for idx in range(1, len(x)):
        w = x[idx] - x[idx-1]
        h = (y[idx] + y[idx-1])/2.
        area += w*h
    return area


def get_areas(rootfile):
    test_tree_name = 'TestTree'
 
    mva_name = rootfile.split('/')[-1].replace('.root', '')    
    r_file = ROOT.TFile(rootfile)
    te_tree = r_file.Get(test_tree_name)
    
    sgn = []
    bck = []
    #sgn_w = []
    #bck_w = []

    print('[get areas] getting BDT values')
    n_te = te_tree.GetEntries()
    for i_evt in range(n_te):
        te_tree.GetEntry(i_evt)
        if 'Signal' in te_tree.className:
            sgn.append(getattr(te_tree, mva_name))
            #sgn_w.append(te_tree.weight) 
        else:
            bck.append(getattr(te_tree, mva_name))
            #bck_w.append(te_tree.weight) 

    #sgn, sgn_w = (list(t) for t in zip(*sorted(zip(sgn, sgn_w)))) 
    #bck, bck_w = (list(t) for t in zip(*sorted(zip(bck, bck_w))))
    sgn.sort()
    bck.sort()
    print('[get areas] calculating ROC area and Significance area')
    #roc = ROC_area(rootfile)
    sig, roc, eff, rej, sig_lis, bdt_lis = ROC_area2(sgn, bck)
    print('[get areas] '+str(roc)+', '+str(sig))

    return sig, roc, eff, rej, sig_lis, bdt_lis

def tree_sort(trees, lis):
    tr_s, lis_s = (list(t) for t in zip(*sorted(zip(trees, lis))))
    return lis_s
