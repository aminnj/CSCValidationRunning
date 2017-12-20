import FWCore.ParameterSet.Config as cms
import subprocess

from Configuration.StandardSequences.Eras import eras
process = cms.Process("TEST",eras.Run2_2016)

process.load("Configuration/StandardSequences/GeometryDB_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_Prompt_v8', '')

inputFiles = cms.untracked.vstring(
# "/store/relval/CMSSW_9_1_1/RelValTTbar_14TeV/GEN-SIM-RECO/PU25ns_91X_upgrade2023_realistic_v1_D17PU140-v1/10000/F2A0BF44-6C40-E711-A8A5-0CC47A4D76B6.root",
# "file:relvalttbarpu140.root",
# "/store/data/Run2017D/ZeroBiasNominalTrains5/RECO/PromptReco-v1/000/302/674/00000/AC53BD8B-F299-E711-8DEB-02163E019B2D.root",
# /SingleMuon/Run2017D-v1/RAW
# "/store/data/Run2017D/SingleMuon/RAW/v1/000/302/159/00000/569EC0CD-B48E-E711-A3F5-02163E0126BE.root"
# http://uaf-10.t2.ucsd.edu/~namin/makers/disMaker/?query=%2FZeroBiasNominalTrains1%2FRun2017D-v1%2FRAW&type=files&short=short
# "/store/data/Run2017D/ZeroBiasNominalTrains1/RAW/v1/000/302/674/00000/DCEC35D6-6C98-E711-ABD6-02163E0144D5.root"
"file:DCEC35D6-6C98-E711-ABD6-02163E0144D5.root"
# /Cosmics/Run2017D-v1/RAW
# "/store/data/Run2017D/Cosmics/RAW/v1/000/302/046/00000/9063B133-CE8D-E711-B5D3-02163E019BBF.root"
)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(9000) )
process.options = cms.untracked.PSet( SkipEvent =
cms.untracked.vstring('ProductNotFound') )
readFiles = cms.untracked.vstring(

)
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",
    fileNames = inputFiles
)
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
        FwkJob = cms.untracked.PSet( limit = cms.untracked.int32(0) )
    ),
    categories = cms.untracked.vstring('FwkJob'),
    destinations = cms.untracked.vstring('cout')
)


"""Customise digi/reco geometry to use unganged ME1/a channels"""
process.CSCGeometryESModule.useGangedStripsInME1a = False
process.idealForDigiCSCGeometry.useGangedStripsInME1a = False


# filter on trigger path
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(''),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "gtDigis" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)

process.cscValidation = cms.EDAnalyzer("CSCValidation",
    rootFileName = cms.untracked.string('output.root'),
    isSimulation = cms.untracked.bool(False),
    writeTreeToFile = cms.untracked.bool(True),
    useDigis = cms.untracked.bool(True),
    makeRecHitPlots = cms.untracked.bool(False),
    detailedAnalysis = cms.untracked.bool(False),
    useTriggerFilter = cms.untracked.bool(False),
    useQualityFilter = cms.untracked.bool(False),
    makeStandalonePlots = cms.untracked.bool(False),
    makeTimeMonitorPlots = cms.untracked.bool(False),
    scalersTag = cms.InputTag("scalersRawToDigi"),
    rawDataTag = cms.InputTag("rawDataCollector"),# FOR RAW RUNS
    alctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
    clctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCCLCTDigi"),
    corrlctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
    stripDigiTag = cms.InputTag("muonCSCDigis","MuonCSCStripDigi"),
    wireDigiTag = cms.InputTag("muonCSCDigis","MuonCSCWireDigi"),
    compDigiTag = cms.InputTag("muonCSCDigis","MuonCSCComparatorDigi"),
    cscRecHitTag = cms.InputTag("csc2DRecHits"),
    cscSegTag = cms.InputTag("cscSegments"),
    saMuonTag = cms.InputTag("standAloneMuons"),
    l1aTag = cms.InputTag("gtDigis"),
    hltTag = cms.InputTag("TriggerResults::HLT"),
    makeHLTPlots = cms.untracked.bool(True),
    simHitTag = cms.InputTag("g4SimHits", "MuonCSCHits")
)


process.load("L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi")
process.cscTriggerPrimitiveDigis.CSCComparatorDigiProducer = "muonCSCDigis:MuonCSCComparatorDigi"
process.cscTriggerPrimitiveDigis.CSCWireDigiProducer = "muonCSCDigis:MuonCSCWireDigi"
process.cscTriggerPrimitiveDigis.tmbParam.mpcBlockMe1a = 0
process.load("L1TriggerConfig.L1CSCTPConfigProducers.L1CSCTriggerPrimitivesConfig_cff")
process.l1csctpconf.alctParamMTCC2.alctNplanesHitPretrig = 3
process.l1csctpconf.alctParamMTCC2.alctNplanesHitAccelPretrig = 3
process.l1csctpconf.clctParam.clctNplanesHitPretrig = 3
process.l1csctpconf.clctParam.clctHitPersist = 4

