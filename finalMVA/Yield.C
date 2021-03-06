#define Yield_cxx
#include <boost/algorithm/string/replace.hpp>

#include "Yield.h"
#include <TH2.h>
#include <TStyle.h>

void Yield::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
}

void Yield::SlaveBegin(TTree * /*tree*/)
{
  TString option = GetOption();
  string sample = option.Data();
  string era_s = sample.substr(sample.find_first_of("-")+1,4); //STTH1L3BHct_000-2017Hct_0101010101__syst
  string train_scheme = sample.substr(sample.find_first_of("-")+5,string::npos); //Hct_0101010101__syst
  sample.erase(sample.find_first_of("-"),string::npos); //STTH1L3BHct_000

  dosyst = true;
  if( option.Contains("Run201") ) dosyst = false;

  //Delete ntuple number and data era so that we can merge histos w.r.t. dataset, prepare assign ntuple
  int era = stoi(era_s);
  if( train_scheme.find("jec") != string::npos or train_scheme.find("jer") != string::npos 
    or train_scheme.find("TuneCP5") != string::npos or train_scheme.find("hdamp") != string::npos ){
    dosyst = false;//make another hist file
    if     ( train_scheme.find("jecup") != string::npos )       syst_ext = "jecup";
    else if( train_scheme.find("jecdown") != string::npos )     syst_ext = "jecdown";
    else if( train_scheme.find("jerup") != string::npos )       syst_ext = "jerup";
    else if( train_scheme.find("jerdown") != string::npos )     syst_ext = "jerdown";
    else if( train_scheme.find("TuneCP5up") != string::npos )   syst_ext = "TuneCP5up";
    else if( train_scheme.find("TuneCP5down") != string::npos ) syst_ext = "TuneCP5down";
    else if( train_scheme.find("hdampup") != string::npos )     syst_ext = "hdampup";
    else if( train_scheme.find("hdampdown") != string::npos )   syst_ext = "hdampdown";
    //Regrouped JEC V2
    else if( train_scheme.find("jecAbsoluteup") != string::npos )                     syst_ext = "jecAbsoluteup";
    else if( train_scheme.find("jecAbsolutedown") != string::npos )                   syst_ext = "jecAbsolutedown";
    else if( train_scheme.find(Form("jecAbsolute%iup",era)) != string::npos )         syst_ext = Form("jecAbsolute%iup",era);
    else if( train_scheme.find(Form("jecAbsolute%idown",era)) != string::npos )       syst_ext = Form("jecAbsolute%idown",era);
    else if( train_scheme.find("jecBBEC1up") != string::npos )                        syst_ext = "jecBBEC1up";
    else if( train_scheme.find("jecBBEC1down") != string::npos )                      syst_ext = "jecBBEC1down";
    else if( train_scheme.find(Form("jecBBEC1%iup",era)) != string::npos )            syst_ext = Form("jecBBEC1%iup",era);
    else if( train_scheme.find(Form("jecBBEC1%idown",era)) != string::npos )          syst_ext = Form("jecBBEC1%idown",era);
    else if( train_scheme.find("jecFlavorQCDup") != string::npos )                    syst_ext = "jecFlavorQCDup";
    else if( train_scheme.find("jecFlavorQCDdown") != string::npos )                  syst_ext = "jecFlavorQCDdown";
    else if( train_scheme.find("jecRelativeBalup") != string::npos )                  syst_ext = "jecRelativeBalup";
    else if( train_scheme.find("jecRelativeBaldown") != string::npos )                syst_ext = "jecRelativeBaldown";
    else if( train_scheme.find(Form("jecRelativeSample%iup",era)) != string::npos )   syst_ext = Form("jecRelativeSample%iup",era);
    else if( train_scheme.find(Form("jecRelativeSample%idown",era)) != string::npos ) syst_ext = Form("jecRelativeSample%idown",era);
  }
  string syst_str = "-" + train_scheme.substr(train_scheme.find_last_of("_")+1,string::npos );
  if( syst_ext == "" ) syst_str.erase(0,1);
  train_scheme.erase( train_scheme.find_last_of("_"),string::npos );

  if( !dosyst and syst_ext.length() < 5 and !option.Contains("Run201") ) cout << sample.c_str() << " " << "No external systematic option!" << endl;


  //cout << "SlaveBegin" << endl;
  for( int ich=0; ich < 3; ich++ ){
    for( int syst = 0; syst != syst_num; ++syst ){
      if( syst > 0 and !dosyst ) continue;

      h_Events[ich][syst] = new TH1D(Form("h_Events_Ch%i%s",ich,syst_name[syst]), "Events per category", 5, 0, 5);
      h_Events[ich][syst]->SetXTitle("Jet category");
      h_Events[ich][syst]->Sumw2();
      h_Events[ich][syst]->GetXaxis()->SetBinLabel(1,"b2j3");
      h_Events[ich][syst]->GetXaxis()->SetBinLabel(2,"b2j4");
      h_Events[ich][syst]->GetXaxis()->SetBinLabel(3,"b3j3");
      h_Events[ich][syst]->GetXaxis()->SetBinLabel(4,"b3j4");
      h_Events[ich][syst]->GetXaxis()->SetBinLabel(5,"b4j4");
      fOutput->Add(h_Events[ich][syst]);
    }

    for(int ijet=0; ijet<3; ijet++){
      bSFInfo[ich][ijet] = new TH1D(Form("bSFInfo_Ch%i_J%i",ich,ijet), "bSF info for normalization", 18, 0, 18);
      bSFInfo[ich][ijet]->SetXTitle("bSF Sum of weight");
      bSFInfo[ich][ijet]->Sumw2();
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(1,"Raw");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(2,"Nominal");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(3,"lfup");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(4,"lfdown");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(5,"hfup");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(6,"hfdown");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(7,"hfstat1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(8,"hfstat1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(9,"hfstat2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(10,"hfstat2down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(11,"lfstat1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(12,"lfstat1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(13,"lfstat2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(14,"lfstat2down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(15,"cferr1up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(16,"cferr1down");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(17,"cferr2up");
      bSFInfo[ich][ijet]->GetXaxis()->SetBinLabel(18,"cferr2down");
      fOutput->Add(bSFInfo[ich][ijet]);
    }
  }
} 

Bool_t Yield::Process(Long64_t entry)
{
  fReader.SetEntry(entry);
  TString option = GetOption();
  string sample = option.Data();

  int era = 0;
  TString current_file_name = Yield::fReader.GetTree()->GetCurrentFile()->GetName();
  if     ( current_file_name.Contains("V9") ) era = 2017;
  else if( current_file_name.Contains("V10") ) era = 2018;

  int mode = 999; 
  mode = *channel;

  if( mode > 2 ) return kTRUE;

  float wrongPVrate = 1;
  if( era == 2017 ){
    if( !option.Contains("Run2017") ){
      if     ( option.Contains("DYJets10to50") ) wrongPVrate = 1.02921;
      else if( option.Contains("QCDEM15to20") ) wrongPVrate = 1.01333;
      else if( option.Contains("QCDEM20to30") ) wrongPVrate = 1.01227;
      else if( option.Contains("QCDEM300toInf") ) wrongPVrate = 1.01194;
      else if( option.Contains("QCDEM50to80") ) wrongPVrate = 1.02226;
      else if( option.Contains("QCDMu120to170") ) wrongPVrate = 1.01289;
      else if( option.Contains("QCDMu170to300") ) wrongPVrate = 1.01181;
      else if( option.Contains("QCDMu20to30") ) wrongPVrate = 1.0253;
      else if( option.Contains("QCDMu30to50") ) wrongPVrate = 1.02105;
      else if( option.Contains("QCDMu470to600") ) wrongPVrate = 1.0141;
      else if( option.Contains("QCDMu50to80") ) wrongPVrate = 1.01149;
      else if( option.Contains("QCDMu80to120") ) wrongPVrate = 1.01278;
      else if( option.Contains("TTLLpowhegttbbhdampup") ) wrongPVrate = 1.01807;
      else if( option.Contains("TTLLpowhegttcchdampup") ) wrongPVrate = 1.01978;
      else if( option.Contains("TTLLpowhegttlfhdampup") ) wrongPVrate = 1.01938;
      else if( option.Contains("TTZToLLNuNu") ) wrongPVrate = 1.02425;
      else if( option.Contains("TTpowhegttbbTuneCP5down") ) wrongPVrate = 1.02715;
      else if( option.Contains("TTpowhegttbbhdampdown") ) wrongPVrate = 1.02717;
      else if( option.Contains("TTpowhegttccTuneCP5down") ) wrongPVrate = 1.0273;
      else if( option.Contains("TTpowhegttcchdampdown") ) wrongPVrate = 1.02746;
      else if( option.Contains("TTpowhegttlfTuneCP5down") ) wrongPVrate = 1.02742;
      else if( option.Contains("TTpowhegttlfhdampdown") ) wrongPVrate = 1.02774;
      else if( option.Contains("W3JetsToLNu") ) wrongPVrate = 1.02348;
      else if( option.Contains("WW") ) wrongPVrate = 1.0295;
      else if( option.Contains("WZ") ) wrongPVrate = 1.02298;
      else if( option.Contains("ZZ") ) wrongPVrate = 1.01508;
      else   wrongPVrate = 1.0;
    }
    if( wrongPVrate > 1.01 ){
      if( *TruePV <=0 ) return kTRUE;
    }
  }

  //Object selection
  int njets = 0;
  int nbjets_m = 0; 

  TLorentzVector lepton;
  lepton.SetPtEtaPhiE(*lepton_pt, *lepton_eta, *lepton_phi, *lepton_e);

  //Event selection 
  bool passmuon = (mode == 0) && (lepton.Pt() > 30) && (abs(lepton.Eta()) <= 2.4);
  bool passelectron = (mode == 1) && (lepton.Pt() > 30) && (abs(lepton.Eta()) <= 2.4);

  //Single Lepton only
  if( option.Contains("SingleMuon") ){
    if( !passmuon ) return kTRUE;//RDMu
    if( passelectron) return kTRUE;//RDMu
  }
  else if( option.Contains("SingleElectron") ){
    if( !passelectron ) return kTRUE;//RDelec
    if( passmuon ) return kTRUE;//RDelec
  }
  else{
    if( !passmuon && !passelectron ) return kTRUE;
  }
  //if( passmuon || passelectron ){

  float bWP_M;
  if      ( era == 2017 ) bWP_M = 0.4941;
  else if ( era == 2018 ) bWP_M = 0.4184;
  else                    bWP_M = 0.0;

  int jetIdx[4];
  TLorentzVector jetP4s[4];

  for (unsigned int iJet = 0; iJet < jet_pt.GetSize() ; ++iJet) {

    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt[iJet], jet_eta[iJet], jet_phi[iJet], jet_e[iJet]);

    if( !option.Contains("Run201") ){
      if     ( syst_ext == "jecup" )   jet = jet * jet_JER_Nom[iJet] * jet_JES_Up[iJet];
      else if( syst_ext == "jecdown" ) jet = jet * jet_JER_Nom[iJet] * jet_JES_Down[iJet];
      else if( syst_ext == "jerup" )   jet = jet * jet_JER_Up[iJet];
      else if( syst_ext == "jerdown" ) jet = jet * jet_JER_Down[iJet];
      else                             jet = jet * jet_JER_Nom[iJet];

      //ORDER DOES MATTER
      if     ( syst_ext == "jecAbsoluteup" )                     jet = jet * (1+jet_JESCom_Up[iJet].at(0));
      else if( syst_ext == "jecAbsolutedown" )                   jet = jet * (1-jet_JESCom_Down[iJet].at(0));
      else if( syst_ext == Form("jecAbsolute%iup",era) )         jet = jet * (1+jet_JESCom_Up[iJet].at(1));
      else if( syst_ext == Form("jecAbsolute%idown",era) )       jet = jet * (1-jet_JESCom_Down[iJet].at(1));
      else if( syst_ext == "jecBBEC1up" )                        jet = jet * (1+jet_JESCom_Up[iJet].at(2));
      else if( syst_ext == "jecBBEC1down" )                      jet = jet * (1-jet_JESCom_Down[iJet].at(2));
      else if( syst_ext == Form("jecBBEC1%iup",era) )            jet = jet * (1+jet_JESCom_Up[iJet].at(3));
      else if( syst_ext == Form("jecBBEC1%idown",era) )          jet = jet * (1-jet_JESCom_Down[iJet].at(3));
      else if( syst_ext == "jecFlavorQCDup" )                    jet = jet * (1+jet_JESCom_Up[iJet].at(4));
      else if( syst_ext == "jecFlavorQCDdown" )                  jet = jet * (1-jet_JESCom_Down[iJet].at(4));
      else if( syst_ext == "jecRelativeBalup" )                  jet = jet * (1+jet_JESCom_Up[iJet].at(5));
      else if( syst_ext == "jecRelativeBaldown" )                jet = jet * (1-jet_JESCom_Down[iJet].at(5));
      else if( syst_ext == Form("jecRelativeSample%iup",era) )   jet = jet * (1+jet_JESCom_Up[iJet].at(6));
      else if( syst_ext == Form("jecRelativeSample%idown",era) ) jet = jet * (1-jet_JESCom_Down[iJet].at(6));
    }

    if( jet.Pt() > 30 && abs(jet.Eta()) <= 2.4){
      njets++;
      if( jet_deepCSV[iJet] > bWP_M ) nbjets_m++;
    }
  }

  //Event selection is done here!
  if( njets < 3 ) return kTRUE;
  //if( nbjets_m < 2 or nbjets_m > 4 ) return kTRUE; commented out to compute sumW of bSF

  /////Fill histograms
  int modeArray[1] = {mode};
  int jetmode = 0; //for b SF normalization, 0 is useless in mva hist
  if     ( njets == 3 ) jetmode = 1;
  else if( njets >= 4 ) jetmode = 2;

  for( int MODE : modeArray ){
    for( int syst = 0; syst != syst_num; ++syst ){
      if( syst > 0 and !dosyst ) continue;

      float EventWeight = 1;
      //Multiply syst. to event weight
      if( !option.Contains("Run201") ){
        EventWeight *= wrongPVrate;
        EventWeight *= *genweight;
        if( era == 2017 ){
          //PrefireWeight
          if     ( isPartOf("prefireup", std::string(syst_name[syst])) )   EventWeight *= prefireweight[1];
          else if( isPartOf("prefiredown", std::string(syst_name[syst])) ) EventWeight *= prefireweight[2];
          else   EventWeight *= prefireweight[0];
        }
        //if( option.Contains("owheg") ) EventWeight *= *topptweight; //for now, apply only for ttbar
        //PUWight
        if     ( isPartOf("puup", std::string(syst_name[syst])) )   EventWeight *= PUWeight[1];
        else if( isPartOf("pudown", std::string(syst_name[syst])) ) EventWeight *= PUWeight[2];
        else   EventWeight *= PUWeight[0];
        //leptonSF
        if( passmuon ){
          //mu ID: 0, mu Iso: 1, mu Trg: 2
          //if     ( isPartOf("__muidup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[1];
          //else if( isPartOf("__muiddown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[2];
          //else   EventWeight *= lepton_SF[0];
          //if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[4];
          //else if( isPartOf("__muisodown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[5];
          //else   EventWeight *= lepton_SF[3];
          //if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[7];
          //else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[8];
          //else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__muidup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[0] + sqrt(pow(lepton_SF[1] - lepton_SF[0], 2) + pow(0.005, 2));
          else if( isPartOf("__muiddown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[0] - sqrt(pow(lepton_SF[2] - lepton_SF[0], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[0];
          if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[3] + sqrt(pow(lepton_SF[4] - lepton_SF[3], 2) + pow(0.005, 2));
          else if( isPartOf("__muisodown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[3] - sqrt(pow(lepton_SF[5] - lepton_SF[3], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[3];
          if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[6] + sqrt(pow(lepton_SF[7] - lepton_SF[6], 2) + pow(0.005, 2));
          else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) )
                 EventWeight *= lepton_SF[6] - sqrt(pow(lepton_SF[8] - lepton_SF[6], 2) + pow(0.005, 2));
          else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__elidup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__eliddown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elrecoup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__elrecodown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elzvtxup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__elzvtxdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__eltrgup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__eltrgdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
        }
        else if( passelectron ){
          //elec ID: 0, elec Reco: 1, elec Zvtx: 2
          if     ( isPartOf("__muidup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__muiddown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__muisoup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__muisodown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__mutrgup", std::string(syst_name[syst])) )   EventWeight *= 1.0;
          else if( isPartOf("__mutrgdown", std::string(syst_name[syst])) ) EventWeight *= 1.0;
          else   EventWeight *= 1.0;
          if     ( isPartOf("__elidup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[1];
          else if( isPartOf("__eliddown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[2];
          else   EventWeight *= lepton_SF[0];
          if     ( isPartOf("__elrecoup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[4];
          else if( isPartOf("__elrecodown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[5];
          else   EventWeight *= lepton_SF[3];
          if     ( isPartOf("__elzvtxup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[7];
          else if( isPartOf("__elzvtxdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[8];
          else   EventWeight *= lepton_SF[6];
          if     ( isPartOf("__eltrgup", std::string(syst_name[syst])) )   EventWeight *= lepton_SF[10];
          else if( isPartOf("__eltrgdown", std::string(syst_name[syst])) ) EventWeight *= lepton_SF[11];
          else   EventWeight *= lepton_SF[9];
        }
        //ME&PS
        //[0] = muF up , [1] = muF down, [2] = muR up, [3] = muR up && muF up, [4] = muR down, [5] = muF down && muF down
        if( option.Contains("TTpowheg") or option.Contains("TTLL") or option.Contains("TTHad")
          or option.Contains("STTH") or option.Contains("TTTH") ){
          //Scale weight
          if     ( isPartOf("scale0", std::string(syst_name[syst])) ) EventWeight *= scaleweight[0];
          else if( isPartOf("scale1", std::string(syst_name[syst])) ) EventWeight *= scaleweight[1];
          else if( isPartOf("scale2", std::string(syst_name[syst])) ) EventWeight *= scaleweight[2];
          else if( isPartOf("scale3", std::string(syst_name[syst])) ) EventWeight *= scaleweight[3];
          else if( isPartOf("scale4", std::string(syst_name[syst])) ) EventWeight *= scaleweight[4];
          else if( isPartOf("scale5", std::string(syst_name[syst])) ) EventWeight *= scaleweight[5];
          else   EventWeight *= 1;
          //PS weight
          if     ( isPartOf("ps0", std::string(syst_name[syst])) ) EventWeight *= psweight[0];//isr down
          else if( isPartOf("ps1", std::string(syst_name[syst])) ) EventWeight *= psweight[1];//fsr down
          else if( isPartOf("ps2", std::string(syst_name[syst])) ) EventWeight *= psweight[2];//isr up
          else if( isPartOf("ps3", std::string(syst_name[syst])) ) EventWeight *= psweight[3];//fsr up
          else   EventWeight *= 1;
          //PDF uncdertainty
          if( isPartOf("pdf", std::string(syst_name[syst])) ){
            string tmp_name = string(syst_name[syst]);
            int pdf_unc_idx = std::stoi(tmp_name.erase(0,5));
            float tmppdf = 1.0;
            if( option.Contains("STTH")
                 and pdf_unc_idx >= 0 and pdf_unc_idx < 31 ) tmppdf = pdfweight[pdf_unc_idx];
            else if( option.Contains("STTH") and pdf_unc_idx >= 31 ) tmppdf = 1.0;
            else if( pdf_unc_idx >= 0 and pdf_unc_idx < 103 ) tmppdf = pdfweight[pdf_unc_idx];
            EventWeight *= tmppdf;
          }
          //toppt reweight
          float top1weight = 1.0; float top2weight = 1.0;
          if( option.Contains("TTpowheg") or option.Contains("TTLL") or option.Contains("TTHad") ){
            top1weight = topPtNLOtoNNLO( *gentop1_pt );
            top2weight = topPtNLOtoNNLO( *gentop2_pt );
          }
          else if( option.Contains("TTTH") ){
            top1weight = topPtLOtoNLO( *gentop1_pt ) * topPtNLOtoNNLO( *gentop1_pt );
            top2weight = topPtLOtoNLO( *gentop2_pt ) * topPtNLOtoNNLO( *gentop2_pt );
          }
          if     ( top1weight != 1.0 and top2weight != 1.0 ) EventWeight *= sqrt(top1weight * top2weight);
          else if( top1weight == 1.0 or top2weight == 1.0 )  EventWeight *= top1weight * top2weight;
          else                                               EventWeight *= 1.0;
        }
        else EventWeight *= 1;
        //Deep CSV shape
        float bSF = 1.0;
        if     ( isPartOf("lfup",        std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[3];
          bSFInfo[MODE][jetmode]->Fill(2.5, EventWeight*bSF);
        }
        else if( isPartOf("lfdown",      std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[4];
          bSFInfo[MODE][jetmode]->Fill(3.5, EventWeight*bSF);
        }
        else if( isPartOf("hfup",        std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[5];
          bSFInfo[MODE][jetmode]->Fill(4.5, EventWeight*bSF);
        }
        else if( isPartOf("hfdown",      std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[6];
          bSFInfo[MODE][jetmode]->Fill(5.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat1up",   std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[7];
          bSFInfo[MODE][jetmode]->Fill(6.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat1down", std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[8];
          bSFInfo[MODE][jetmode]->Fill(7.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat2up",   std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[9];
          bSFInfo[MODE][jetmode]->Fill(8.5, EventWeight*bSF);
        }
        else if( isPartOf("hfstat2down", std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[10];
          bSFInfo[MODE][jetmode]->Fill(9.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat1up",   std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[11];
          bSFInfo[MODE][jetmode]->Fill(10.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat1down", std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[12];
          bSFInfo[MODE][jetmode]->Fill(11.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat2up",   std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[13];
          bSFInfo[MODE][jetmode]->Fill(12.5, EventWeight*bSF);
        }
        else if( isPartOf("lfstat2down", std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[14];
          bSFInfo[MODE][jetmode]->Fill(13.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr1up",    std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[15];
          bSFInfo[MODE][jetmode]->Fill(14.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr1down",  std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[16];
          bSFInfo[MODE][jetmode]->Fill(15.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr2up",    std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[17];
          bSFInfo[MODE][jetmode]->Fill(16.5, EventWeight*bSF);
        }
        else if( isPartOf("cferr2down",  std::string(syst_name[syst])) ){
          bSF = jet_SF_deepCSV_30[18];
          bSFInfo[MODE][jetmode]->Fill(17.5, EventWeight*bSF);
        }
        else{
          bSF = jet_SF_deepCSV_30[0];
          if( !isPartOf("__", std::string(syst_name[syst])) ){
            bSFInfo[MODE][jetmode]->Fill(0.5, EventWeight);
            bSFInfo[MODE][jetmode]->Fill(1.5, EventWeight*bSF);
          }
        }
        EventWeight *= bSF;
      }

      if( nbjets_m < 2 or nbjets_m > 4 ) continue;
      if     ( njets == 3 and nbjets_m == 2 ) h_Events[MODE][syst]->Fill(0.5,EventWeight);
      else if( njets == 3 and nbjets_m == 3 ) h_Events[MODE][syst]->Fill(2.5,EventWeight);
      else if( njets >= 4 and nbjets_m == 2 ) h_Events[MODE][syst]->Fill(1.5,EventWeight);
      else if( njets >= 4 and nbjets_m == 3 ) h_Events[MODE][syst]->Fill(3.5,EventWeight);
      else if( njets >= 4 and nbjets_m == 4 ) h_Events[MODE][syst]->Fill(4.5,EventWeight);
      else cout << "Wrong jet multiplicity" << endl;

    }//syst loop
  }//mode loop : e or mu + emu
  evtNum++;
  //cout << evtNum << '\r';

  return kTRUE;
}

void Yield::SlaveTerminate()
{
  TString option = GetOption();
}
   

void Yield::Terminate()
{
  TString option = GetOption();
  string sample = option.Data();
  string era_s = sample.substr(sample.find_first_of("-")+1,4);
  string train_scheme = sample.substr(sample.find_first_of("-")+5,string::npos);
  sample.erase(sample.find_first_of("-"),string::npos);

  dosyst = true;
  if( option.Contains("Run201") ) dosyst = false;

  syst_ext = "";
  int era = stoi(era_s);
  if( train_scheme.find("jec") != string::npos or train_scheme.find("jer") != string::npos
    or train_scheme.find("TuneCP5") != string::npos or train_scheme.find("hdamp") != string::npos ){
    if     ( train_scheme.find("jecup") != string::npos )       syst_ext = "__jecup";
    else if( train_scheme.find("jecdown") != string::npos )     syst_ext = "__jecdown";
    else if( train_scheme.find("jerup") != string::npos )       syst_ext = "__jerup";
    else if( train_scheme.find("jerdown") != string::npos )     syst_ext = "__jerdown";
    else if( train_scheme.find("TuneCP5up") != string::npos )   syst_ext = "__TuneCP5up";
    else if( train_scheme.find("TuneCP5down") != string::npos ) syst_ext = "__TuneCP5down";
    else if( train_scheme.find("hdampup") != string::npos )     syst_ext = "__hdampup";
    else if( train_scheme.find("hdampdown") != string::npos )   syst_ext = "__hdampdown";
    //Regrouped JEC V2
    else if( train_scheme.find("jecAbsoluteup") != string::npos )                     syst_ext = "__jecAbsoluteup";
    else if( train_scheme.find("jecAbsolutedown") != string::npos )                   syst_ext = "__jecAbsolutedown";
    else if( train_scheme.find(Form("jecAbsolute%iup",era)) != string::npos )         syst_ext = Form("__jecAbsolute%iup",era);
    else if( train_scheme.find(Form("jecAbsolute%idown",era)) != string::npos )       syst_ext = Form("__jecAbsolute%idown",era);
    else if( train_scheme.find("jecBBEC1up") != string::npos )                        syst_ext = "__jecBBEC1up";
    else if( train_scheme.find("jecBBEC1down") != string::npos )                      syst_ext = "__jecBBEC1down";
    else if( train_scheme.find(Form("jecBBEC1%iup",era)) != string::npos )            syst_ext = Form("__jecBBEC1%iup",era);
    else if( train_scheme.find(Form("jecBBEC1%idown",era)) != string::npos )          syst_ext = Form("__jecBBEC1%idown",era);
    else if( train_scheme.find("jecFlavorQCDup") != string::npos )                    syst_ext = "__jecFlavorQCDup";
    else if( train_scheme.find("jecFlavorQCDdown") != string::npos )                  syst_ext = "__jecFlavorQCDdown";
    else if( train_scheme.find("jecRelativeBalup") != string::npos )                  syst_ext = "__jecRelativeBalup";
    else if( train_scheme.find("jecRelativeBaldown") != string::npos )                syst_ext = "__jecRelativeBaldown";
    else if( train_scheme.find(Form("jecRelativeSample%iup",era)) != string::npos )   syst_ext = Form("__jecRelativeSample%iup",era);
    else if( train_scheme.find(Form("jecRelativeSample%idown",era)) != string::npos ) syst_ext = Form("__jecRelativeSample%idown",era);
  }
  string syst_str = "-" + train_scheme.substr(train_scheme.find_last_of("_")+1,string::npos );
  if( syst_ext == "" ) syst_str.erase(0,1);
  train_scheme.erase( train_scheme.find_last_of("_"),string::npos );

  out = TFile::Open(Form("./histos/temp/hist_%s%s.root", sample.c_str(), syst_ext.c_str()),"RECREATE");

  TList * l = GetOutputList();
  TIter next(l);
  TObject *object = 0;
  while( ( object = next()) ){
    const char * name = object->GetName();
    std::string str(name);

    if(str.find("Ch2") != std::string::npos ){
      std::string strMuon = boost::replace_all_copy(str, "Ch2", "Ch0");
      std::string strElec = boost::replace_all_copy(str, "Ch2", "Ch1");
      TList *tmp = new TList;
      if( object->InheritsFrom(TH1D::Class()) ){
        auto htmp = dynamic_cast<TH1D *>(object);
        tmp->Add((TH1D *)fOutput->FindObject(strMuon.c_str()));
        tmp->Add((TH1D *)fOutput->FindObject(strElec.c_str()));
        htmp->Merge(tmp);
      }
    }

    if (str.find("h_") !=std::string::npos or str.find("Info") !=std::string::npos){
      object->Write();
    }
  }

  out->Write();
  out->Close();
}

bool Yield::isPartOf(const std::string& word, const std::string& sentence) {
    return sentence.find(word)    // this returns the index of the first instance
                                  // word
           != std::string::npos;  // which will take this value if it's not found
}

float Yield::topPtLOtoNLO( float toppt ){

  float weight = 1.0;

  if     ( toppt <= 0 )                     weight = 1.0;
  else if( toppt > 0 and toppt < 50 )       weight = ( 1.748647 + 1.752149 + 1.747765 + 1.748996 ) / 4.0;
  else if( toppt >= 50 and toppt < 100 )    weight = ( 1.759757 + 1.754686 + 1.754359 + 1.753767 ) / 4.0;
  else if( toppt >= 100 and toppt < 150 )   weight = ( 1.710515 + 1.714042 + 1.70896  + 1.707913 ) / 4.0;
  else if( toppt >= 150 and toppt < 200 )   weight = ( 1.634742 + 1.635692 + 1.637983 + 1.639704 ) / 4.0;
  else if( toppt >= 200 and toppt < 250 )   weight = ( 1.539141 + 1.541837 + 1.547243 + 1.544309 ) / 4.0;
  else if( toppt >= 250 and toppt < 300 )   weight = ( 1.454174 + 1.449138 + 1.453648 + 1.4566   ) / 4.0;
  else if( toppt >= 300 and toppt < 350 )   weight = ( 1.376844 + 1.360644 + 1.386458 + 1.367805 ) / 4.0;
  else if( toppt >= 350 and toppt < 400 )   weight = ( 1.307372 + 1.312938 + 1.305165 + 1.320008 ) / 4.0;
  else if( toppt >= 400 and toppt < 450 )   weight = ( 1.197963 + 1.227374 + 1.255603 + 1.270746 ) / 4.0;
  else if( toppt >= 450 and toppt < 500 )   weight = ( 1.19234  + 1.179497 + 1.178465 + 1.209439 ) / 4.0;
  else if( toppt >= 500 and toppt < 550 )   weight = ( 1.139534 + 1.172257 + 1.160954 + 1.210782 ) / 4.0;
  else if( toppt >= 550 and toppt < 600 )   weight = ( 1.135917 + 1.141849 + 1.205256 + 1.195735 ) / 4.0;
  else if( toppt >= 600 and toppt < 800 )   weight = ( 1.089117 + 1.072375 + 1.18809  + 1.11811  ) / 4.0;
  else if( toppt >= 800 and toppt < 1000 )  weight = ( 1.10766  + 1.115636 + 1.170683 + 1.091182 ) / 4.0;
  else if( toppt >= 1000 and toppt < 2000 ) weight = ( 1.07664  + 1.152181 + 1.035299 + 1.203024 ) / 4.0;

  return weight;

}

float Yield::topPtNLOtoNNLO( float toppt ){

  float weight = 1.0;

  if( toppt > 0. )
    weight = 0.103 * TMath::Exp(-0.0118 * toppt) - 0.000134 * toppt + 0.973;

  return weight;
}
