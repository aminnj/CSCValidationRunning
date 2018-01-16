from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'ZeroBias_Run2017D_NominalTrains1_v1'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ["output.root"]
config.JobType.psetName = '../test/pset_me42.py'

config.section_("Data")
config.Data.inputDataset = '/ZeroBiasNominalTrains1/Run2017D-v1/RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10

config.section_("Site")
config.Site.storageSite = 'T2_US_UCSD'
