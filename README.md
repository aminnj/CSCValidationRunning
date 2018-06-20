# CSCValidationRunning
Repository to setup the latest version of CSCValidation to be run automatically on CAF

## Setup

Setup a new release
```
cmsrel CMSSW_10_1_5 # or current CMSSW version
cd CMSSW_10_1_5/src
cmsenv
git cms-init # make sure this happens before checking out this repository
```

Clone the repository and set up
```
git clone git@github.com:aminnj/CSCValidationRunning.git
cd CSCValidationRunning
git checkout me42
./setup.sh
```

## Standalone instructions

While the full-blown machinery of CSCValidation can be used (see README from parent of this forked repo),
this is intended to be just a lcoal copy of CSCValidation with simple submission. Simple submission
with CRAB can be found in
`cd RecoLocalMuon/CSCValidation/batch/`

Instructions for editing and analyzing are in `RecoLocalMuon/CSCValidation/src`
