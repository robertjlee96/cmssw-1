#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "TrackingTools/GeomPropagators/interface/HelixExtrapolatorToLine2Order.h"
#include "Geometry/Surface/interface/Line.h"
#include "Geometry/Vector/interface/Pi.h"

using namespace edm;
using namespace reco;
using namespace std;

double TrackAssociatorByChi2::compareTracksParam ( TrackCollection::const_iterator rt, 
						   SimTrackContainer::const_iterator st, 
						   const HepLorentzVector vertexPosition, 
						   GlobalVector magField,
						   TrackBase::CovarianceMatrix  
						   invertedCovariance  ) {
  
  Basic3DVector<double> momAtVtx(st->momentum().x(),st->momentum().y(),st->momentum().z());
  Basic3DVector<double> vert = (Basic3DVector<double>) vertexPosition;
      
  //should use st->charge()
  TrackBase::ParameterVector sParameters=parametersAtClosestApproachGeom(vert, momAtVtx, rt->charge());
  TrackBase::ParameterVector rParameters = rt->parameters();
  
  TrackBase::ParameterVector diffParameters = rParameters - sParameters;
  double chi2 = ROOT::Math::Dot(diffParameters * invertedCovariance, diffParameters);
  
  return chi2;
}


TrackAssociatorByChi2::RecoToSimPairAssociation 
TrackAssociatorByChi2::compareTracksParam(const TrackCollection& rtColl,
					  const SimTrackContainer& stColl,
					  const SimVertexContainer& svColl) {
  
  RecoToSimPairAssociation outputVec;

  for (TrackCollection::const_iterator track=rtColl.begin(); track!=rtColl.end(); track++){
     Chi2SimMap outMap;

    TrackBase::ParameterVector rParameters = track->parameters();
    TrackBase::CovarianceMatrix recoTrackCovMatrix = track->covariance();
    if (onlyDiagonal){
      for (unsigned int i=0;i<5;i++){
	for (unsigned int j=0;j<5;j++){
	  if (i!=j) recoTrackCovMatrix(i,j)=0;
	}
      }
    }
    recoTrackCovMatrix.Invert();

    for (SimTrackContainer::const_iterator st=stColl.begin(); st!=stColl.end(); st++){

      Basic3DVector<double> momAtVtx(st->momentum().x(),st->momentum().y(),st->momentum().z());
      Basic3DVector<double> vert = (Basic3DVector<double>)  svColl[st->vertIndex()].position();

      //should use st->charge()
      TrackBase::ParameterVector sParameters=parametersAtClosestApproachGeom(vert, momAtVtx, track->charge());
      
      TrackBase::ParameterVector diffParameters = rParameters - sParameters;
      double chi2 = ROOT::Math::Dot(diffParameters * recoTrackCovMatrix, diffParameters);
      chi2/=5;
      if (chi2<chi2cut) outMap[chi2]=*st;
    }
    outputVec.push_back(RecoToSimPair(*track,outMap));
  }
  return outputVec;
}


