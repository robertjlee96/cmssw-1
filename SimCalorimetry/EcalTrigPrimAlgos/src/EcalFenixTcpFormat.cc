#include <SimCalorimetry/EcalTrigPrimAlgos/interface/EcalFenixTcpFormat.h>
#include <SimCalorimetry/EcalTrigPrimAlgos/interface/DBInterface.h>

EcalFenixTcpFormat::EcalFenixTcpFormat(DBInterface * db, bool tcpFormat)
  : db_(db),tcpFormat_(tcpFormat)
{
}
 
EcalFenixTcpFormat::~EcalFenixTcpFormat() {
}

 
void EcalFenixTcpFormat::process(std::vector<int> &Et, std::vector<int> &fgvb, 
				 std::vector<EcalTriggerPrimitiveSample> & out,
				 std::vector<EcalTriggerPrimitiveSample> & out2){
  // put TP-s in the output
  // on request also in TcpFormat    
  for (unsigned int i=0; i<Et.size();++i) {
    Et[i]=Et[i]>>2;
    if (Et[i]>0x3ff) Et[i]=0x3ff ;
    int lut_out = lut_[Et[i]] ;
    int ttFlag = (lut_out & 0x700) >> 8 ;
    if (tcpFormat_)  {
      int data = ((ttFlag&0x7)<<11) | ((fgvb[i] & 0x1)<<10) |  (Et [i] & 0x3ff) ;
      out2.push_back(EcalTriggerPrimitiveSample(data));
    }
    Et[i] = lut_out & 0xff ;
    out.push_back(EcalTriggerPrimitiveSample( Et[i],fgvb[i],ttFlag)); 
  }
}

void EcalFenixTcpFormat::setParameters(int SM, int towerInSM) 
{
  lut_ = db_->getTowerParameters(SM, towerInSM) ;
}

