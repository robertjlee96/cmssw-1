/*
 * =====================================================================================
 *
 *       Filename:  SiPixelGainCalibrationOfflineSimService.cc
 *
 *    Description:  
 *
 *        Version:  1.0 (some functionality moved from ../interface/SiPixelGainCalibrationOfflineSimService.h)
 *        Created:  04/16/2008 10:35:35 AM
 *
 *         Author:  Evan Friis (evan.klose.friis@cern.ch)
 *                  University of California, Davis
 *
 * =====================================================================================
 */

#include "CalibTracker/SiPixelESProducers/interface/SiPixelGainCalibrationOfflineSimService.h"

float SiPixelGainCalibrationOfflineSimService::getPedestal( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   float pedestalValue = this->getPedestalByPixel(detID, col, row, isDead, isNoisy);
   if (isDead || isNoisy)
   {
      this->throwExepctionForBadRead("Offline getPedestal()", detID, col, row, pedestalValue);
      return 0.0;
   }
   return pedestalValue;
}

float SiPixelGainCalibrationOfflineSimService::getGain( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   float gainValue = this->getGainByColumn(detID, col, row, isDead, isNoisy);
   if (isDead || isNoisy)
   {
      this->throwExepctionForBadRead("Offline getGain()", detID, col, row, gainValue);
      return 0.0;
   }
   return gainValue;
}

bool SiPixelGainCalibrationOfflineSimService::isDead( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   try  
   {
      this->getPedestalByPixel(detID, col, row, isDead, isNoisy); 
   }
   catch (cms::Exception& e) 
   {
      // Do not stop processing if you check if a nonexistant pixel is dead
      edm::LogInfo("SiPixelGainCalibrationOfflineSimService") << "Attempting to check if nonexistant pixel is dead.  Exception message: " << e.what();
      isDead = false; 
   }
   return isDead;
}

bool SiPixelGainCalibrationOfflineSimService::isNoisy( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   try  
   {
      this->getPedestalByPixel(detID, col, row, isDead, isNoisy); 
   }
   catch (cms::Exception& e) 
   {
      // Do not stop processing if you check if a nonexistant pixel is dead
      edm::LogInfo("SiPixelGainCalibrationOfflineSimService") << "Attempting to check if nonexistant pixel is noisy.  Exception message: " << e.what();
      isNoisy = false; 
   }
   return isNoisy;
}
   
bool SiPixelGainCalibrationOfflineSimService::isDeadColumn( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   try  
   {
      this->getGainByColumn(detID, col, row, isDead, isNoisy); // the gain column average can flag a whole column as bad
   }
   catch (cms::Exception& e) 
   {
      // Do not stop processing if you check if a nonexistant pixel is dead
      edm::LogInfo("SiPixelGainCalibrationOfflineSimService") << "Attempting to check if nonexistant pixel is dead.  Exception message: " << e.what();
      isDead = false; 
   }
   return isDead;
}

bool SiPixelGainCalibrationOfflineSimService::isNoisyColumn( const uint32_t& detID,const int& col, const int& row)
{
   bool isDead = false;
   bool isNoisy = false;
   try  
   {
      this->getGainByColumn(detID, col, row, isDead, isNoisy); // the gain column average can flag a whole column as bad
   }
   catch (cms::Exception& e) 
   {
      // Do not stop processing if you check if a nonexistant pixel is dead
      edm::LogInfo("SiPixelGainCalibrationOfflineSimService") << "Attempting to check if nonexistant pixel is Noisy.  Exception message: " << e.what();
      isNoisy = false; 
   }
   return isNoisy;
}