RecoToSimCollection TrackAssociatorByChi2::associateRecoToSim(edm::Handle<reco::TrackCollection>& tCH, 
							      edm::Handle<TrackingParticleCollection>& tPCH,
							      const edm::Event * e ){

  RecoToSimCollection  outputCollection;
  double chi2;

  const TrackCollection tC = *(tCH.product());
  const TrackingParticleCollection tPC= *(tPCH.product());

  int tindex=0;
  for (TrackCollection::const_iterator rt=tC.begin(); rt!=tC.end(); rt++, tindex++){

    LogDebug("TrackAssociator") << "=========LOOKING FOR ASSOCIATION===========" << "\n"
				 << "rec::Track #"<<tindex<<" with pt=" << rt->pt() <<  "\n"
				 << "===========================================" << "\n";
 
    TrackBase::ParameterVector rParameters = rt->parameters();
    TrackBase::CovarianceMatrix recoTrackCovMatrix = rt->covariance();
    if (onlyDiagonal){
      for (unsigned int i=0;i<5;i++){
	for (unsigned int j=0;j<5;j++){
	  if (i!=j) recoTrackCovMatrix(i,j)=0;
	}
      }
    } 

    recoTrackCovMatrix.Invert();

    int tpindex =0;
    for (TrackingParticleCollection::const_iterator tp=tPC.begin(); tp!=tPC.end(); tp++, ++tpindex){
      for (TrackingParticle::g4t_iterator t=tp->g4Track_begin(); t!=tp->g4Track_end(); ++t) {
	
	//FIXME correct?
	if ((*t)->momentum().perp()<0.5) continue;
	
	Basic3DVector<double> momAtVtx((*t)->momentum().x(),(*t)->momentum().y(),(*t)->momentum().z());
	Basic3DVector<double> vert;
	const TrackingVertex * tv = &(*(tp->parentVertex()));
	int vind=0;
	for (TrackingVertex::g4v_iterator v=tv->g4Vertices_begin(); v!=tv->g4Vertices_end(); v++){
	  if (vind==(*t)->vertIndex()) 
	    vert=Basic3DVector<double>((*v)->position().x(),(*v)->position().y(),(*v)->position().z());
	  vind++;
	}

	TrackBase::ParameterVector gParameters=parametersAtClosestApproachGeom(vert, momAtVtx, rt->charge());

	//sParameters[0] = k;
	//sParameters[1] = theta;
	//sParameters[2] = phi0;
	//sParameters[3] = d0;
	//sParameters[4] = dz;
	
	//use parametersAtClosestApproachGeom
	TrackBase::ParameterVector diffParameters = rParameters - gParameters;

	chi2 = ROOT::Math::Similarity(diffParameters, recoTrackCovMatrix);
	chi2 /= 5;

	LogDebug("TrackAssociator") << "====NEW TRACKING PARTICLE WITH PT=" << (*t)->momentum().perp() << "====\n" 
				    << "k     simG: " << gParameters[0] << "\n" 
				    << "theta simG: " << gParameters[1] << "\n" 
				    << "phi0  simG: " << gParameters[2] << "\n" 
				    << "d0    simG: " << gParameters[3] << "\n" 
				    << "dz    simG: " << gParameters[4] << "\n" 
				    << ": " /*<< */ << "\n" 
				    << "k     rec: " << rt->transverseCurvature() << "\n" 
				    << "theta rec: " << rt->theta() << "\n" 
				    << "phi0  rec: " << rt->phi0() << "\n" 
				    << "d0    rec: " << rt->d0() << "\n" 
				    << "dz    rec: " << rt->dz() << "\n" 
				    << ": " /*<< */ << "\n" 
				    << "chi2: " << chi2 << "\n";

	if (chi2<chi2cut) {
	  outputCollection.insert(reco::TrackRef(tCH,tindex), 
				  std::make_pair(edm::Ref<TrackingParticleCollection>(tPCH, tpindex),chi2));
	}
      }
    }
    
  }
  return outputCollection;
}



