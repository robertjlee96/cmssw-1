
'''
central place for all the table names
'''
def lumisummaryTableName():
	return 'LUMISUMMARY'

def lumidetailTableName():
	return 'LUMIDETAIL'

def trgTableName():
	return 'TRG'

def hltTableName():
	return 'HLT'

def trghltMapTableName():
        return 'TRGHLTMAP'

def lumiresultTableName():
	return 'INTLUMI'

def lumihltresultTableName():
	return 'INTLUMIHLT'

def idTableName( dataTableName ):
	return dataTableName+"_ID"

if __name__ == "__main__":
    pass

