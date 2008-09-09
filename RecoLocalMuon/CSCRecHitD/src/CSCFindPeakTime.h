/** This is CSCFindPeakTime
 *
 * \author Dominique Fortin
 *
 * Based on RecoLocalMuon/CSCStandAlone/interface/PulseTime.h  by S. Durkin 
 *
 * Fast fit adc values to <BR>
 *    N*(p0**2/256/exp(-4))*(t-t0)**4*exp(-p0*(t-t0)
 *	
 * PulseTime has peaking time fixed to 133 nsec.
 * It actually fits for the t0.
 *	
 * Fit the log of data <BR>
 * chi2=(log(Data)-log(N)-4*log(t-t0)+p0*(t-t0))**2
 *
 * Becomes least square fit in p0 and log(N); and binary search in t0 yields N, p0, t0(nsec).
 *	
 * Note: tpeak=4/p0 (nsec) and adc[0] is arbitrarily defined a time of 0.0 nsec. 
 *
 * Finally, a fit to the charge deposition for each time bin is performed using the fitted t0.
 */

#include <vector>


class CSCFindPeakTime
{
 public:
  
  CSCFindPeakTime(){}; 
  
  ~CSCFindPeakTime(){}; 
  
  /// Member functions

  /// Finding the peak time and zeroth time
  bool FindPeakTime( const int& tmax, const float* adc, float& t_zero, float& t_peak ); 

  /// Fitting the charge for a given t_zero and t_peak
  void FitCharge( const int& tmax, const float* adc, const float& t_zero, const float& t_peak, std::vector<float>& adcsFit );
  
 private:
  
  
};