SimToRecoCollection TrackAssociatorByChi2::associateSimToReco(edm::Handle<reco::TrackCollection>& tCH, 
							      edm::Handle<TrackingParticleCollection>& tPCH,
							      const edm::Event * e ){

  SimToRecoCollection  outputCollection;
  double chi2;

  const TrackCollection tC = *(tCH.product());
  const TrackingParticleCollection tPC= *(tPCH.product());

  int tpindex =0;
  for (TrackingParticleCollection::const_iterator tp=tPC.begin(); tp!=tPC.end(); tp++, ++tpindex){
    for (TrackingParticle::g4t_iterator t=tp->g4Track_begin(); t!=tp->g4Track_end(); ++t) {

      if ((*t)->momentum().perp()<0.5) continue;

    LogDebug("TrackAssociator") << "=========LOOKING FOR ASSOCIATION===========" << "\n"
				 << "TrackingParticle #"<<tpindex<<" with pt=" << (*t)->momentum().perp() << "\n"
				 << "===========================================" << "\n";
      
      Basic3DVector<double> momAtVtx((*t)->momentum().x(),(*t)->momentum().y(),(*t)->momentum().z());
      Basic3DVector<double> vert;//(tp->vertex().x(),tp->vertex().y(),tp->vertex().z());
      const TrackingVertex * tv = &(*(tp->parentVertex()));
      int vind=0;
      for (TrackingVertex::g4v_iterator v=tv->g4Vertices_begin(); v!=tv->g4Vertices_end(); v++){
	if (vind==(*t)->vertIndex()) vert=Basic3DVector<double>((*v)->position().x(),(*v)->position().y(),(*v)->position().z());
	vind++;
      }
     
      int tindex=0;
      for (TrackCollection::const_iterator rt=tC.begin(); rt!=tC.end(); rt++, tindex++){

	TrackBase::ParameterVector sParameters=parametersAtClosestApproachGeom(vert, momAtVtx, rt->charge());
	
	TrackBase::ParameterVector rParameters = rt->parameters();
	TrackBase::CovarianceMatrix recoTrackCovMatrix = rt->covariance();
	if (onlyDiagonal) {
	  for (unsigned int i=0;i<5;i++){
	    for (unsigned int j=0;j<5;j++){
	      if (i!=j) recoTrackCovMatrix(i,j)=0;
	    }
	  }
	}

	recoTrackCovMatrix.Invert();
	
	TrackBase::ParameterVector diffParameters = rParameters - sParameters;

	chi2 = ROOT::Math::Similarity(recoTrackCovMatrix, diffParameters);
	chi2 /= 5;
	LogDebug("TrackAssociator") << "====NEW RECO TRACK WITH PT=" << rt->pt() << "====\n" 
				    << "k     simG: " << sParameters[0] << "\n" 
				    << "theta simG: " << sParameters[1] << "\n" 
				    << "phi0  simG: " << sParameters[2] << "\n" 
				    << "d0    simG: " << sParameters[3] << "\n" 
				    << "dz    simG: " << sParameters[4] << "\n" 
				    << ": " /*<< */ << "\n" 
				    << "k     rec: " << rt->transverseCurvature() << "\n" 
				    << "theta rec: " << rt->theta() << "\n" 
				    << "phi0  rec: " << rt->phi0() << "\n" 
				    << "d0    rec: " << rt->d0() << "\n" 
				    << "dz    rec: " << rt->dz() << "\n" 
				    << ": " /*<< */ << "\n" 
				    << "chi2: " << chi2 << "\n";

	if (chi2<chi2cut) {
	  outputCollection.insert(edm::Ref<TrackingParticleCollection>(tPCH, tpindex),
				  std::make_pair(reco::TrackRef(tCH,tindex),chi2));
	}
      }
    }
    
  }
  return outputCollection;

}


