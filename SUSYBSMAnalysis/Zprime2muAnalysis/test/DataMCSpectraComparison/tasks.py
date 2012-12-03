#!/usr/bin/env python

import sys, os, glob
from itertools import combinations
from FWCore.PythonUtilities.LumiList import LumiList
from SUSYBSMAnalysis.Zprime2muAnalysis.hadd import hadd
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

#latest_dataset = '/SingleMu/Run2012A-PromptReco-v1/AOD'
#latest_dataset = '/SingleMu/Run2012B-PromptReco-v1/AOD'
#latest_dataset = '/SingleMu/Run2012C-PromptReco-v1/AOD'
#latest_dataset = '/SingleMu/Run2012C-PromptReco-v2/AOD'
latest_dataset = '/SingleMu/Run2012D-PromptReco-v1/AOD'
#lumi_masks = ['Run2012PlusDCSOnlyMuonsOnly', 'Run2012MuonsOnly'] #, 'DCSOnly', 'Run2012']
lumi_masks = ['Run2012MuonsOnly', 'Run2012'] #, 'Run2012PlusDCSOnlyMuonsOnly', 'DCSOnly']

if cmd == 'setdirs':
    crab_dirs_location = extra[0]
    do('mkdir -p ' + os.path.join(crab_dirs_location, 'psets'))
    do('ln -s %s crab' % crab_dirs_location)
    do('ln -s . crab/crab') # this is so crab -publish won't complain about being unable to find the pset if you launch it from crab/
    do('mkdir crab/publish_logs')

elif cmd == 'maketagdirs':
    extra = extra[0]
    do('rm data mc plots')
    for which in ['data', 'mc', 'plots']:
        d = '~/nobackup/zp2mu_ana_datamc_%s/%s' % (which,extra)
        do('mkdir -p %s' % d)
        do('ln -s %s %s' % (d, which))

elif cmd == 'checkevents':
    from SUSYBSMAnalysis.Zprime2muAnalysis.MCSamples import samples
    for sample in samples:
        print sample.name
        do('grep TrigReport crab/crab_datamc_%s/res/*stdout | grep \' p$\' | sed -e "s/ +/ /g" | awk \'{ s += $4; t += $5; u += $6; } END { print "summary: total: ", s, "passed: ", t, "failed: ", u }\'' % sample.name)

elif cmd == 'publishmc':
    from SUSYBSMAnalysis.Zprime2muAnalysis.MCSamples import samples
    for sample in samples:
        do('crab -c crab/crab_datamc_%(name)s -publish >& crab/publish_logs/publish.crab_datamc_%(name)s &' % sample)

elif cmd == 'anadatasets':
    print 'paste this into python/MCSamples.py:\n'
    from SUSYBSMAnalysis.Zprime2muAnalysis.MCSamples import samples
    for sample in samples:
        ana_dataset = None
        fn = 'crab/publish_logs/publish.crab_datamc_%s' % sample.name
        # yay fragile parsing
        for line in open(fn):
            if line.startswith(' total events'):
                ana_dataset = line.split(' ')[-1].strip()
                break
        if ana_dataset is None:
            raise ValueError('could not find ana_dataset from %s' % fn)
        print '%s.ana_dataset = "%s"' % (sample.name, ana_dataset)

elif cmd == 'gathermc':
    from SUSYBSMAnalysis.Zprime2muAnalysis.MCSamples import samples
    extra = '_' + extra[0] if extra else ''
    for sample in samples:
        name = sample.name
        pattern = 'crab/crab_ana%(extra)s_datamc_%(name)s/res/zp2mu_histos*root' % locals()
        fn = 'ana_datamc_%(name)s.root' % locals()
        n = len(glob.glob(pattern))
        if n == 0:
            big_warn('no files matching %s' % pattern)
        else:
            files = glob.glob('crab/crab_ana%(extra)s_datamc_%(name)s/res/zp2mu_histos*root' % locals())
            hadd('mc/ana_datamc_%s.root' % name, files)

