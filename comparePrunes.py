import copy
import ROOT
from array import array
from analyze import get_areas

def set_style():
   ROOT.gStyle.SetPadBorderMode(0)
   ROOT.gStyle.SetFrameBorderMode(0)
   ROOT.gStyle.SetPadBottomMargin(0.12)
   ROOT.gStyle.SetPadLeftMargin(0.12)
   ROOT.gStyle.SetCanvasColor(ROOT.kWhite)
   ROOT.gStyle.SetCanvasDefH(600) #Height of canvas
   ROOT.gStyle.SetCanvasDefW(600) #Width of canvas
   ROOT.gStyle.SetCanvasDefX(0)   #Position on screen
   ROOT.gStyle.SetCanvasDefY(0)

   ROOT.gStyle.SetPadTopMargin(0.05)
   ROOT.gStyle.SetPadBottomMargin(0.15)#0.13)
   ROOT.gStyle.SetPadLeftMargin(0.15)#0.16)
   ROOT.gStyle.SetPadRightMargin(0.05)#0.02)

   # For the Pad:
   ROOT.gStyle.SetPadBorderMode(0)
   # ROOT.gStyle.SetPadBorderSize(Width_t size = 1)
   ROOT.gStyle.SetPadColor(ROOT.kWhite)
   ROOT.gStyle.SetPadGridX(False)
   ROOT.gStyle.SetPadGridY(False)
   ROOT.gStyle.SetGridColor(0)
   ROOT.gStyle.SetGridStyle(3)
   ROOT.gStyle.SetGridWidth(1)

   # For the Frame:
   ROOT.gStyle.SetFrameBorderMode(0)
   ROOT.gStyle.SetFrameBorderSize(1)
   ROOT.gStyle.SetFrameFillColor(0)
   ROOT.gStyle.SetFrameFillStyle(0)
   ROOT.gStyle.SetFrameLineColor(1)
   ROOT.gStyle.SetFrameLineStyle(1)
   ROOT.gStyle.SetFrameLineWidth(1)

   ROOT.gStyle.SetAxisColor(1, "XYZ")
   ROOT.gStyle.SetStripDecimals(ROOT.kTRUE)
   ROOT.gStyle.SetTickLength(0.03, "XYZ")
   ROOT.gStyle.SetNdivisions(605, "XYZ")
   ROOT.gStyle.SetPadTickX(1)  # To get tick marks on the opposite side of the frame
   ROOT.gStyle.SetPadTickY(1)
   ROOT.gStyle.SetGridColor(0)
   ROOT.gStyle.SetGridStyle(3)
   ROOT.gStyle.SetGridWidth(1)


   ROOT.gStyle.SetTitleColor(1, "XYZ")
   ROOT.gStyle.SetTitleFont(42, "XYZ")
   ROOT.gStyle.SetTitleSize(0.05, "XYZ")
   ROOT.gStyle.SetTitleXOffset(1.15)#0.9)
   ROOT.gStyle.SetTitleYOffset(1.3) # => 1.15 if exponents
   ROOT.gStyle.SetLabelColor(1, "XYZ")
   ROOT.gStyle.SetLabelFont(42, "XYZ")
   ROOT.gStyle.SetLabelOffset(0.007, "XYZ")
   ROOT.gStyle.SetLabelSize(0.045, "XYZ")

   ROOT.gStyle.SetPadBorderMode(0)
   ROOT.gStyle.SetFrameBorderMode(0)
   ROOT.gStyle.SetTitleTextColor(1)
   ROOT.gStyle.SetTitleFillColor(10)
   ROOT.gStyle.SetTitleFontSize(0.05)

set_style()

root_files = {
    '21VarAda': {
        'file': '2HDMa_Ada_20vars/all/rootfiles/2HDMaVWjets_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_21Var.root',
        'color': ROOT.kBlue,
    },
    #'17Var': {
    #    'file': '2HDMa_Ada_16vars/all/rootfiles/2HDMaVWjets_2017_BDT_35Trees_AdaBoost_GiniIndex_20Cuts_CostComplexity_12PruneStrength_17Var.root',
    #    'color': ROOT.kOrange,
    #},
    '21VarGrad': {
        'file': '2HDMa_Grad_20vars/all/rootfiles/2HDMaVWjets_2017_BDT_200Trees_Grad_FalseBagged_0.6BagFrac_1BagShrink_GiniIndex_20Cuts_CostComplexity_12PruneStrength_21Var.root',
        'color': ROOT.kOrange,
    },
}

