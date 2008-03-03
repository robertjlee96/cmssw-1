#include "EventFilter/EcalRawToDigiDev/interface/DCCSCBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCEventBlock.h"
#include "EventFilter/EcalRawToDigiDev/interface/DCCDataUnpacker.h"
#include <stdio.h>
#include "EventFilter/EcalRawToDigiDev/interface/EcalElectronicsMapper.h"



DCCSCBlock::DCCSCBlock( DCCDataUnpacker * u,EcalElectronicsMapper * m , DCCEventBlock * e, bool unpack)
: DCCFEBlock(u,m,e,unpack){}


void DCCSCBlock::updateCollectors(){

  DCCFEBlock::updateCollectors();
  
  // needs to be update for eb/ee
  digis_               = unpacker_->eeDigisCollection();

  invalidGains_        = unpacker_->invalidEEGainsCollection();
  invalidGainsSwitch_  = unpacker_->invalidEEGainsSwitchCollection();
  invalidChIds_        = unpacker_->invalidEEChIdsCollection();

}




void DCCSCBlock::unpackXtalData(uint expStripID, uint expXtalID){
  
  bool errorOnXtal(false);
 
  uint16_t * xData_= reinterpret_cast<uint16_t *>(data_);

 
  // Get xtal data ids
  uint stripId = (*xData_) & TOWER_STRIPID_MASK;
  uint xtalId  =((*xData_)>>TOWER_XTALID_B ) & TOWER_XTALID_MASK;
  
  // cout<<"\n DEBUG : unpacked xtal data for strip id "<<stripId<<" and xtal id "<<xtalId<<endl;
  // cout<<"\n DEBUG : expected strip id "<<expStripID<<" expected xtal id "<<expXtalID<<endl;
  

  if( !zs_ && (expStripID != stripId || expXtalID != xtalId)){ 
	 
    edm::LogWarning("EcalRawToDigiDevChId")
      <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
      <<"\n The expected strip is "<<expStripID<<" and "<<stripId<<" was found"
      <<"\n The expected xtal  is "<<expXtalID <<" and "<<xtalId<<" was found";	
    
    
    // using expected cry_di to raise warning about xtal_id problem
    pDetId_ = (EEDetId*) mapper_->getDetIdPointer(towerId_,expStripID,expXtalID);
    (*invalidChIds_)->push_back(*pDetId_);
    
    stripId = expStripID;
    xtalId  = expXtalID;
    errorOnXtal = true;
    
    // return here, so to skip all following checks
    data_ += numbDWInXtalBlock_;
    return;
  }


  // check id in case of 0suppressed data

  else if(zs_){

    // Check for valid Ids 1) values out of range

    if(stripId == 0 || stripId > 5 || xtalId == 0 || xtalId > 5){
      edm::LogWarning("EcalRawToDigiDevChId")
        <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
        <<"\n Invalid strip : "<<stripId<<" or xtal : "<<xtalId
        <<" ids ( last strip was: " << lastStripId_ << " last ch was: " << lastXtalId_ << ")";
      
      int st = lastStripId_;
      int ch = lastXtalId_;
      ch++;
      if (ch > NUMB_XTAL) 	{ch=1; st++;}
      if (st > NUMB_STRIP)	{ch=-1; st=-1;}

      // adding channel following the last valid
      pDetId_ = (EEDetId*) mapper_->getDetIdPointer(towerId_,st,ch);
      (*invalidChIds_)->push_back(*pDetId_);

      errorOnXtal = true;

      //return here, so to skip all the rest
      //Point to begin of next xtal Block
      data_ += numbDWInXtalBlock_;
      return;
		
    }else{
	 
	 
      // Check for zs valid Ids 2) if channel-in-strip has increased wrt previous xtal

      if ( stripId >= lastStripId_ ){

        if( stripId == lastStripId_ && xtalId <= lastXtalId_ ){ 
		  
          edm::LogWarning("EcalRawToDigiDevChId")
            <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
            <<"\n Xtal id was expected to increase but it didn't. "
            <<"\n Last unpacked xtal was "<<lastXtalId_<<" while current xtal is "<<xtalId<<".";

	  int st = lastStripId_;
	  int ch = lastXtalId_;
	  ch++;
	  if (ch > NUMB_XTAL)	{ch=1; st++;}
	  if (st > NUMB_STRIP)	{ch=-1; st=-1;}
	  
	  // adding channel following the last valid
           pDetId_ = (EEDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);
	   (*invalidChIds_)->push_back(*pDetId_);
	   
	   errorOnXtal = true;
	   
	   // return here, so to skip all the rest
	   //Point to begin of next xtal Block
	   lastXtalId_++;
	   if (lastXtalId_ > NUMB_XTAL) 	{lastXtalId_=1; lastStripId_++;}
	   data_ += numbDWInXtalBlock_;
	   return;
        }
	
       }

      // Check for zs valid Ids 3) if strip has increased wrt previous xtal
      else if( stripId < lastStripId_){
      
        edm::LogWarning("EcalRawToDigiDevChId")
          <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n Strip id was expected to increase but it didn't "
          <<"\n Last unpacked strip was "<<lastStripId_<<" while current strip is "<<stripId;

	int st = lastStripId_;
	int ch = lastXtalId_;
	ch++;
	if (ch > NUMB_XTAL)	{ch=1; st++;}
	if (st > NUMB_STRIP)	{ch=-1; st=-1;}
	
	// adding channel following the last valid
	pDetId_ = (EEDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);
        (*invalidChIds_)->push_back(*pDetId_);
       
        errorOnXtal = true;		  
	
	// return here, so to skip all following checks
	lastXtalId_++;
	if (lastXtalId_ > NUMB_XTAL) 	{lastXtalId_=1; lastStripId_++;}
	data_ += numbDWInXtalBlock_;
	return;
	
      }
		
      lastStripId_  = stripId;
      lastXtalId_   = xtalId;
    }
  }// end if(zs_)
 
  bool frameAdded=false;

  // if there is an error on xtal id ignore next error checks  
  // otherwise, assume channel_id is valid and proceed with making and checking the data frame
  if(!errorOnXtal){ 

    pDetId_ = (EEDetId*) mapper_->getDetIdPointer(towerId_,stripId,xtalId);

    if(pDetId_){
      (*digis_)->push_back(*pDetId_);
      EEDataFrame df( (*digis_)->back() );
      frameAdded=true;

      bool wrongGain(false);
	 
      //set samples in the frame
      for(uint i =0; i< nTSamples_ ;i++){ 
        xData_++;
        uint data =  (*xData_) & TOWER_DIGI_MASK;
        uint gain =  data>>12;
        xtalGains_[i]=gain;
        if(gain == 0){	  wrongGain = true; } 
 
        df.setSample(i,data);
      }
	
    
      if(wrongGain){ 
        edm::LogWarning("EcalRawToDigiDevGainZero")
        <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
        <<"\n Gain zero was found in strip "<<stripId<<" and xtal "<<xtalId;   
	
	(*invalidGains_)->push_back(*pDetId_); 
	
        errorOnXtal = true;
	
	//return here, so to skip all the rest
	//make special collection for gain0 data frames (saturation)
	//Point to begin of next xtal Block
	data_ += numbDWInXtalBlock_;
	
	return;

      }
	
   
      short firstGainWrong=-1;
      short numGainWrong=0;
	    
      for (uint i=0; i<nTSamples_; i++ ) {
        if (i>0 && xtalGains_[i-1]>xtalGains_[i]) {
          numGainWrong++;
          if (firstGainWrong == -1) { firstGainWrong=i;}
        }
      }
      
      if (numGainWrong>0) {
	
	
        edm::LogWarning("EcalRawToDigiDevGainSwitch")
          <<"\n For event LV1: "<<event_->l1A()<<", fed "<<mapper_->getActiveDCC()<<" and tower "<<towerId_
          <<"\n A wrong gain transition switch was found in strip "<<stripId<<" and xtal "<<xtalId;    
	
	(*invalidGainsSwitch_)->push_back(*pDetId_);
	
	errorOnXtal = true;
      } 

      //Add frame to collection only if all data format and gain rules are respected
      if(errorOnXtal&&frameAdded) {
	(*digis_)->pop_back();
      }

   }// End on check of det id
  
  }//End errorOn Xtal 	
  
  //Point to begin of next xtal Block
  data_ += numbDWInXtalBlock_;
}





