// $Id: Tm.cc,v 1.3 2008/10/23 09:47:23 fra Exp $

#include <time.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <math.h>
#include <cstdio>

#include "OnlineDB/EcalCondDB/interface/Tm.h"

using namespace std;

// Default Constructor
Tm::Tm()
{
  this->setNull();
}



// Initialized Constructor
Tm::Tm(struct tm* initTm)
{
  m_tm = *initTm;
}

Tm::Tm(uint64_t micros)
{
  this->setNull();
  this->setToMicrosTime(micros);
}


// Destructor
Tm::~Tm()
{
}



struct tm Tm::c_tm() const
{
  return m_tm;
}



int Tm::isNull() const
{
  if (m_tm.tm_year == 0
      && m_tm.tm_mon == 0
      && m_tm.tm_mday == 0 ) {
    return 1;
  } else { return 0; }
}



void Tm::setNull()
{
  m_tm.tm_hour  = 0;
  m_tm.tm_isdst = 0;
  m_tm.tm_mday  = 0;
  m_tm.tm_min   = 0;
  m_tm.tm_mon   = 0;
  m_tm.tm_sec   = 0;
  m_tm.tm_wday  = 0;
  m_tm.tm_yday  = 0;
  m_tm.tm_year  = 0;
}



string Tm::str() const
{
  if (this->isNull()) {
    return "";
  }

  char timebuf[20] = "";
  strftime(timebuf, 20, "%Y-%m-%d %H:%M:%S", &m_tm);
  return string(timebuf);
}



uint64_t Tm::microsTime() const
{
  uint64_t result = 0;
  
  result += (uint64_t)ceil((m_tm.tm_year + 1900 - 1970) * 365.25) * 24 * 3600;
  result += (m_tm.tm_yday-1) * 24 * 3600;
  result += m_tm.tm_hour * 3600;
  result += m_tm.tm_min * 60;
  result += m_tm.tm_sec;
  
  return (uint64_t) (result * 1000000);
}

void Tm::setToMicrosTime(uint64_t micros)
{
  time_t t = micros / 1000000;
  m_tm = *gmtime(&t);
}

void Tm::setToCurrentLocalTime()
{
  time_t t = time(NULL);
  m_tm = *localtime( &t );
}

void Tm::setToCurrentGMTime()
{
  time_t t = time(NULL);
  m_tm = *gmtime( &t );
}

void Tm::setToLocalTime(time_t t)
{
  m_tm = *localtime( &t );
}

void Tm::setToGMTime(time_t t)
{
  m_tm = *gmtime( &t );
}

void Tm::setToString(const string s)
  throw(runtime_error)
{
  sscanf(s.c_str(), "%04d-%02d-%02d %02d:%02d:%02d", 
	 &m_tm.tm_year, &m_tm.tm_mon, &m_tm.tm_mday,
	 &m_tm.tm_hour, &m_tm.tm_min, &m_tm.tm_sec);

  try {
    if (m_tm.tm_year > 9999 || m_tm.tm_year < 1970) {
      throw(runtime_error("Year out of bounds"));
    } else if (m_tm.tm_mon > 12 || m_tm.tm_mon < 1) {
      throw(runtime_error("Month out of bounds"));
    } else if (m_tm.tm_mday > 31 || m_tm.tm_mday < 1) {
      throw(runtime_error("Day out of bounds"));
    } else if (m_tm.tm_hour > 23 || m_tm.tm_mday < 0) {
      throw(runtime_error("Hour out of bounds"));
    } else if (m_tm.tm_min > 59 || m_tm.tm_min < 0) {
      throw(runtime_error("Minute out of bounds"));
    } else if (m_tm.tm_sec > 59 || m_tm.tm_sec < 0) {
      throw(runtime_error("Day out of bounds"));
    }

    m_tm.tm_year -= 1900;
    m_tm.tm_mon -= 1;
  } catch (runtime_error &e) {
    this->setNull();
    string msg("Tm::setToString():  ");
    msg.append(e.what());
    throw(runtime_error(msg));
  }
}

void Tm::dumpTm()
{
  cout << "=== dumpTm() ===" << endl;
  cout << "tm_year  " << m_tm.tm_year << endl;
  cout << "tm_mon   " << m_tm.tm_mon << endl;
  cout << "tm_mday  " << m_tm.tm_mday << endl;
  cout << "tm_hour  " << m_tm.tm_hour << endl;
  cout << "tm_min   " << m_tm.tm_min << endl;
  cout << "tm_sec   " << m_tm.tm_sec << endl;
  cout << "tm_yday  " << m_tm.tm_yday << endl;
  cout << "tm_wday  " << m_tm.tm_wday << endl;
  cout << "tm_isdst " << m_tm.tm_isdst << endl;
  cout << "================" << endl;
}