TrackBase::ParameterVector TrackAssociatorByChi2::parametersAtClosestApproach2Order (Basic3DVector<double> vertex,
										     Basic3DVector<double> momAtVtx,
										     int charge) {
  GlobalVector magField = theMF->inTesla( (GlobalPoint) vertex );
  double simTrCurv = -charge*2.99792458e-3 * magField.z()/momAtVtx.perp();
  HelixExtrapolatorToLine2Order estr(vertex, momAtVtx, simTrCurv);
  
  GlobalPoint gp(0,0,0);
  GlobalVector gv(0,0,1);
  Line beamLine(gp,gv);
	
  double path = estr.pathLength(beamLine).second;
  Basic3DVector<double> pca = estr.positionInDouble(path);
  Basic3DVector<double> momAtPca = estr.directionInDouble(path);
  momAtPca*=10;
	
  double helixCenterX = 0.5*pca.x()*(vertex.perp2()-pca.perp2())/(vertex.x()*pca.x()+vertex.y()*pca.y()-pca.perp2());
  double helixCenterY = helixCenterX*pca.y()/pca.x();
  double centerToPcaX = (helixCenterX-pca.x());
  double centerToPcaY = (helixCenterY-pca.y());

  double d0simT=pca.perp();
  if ((helixCenterX*helixCenterX+helixCenterY*helixCenterY) > //should be < ???? 
      (centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY) ){
    d0simT = -d0simT;
  }

  TrackBase::ParameterVector sParameters;
  sParameters[0] = -charge*2.99792458e-3 * magField.z()/momAtPca.perp();
  sParameters[1] = momAtPca.theta();
  sParameters[2] = momAtPca.phi();
  sParameters[3] = d0simT;
  sParameters[4] = pca.z();
#if 0
  LogDebug("TrackAssociator") << "+++++++++++++++parametersAtClosestApproach2ORDER++++++++++++++" << "\n"
			       << "vertex.x(): " << vertex.x() << "\n"
			       << "vertex.y(): " << vertex.y() << "\n"
			       << "vertex.z(): " << vertex.z() << "\n"
			       << "pca.x(): " << pca.x() << "\n"
			       << "pca.y(): " << pca.y() << "\n"
			       << "pca.z(): " << pca.z() << "\n"
			       << "helixCenterX: " << helixCenterX << "\n"
			       << "helixCenterY: " << helixCenterY << "\n"
			       << "centerToPcaX: " << centerToPcaX << "\n"
			       << "centerToPcaY: " << centerToPcaY << "\n"
			       << "CO^2: " << helixCenterX*helixCenterX+helixCenterY*helixCenterY << "\n"
			       << "CA^2: " << centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY << "\n"
			       << "R: " << sqrt(centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY) << "\n"
			       << "1/R: " << 1/sqrt(centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY) << "\n"
			       << "path: " << path << "\n"
			       << "momAtPca.x(): " << momAtPca.x() << "\n"
			       << "momAtPca.y(): " << momAtPca.y() << "\n"
			       << "momAtPca.z(): " << momAtPca.z() << "\n"
			       << "momAtVtx.x(): " << momAtVtx.x() << "\n"
			       << "momAtVtx.y(): " << momAtVtx.y() << "\n"
			       << "momAtVtx.z(): " << momAtVtx.z() << "\n"
			       << "magField.z()   : " << magField.z() << "\n"
			       << "magField.perp(): " << magField.perp() << "\n"
			       << " " /*<< */ << "\n"
			       << "k    : " << sParameters[0] << "\n"
			       << "theta: " << sParameters[1] << "\n"
			       << "phi0 : " << sParameters[2] << "\n"
			       << "d0   : " << sParameters[3] << "\n"
			       << "dz   : " << sParameters[4] << "\n"
			       << " " /*<< */ << "\n"
			       << " " /*<< */ << "\n";
#endif  
  return sParameters;
}