process.lctreader = cms.EDAnalyzer("CSCTriggerPrimitivesReader",
    debug = cms.untracked.bool(False),
    dataLctsIn = cms.bool(True),
    emulLctsIn = cms.bool(True),
    isMTCCData = cms.bool(False),
    printps = cms.bool(False),
    CSCLCTProducerData = cms.untracked.string("muonCSCDigis"),
    CSCLCTProducerEmul = cms.untracked.string("cscTriggerPrimitiveDigis"),
    CSCSimHitProducer = cms.InputTag("g4SimHits", "MuonCSCHits"),  # Full sim.
    CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"),
    CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")
)

# CSCTF stuff
process.load("DQMServices.Components.DQMEnvironment_cfi")

#process.load("DQM.L1TMonitor.environment_file_cff")
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.dqmSaver.convention = 'Online'
process.dqmSaver.referenceHandling = 'all'
process.dqmSaver.dirName = '.'
process.dqmSaver.producer = 'DQM'
process.dqmSaver.saveByLumiSection = -1
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True
process.load("DQM.Integration.config.dqmPythonTypes")
es_prefer_GlobalTag = cms.ESPrefer('GlobalTag')
process.load("DQM.L1TMonitor.L1TMonitor_cff")
process.load("DQM.L1TMonitor.L1TSync_cff")
process.load("DQM.L1TMonitorClient.L1TMonitorClient_cff")
process.l1tSync.oracleDB = cms.string("oracle://cms_orcon_adg/CMS_COND_31X_L1T")
process.l1tSync.pathCondDB = cms.string("/afs/cern.ch/cms/DB/conddb/ADG")
process.l1tRate.oracleDB = cms.string("oracle://cms_orcon_adg/CMS_COND_31X_L1T")
process.l1tRate.pathCondDB = cms.string("/afs/cern.ch/cms/DB/conddb/ADG")
process.l1tMonitorPath = cms.Path(process.l1tMonitorOnline)
process.l1tMonitorClientPath = cms.Path(process.l1tMonitorClient)
process.l1tSyncPath = cms.Path(process.l1tSyncHltFilter+process.l1tSync)
process.l1tMonitorEndPath = cms.EndPath(process.l1tMonitorEndPathSeq)
process.l1tMonitorClientEndPath = cms.EndPath(process.l1tMonitorClientEndPathSeq)
process.l1tMonitorPath = cms.Path(process.csctfDigis * process.l1tCsctf)
process.dqmEndPath = cms.EndPath(
                                 process.dqmEnv *
                                 process.dqmSaver
                                 )
process.schedule = cms.Schedule(#process.rawToDigiPath,
                                process.l1tMonitorPath,
                                process.l1tSyncPath,
                                process.l1tMonitorClientPath,
                                #process.l1tMonitorEndPath,
                                process.l1tMonitorClientEndPath,
                                process.dqmEndPath
                                )
process.l1tMonitorOnline.remove(process.bxTiming)
process.l1tMonitorOnline.remove(process.l1tBPTX)
process.l1tMonitorOnline.remove(process.l1tDttf)
#process.l1tMonitorOnline.remove(process.l1tCsctf)
process.l1tMonitorOnline.remove(process.l1tRpctf)
process.l1tMonitorOnline.remove(process.l1tGmt)
process.l1tMonitorOnline.remove(process.l1tGt)
process.l1tMonitorOnline.remove(process.l1ExtraDqmSeq)
process.l1tMonitorOnline.remove(process.l1tRate)
process.l1tMonitorOnline.remove(process.l1tRctSeq)
process.l1tMonitorOnline.remove(process.l1tGctSeq)
#process.l1tMonitorEndPathSeq.remove(process.l1s)
#process.l1tMonitorEndPathSeq.remove(process.l1tscalers)
process.schedule.remove(process.l1tSyncPath)

process.l1tCsctf.gmtProducer = "null"
process.l1tCsctf.gangedME11a = False

from Configuration.StandardSequences.Eras import eras
eras.run2_common.toModify( process.l1tCsctf, gangedME11a = False )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('TPEHists_157.root')
                                   )

process.p = cms.Path(process.gtDigis * process.muonCSCDigis * process.csc2DRecHits * process.cscSegments * process.cscTriggerPrimitiveDigis * process.cscValidation)

process.Timing = cms.Service("Timing",
        summaryOnly = cms.untracked.bool(True)
        )

process.schedule.append(process.p)
