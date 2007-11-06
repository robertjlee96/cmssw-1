package confdb.data;

import java.util.ArrayList;
import java.util.Iterator;


/**
 * ModifierInstructions
 * --------------------
 * @author Philipp Schieferdecker
 *
 * Instructions for the ConfigurationModifier how to filter/manipulate its
 * master configuration. 
 */
public class ModifierInstructions
{
    //
    // member data
    //

    /** global PSets */
    private boolean filterAllPSets = false;
    private ArrayList<String> psetBlackList = new ArrayList<String>();
    private ArrayList<String> psetWhiteList = new ArrayList<String>();
    
    /** EDSources */
    private boolean filterAllEDSources = false;
    private ArrayList<String> edsourceBlackList = new ArrayList<String>();
    private ArrayList<String> edsourceWhiteList = new ArrayList<String>();

    /** ESSources */
    private boolean filterAllESSources = false;
    private ArrayList<String> essourceBlackList = new ArrayList<String>();
    private ArrayList<String> essourceWhiteList = new ArrayList<String>();

    /** ESModules */
    private boolean filterAllESModules = false;
    private ArrayList<String> esmoduleBlackList = new ArrayList<String>();
    private ArrayList<String> esmoduleWhiteList = new ArrayList<String>();
    
    /** Services */
    private boolean filterAllServices = false;
    private ArrayList<String> serviceBlackList = new ArrayList<String>();
    private ArrayList<String> serviceWhiteList = new ArrayList<String>();
    
    /** Paths */
    private boolean filterAllPaths = false;
    private ArrayList<String> pathBlackList = new ArrayList<String>();
    private ArrayList<String> pathWhiteList = new ArrayList<String>();

    /** sequences requested regardless of being referenced in requested paths */
    private ArrayList<String> requestedSequences = new ArrayList<String>();

    /** modules reqested regardless of being referenced in requested path */
    private ArrayList<String> requestedModules = new ArrayList<String>();
    
    /** template for the EDSource to be substituted (if any) */
    private EDSourceTemplate edsourceT = null;
    
    
    //
    // construction
    //

    /** standard constructor */
    public ModifierInstructions() {}
    
    
    //
    // member functions
    //

    /** resolve white-lists based on a given configuration */
    public boolean resolve(IConfiguration config)
    {
	if (!filterAllPSets&&psetWhiteList.size()>0) {
	    if (psetBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for global psets.");
		return false;
	    }
	    else {
		Iterator it = config.psetIterator();
		while (it.hasNext()) {
		    PSetParameter pset = (PSetParameter)it.next();
		    if (!psetWhiteList.contains(pset.name()))
			psetBlackList.add(pset.name());
		}
	    }
	}
	
	if (!filterAllEDSources&&edsourceWhiteList.size()>0) {
	    if (edsourceBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for edsources.");
		return false;
	    }
	    else {
		Iterator it = config.edsourceIterator();
		while (it.hasNext()) {
		    EDSourceInstance edsource = (EDSourceInstance)it.next();
		    if (!edsourceWhiteList.contains(edsource.name()))
			edsourceBlackList.add(edsource.name());
		}
	    }
	}
	
	if (!filterAllESSources&&essourceWhiteList.size()>0) {
	    if (essourceBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for essources.");
		return false;
	    }
	    else {
		Iterator it = config.essourceIterator();
		while (it.hasNext()) {
		    ESSourceInstance essource = (ESSourceInstance)it.next();
		    if (!essourceWhiteList.contains(essource.name()))
			essourceBlackList.add(essource.name());
		}
	    }
	}
	
	if (!filterAllESModules&&esmoduleWhiteList.size()>0) {
	    if (esmoduleBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for esmodules.");
		return false;
	    }
	    else {
		Iterator it = config.esmoduleIterator();
		while (it.hasNext()) {
		    ESModuleInstance esmodule = (ESModuleInstance)it.next();
		    if (!esmoduleWhiteList.contains(esmodule.name()))
			esmoduleBlackList.add(esmodule.name());
		}
	    }
	}
	
	if (!filterAllServices&&serviceWhiteList.size()>0) {
	    if (serviceBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for services.");
		return false;
	    }
	    else {
		Iterator it = config.serviceIterator();
		while (it.hasNext()) {
		    ServiceInstance service = (ServiceInstance)it.next();
		    if (!serviceWhiteList.contains(service.name()))
			serviceBlackList.add(service.name());
		}
	    }
	}
	
	if (!filterAllPaths&&pathWhiteList.size()>0) {
	    if (pathBlackList.size()>0) {
		System.err.println("ModifierInstructions.resolve ERROR: " +
				   "white&black lists provided for paths.");
		return false;
	    }
	    else {
		Iterator it = config.pathIterator();
		while (it.hasNext()) {
		    Path path = (Path)it.next();
		    if (!pathWhiteList.contains(path.name()))
			pathBlackList.add(path.name());
		}
	    }
	}
	
	return true;
    }

