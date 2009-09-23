#include "CondCore/DBCommon/interface/DbConnection.h"
#include "CondCore/DBCommon/interface/DbTransaction.h"
#include "CondCore/DBCommon/interface/Time.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/IOVService/interface/IOVService.h"
#include "CondCore/IOVService/interface/IOVEditor.h"
#include "CondCore/MetaDataService/interface/MetaData.h"
#include "CondFormats/Calibration/interface/Pedestals.h"
int main(){
  try{
    // for runnumber
    cond::TimeType timetype = cond::runnumber;
    cond::Time_t globalTill = cond::timeTypeSpecs[timetype].endValue;

    cond::DbConnection connection;
    connection.configuration().setMessageLevel( coral::Error );
    connection.configure();
    cond::DbSession session = connection.createSession();
    session.open( "sqlite_file:extradata.db" );
    cond::IOVService iovmanager(session);
    cond::IOVEditor* ioveditor=iovmanager.newIOVEditor();
    session.transaction().start(false);
    ioveditor->create(timetype,globalTill);
    std::string mytestiovtoken;
    for(unsigned int i=0; i<3; ++i){ //inserting 3 payloads
      Pedestals* myped=new Pedestals;
      for(int ichannel=1; ichannel<=5; ++ichannel){
        Pedestals::Item item;
        item.m_mean=1.11*ichannel+i;
        item.m_variance=1.12*ichannel+i*2;
        myped->m_pedestals.push_back(item);
      }
      pool::Ref<Pedestals> myref = session.storeObject(myped,"anotherPedestalsRcd");
      std::string payloadToken=myref.toString();
      std::cout<<"payloadToken "<<payloadToken<<std::endl;
      ioveditor->append(cond::Time_t(2+2*i),payloadToken);
    }
    std::string mytoken=ioveditor->token();
    session.transaction().commit();
    delete ioveditor;
    
    cond::MetaData metadata(session);
    session.transaction().start(false);
    metadata.addMapping("anothertag",mytoken,cond::runnumber);
    session.transaction().commit();
  }catch(const cond::Exception& er){
    std::cout<<"error "<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<"std error "<<er.what()<<std::endl;
  }
}
