#ifndef EESummaryClient_H
#define EESummaryClient_H

/*
 * \file EESummaryClient.h
 *
 * $Date: 2007/12/15 11:34:31 $
 * $Revision: 1.12 $
 * \author G. Della Ricca
 *
*/

#include <vector>
#include <string>
#include <fstream>

#include "TROOT.h"
#include "TProfile2D.h"
#include "TH1F.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/EcalEndcapMonitorClient/interface/EEClient.h"

class MonitorElement;
class MonitorUserInterface;
class DaqMonitorBEInterface;
class EcalCondDBInterface;
class RunIOV;
class MonRunIOV;

class EESummaryClient : public EEClient {

public:

/// Constructor
EESummaryClient(const edm::ParameterSet& ps);

/// Destructor
virtual ~EESummaryClient();

/// softReset
void softReset(void);

/// Analyze
void analyze(void);

/// BeginJob
void beginJob(MonitorUserInterface* mui);

/// EndJob
void endJob(void);

/// BeginRun
void beginRun(void);

/// EndRun
void endRun(void);

/// Setup
void setup(void);

/// Cleanup
void cleanup(void);

/// HtmlOutput
void htmlOutput(int run, std::string htmlDir, std::string htmlName);

/// WriteDB
bool writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov);

/// Get Functions
inline int getEvtPerJob() { return ievt_; }
inline int getEvtPerRun() { return jevt_; }

/// Set Clients
inline void setFriends(std::vector<EEClient*> clients) { clients_ = clients; }

private:

void writeMap( std::ofstream& hf, std::string mapname );

int ievt_;
int jevt_;

bool cloneME_;

bool verbose_;

bool enableMonitorDaemon_;

std::string prefixME_;

std::vector<int> superModules_;

std::vector<EEClient*> clients_;

MonitorUserInterface* mui_;
DaqMonitorBEInterface* dbe_;

MonitorElement* meIntegrity_[2], *meIntegrityErr_;
MonitorElement* meOccupancy_[2], *meOccupancy1DSummary_;
MonitorElement* mePedestalOnline_[2], *mePedestalOnlineErr_;
MonitorElement* meLaserL1_[2], *meLaserL1Err_;
MonitorElement* meLaserL1PN_[2], *meLaserL1PNErr_;
MonitorElement* meLed_[2], *meLedErr_;
MonitorElement* meLedPN_[2], *meLedPNErr_;
MonitorElement* mePedestal_[2], *mePedestalErr_;
MonitorElement* mePedestalPN_[2], *mePedestalPNErr_;
MonitorElement* meTestPulse_[2], *meTestPulseErr_;
MonitorElement* meTestPulsePN_[2], *meTestPulsePNErr_;

MonitorElement* meCosmic_[2];
MonitorElement* meTiming_[2];
MonitorElement* meTriggerTowerEt_[2];
MonitorElement* meTriggerTowerEmulError_[2];

MonitorElement* meGlobalSummary_[2];

};

#endif
