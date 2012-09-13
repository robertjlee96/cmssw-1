from DQM.EcalBarrelMonitorTasks.PresampleTask_cfi import ecalPresampleTask

ecalPresampleClient = dict(
    minChannelEntries = 3,
    minTowerEntries = 30,
    expectedMean = 200.,
    toleranceMean = 25.,
    toleranceRMS = 3.,
    toleranceRMSFwd = 6.,
    MEs = dict(
        Quality = dict(path = "Presample/Quality/PresampleClient presample quality", otype = 'SM', btype = 'Crystal', kind = 'TH2F'),
        Mean = dict(path = "Presample/Mean/PresampleClient mean", otype = 'SM', btype = 'User', kind = 'TH1F', xaxis = {'nbins': 120, 'low': 170., 'high': 230.}),
        MeanDCC = dict(path = "Presample/Mean/PresampleClient DCC mean", otype = 'Ecal2P', btype = 'DCC', kind = 'TProfile', yaxis = {'nbins': 120, 'low': 170., 'high': 230.}),
        RMS = dict(path = "Presample/RMS/PresampleClient rms", otype = 'SM', btype = 'User', kind = 'TH1F', xaxis = {'nbins': 100, 'low': 0., 'high': 10.}),
        RMSMap = dict(path = "Presample/RMSMap/PresampleClient rms", otype = 'Ecal2P', btype = 'Crystal', kind = 'TH2F'),
        QualitySummary = dict(path = "Summary/PresampleClient presample quality", otype = 'Ecal2P', btype = 'Crystal', kind = 'TH2F'),
        TrendMean = dict(path = 'Trend/PresampleClient presample mean max - min', otype = 'Ecal2P', btype = 'Trend', kind = 'TProfile'),
        TrendRMS = dict(path = 'Trend/PresampleClient presample rms max', otype = 'Ecal2P', btype = 'Trend', kind = 'TProfile')
    ),
    sources = dict(
        Pedestal = ecalPresampleTask['MEs']['Pedestal']
    )
)

