#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

const float CaloCellGeometry::k_ScaleFromDDDtoGeant ( 0.1 ) ;

const CaloCellGeometry::CornersVec&
CaloCellGeometry::getCorners() const
{
   return m_corners ;
}

std::ostream& operator<<( std::ostream& s, const CaloCellGeometry& cell ) 
{
   s << ", Center: " <<  cell.getPosition() << std::endl;

   if( cell.emptyCorners() )
   {
      s << "Corners vector is empty." << std::endl ;
   }
   else
   {
      const CaloCellGeometry::CornersVec& corners ( cell.getCorners() ) ;
      for ( unsigned int i ( 0 ) ; i != corners.size() ; ++i ) 
      {
	 s << "Corner: " << corners[ i ] << std::endl;
      }
   }
   return s ;
}

const float* 
CaloCellGeometry::getParmPtr(
   const std::vector<double>&   vd ,
   CaloCellGeometry::ParMgr*    mgr ,
   CaloCellGeometry::ParVecVec& pvv  )
{
   const float* pP ( 0 ) ;

   std::vector<float> vv ;
   vv.reserve( vd.size() ) ;

   for( unsigned int i ( 0 ) ; i != vd.size() ; ++i )
   {
      vv.push_back( CaloCellGeometry::k_ScaleFromDDDtoGeant*vd[i] ) ;
   }
   for( unsigned int ii ( 0 ) ; ii != pvv.size() ; ++ii )
   {
      const ParVec& v ( pvv[ii] ) ;
      assert( v.size() == vd.size() ) ;

      bool same ( true ) ;
      for( unsigned int j ( 0 ) ; j != vd.size() ; ++j )
      {
	 same = same && ( vv[j] == v[j] ) ;
	 if( !same ) break ;
      }
      if( same )
      {
	 pP = &(*v.begin()) ;
	 break ;
      }
   }
   if( 0 == pP )
   {
      pvv.push_back( ParVec( mgr ) ) ;
      ParVec& back ( pvv.back() ) ;
      for( unsigned int i ( 0 ) ; i != vd.size() ; ++i )
      {
	 back[i] = vv[i] ;
      }
      pP = &(*back.begin()) ;
   }
   return pP ;
}
