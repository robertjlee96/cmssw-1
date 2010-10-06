#!/usr/bin/env python

import sys
from SUSYBSMAnalysis.Zprime2muAnalysis.roottools import *
set_zp2mu_style()
ROOT.gStyle.SetPadTopMargin(0.02)
ROOT.gStyle.SetPadRightMargin(0.02)
ROOT.gStyle.SetTitleX(0.12)
#ROOT.gStyle.SetTitleH(0.07)
c = ROOT.TCanvas('c', '', 820, 630)

def integ(h,a,b=1e9):
    return h.Integral(h.FindBin(a), h.FindBin(b))

nminus1s = [
    'VBTFNoGlbChi2',
    'VBTFNoTkMuon',
    'VBTFNoDB',
    'VBTFNoEta24',
    'VBTFNoEta21',
    'VBTFNoTkHits',
    'VBTFNoPxHits',
    'VBTFNoMuStns',
    'VBTFNoTrgMtch',
    'VBTFNoIso3',
    'VBTFNoIso10',
]

pretty = {
    'VBTFNoEta24': '|#eta| < 2.4',
    'VBTFNoEta21': '|#eta| < 2.1',
    'VBTFNoIso3': 'iso #Sigmap_{T} < 3',
    'VBTFNoIso10': 'iso #Sigmap_{T} < 10',
    'VBTFNoTkHits': '# tk hits #geq 10',
    'VBTFNoPxHits': '# px hits #geq 1',
    'VBTFNoMuStns': '# muon st #geq 2',
    'VBTFNoDB': '|d_{xy}| < 0.2',
    'VBTFNoGlbChi2': 'glb #chi^{2}/ndf < 10',
    'VBTFNoTkMuon': 'isTrackerMuon',
    'VBTFNoTrgMtch': 'HLT_Mu9 match',
    'ana_nminus1_data.root': 'Data, 3.0 pb ^{-1}',
    'ana_nminus1_zmumu.root': 'Z#rightarrow#mu#mu',
    'ana_nminus1_dy120.root': 'Z#rightarrow#mu#mu',
    'ana_nminus1_ttbar.root': 't#bar{t}',
    'ana_nminus1_zp1000.root': 'Z\' SSM, M=1 TeV  ',
    }

def table(fn):
    f = ROOT.TFile(fn)
    hnum = f.Get('VBTFNoNo').Get('DileptonMass')
    num60120 = integ(hnum, 60, 120)
    num120 = integ(hnum, 120)
    print 'numerator: 60-120:', num60120, ' 120-:', num120
    print '%20s%20s%20s%20s%20s' % ('name', 'den 60-120', 'eff 60-120', 'den 120', 'eff 120')
    for nminus1 in nminus1s:
        hden = f.Get(nminus1).Get('DileptonMass')
        den60120 = integ(hden, 60, 120)
        den120 = integ(hden, 120)
        print '%20s%20f%20f%20f%20f' % (nminus1, den60120, num60120/den60120, den120, num120/den120)

    hnum = f.Get('VBTFNoNoIso10').Get('DileptonMass')
    num60120 = integ(hnum, 60, 120)
    num120 = integ(hnum, 120)
    hden = f.Get('VBTFNoIso10').Get('DileptonMass')
    den60120 = integ(hden, 60, 120)
    den120 = integ(hden, 120)
    print '%20s%20f%20f%20f%20f%20f%20f' % ('VBTFNoIso10/VBTFNoNoIso10', num60120, den60120, num60120/den60120, num120, den120, num120/den120)

if '120' in sys.argv:
    items = [
        ('ana_nminus1_data.root', (120, 1e9), ROOT.kBlack),
#       ('ana_nminus1_zmumu.root', (120, 1e9), ROOT.kCyan),
        ('ana_nminus1_dy120.root', (120, 1e9), ROOT.kBlue),
        ('ana_nminus1_ttbar.root', (120, 1e9), ROOT.kRed),
        ('ana_nminus1_zp1000.root', (120, 1e9), ROOT.kGreen+2)
        ]
    lg = ROOT.TLegend(0.14, 0.14, 0.60, 0.45)
else:         
    items = [
        ('ana_nminus1_data.root', (60,120), ROOT.kBlack),
        ('ana_nminus1_zmumu.root', (60,120), ROOT.kBlue),
#       ('ana_nminus1_ttbar.root', (60, 120), ROOT.kRed),
        ]
    lg = ROOT.TLegend(0.14, 0.14, 0.60, 0.30)

same = 'A'
effs = []
lg.SetFillColor(0)
for fn, mass_range, color in items:
    f = ROOT.TFile(fn)

    l = len(nminus1s)
    nminus1_num = ROOT.TH1F('num', '', l, 0, l)
    nminus1_den = ROOT.TH1F('den', '', l, 0, l)

    hnum10 = f.Get('VBTFNoNoIso10').Get('DileptonMass')
    num10 = integ(hnum10, *mass_range)
    hnum = f.Get('VBTFNoNo').Get('DileptonMass')
    num = integ(hnum, *mass_range)
    for i,nminus1 in enumerate(nminus1s):
        hden = f.Get(nminus1).Get('DileptonMass')
        den = integ(hden, *mass_range)
        n = num if nminus1 != 'VBTFNoIso10' else num10
        nminus1_num.SetBinContent(i+1, n)
        nminus1_den.SetBinContent(i+1, den)

    eff = binomial_divide(nminus1_num, nminus1_den)
    if '120' in sys.argv:
        eff.SetTitle('M_{#mu#mu} > 120 GeV')
    else:
        eff.SetTitle('60 < M_{#mu#mu} < 120 GeV')
    eff.GetXaxis().SetRangeUser(0,12)
#    eff.GetYaxis().SetRangeUser(0.5,1.05)
    eff.GetXaxis().SetTitle('cut')
    eff.GetYaxis().SetLabelSize(0.03)
    eff.GetYaxis().SetTitle('n-1 efficiency')
    eff.GetYaxis().SetTitle('n-1 efficiency')
    if 'data' in fn:
        draw = 'P'
        eff.SetLineColor(color)
        lg.AddEntry(eff, pretty[fn], 'L')
    else:
        draw = '2'
        eff.SetFillColor(color)
        lg.AddEntry(eff, pretty[fn], 'F')
    draw += same
    eff.Draw(draw)
    effs.append(eff)
    same = ' same'
    bnr = eff.GetXaxis().GetNbins()/eff.GetN()
    for i in xrange(1,l+1):
        eff.GetXaxis().SetBinLabel((i-1)*bnr+1, pretty[nminus1s[i-1]])
    eff.GetXaxis().LabelsOption('u')

    print fn
    table(fn)

lg.Draw()
c.SaveAs('nminus1.png')
c.SaveAs('nminus1.root')