TG_dict = {}
#xy_dict = {}
max_sig = 0.
for key in root_files:
    print('Extracting in: '+key )
    sig, roc, eff, rej, sig_lis, bdt_lis = get_areas(root_files[key]['file'])

    eff_a = array('d', eff)
    rej_a = array('d', rej)
    sig_a = array('d', sig_lis)
    bdt_a = array('d', bdt_lis)

    TG_dict[key] = {}
    TG_dict[key]['ROC'] = ROOT.TGraph(len(eff_a), copy.deepcopy(eff_a), copy.deepcopy(rej_a)) 
    TG_dict[key]['ROC'].SetTitle(key+'_roc')
    TG_dict[key]['SIG'] = ROOT.TGraph(len(bdt_a), copy.deepcopy(bdt_a), copy.deepcopy(sig_a))
    TG_dict[key]['SIG'].SetTitle(key+'_sig')

    TG_dict[key]['ROC'].SetLineColor(root_files[key]['color'])
    #TG_dict[key]['ROC'].SetLineWidth(3)
    TG_dict[key]['SIG'].SetLineColor(root_files[key]['color'])
    #TG_dict[key]['SIG'].SetLineWidth(3)

    if max_sig < max(sig_lis): max_sig = max(sig_lis)



# Plot ROC loop
roc_canvas = ROOT.TCanvas("c_roc", "canvas with ROC's", 650, 600) 
roc_canvas.SetGridx(1)
roc_canvas.SetGridy(1)
hr = roc_canvas.DrawFrame(0., 0., 1.1, 1.1, "")

hr.SetXTitle("S_{eff}")
#hr.SetYTitle("#sigma_{95%} [pb]") # #rightarrow 2l2q
hr.SetYTitle("1 - B_{eff}")
hr.SetMinimum(0.)
hr.SetMaximum(1.1)
roc_canvas.Update()

roc_leg = ROOT.TLegend(.2, .2, .4, .4)
roc_leg.SetLineColor(0)
roc_leg.SetHeader('ROC curves')

first_roc = True
for key in root_files:
    if first_roc: 
        first_roc = False
        TG_dict[key]['ROC'].Draw()
    else: TG_dict[key]['ROC'].Draw('same')
    roc_canvas.Update()
    roc_leg.AddEntry(TG_dict[key]['ROC'], key, 'L')
roc_leg.Draw()
roc_canvas.Update()

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextSize(0.04)
latex.SetTextAlign(31)
latex.SetTextAlign(11) # align left
latex.DrawLatex(0.18, 0.96, "CMS preliminary")
latex.DrawLatex(0.60, 0.96, "BDT variable pruning")


ROOT.gStyle.SetOptStat(0)
postGrid = ROOT.TH1D("postGrid", "", 1, 0., 1.1)
postGrid.GetYaxis().SetRangeUser(0., 1.1)
postGrid.Draw("AXIGSAME")

roc_canvas.Update()

# Plot sig loop
sig_canvas = ROOT.TCanvas("c_sig", "canvas with significances", 650, 600) 
sig_canvas.SetGridx(1)
sig_canvas.SetGridy(1)
hr = sig_canvas.DrawFrame(-1., 0., 1., max_sig*1.1, "")

hr.SetXTitle("BDT")
#hr.SetYTitle("#sigma_{95%} [pb]") # #rightarrow 2l2q
hr.SetYTitle("S/#sqrt{S+B}")
hr.SetMinimum(0.)
hr.SetMaximum(max_sig*1.1)
sig_canvas.Update()

sig_leg = ROOT.TLegend(.2, .7, .4, .9)
sig_leg.SetLineColor(0)
sig_leg.SetHeader('Significance curves')

first_roc = True
for key in root_files:
    if first_roc: 
        first_roc = False
        TG_dict[key]['SIG'].Draw()
    else: TG_dict[key]['SIG'].Draw('same')
    sig_canvas.Update()
    sig_leg.AddEntry(TG_dict[key]['ROC'], key, 'L')
sig_leg.Draw()
sig_canvas.Update()
latex.DrawLatex(0.18, 0.96, "CMS preliminary")
latex.DrawLatex(0.60, 0.96, "BDT variable pruning")

sig_canvas.Update()
raw_input('continue')

 
