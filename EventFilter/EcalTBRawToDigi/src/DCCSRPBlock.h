// Date   : 30/05/2005
// Author : N.Almeida (LIP)


#ifndef DCCTBSRPBLOCK_HH
#define DCCTBSRPBLOCK_HH

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>


#include "DCCBlockPrototype.h"

class DCCTBEventBlock;
class DCCTBXtalBlock;
class DCCTBDataParser;

class DCCTBSRPBlock : public DCCTBBlockPrototype {
	
	public :
		
		DCCTBSRPBlock(
			DCCTBEventBlock * dccBlock,
			DCCTBDataParser * parser, 
			ulong * buffer, 
			ulong numbBytes,
			ulong wordsToEnd, 
			ulong wordEventOffset
		);
	
		
		
	protected :
		
		void dataCheck();
		
		void  increment(ulong numb);
		
		enum srpFields{ 
			BXMASK = 0xFFF,
			L1MASK = 0xFFF, 
			BPOSITION_BLOCKID = 29,
			BLOCKID = 4
		};
	
		DCCTBEventBlock * dccBlock_;
		
		
		
};

#endif
