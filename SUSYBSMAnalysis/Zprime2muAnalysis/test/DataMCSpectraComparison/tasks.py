#!/usr/bin/env python

import sys, os, glob
from itertools import combinations
from FWCore.PythonUtilities.LumiList import LumiList
from SUSYBSMAnalysis.Zprime2muAnalysis.tools import big_warn

just_testing = 'testing' in sys.argv
if just_testing:
    sys.argv.remove('testing')
    
try:
    cmd, extra = sys.argv[1].lower(), sys.argv[2:]
except IndexError:
    print 'usage: utils.py command [extra]'
    sys.exit(1)

def do(cmd):
    print cmd
    ret = []
    if not just_testing:
        cmds = cmd.split('\n') if '\n' in cmd else [cmd]
        for cmd in cmds:
            if cmd != '' and not cmd.startswith('#'):
                ret.append(os.system(cmd))
    if len(ret) == 1:
        ret = ret[0]
    return ret

latest_dataset = '/SingleMu/Run2011A-PromptReco-v6/AOD'

if cmd == 'setdirs':
    do('''
ln -s /uscms/home/tucker/nobackup/crab_dirs crab
mkdir -p psets
ln -s `pwd`/psets crab/psets
''')

elif cmd == 'checkevents':
    from samples import samples
    for sample in samples:
        print sample.name
        do('grep TrigReport crab/crab_datamc_%s/res/*stdout | grep \' p$\' | sed -e "s/ +/ /g" | awk \'{ s += $4; t += $5; u += $6; } END { print "summary: total: ", s, "passed: ", t, "failed: ", u }\'' % sample.name)

elif cmd == 'publishmc':
    from samples import samples
    for sample in samples:
        do('crab -c crab/crab_datamc_${x} -publish >&! crab/publish_logs/publish.${x} &')

elif cmd == 'hadd':
    from samples import samples
    extra = '_' + extra[0] if extra else ''
    for sample in samples:
        name = sample.name
        pattern = 'crab/crab_ana%(extra)s_datamc_%(name)s/res/zp2mu_histos*root' % locals()
        fn = 'ana_datamc_%(name)s.root' % locals()
        n = len(glob.glob(pattern))
        if n == 0:
            big_warn('no files matching %s' % pattern)
        elif n == 1:
            do('cp %s mc/%s' % (pattern, fn))
        else:
            do('hadd mc/ana_datamc_%(name)s.root crab/crab_ana%(extra)s_datamc_%(name)s/res/zp2mu_histos*root' % locals())

elif cmd == 'gatherhistos':
    extra = (extra[0] + '_') if extra else ''

    for which in ['Run2011APlusDCSOnlyMuonsOnly']:
        print which
        dirs = glob.glob('crab/crab_ana_datamc_%s_SingleMu2011A_*' % which)
        files_glob = ' '.join([os.path.join(x, 'res/*.root') for x in dirs])

        wdir = 'ana_datamc_%(extra)s%(which)s' % locals()
        os.mkdir(wdir)
        do('ln -s /uscms_data/d2/tucker/zp2mu_ana_datamc_mc/latest %(wdir)s/mc' % locals())
        do('hadd %(wdir)s/ana_datamc_data.root %(files_glob)s' % locals())

        for dir in dirs:
            do('crab -c %(dir)s -status ; crab -c %(dir)s -report' % locals())

        jsons = [os.path.join(dir, 'res/lumiSummary.json') for dir in dirs]
        lls = [(j, LumiList(j)) for j in jsons]
        for (j1, ll1), (j2, ll2) in combinations(lls, 2):
            cl = (ll1 & ll2).getCompactList()
            print 'checking overlap between', j1, j2,
            if cl:
                raise RuntimeError('\noverlap between %s and %s lumisections' % (j1,j2))
            else:
                print cl
                                        
        reduce(lambda x,y: x|y, (LumiList(j) for j in jsons)).writeJSON('%(wdir)s/ana_datamc_data.forlumi.json' % locals())
        do('lumiCalc2.py -i %(wdir)s/ana_datamc_data.forlumi.json overview > %(wdir)s/ana_datamc_data.lumi' % locals())
        print 'done with', which, '\n'

elif cmd == 'mclinks':
    extra = extra[0] if extra else 'renameme'
    for x in ['muonsonly', 'allgood']:
        print('ln -s ~/nobackup/ana_datamc_mc/%s %s' % (extra, x))

elif cmd == 'runrange':
    cmd = 'dbs search --query="find min(run),max(run) where dataset=%s"' % latest_dataset
    do(cmd)

elif cmd == 'checkavail':
    cmd = 'dbs search --query="find run,lumi where dataset=%s"' % latest_dataset
    print '\n',cmd

    lumis = []
    for line in os.popen(cmd):
    #for line in open('temp'):
        line = line.strip().split()
        if len(line) != 2:
            continue
        try:
            a = int(line[0])
            b = int(line[1])
        except ValueError:
            continue
        lumis.append((a,b))

    ll = LumiList(lumis=lumis)
    runrange = sorted(int(x) for x in ll.getCompactList().keys())

    dcs_ll = LumiList('/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt')
    dcs_runrange = sorted(int(x) for x in dcs_ll.getCompactList().keys())

    dcs_ll.removeRuns(xrange(dcs_runrange[0], runrange[0]))
    dcs_ll.removeRuns(xrange(runrange[-1]+1, dcs_runrange[-1]))

    ok = LumiList(compactList={"172630": [[161, 167]],
                               "172791": [[570, 570], [1646, 1648], [1659, 1664]],
                               "172798": [[32, 35]],
                               "172799": [[368, 372]],
                               "172801": [[680, 680], [751, 752], [767, 767], [816, 818], [838, 838], [862, 862], [910, 910], [1140, 1144]],
                               "172819": [[255, 259]],
                               })

    print 'run range for', latest_dataset, ':', runrange[0], runrange[-1]
    print 'these lumis are in the DCS-only JSON but not (yet) in', latest_dataset
    print str(dcs_ll - ll - ok)

elif cmd == 'drawall':
    for x in glob.glob('ana_datamc_*'):
        r = do('python draw.py %s > out.draw.%s' % (x,x))
        if r != 0:
            sys.exit(r)
    do('mv out.draw.* plots/')
    do('tlock ~/asdf/plots.tgz plots/datamc_* plots/out.draw.*')
else:
    raise ValueError('command %s not recognized!' % cmd)