elif cmd == 'gatherdata':
    extra = (extra[0] + '_') if extra else ''

    for lumi_mask in lumi_masks:
        print lumi_mask
        dirs = glob.glob('crab/crab_ana_datamc_%s_SingleMuRun2012*' % lumi_mask)
        files = []
        for d in dirs:
            files += glob.glob(os.path.join(d, 'res/*.root'))

        wdir = os.path.join('data', lumi_mask)
        os.mkdir(wdir)
        hadd(os.path.join(wdir, 'ana_datamc_data.root'), files)

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
        do('tail -5 %(wdir)s/ana_datamc_data.lumi' % locals())
        print 'done with', lumi_mask, '\n'

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

    dcs_ll = LumiList('/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt') # JMTBAD import from goodlumis
    dcs_runrange = sorted(int(x) for x in dcs_ll.getCompactList().keys())

    dcs_ll.removeRuns(xrange(dcs_runrange[0], runrange[0]))
    dcs_ll.removeRuns(xrange(runrange[-1]+1, dcs_runrange[-1]))

#- 2012A:    190450-193686
#- 2012B:    193752-196531
#- 2012C v1: 197556-198913
#- 2012C v2: 198934-203772
#- 2012D:    203773-
#- Reasons for excluding some runs:
#    191350, 192989, 192890, 193091, 204900, 206251, 207871: VdM scan
#    193092: very low pile-up run
    ok = LumiList(compactList={"190456": [[68, 70], [87, 88], [107, 108], [116, 117], [131, 132], [148, 150], [163, 164], [172, 174], [185, 188]], "190459": [[58, 59]], "190462": [[3, 6], [16, 21]], "190465": [[12, 14], [44, 46], [70, 71]], "190482": [[179, 186], [187, 188], [189, 203]], "190491": [[52, 60]], "190538": [[120, 125]], "190591": [[150, 151], [209, 211], [212, 212], [213, 218]], "190592": [[2, 2], [29, 35]], "190593": [[5, 10]], "190645": [[111, 112]], "190661": [[360, 366]], "190662": [[4, 5], [8, 8], [11, 11]], "190678": [[612, 612], [620, 627]], "190702": [[54, 54], [170, 176]], "190703": [[253, 254]], "190704": [[4, 9]], "190705": [[6, 6], [77, 77]], "190706": [[127, 138]], "190708": [[190, 202]], "190710": [[3, 9]], "190733": [[461, 471]], "190734": [[1, 9]], "190735": [[2, 8]], "190736": [[186, 189]], "190738": [[356, 356]], "190895": [[203, 209], [585, 586]], "190906": [[355, 355]], "190945": [[208, 214]], "190949": [[220, 220], [1138, 1141]], "191043": [[48, 60]], "191046": [[22, 23], [184, 184], [240, 245]], "191056": [[2, 3], [10, 15], [18, 18], [20, 22]], "191057": [[2, 3], [66, 73]], "191062": [[2, 2], [550, 557]], "191201": [[80, 86]], "191247": [[1225, 1232]], "191271": [[364, 365]], "191276": [[17, 23]], "191277": [[536, 536]], "191306": [[8, 106], [107, 179], [180, 180]], "191350": [[5, 755]], "191393": [[1, 85]], "191394": [[1, 8]], "191410": [[1, 22]], "191691": [[81, 86]], "191692": [[29, 35]], "191694": [[44, 48]], "191695": [[2, 9]], "191697": [[2, 2], [3, 9]], "191700": [[637, 637], [639, 643], [645, 653]], "191718": [[208, 215]], "191720": [[2, 2], [182, 192]], "191721": [[2, 2], [190, 202]], "191723": [[1, 9]], "191800": [[112, 112]], "191808": [[1, 9]], "191811": [[102, 102], [152, 153]], "191830": [[394, 398]], "191833": [[2, 2], [106, 112]], "191834": [[353, 356]], "191837": [[66, 73]], "191839": [[2, 2], [37, 37]], "191842": [[18, 22]], "191845": [[27, 35]], "191856": [[134, 138]], "191857": [[31, 35]], "192989": [[31, 47]], "192990": [[1, 54], [55, 55], [56, 60]], "193091": [[75, 372]], "193092": [[1, 587]], "193112": [[220, 228]], "193115": [[1, 19]], "193116": [[677, 678]], "193123": [[28, 35]], "193192": [[87, 125]], "193193": [[7, 7], [9, 10]], "193334": [[173, 176]], "193541": [[102, 102]], "193556": [[147, 147], [148, 149], [150, 150]], "193575": [[753, 753]], "193834": [[36, 36]], "193835": [[21, 21], [27, 34]], "193998": [[279, 292]], "194050": [[114, 115], [274, 274], [356, 356], [1889, 1890]], "194051": [[13, 22]], "194075": [[102, 102], [112, 112]], "194115": [[185, 185], [858, 861]], "194119": [[266, 267], [268, 275]], "194150": [[465, 482], [483, 484], [485, 485]], "194151": [[620, 620], [625, 626], [628, 628]], "194199": [[403, 407]], "194223": [[113, 125]], "194224": [[413, 421]], "194303": [[103, 110]], "194304": [[47, 49]], "194314": [[302, 318]], "194315": [[468, 472]], "194424": [[709, 717]], "194439": [[107, 112]], "194455": [[304, 304], [305, 307]], "194464": [[211, 215]], "194479": [[567, 575]], "194619": [[112, 125]], "194631": [[223, 228]], "194643": [[288, 289], [290, 293]], "194691": [[319, 332], [333, 333], [334, 364], [365, 366], [367, 386], [387, 387], [388, 390], [391, 403]], "194699": [[260, 260], [261, 261], [262, 266]], "194702": [[192, 203]], "194704": [[593, 601]], "194711": [[620, 626]], "194778": [[220, 228]], "194788": [[20, 20]], "194789": [[567, 575]], "194790": [[46, 49]], "194825": [[118, 119], [223, 228]], "194896": [[104, 112], [113, 113]], "194912": [[52, 52], [1663, 1669]], "194914": [[39, 47]], "195013": [[527, 527], [542, 562]], "195014": [[161, 163]], "195015": [[14, 24]], "195109": [[338, 344]], "195110": [[7, 9]], "195111": [[45, 48], [49, 56]], "195112": [[27, 33]], "195113": [[580, 588]], "195114": [[104, 111]], "195163": [[348, 352]], "195164": [[65, 68]], "195165": [[5, 6]], "195251": [[250, 250]], "195265": [[157, 163]], "195303": [[410, 411]], "195378": [[1303, 1304]], "195379": [[278, 279]], "195387": [[8, 9]], "195396": [[132, 139]], "195397": [[1210, 1219]], "195398": [[1796, 1798]], "195522": [[57, 57]], "195523": [[2, 6]], "195524": [[1, 7]], "195526": [[1, 9]], "195527": [[2, 9]], "195528": [[2, 5], [6, 7], [8, 9]], "195529": [[60, 60]], "195530": [[593, 594], [595, 600]], "195551": [[107, 112]], "195633": [[43, 48]], "195634": [[57, 61]], "195644": [[58, 59], [60, 60]], "195647": [[42, 47]], "195649": [[248, 253]], "195655": [[499, 501]], "195656": [[480, 485]], "195658": [[387, 395]], "195660": [[1, 228]], "195757": [[207, 210]], "195758": [[19, 22]], "195774": [[964, 964]], "195775": [[171, 176]], "195841": [[91, 99]], "195913": [[60, 69]], "195915": [[852, 865]], "195916": [[214, 214], [215, 216]], "195917": [[6, 6], [7, 7], [8, 9]], "195918": [[66, 73]], "195919": [[17, 22]], "195923": [[16, 22]], "195925": [[14, 21]], "195926": [[36, 36]], "195927": [[3, 9]], "195929": [[31, 35]], "195930": [[598, 601]], "195947": [[89, 97]], "195948": [[616, 621]], "195970": [[50, 50], [86, 86]], "196019": [[101, 102]], "196023": [[48, 48], [57, 57], [64, 64], [72, 86]], "196046": [[41, 48]], "196047": [[65, 67], [68, 68], [76, 76], [77, 77], [78, 86]], "196048": [[45, 45]], "196090": [[107, 112]], "196096": [[23, 36]], "196097": [[20, 22]], "196098": [[9, 9]], "196099": [[21, 35]], "196107": [[440, 443], [444, 445], [446, 447]], "196122": [[32, 37]], "196131": [[210, 215]], "196197": [[563, 564], [565, 570]], "196199": [[535, 538]], "196200": [[69, 73]], "196201": [[1, 4], [5, 5], [6, 9]], "196202": [[109, 112]], "196218": [[821, 824], [825, 826], [827, 832]], "196249": [[100, 102]], "196250": [[427, 429], [431, 433]], "196349": [[275, 276], [277, 277]], "196350": [[7, 8]], "196353": [[159, 163]], "196357": [[5, 9]], "196359": [[19, 22]], "196362": [[89, 112]], "196363": [[35, 38]], "196431": [[515, 524]], "196432": [[35, 35]], "196433": [[312, 318]], "196437": [[226, 227]], "196452": [[1092, 1097]], "196453": [[1648, 1656]], "196495": [[273, 274], [275, 279]], "196497": [[57, 60]], "196501": [[34, 35]], "198022": [[188, 190], [202, 203], [215, 216], [228, 232]], "198023": [[17, 20], [34, 35], [49, 51], [64, 66], [80, 82], [95, 96]], "198041": [[12, 12], [14, 15], [19, 19], [35, 36], [38, 39], [59, 60]], "198044": [[6, 7], [9, 10], [17, 17], [32, 33], [35, 36], [58, 59], [61, 73]], "198045": [[14, 14], [16, 17], [32, 32], [34, 35], [42, 43], [45, 46], [253, 254], [263, 267]], "198048": [[11, 11], [22, 22], [34, 34], [47, 47], [59, 60], [73, 73]], "198049": [[58, 60]], "198050": [[156, 159], [160, 162]], "198116": [[32, 32], [34, 35], [113, 120], [121, 123], [124, 124]], "198202": [[105, 111], [112, 114]], "198207": [[98, 111]], "198208": [[210, 215]], "198210": [[222, 228]], "198212": [[575, 575]], "198213": [[108, 112]], "198214": [[2, 9]], "198229": [[68, 73]], "198230": [[1381, 1386]], "198269": [[200, 200], [202, 202], [204, 204]], "198270": [[3, 4], [5, 7]], "198271": [[798, 799], [800, 802]], "198272": [[128, 144], [246, 247]], "198372": [[52, 53], [55, 56]], "198486": [[2, 9]], "198487": [[16, 16], [20, 21], [24, 24], [26, 26], [29, 31], [253, 255], [300, 300], [305, 305], [308, 308], [320, 320], [325, 327]], "198522": [[60, 61], [63, 64], [98, 98], [106, 106], [109, 109], [115, 115], [187, 187], [192, 193], [196, 199], [209, 215]], "198523": [[2, 3], [4, 8], [9, 9]], "198588": [[63, 176]], "198589": [[1, 9]], "198603": [[33, 63]], "198609": [[49, 112]], "198899": [[1, 22]], "198900": [[1, 60]], "198901": [[1, 86]], "198902": [[1, 1064]], "198903": [[1, 717], [718, 723], [724, 724]], "198954": [[278, 282]], "198955": [[1553, 1554]], "199008": [[483, 484], [645, 648], [649, 650]], "199011": [[28, 31], [32, 32]], "199021": [[534, 534]], "199276": [[11, 372], [373, 373], [374, 1238]], "199282": [[55, 1469], [1470, 1476], [1477, 1477]], "199306": [[21, 27], [28, 566]], "199318": [[139, 150]], "199319": [[998, 1008]], "199428": [[383, 384], [385, 385], [386, 386], [649, 652]], "199435": [[1611, 1618]], "199436": [[255, 256]], "199563": [[6, 6], [7, 8]], "199564": [[5, 11]], "199565": [[2, 20]], "199566": [[33, 35]], "199569": [[368, 369]], "199570": [[18, 22]], "199571": [[562, 564], [565, 565]], "199572": [[318, 318]], "199699": [[757, 765], [766, 767]], "199739": [[175, 176]], "199751": [[131, 137]], "199752": [[323, 328], [329, 329]], "199753": [[60, 61]], "199832": [[287, 292]], "199833": [[1240, 1243]], "199862": [[143, 146]], "199867": [[321, 330], [331, 331]], "199876": [[377, 382]], "199960": [[486, 488]], "199967": [[171, 171]], "200041": [[1241, 1241], [1242, 1244]], "200042": [[537, 549]], "200091": [[1000, 1000], [1712, 1720]], "200160": [[69, 73]], "200174": [[85, 88]], "200177": [[57, 61]], "200180": [[19, 22]], "200186": [[25, 33], [34, 34]], "200188": [[353, 356]], "200190": [[257, 257], [566, 566]], "200228": [[130, 136]], "200229": [[34, 35], [36, 40]], "200243": [[140, 150]], "200244": [[620, 626], [627, 628]], "200245": [[379, 382]], "200515": [[184, 189]], "200525": [[991, 1000]], "200599": [[138, 140]], "200600": [[1140, 1140], [1143, 1144], [1145, 1146], [1148, 1151], [1152, 1153], [1154, 1157]], "200601": [[11, 13]], "200990": [[144, 148], [149, 150]], "200991": [[1050, 1054]], "201114": [[15, 22]], "201159": [[212, 215]], "201164": [[419, 421], [422, 427]], "201173": [[587, 597]], "201174": [[452, 459]], "201191": [[1383, 1383], [1387, 1387]], "201193": [[20, 24]], "201195": [[1, 7]], "201196": [[842, 843], [844, 844]], "201197": [[24, 34]], "201199": [[7, 10]], "201200": [[185, 189]], "201228": [[34, 35]], "201611": [[126, 128], [145, 145], [147, 147], [158, 158], [163, 163], [165, 165], [170, 172], [174, 189]], "201624": [[271, 278]], "201625": [[759, 759]], "201657": [[109, 109], [119, 126]], "201668": [[158, 163]], "201669": [[166, 167], [168, 169], [170, 174]], "201671": [[463, 463], [1008, 1014]], "201678": [[121, 125]], "201705": [[188, 190]], "201706": [[38, 38], [49, 62]], "201707": [[965, 974]], "201727": [[238, 240]], "201794": [[121, 125]], "201812": [[5, 12]], "201813": [[12, 12]], "201815": [[5, 25], [27, 47]], "201816": [[158, 162]], "201817": [[275, 279]], "201818": [[3, 9]], "201819": [[242, 242]], "202012": [[122, 125], [132, 132], [133, 133], [134, 136]], "202013": [[36, 37], [58, 60]], "202014": [[6, 7], [15, 15], [19, 19], [78, 78], [103, 103], [197, 202]], "202044": [[467, 472]], "202045": [[810, 821], [826, 831]], "202047": [[5, 9]], "202074": [[175, 179]], "202084": [[240, 247]], "202086": [[1, 9]], "202087": [[1094, 1100]], "202088": [[287, 290]], "202116": [[97, 97]], "202207": [[2, 8]], "202208": [[2, 5]], "202328": [[615, 619], [620, 620]], "202469": [[353, 356]], "202472": [[113, 114]], "202792": [[317, 1014]], "202793": [[46, 218]], "202794": [[38, 284]], "202970": [[174, 175]], "202972": [[601, 601]], "203708": [[108, 189]], "203830": [[183, 189]], "203832": [[12, 22]], "203833": [[129, 137]], "203834": [[41, 47]], "203835": [[359, 366], [367, 368]], "203909": [[383, 393]], "203981": [[122, 125]], "203986": [[46, 47]], "203987": [[1015, 1027]], "203991": [[4, 5]], "203992": [[16, 20]], "204100": [[140, 150]], "204101": [[84, 84], [85, 85]], "204113": [[691, 692], [694, 695], [697, 704]], "204506": [[511, 511]], "204544": [[427, 433]], "204551": [[4, 7], [8, 9]], "204552": [[10, 12], [13, 15]], "204553": [[102, 102], [105, 107]], "204554": [[515, 517], [518, 520]], "204563": [[572, 577]], "204565": [[49, 50]], "204566": [[13, 22]], "204576": [[302, 305]], "204599": [[336, 337]], "204600": [[3, 9]], "204900": [[97, 408]], "205193": [[903, 910]], "205214": [[1, 3], [4, 6]], "205215": [[1, 2], [3, 3], [4, 9]], "205217": [[323, 323]], "205233": [[154, 155], [156, 157]], "205235": [[1, 9]], "205236": [[353, 355]], "205238": [[305, 305]], "205310": [[560, 562]], "205339": [[405, 415]], "205515": [[217, 218]], "205519": [[473, 485]], "205526": [[346, 348], [349, 354], [355, 356]], "205595": [[20, 22]], "205598": [[10, 19], [20, 21], [22, 22]], "205599": [[6, 9]], "205605": [[66, 72], [73, 74]], "205611": [[1, 9]], "205614": [[41, 46]], "205617": [[548, 552]], "205618": [[13, 22]], "205620": [[207, 207], [208, 212], [213, 215]], "205666": [[740, 743]], "205683": [[306, 307]], "205690": [[41, 48]], "205774": [[81, 82], [83, 83], [84, 86]], "205777": [[10, 10]], "205833": [[367, 368]], "206088": [[287, 290]], "206098": [[84, 85]], "206187": [[709, 717]], "206208": [[548, 548]], "206243": [[924, 924]], "206245": [[63, 76]], "206246": [[1291, 1296]], "206251": [[1, 212], [213, 213], [214, 215]], "206257": [[30, 37]], "206297": [[79, 80]], "206302": [[277, 282]], "206303": [[287, 295]], "206389": [[393, 410]], "206390": [[198, 205]], "206446": [[1150, 1153]], "206448": [[1232, 1234]], "206476": [[130, 132], [142, 142], [220, 227]], "206477": [[15, 15], [32, 32], [52, 52], [185, 190]], "206512": [[1212, 1214], [1215, 1216]], "206539": [[61, 64]], "206572": [[48, 51]], "206573": [[15, 24]], "206574": [[88, 101]], "206575": [[77, 77]], "206594": [[282, 294]], "206595": [[194, 204]], "206596": [[811, 815]], "206598": [[720, 724]], "206744": [[478, 482], [483, 484], [485, 487]], "206745": [[1997, 2006]], "206859": [[849, 852], [853, 853], [854, 860]], "206867": [[14, 19], [20, 21]], "206868": [[17, 24]], "206897": [[35, 37], [62, 62], [103, 108], [110, 110], [113, 113], [132, 132], [138, 140]], "206901": [[99, 102]], "206906": [[32, 37], [95, 95], [150, 150], [176, 176], [219, 220], [221, 221]], "206939": [[209, 217]], "207099": [[1172, 1179]], "207214": [[845, 846], [847, 857]], "207219": [[113, 114]], "207220": [[161, 166]], "207221": [[103, 110]], "207231": [[85, 85], [122, 122], [1506, 1513]], "207269": [[578, 578]], "207320": [[351, 358]], "207371": [[125, 137]], "207397": [[180, 192]], "207468": [[82, 89]], "207487": [[473, 475]], "207488": [[441, 447]], "207490": [[143, 143], [144, 150]], "207491": [[459, 460], [461, 463], [464, 464]], "207515": [[1212, 1213], [1214, 1218]], "207517": [[60, 71]], "207871": [[40, 719]], "207882": [[46, 50]], "207883": [[2, 2], [76, 76]], "207884": [[184, 192]], "207885": [[91, 94]], "207886": [[167, 167], [172, 179]], "207887": [[31, 37]], "207889": [[304, 305]], "207897": [[143, 145]], "207898": [[289, 295]]})

    print 'run range for', latest_dataset, ':', runrange[0], runrange[-1]
    print 'these lumis are in the DCS-only JSON but not (yet) in', latest_dataset
    print str(dcs_ll - ll - ok)

elif cmd == 'drawall':
    extra = extra[0] if extra else ''
    for lumi_mask in lumi_masks:
        r = do('python draw.py data/ana_datamc_%s %s > out.draw.%s' % (lumi_mask,extra,lumi_mask))
        if r != 0:
            sys.exit(r)
    do('mv out.draw.* plots/')
    do('tlock ~/asdf/plots.tgz plots/datamc_* plots/out.draw.*')

else:
    raise ValueError('command %s not recognized!' % cmd)