    /** check filter flags */
    public boolean doFilterAllPSets()     { return filterAllPSets; }
    public boolean doFilterAllEDSources() { return filterAllEDSources; }
    public boolean doFilterAllESSources() { return filterAllESSources; }
    public boolean doFilterAllESModules() { return filterAllESModules; }
    public boolean doFilterAllServices()  { return filterAllServices; }
    public boolean doFilterAllPaths()     { return filterAllPaths; }
    
    /** check if passed object is in a blacklist */
    public boolean isInBlackList(Object obj)
    {
	if (obj instanceof PSetParameter) {
	    PSetParameter pset = (PSetParameter)obj;
	    return psetBlackList.contains(pset.name());
	}
	else if (obj instanceof EDSourceInstance) {
	    EDSourceInstance edsource = (EDSourceInstance)obj;
	    return edsourceBlackList.contains(edsource.name());
	}
	else if (obj instanceof ESSourceInstance) {
	    ESSourceInstance essource = (ESSourceInstance)obj;
	    return essourceBlackList.contains(essource.name());
	}
	else if (obj instanceof ESModuleInstance) {
	    ESModuleInstance esmodule = (ESModuleInstance)obj;
	    return esmoduleBlackList.contains(esmodule.name());
	}
	else if (obj instanceof ServiceInstance) {
	    ServiceInstance edsource = (ServiceInstance)obj;
	    return edsourceBlackList.contains(edsource.name());
	}
	else if (obj instanceof Path) {
	    Path path = (Path)obj;
	    return pathBlackList.contains(path.name());
	}
	return false;
    }

    /** check if an EDSource is to be added */
    public boolean doInsertEDSource() { return (edsourceT!=null); }
    
    /** retrieve the EDSource to be added */
    public EDSourceInstance edsourceToBeAdded()
    {
	if (edsourceT==null) return null;
	edsourceT.removeAllInstances();
	EDSourceInstance result = null;
	try {
	    result = (EDSourceInstance)edsourceT.instance();
	}
	catch (DataException e) {
	    System.err.println(e.getMessage());
	}
	return result;
    }

    /** get iterator for requested sequences */
    public Iterator requestedSequenceIterator()
    {
	return requestedSequences.iterator();
    }

    /** get iterator for requested modules */
    public Iterator requestedModuleIterator()
    {
	return requestedModules.iterator();
    }
    
    /** filter all plugins of a certain type */
    public void filterAllPSets()     { filterAllPSets     = true; }
    public void filterAllEDSources() { filterAllEDSources = true; }
    public void filterAllESSources() { filterAllESSources = true; }
    public void filterAllESModules() { filterAllESModules = true; }
    public void filterAllServices()  { filterAllServices  = true; }
    public void filterAllPaths()     { filterAllPaths     = true; }
    
    /** request a sequence regardless of it being referenced in path */
    public void requestSequence(String sequenceName)
    {
	requestedSequences.add(sequenceName);
    }

    /** request a module regardless of it being referenced in path */
    public void requestModule(String moduleName)
    {
	requestedModules.add(moduleName);
    }
    
    /** insert a DaqSource */
    public void insertDaqSource()
    {
	filterAllEDSources = true;
	ArrayList<Parameter> params = new ArrayList<Parameter>();
	params.add(new StringParameter("readerPluginName","FUShmReader",true,false));
	edsourceT = new EDSourceTemplate("DaqSource","UNKNOWN",-1,params);
    }
    
    /** insert PoolSource [inputFiles = comma-separated list!] */
    public void insertPoolSource(String inputFiles)
    {
	filterAllEDSources = true;
	ArrayList<Parameter> params = new ArrayList<Parameter>();
	params.add(new VStringParameter("fileNames",inputFiles,false,false));
	edsourceT = new EDSourceTemplate("PoolSource","UNKNOWN",-1,params);
    }
    
}
