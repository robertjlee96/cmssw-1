//************************************
//* Generators interface with the LCG MCDB
//*
//* Hector Naves Sordo 
//* 
//* First version: 25/10/06 
//
//*Sandro Fonseca de Souza: 20/11/08
// Fixed a MCDB  
//************************************

#include "FWCore/Framework/interface/Event.h"
#include "Utilities/StorageFactory/interface/StorageFactory.h"
#include "Utilities/StorageFactory/interface/StorageAccount.h"
#include "FWCore/PluginManager/interface/PluginManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

// Makes a local copy of a CASTOR file.
// This code is a modified version of 
// Utilities/StorageFactory/test/any.cpp by Vincenzo Innocente
//
void mcdbGetInputFile( std::string &compHEPInputFile, int &mcdbArticleID);