TrackBase::ParameterVector TrackAssociatorByChi2::parametersAtClosestApproachGeom (Basic3DVector<double> vertex,
										   Basic3DVector<double> momAtVtx,
										   int charge) {
  GlobalVector magField = theMF->inTesla( (GlobalPoint) vertex );
  double simTrCurv = -charge*2.99792458e-3 * magField.z()/momAtVtx.perp();

  double rho = fabs(1/simTrCurv);

  double phiAtVtx = momAtVtx.phi();
  
  double d0sim1,s,dzsim1,beta,phi0sim ;

  d0sim1 = rho-sqrt(rho*rho+vertex.x()*vertex.x()+vertex.y()*vertex.y()
		    +2*rho*(-sin(phiAtVtx)*vertex.x()+cos(phiAtVtx)*vertex.y()));
  
  s = rho*(atan2(cos(phiAtVtx)*vertex.x()+sin(phiAtVtx)*vertex.y(),
		 rho-sin(phiAtVtx)*vertex.x()+cos(phiAtVtx)*vertex.y()));
  
  dzsim1 = vertex.z() - s*momAtVtx.z()/momAtVtx.perp();
  
  beta = atan2(rho*cos(phiAtVtx)+vertex.y(),
	       rho*sin(phiAtVtx)-vertex.x() );
  
  phi0sim = +beta-Geom::halfPi();
  
  if (beta<0) {
    phi0sim = +beta+3*Geom::halfPi();
  }
  
  //FIXME??
  phi0sim=-phi0sim;
  
  if (phi0sim<-Geom::pi()) {
    phi0sim+=2*Geom::pi();
  }

  GlobalVector pca(d0sim1*sin(phi0sim),d0sim1*cos(phi0sim),dzsim1);
  GlobalVector momAtPca(momAtVtx.perp()*cos(phi0sim),momAtVtx.perp()*sin(phi0sim),momAtVtx.z());
  double helixCenterX = (rho-d0sim1)*sin(phi0sim);
  double helixCenterY = (rho-d0sim1)*cos(phi0sim);
  double centerToPcaX = (helixCenterX-pca.x());
  double centerToPcaY = (helixCenterY-pca.y());

  TrackBase::ParameterVector sParameters;
  sParameters[0] = simTrCurv;
  sParameters[1] = momAtVtx.theta();
  sParameters[2] = phi0sim;
  sParameters[3] = d0sim1;
  sParameters[4] = dzsim1;

#if 0
  LogDebug("TrackAssociator") << "+++++++++++++++parametersAtClosestApproachGEOM++++++++++++++" << "\n"
    //<< "alpha: " << atan2(cos(phiAtVtx)*vertex.x()+sin(phiAtVtx)*vertex.y(),
    //rho-sin(phiAtVtx)*vertex.x()+cos(phiAtVtx)*vertex.y()) << "\n"
    //<< "alph1: " << Geom::pi()+atan2(cos(phiAtVtx)*vertex.x()+sin(phiAtVtx)*vertex.y(),
    //rho-sin(phiAtVtx)*vertex.x()+cos(phiAtVtx)*vertex.y()) << "\n"
    //<< "alph2: " << atan2(rho-sin(phiAtVtx)*vertex.x()+cos(phiAtVtx)*vertex.y(),
    //cos(phiAtVtx)*vertex.x()+sin(phiAtVtx)*vertex.y()) << "\n"
    //<< "beta: " << beta << "\n"    
			      << "vertex.x(): " << vertex.x() << "\n"
			      << "vertex.y(): " << vertex.y() << "\n"
			      << "vertex.z(): " << vertex.z() << "\n"
			      << "pca.x(): " << pca.x() << "\n"
			      << "pca.y(): " << pca.y() << "\n"
			      << "pca.z(): " << pca.z() << "\n"
			      << "helixCenterX: " << helixCenterX << "\n"
			      << "helixCenterY: " << helixCenterY << "\n"
			      << "centerToPcaX: " << centerToPcaX << "\n"
			      << "centerToPcaY: " << centerToPcaY << "\n"
			      << "CO^2: " << helixCenterX*helixCenterX+helixCenterY*helixCenterY << "\n"
			      << "CA^2: " << centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY << "\n"
			      << "R: " << sqrt(centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY) << "\n"
			      << "1/R: " << 1/sqrt(centerToPcaX*centerToPcaX+centerToPcaY*centerToPcaY) << "\n"
			      << "path: " << s << "\n"
			      << "momAtPca.x(): " << momAtPca.x() << "\n"
			      << "momAtPca.y(): " << momAtPca.y() << "\n"
			      << "momAtPca.z(): " << momAtPca.z() << "\n"
			      << "momAtVtx.x(): " << momAtVtx.x() << "\n"
			      << "momAtVtx.y(): " << momAtVtx.y() << "\n"
			      << "momAtVtx.z(): " << momAtVtx.z() << "\n"
			      << "magField.z()   : " << magField.z() << "\n"
			      << "magField.perp(): " << magField.perp() << "\n"
			      << " " /*<< */ << "\n"
			      << "k    : " << sParameters[0] << "\n"
			      << "theta: " << sParameters[1] << "\n"
			      << "phi0 : " << sParameters[2] << "\n"
			      << "d0   : " << sParameters[3] << "\n"
			      << "dz   : " << sParameters[4] << "\n"
			      << " " /*<< */ << "\n"
			      << " " /*<< */ << "\n";
#endif
  return sParameters;
}
	
