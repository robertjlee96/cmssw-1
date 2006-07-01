//<<<<<< INCLUDES                                                       >>>>>>
#include "Utilities/StorageFactory/interface/StorageAccount.h"
#include "SealBase/TimeInfo.h"
#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

StorageAccount::StorageStats StorageAccount::s_stats;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

std::string
StorageAccount::summaryText (void)
{
  bool first = true;
  std::ostringstream os;
  for (StorageStats::iterator i = s_stats.begin (); i != s_stats.end(); ++i)
    for (OperationStats::iterator j = i->second->begin (); j != i->second->end (); ++j, first = false)
      os << (first ? "" : "; ")
	 << i->first << '/'
	 << j->first << '='
	 << j->second.attempts << '/'
	 << j->second.successes << '/'
	 << (j->second.amount / 1024 / 1024) << "MB/"
	 << (j->second.time / 1000 / 1000) << "ms";
  
  return os.str ();
}

const StorageAccount::StorageStats &
StorageAccount::summary (void)
{ return s_stats; }

StorageAccount::Counter &
StorageAccount::counter (const std::string &storageClass, const std::string &operation)
{
  boost::shared_ptr<OperationStats> &opstats = s_stats [storageClass];
  if (! opstats) opstats.reset(new OperationStats);
  
  OperationStats::iterator pos = opstats->find (operation);
  if (pos == opstats->end ())
    {
      Counter x = { 0, 0, 0 }; x.idTag = storageClass + "/" + operation;
      pos = opstats->insert (OperationStats::value_type (operation, x)).first;
    }
  
  return pos->second;
}

StorageAccount::Stamp::Stamp (Counter &counter)
  : m_counter (counter),
    m_start (seal::TimeInfo::realNsecs ())
{
  m_counter.attempts++;
  StorageAccount::setCurrentOp(&m_counter,m_start);
}

void
StorageAccount::Stamp::tick (double amount) const
{
  m_counter.successes++;
  m_counter.amount += amount;
  double elapsed = seal::TimeInfo::realNsecs () - m_start;
  m_counter.time += elapsed;
  StorageAccount::setCurrentOp(0,elapsed);
}


//FIXME (not thread safe)
StorageAccount::LastOp & 
StorageAccount::lastOp() 
{
  static LastOp local;
  return local;
}


void 
StorageAccount::setCurrentOp(const Counter * currOp, double stime) {
  if (currOp) {
    lastOp().idTag = (*currOp).idTag;
    lastOp().startTime = stime;
    lastOp().elapsed = 0;
  } else {
    lastOp().elapsed = stime;
  }
}
