#include "RecoLocalMuon/CSCValidation/src/CSCValHists.h"
#include <algorithm>

using namespace std;


  CSCValHists::CSCValHists(){

    std::cout << "Initializing Histogram Manager..." << std::endl;

  }  


  CSCValHists::~CSCValHists(){

  }


  void CSCValHists::writeHists(TFile* theFile){

    std::vector<std::string> theFolders;
    std::vector<std::string>::iterator fit;
    theFile->cd();

    std::map<std::string,std::pair<TH1*,string> >::const_iterator mapit;
    for (mapit = theMap.begin(); mapit != theMap.end(); mapit++){
      std::string folder = (*mapit).second.second.c_str();
      fit = find(theFolders.begin(), theFolders.end(), folder);
      if (fit == theFolders.end()){
        theFolders.push_back(folder);
        theFile->mkdir(folder.c_str());
      }
      theFile->cd((*mapit).second.second.c_str());
      (*mapit).second.first->Write();
      theFile->cd();
    }

  }

  
  void CSCValHists::writeTrees(TFile* theFile){

    theFile->cd("recHits");
    rHTree->Write();
    theFile->cd();

    theFile->cd("Segments");
    segTree->Write();
    theFile->cd();


  }

  
  void CSCValHists::setupTrees(){

    // Create the root tree to hold position info
    rHTree  = new TTree("rHPositions","Local and Global reconstructed positions for recHits");
    segTree = new TTree("segPositions","Local and Global reconstructed positions for segments");

    // Create a branch on the tree
    rHTree->Branch("rHpos",&rHpos,"endcap/I:station/I:ring/I:chamber/I:layer/I:localx/F:localy/F:globalx/F:globaly/F");
    segTree->Branch("seg",&segpos,"event/I:run/I:lumi/I:endcap/I:station/I:ring/I:chamber/I:layer/I:nhits/I:localx/F:localy/F:globalx/F:globaly/F:segDirX/F:segDirY/F:segDirZ/F:eta/F:phi/F:chi2prob/F:time/F:tbincode/I:theta/F:globTheta/F:nSegments/I:nME42Segments/I:dOpposingR/F:dOpposingPhi/F:nbh/F:station2Ring/F:station2Chamber/F:station3Ring/F:station3Chamber/F:station4Ring/F:station4Chamber/F:station2globX/F:station2globY/F:station3globX/F:station3globY/F:station4globX/F:station4globY/F:station2HasSeg/I:station3HasSeg/I:station4HasSeg/I");
    segTree->Branch("tbins",&seg_tbins);

  }

  
  void CSCValHists::fillRechitTree(float x, float y, float gx, float gy, int en, int st, int ri, int ch, int la){

    // Fill the rechit position branch
    rHpos.localx  = x;
    rHpos.localy  = y;
    rHpos.globalx = gx;
    rHpos.globaly = gy;
    rHpos.endcap  = en;
    rHpos.ring    = ri;
    rHpos.station = st;
    rHpos.chamber = ch;
    rHpos.layer   = la;
    rHTree->Fill();

  }
  
  void CSCValHists::fillSegmentTree(
          unsigned int event, int run, int lumi,
          float x, float y, 
          float gx, float gy, 
          float segDirX, float segDirY, float segDirZ,
          int en, int st, int ri, int ch,
          int nhits,
          float eta, float phi,
          float chi2prob,
          float time,
          // float tslope, int wgbx, float avglayer,
          int tbincode,
          std::vector<int> tbins,
          // int* tbins,
          float theta, float globTheta,
          int nSegments, int nME42Segments,
          float dOpposingR, float dOpposingPhi, int nbh,
          float station2Ring, float station2Chamber,
          float station3Ring, float station3Chamber,
          float station4Ring, float station4Chamber,
          float station2globX, float station2globY,
          float station3globX, float station3globY,
          float station4globX, float station4globY,
          int station2HasSeg, int station3HasSeg, int station4HasSeg
          ){

              // std::cout <<  " st: " << st <<  " ri: " << ri <<  " tbins.size(): " << tbins.size() <<  std::endl;

    // Fill the segment position branch
    segpos.event = event;
    segpos.run = run;
    segpos.lumi = lumi;
    segpos.localx  = x;
    segpos.localy  = y;
    segpos.globalx = gx;
    segpos.globaly = gy;
    segpos.segDirX = segDirX;
    segpos.segDirY = segDirY;
    segpos.segDirZ = segDirZ;
    segpos.endcap  = en;
    segpos.ring    = ri;
    segpos.station = st;
    segpos.chamber = ch;
    segpos.nhits = nhits;
    segpos.layer   = 0;
    segpos.eta = eta;
    segpos.phi = phi;
    segpos.chi2prob = chi2prob;
    segpos.time = time;
    // segpos.tslope = tslope;
    // segpos.wgbx = wgbx;
    // segpos.avglayer = avglayer;
    segpos.tbincode = tbincode;
    // segpos.tbins = tbins;
    segpos.theta = theta;
    segpos.globTheta = globTheta;
    segpos.nSegments = nSegments;
    segpos.nME42Segments = nME42Segments;
    segpos.dOpposingR = dOpposingR;
    segpos.dOpposingPhi = dOpposingPhi;
    segpos.nbh = nbh;
    segpos.station2Ring = station2Ring;
    segpos.station2Chamber = station2Chamber;
    segpos.station3Ring = station3Ring;
    segpos.station3Chamber = station3Chamber;
    segpos.station4Ring = station4Ring;
    segpos.station4Chamber = station4Chamber;
    segpos.station2globX = station2globX;
    segpos.station2globY = station2globY;
    segpos.station3globX = station3globX;
    segpos.station3globY = station3globY;
    segpos.station4globX = station4globX;
    segpos.station4globY = station4globY;
    segpos.station2HasSeg = station2HasSeg;
    segpos.station3HasSeg = station3HasSeg;
    segpos.station4HasSeg = station4HasSeg;

    seg_tbins.clear();
    for (auto tbin : tbins) {
        seg_tbins.push_back(tbin);
    }

    segTree->Fill();

  }

  void CSCValHists::insertPlot(TH1* thePlot, std::string name, std::string folder){

    theMap[name] = std::pair<TH1*,string>(thePlot, folder);

  }
  

  void CSCValHists::fillCalibHist(float x, std::string name, std::string title, int bins, float xmin, float xmax,
                                  int bin, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH1I(name.c_str(),title.c_str(),bins,xmin,xmax), folder);
    }

    theMap[name].first->SetBinContent(bin,x);

  }


  void CSCValHists::fill1DHist(float x, std::string name, std::string title,
                               int bins, float xmin, float xmax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH1I(name.c_str(),title.c_str(),bins,xmin,xmax), folder);
    }


    theMap[name].first->Fill(x);

  }


  void CSCValHists::fill2DHist(float x, float y, std::string name, std::string title,
                               int binsx, float xmin, float xmax,
                               int binsy, float ymin, float ymax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax),folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fill1DHistByType(float x, std::string name, std::string title, CSCDetId id,
                                     int bins, float xmin, float xmax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << endcap << id.station() << id.ring();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax), folder);
    }

    theMap[name].first->Fill(x);

  }

  void CSCValHists::fill2DHistByType(float x, float y, std::string name, std::string title, CSCDetId id,
                                   int binsx, float xmin, float xmax,
                                   int binsy, float ymin, float ymax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << endcap << id.station() << id.ring();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax), folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fill1DHistByCrate(float x, string name, string title, CSCDetId id,
                                     int bins, float xmin, float xmax, string folder){

    int crate = crate_lookup(id); 
  
    map<string,pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_crate_" << crate;
    oss2 << title << "  (crate " << crate << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = pair<TH1*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax), folder);
    }
  
    theMap[name].first->Fill(x);
  
  }

  void CSCValHists::fill2DHistByCrate(float x, float y, string name, string title, CSCDetId id,
				      int binsx, float xmin, float xmax,
				      int binsy, float ymin, float ymax, string folder){
    
    int crate = crate_lookup(id); 
     
    map<string,pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_crate_" << crate;
    oss2 << title << "  (crate " << crate << ")";   
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax), folder);
    }
    
    theMap[name].first->Fill(x,y);
    
  }



  void CSCValHists::fill1DHistByStation(float x, string name, string title, CSCDetId id,
                                        int bins, float xmin, float xmax, string folder){

    string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    map<string,pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << endcap << id.station();
    oss2 << title << "  (Station " << endcap << id.station() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = pair<TH1*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax), folder);
    }

    theMap[name].first->Fill(x);

  }


  void CSCValHists::fill2DHistByStation(float x, float y, string name, string title, CSCDetId id,
                                        int binsx, float xmin, float xmax,
                                        int binsy, float ymin, float ymax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << endcap << id.station();
    oss2 << title << "  (Station " << endcap << id.station() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax), folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fill1DHistByChamber(float x, std::string name, std::string title, CSCDetId id,
                                       int bins, float xmin, float xmax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << endcap << id.station() << "_" << id.ring() << "_" << id.chamber();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << "/" << id.chamber() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax),folder);
    }

    theMap[name].first->Fill(x);

  }


  void CSCValHists::fill2DHistByChamber(float x, float y, std::string name, std::string title, CSCDetId id,
                                        int binsx, float xmin, float xmax,
                                        int binsy, float ymin, float ymax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << endcap << id.station() << "_" << id.ring() << "_" << id.chamber();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << "/" << id.chamber() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax),folder);
    }

    theMap[name].first->Fill(x,y);

  }

  void CSCValHists::fill2DHistByEvent(int run, int event, float z, string name, string title, CSCDetId id, string folder){

    string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";
    
    map<string,pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << run  << "_" << event ;
    oss2 << title << "  ( Run: " << run  << " Event: " << event  << " )";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),36,0.5,36.5,18,0.5,18.5),folder);
    }

    int x = id.chamber();
    int y = id.ring();
    if (y==4) y =1; //collapsing ME1/1a into ME1/1
    if (id.station() >1)
      y = y + 3 + (id.station()-2)*2;
    
    if (id.endcap()==1)
      y = y+9;
    else
      y = -1*y+10;
    
    dynamic_cast<TH2F*>(theMap[name].first)->Fill(x,y,z);
    
  }

  void CSCValHists::fill2DHist(float z, string name, string title, CSCDetId id, string folder){

    map<string,pair<TH1*,string> >::iterator it;
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),36,0.5,36.5,18,0.5,18.5),folder);
    }

    int x = id.chamber();
    int y = id.ring();
    if (y==4) y =1; //collapsing ME1/1a into ME1/1
    if (id.station() >1)
      y = y + 3 + (id.station()-2)*2;

    if (id.endcap()==1)
      y = y+9;
    else
      y = -1*y+10;

    dynamic_cast<TH2F*>(theMap[name].first)->Fill(x,y,z);

  }

  void CSCValHists::fill1DHistByLayer(float x, string name, string title, CSCDetId id,
                                      int bins, float xmin, float xmax, string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << endcap << id.station() << "_" << id.ring() << "_" << id.chamber() << "_L" << id.layer();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << "/" << id.chamber() << "/L" << id.layer() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH1F(name.c_str(),title.c_str(),bins,xmin,xmax),folder);
    }

    theMap[name].first->Fill(x);

  }


  void CSCValHists::fill2DHistByLayer(float x, float y, std::string name, std::string title, CSCDetId id,
                                      int binsx, float xmin, float xmax,
                                      int binsy, float ymin, float ymax, std::string folder){

    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << endcap << id.station() << "_" << id.ring() << "_" << id.chamber() << "_L" << id.layer();;
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << "/" << id.chamber() << "/L" << id.layer() << ")";
    name = oss1.str();
    title = oss2.str();
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TH1*,string>(new TH2F(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax),folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fillProfile(float x, float y, std::string name, std::string title,
                                int binsx, float xmin, float xmax,
                                float ymin, float ymax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TProfile*,string>(new TProfile(name.c_str(),title.c_str(),binsx,xmin,xmax,ymin,ymax), folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fillProfileByType(float x, float y, std::string name, std::string title, CSCDetId id,
                                      int binsx, float xmin, float xmax,
                                      float ymin, float ymax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << endcap << id.station() << id.ring();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << ")";
    name = oss1.str();
    title = oss2.str();

    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TProfile*,string>(new TProfile(name.c_str(),title.c_str(),binsx,xmin,xmax,ymin,ymax), folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fillProfileByChamber(float x, float y, std::string name, std::string title, CSCDetId id,
                                         int binsx, float xmin, float xmax,
                                         float ymin, float ymax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;
    std::string endcap;
    if (id.endcap() == 1) endcap = "+";
    if (id.endcap() == 2) endcap = "-";

    ostringstream oss1;
    ostringstream oss2;
    oss1 << name << "_" << endcap << id.station() << "_" << id.ring() << "_" << id.chamber();
    oss2 << title << "  (ME " << endcap << id.station() << "/" << id.ring() << "/" << id.chamber() << ")";
    name = oss1.str();
    title = oss2.str();

    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TProfile*,string>(new TProfile(name.c_str(),title.c_str(),binsx,xmin,xmax,ymin,ymax), folder);
    }

    theMap[name].first->Fill(x,y);

  }


  void CSCValHists::fill2DProfile(float x, float y, float z, std::string name, std::string title,
                                  int binsx, float xmin, float xmax,
                                  int binsy, float ymin, float ymax,
                                  float zmin, float zmax, std::string folder){

    std::map<std::string,std::pair<TH1*,string> >::iterator it;

    it = theMap.find(name);
    if (it == theMap.end()){
      theMap[name] = std::pair<TProfile2D*,string>(new TProfile2D(name.c_str(),title.c_str(),binsx,xmin,xmax,binsy,ymin,ymax,zmin,zmax), folder);
    }

    TProfile2D *tempp = (TProfile2D*)theMap[name].first;
    tempp->Fill(x,y,z);

  }

int CSCValHists::crate_lookup(CSCDetId id){

  int crate = 0;

  if (id.station() == 1){
    if (id.chamber() == 36 || id.chamber() == 1  || id.chamber() == 2  ) crate = 1;
    if (id.chamber() == 3  || id.chamber() == 4  || id.chamber() == 5  ) crate = 2;
    if (id.chamber() == 6  || id.chamber() == 7  || id.chamber() == 8  ) crate = 3;
    if (id.chamber() == 9  || id.chamber() == 10 || id.chamber() == 11 ) crate = 4;
    if (id.chamber() == 12 || id.chamber() == 13 || id.chamber() == 14 ) crate = 5;
    if (id.chamber() == 15 || id.chamber() == 16 || id.chamber() == 17 ) crate = 6;
    if (id.chamber() == 18 || id.chamber() == 19 || id.chamber() == 20 ) crate = 7;
    if (id.chamber() == 21 || id.chamber() == 22 || id.chamber() == 23 ) crate = 8;
    if (id.chamber() == 24 || id.chamber() == 25 || id.chamber() == 26 ) crate = 9;
    if (id.chamber() == 27 || id.chamber() == 28 || id.chamber() == 29 ) crate = 10;
    if (id.chamber() == 30 || id.chamber() == 31 || id.chamber() == 32 ) crate = 11;
    if (id.chamber() == 33 || id.chamber() == 34 || id.chamber() == 35 ) crate = 12;
  }
  else{
    crate = 12 + id.triggerSector() + (id.station()-2)*6;
  }

  if (id.endcap() == 2) 
    crate = crate+30;

  return crate;

}
