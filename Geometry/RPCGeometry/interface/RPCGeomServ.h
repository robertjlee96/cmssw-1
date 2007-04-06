#ifndef RPCGeometry_RPCGeomServ_h
#define RPCGeometry_RPCGeomServ_h

#include <string>

class RPCDetId;
class RPCGeomServ{
 public:
  RPCGeomServ(const RPCDetId& id);
  virtual ~RPCGeomServ();
  virtual std::string name();
  virtual int eta_partition(); 
  virtual bool inverted();
  virtual bool zpositive();
  virtual bool aclockwise();

 protected:
  RPCGeomServ();

 protected:
  const RPCDetId* _id;
  std::string _n;
  int _t;
  bool _z;
  bool _a;

};
#endif
