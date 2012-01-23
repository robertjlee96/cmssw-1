import sys, os
from array import array
from MuonAnalysis.Cosmics.ROOTTools import *
from bins import make_bins

tdr_style()
ROOT.gStyle.SetOptStat(111111)
ROOT.gStyle.SetOptFit(1111)

def fit_histo(h, hist_name, draw=False, likelihood=False):
    if 'P' in hist_name: # pull
        factor = 3/(h.GetRMS() if h.GetRMS() > 0 else 1) # fix to mean +/- 3
    elif 'R' in hist_name:
        factor = 1.5 # mean +/- 1.5 * rms
    else:
        raise NotImplementedError('fit_histo with hist_name %s' % hist_name)
    return fit_gaussian(h, factor, draw, likelihood)

def get_histo_stat(h, hist_name, stat):
    if stat == 'rms':
        return h.GetRMS(), h.GetRMSError()
    elif stat == 'mean':
        return h.GetMean(), h.GetMeanError()
    elif stat == 'sigma':
        return fit_histo(h, hist_name)['sigma']
    raise NotImplementedError('get_curve for %s' % stat)
                   
class Drawer:
    tracks = [
        'Global',
        'TkOnly',
        'TPFMS',
        'Picky',
        'TuneP'
        ]
    
    colors = {
        'Global': ROOT.kBlue,
        'TkOnly': ROOT.kRed,
        'TPFMS':  ROOT.kGreen+1,
        'Picky':  ROOT.kOrange+8, 
        'TuneP':  ROOT.kBlack,
        }

    markers = {
        'Global':  21,
        'TkOnly':  20,
        'TPFMS':   22,
        'Picky':   27,
        'TuneP':   23,
        }

    def __init__(self, filename):
        self.file = ROOT.TFile(filename)

    def get_histo(self, bin_name, track, quantity, hist_name):
        return self.file.histos.Get(bin_name).Get(track).Get(quantity).Get(hist_name)

    def draw_histos(self, track, quantity, hist_name, bin_by='pt'):
        hs = []
        for bin in make_bins(bin_by):
            h = self.get_histo(bin.name, track, quantity, hist_name)
            fit_histo(h, hist_name, draw=True)
            hs.append((bin.name, h))
        return hs
            
    def get_curve(self, track, quantity, hist_name, stat, bin_by='pt'):
        x = []
        y = []
        exl = []
        exh = []
        ey = []

        for bin in make_bins(bin_by):
            if not bin.use_by_bin:
                continue

            lower, upper, abscissa = bin.main_var_range()
            x.append(abscissa)
            exl.append(abscissa - lower)
            exh.append(upper - abscissa)

            h = self.get_histo(bin.name, track, quantity, hist_name)
            value, error = get_histo_stat(h, hist_name, stat)
            y.append(value)
            ey.append(error)

        x = array('d', x)
        y = array('d', y)
        exl = array('d', exl)
        exh = array('d', exh)
        ey = array('d', ey)

        return ROOT.TGraphAsymmErrors(len(x), x, y, exl, exh, ey, ey)

    def overlay_curves(self, tracks, quantity, hist_name, stat, ymin, ymax):
        curves = [(track, self.get_curve(track, quantity, hist_name, stat)) for track in tracks]

        first = True
        for track, curve in curves:
            curve.SetLineColor(self.colors[track])
            curve.SetMarkerColor(self.colors[track])
            curve.SetMarkerStyle(self.markers[track])
            curve.SetMinimum(ymin)
            curve.SetMaximum(ymax)
            if first:
                drawopt = 'AP'
                first = False
            else:
                drawopt = 'P same'
            curve.Draw(drawopt)

        return curves

if __name__ == '__main__':
    fn = sys.argv[1]
    fn_name = fn.replace('.histos', '').replace('.root', '')
    plot_path = os.path.join('plots/cosmicres', fn_name)

    drawer = Drawer(fn)

    for hist_name in ['upperR1lower', 'upperPlower']:
        for track in drawer.tracks:
            ps = plot_saver(os.path.join(plot_path, hist_name, track))
            for bin_name, h in drawer.draw_histos(track, 'qinvpt', hist_name):
                h.Draw()
                ps.save(bin_name)

    ps = plot_saver(plot_path, log=False)
    d = drawer.file.histos.Get('copied_histograms')
    d.Get('track_multiplicity').Draw('hist text00')
    ps.save('track_multiplicity', log=True)
    d.Get('muon_multiplicity').Draw('hist text00')
    ps.save('muon_multiplicity', log=True)
    d.Get('errors').Draw('hist text00')
    ps.save('ntuple_errors')
    drawer.file.histos.Get('errors').Draw('hist text00')
    ps.save('histo_errors')

    ps.save_dir('upperR1lower')
    ps.save_dir('upperPlower')

    ps.c.SetLogx(1)
    curves = drawer.overlay_curves(drawer.tracks, 'qinvpt', 'upperR1lower', 'rms', 0, 0.18)
    ps.save('res_rms')
    curves = drawer.overlay_curves(drawer.tracks, 'qinvpt', 'upperR1lower', 'sigma', 0, 0.08)
    ps.save('res_sigma')
    curves = drawer.overlay_curves(drawer.tracks, 'qinvpt', 'upperPlower',  'sigma', 0.5, 2.5)
    ps.save('pull_sigma')
    curves = drawer.overlay_curves(drawer.tracks, 'qinvpt', 'upperPlower',  'mean', -0.2, 0.8)
    ps.save('pull_mean')
