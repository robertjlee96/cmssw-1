#include "CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

CSCReadoutMappingFromFile::CSCReadoutMappingFromFile( std::string filename ) 
  : filename_( filename ) { fill(); }

CSCReadoutMappingFromFile::~CSCReadoutMappingFromFile(){}

void CSCReadoutMappingFromFile::fill( void ) {
  std::ifstream in( filename_.c_str() );
  std::string line;
  const std::string commentFlag = "#";
  if ( !in ) {
    std::cout << "CSCReadoutMappingFromFile: ERROR! Failed to open file containing mapping, " <<
      filename_ << std::endl ;
  }
  else
  {
    std::cout << "CSCReadoutMappingFromFile: opened file containing mapping, " << 
      filename_ << std::endl ;

    while ( getline(in, line) ) { // getline() from <string>
      if ( debugV() ) std::cout << line << std::endl;
      if ( line[0] != commentFlag[0] ) {
        int i1, i2, i3, i4, i5, i6, i7, i8, i9;
	std::istringstream is( line );
        is >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8 >> i9 ;
        if ( debugV() ) std::cout << i1 << " " << i2 << " " << i3 << " " << i4 << " " <<
	  i5 << " " << i6 << " " << i7 << " " << i8 << " " << i9 << std::endl;
        addRecord( i1, i2, i3, i4, i5, i6, i7, i8, i9 );
      }
    }

  }

  return;
}

