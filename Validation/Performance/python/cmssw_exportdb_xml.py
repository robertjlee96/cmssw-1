from xml.dom import minidom
import types, os

def createNode(xml_doc, node_name, values = {}, parent = None):
	if (parent == None):
		parent = xml_doc
	#create node
	node = xml_doc.createElement(node_name)
	# assign the values
	for (key, value) in values.items():
		node.setAttribute(key, str(value))
	parent.appendChild(node)

	return node


def initXML(xmldoc):
	""" opens existing or creates a new XML file 
		
	  ---- one of the erliest functions - quite nasty looking :)"""	
	
	try:
		node_xml = xmldoc.getElementsByTagName("xml")[0]
	except IndexError:
		#doc = minidom.Document()
		node_xml =  createNode(xmldoc, "xml") 

	return node_xml

def xml_delete_children(nodes, child_name): 
	for x in nodes.getElementsByTagName(child_name):
		nodes.removeChild(x) 


def xml_export_ModuleTimeRecord(data_dict, curr_stat_node, xml_doc):
	#print "Dict: "+str(data_dict)
	# create a module tag to be used to put the data into
	module_node =createNode(xml_doc, "Module", values = data_dict, parent = curr_stat_node)

	# clean it up if there are old stats of ModuleTime, now there shouldn't be any anymore!
	#xml_delete_children(module_node, "ModuleTime") 

	# we create a new XML element having the statistics data
	moduletime_stat =createNode(xml_doc, "ModuleTime", values = data_dict["stats"], parent = module_node)

	module_node.appendChild(moduletime_stat)

def xml_export_EventTimeRecord(evt_time_data, curr_stat_node, xml_doc):
	#print "a"+str(evt_time_data)
	(evt_num, evt_time) = evt_time_data

	event_node =createNode(xml_doc, "EventTime", values = {"evt_num": evt_num, "time": evt_time}, parent = curr_stat_node)

def xml_export_EventRssRecord(evt_time_data, curr_stat_node, xml_doc):
	#print "a"+str(evt_time_data)
	(evt_num, evt_time) = evt_time_data
	event_node =createNode(xml_doc, "EventRSS", values = {"evt_num": evt_num, "time": evt_time}, parent = curr_stat_node)

def xml_export_EventVsizeRecord(evt_time_data, curr_stat_node, xml_doc):
	#print "a"+str(evt_time_data)
	(evt_num, evt_time) = evt_time_data
	event_node =createNode(xml_doc, "EventVSIZE", values = {"evt_num": evt_num, "time": evt_time}, parent = curr_stat_node)

def xml_export_EdmRecord(data, curr_stat_node, xml_doc):
	event_node =createNode(xml_doc, "EdmSize", values = data, parent = curr_stat_node)


def xml_export_SequenceRecord(data, curr_seq_node, xml_doc):
	seq_node =createNode(xml_doc, "Sequence", values = data, parent = curr_seq_node)

def xml_init_Sequences(xml_doc, release):
	#TODO: this was copied from init_XML - it should not be like that - refactor
	""" opens existing or creates a new XML file 
		returns the (existing or created) unique element for statiscs"""	
	try:
		node_xml = xml_doc.getElementsByTagName("xml")[0]
	except IndexError:
		#doc = minidom.Document()
		node_xml =  createNode(xml_doc, "xml") 
	
	nodes =node_xml.getElementsByTagName("Sequences")

	currentNode = [node for node in nodes 		
		if (release ==node.attributes["release"].value)]
	#TODO: end of copied

	if (currentNode):
		currentNode = currentNode[0]
		#TODO: this part looks nasty
	else:
		values={"release": release}
		# we create a new XML element having all the statistics data for our candle, step, release
		currentNode = createNode(node_name= "Sequences", xml_doc=xml_doc, parent=node_xml, 
			values=values)
		node_xml.appendChild(currentNode)
	return currentNode

def _getXMLNode(xml_doc):
	""" opens existing or creates a new XML object """
	try:
		node_xml = xml_doc.getElementsByTagName("xml")[0]
	except IndexError:
		#doc = minidom.Document()
		node_xml =  createNode(xml_doc, "xml") 
	return node_xml
	



def xml_export_Sequences(xml_doc, sequences, release):
	 currentNode = xml_init_Sequences(xml_doc, release)
	 xml_delete_children(currentNode, "Sequence") 

	 if (sequences):
		for seq in sequences:
			xml_export_SequenceRecord(data=seq, curr_seq_node=currentNode, xml_doc=xml_doc)


def export_xml(release, jobID, timelog_result, xml_doc, metadata = None, edmSize_result =None, parentNode = None):
	""" jobID is a dictionary now ! """

	if not parentNode:
		#get the root XML node
		parentNode = _getXMLNode(xml_doc)

	#create jobStats node
	values=jobID
	values.update({"release": release})
	if (metadata):
		values.update(metadata)

	# we create a new XML element having all the statistics data for our candle, step, release
	jobStatsNode = createNode(node_name= "jobStats", xml_doc=xml_doc, parent=parentNode, 
		values=values)

	#TODO: load current module data or delete

 	(mod_timelog_result, evt_timelog_result, rss_result, vsize_result) = timelog_result

	# modules data
	for (mod_name, mod_time_result) in mod_timelog_result.items():
		xml_export_ModuleTimeRecord(mod_time_result, jobStatsNode, xml_doc)

	""" #TODO: actualy we're not supposed to have any of the old statistics
	# clean it up if there are old stats of EventTime,RSS,VSIZE
	xml_delete_children(jobStatsNode, "EventTime") 
	xml_delete_children(jobStatsNode, "EventRSS") 
	xml_delete_children(jobStatsNode, "EventVSIZE") 
	xml_delete_children(jobStatsNode, "EdmSize") """

	#events data - so far only total time per event
	if (evt_timelog_result):
		for evt_time_item in evt_timelog_result:
			xml_export_EventTimeRecord(evt_time_item, jobStatsNode, xml_doc)

	# rss
	if (rss_result):
		for evt_time_item in rss_result:
			xml_export_EventRssRecord(evt_time_item, jobStatsNode, xml_doc)

	# vsize
	if (vsize_result):
		for evt_time_item in vsize_result:
			xml_export_EventVsizeRecord(evt_time_item, jobStatsNode, xml_doc)
	# edmSize
	if (edmSize_result):
		for edmItem in edmSize_result:
			xml_export_EdmRecord(edmItem, jobStatsNode, xml_doc)		


def exportRunInfo(xml_doc, run_info, release = None, print_out = False):
	node_xml = _getXMLNode(xml_doc)
	#get the simple string values
	str_values = run_info["General"]
	
	#if we have the forced release name (e.g. icludes some special tags for testing but not official release) 
	# so TODO: probably the test_release_based_on string would be the same so we still save the (original) test release based string

	if release:
		str_values["release"] = release
	else:
		str_values["release"] = str_values["test_release_based_on"]

	runInfoNode = createNode(node_name= "RunInfo", xml_doc=xml_doc, parent=node_xml, 
			values=str_values)
	#create nodes for TestResults:
	for (testName, result) in run_info["TestResults"].items():
		#either we have one node or multiple ones (if list)
		if type(result) == types.ListType:
			for result_item in result:
				result_item.update({"testName": testName})

				#We have JOBS so FAR only for TimeSize which we represent as a list
				jobs = []
				#we don't want jobs to be dumped as string
				if result_item.has_key("jobs"):
					jobs = result_item["jobs"]
					del result_item["jobs"]

				testNode = createNode(node_name="testResult", xml_doc=xml_doc, parent=runInfoNode, values=result_item) 


				for job in jobs:
					 #print job
					 export_xml(xml_doc = xml_doc, parentNode = testNode, **job)
		else:
			result.update({"testName": testName})
			createNode(node_name="testResult", xml_doc=xml_doc, parent=runInfoNode, values=result)

	#DO we have some unrecognized JOBS?
	if len(run_info['unrecognized_jobs']):
		unrecognizedJobsNode = createNode(node_name="Unrecognized_JOBS", xml_doc=xml_doc, parent=runInfoNode, values={}) 


		for job in run_info['unrecognized_jobs']:
			 #print job
			 export_xml(xml_doc = xml_doc, parentNode = unrecognizedJobsNode, **job)		

		
	#cmsSciMark
	cmsSciMarkNode = createNode(node_name="cmsSciMarks", xml_doc=xml_doc, parent=runInfoNode, values = {})
	for csiMark in run_info["cmsSciMark"]:
		#print csiMark
		createNode(node_name="cmsSciMark", xml_doc=xml_doc, parent=cmsSciMarkNode, values=csiMark)
	if print_out:
		print xml_doc.toprettyxml(indent="\t")

def exportECRules(xml_doc, rules):
	node_xml = _getXMLNode(xml_doc)
	runInfoNode = createNode(node_name= "EventContentRules", xml_doc=xml_doc, parent=node_xml, 
			values=rules)

def write_xml(xml_doc, remotedir,xmlFileName):
	xml = xml_doc.toprettyxml(indent="  ")

	# return xml as string (if requested)
	if (xmlFileName == ""):
		return xml
	# or save that as file
	out = open(xmlFileName, "w")
	#print xml locally
	out.write(xml)
	out.close()
	print "Now copying %s to remote directory %s..."%(xmlFileName,remotedir)
	#FIXME: Here we could decide to archive it on CASTOR on a dedicated directory
	if ":" in remotedir: #CAVEAT: since we report the timestamp as part of the xml filename we need to be careful..
		(host,dir)=remotedir.split(":")
		tarpipe_cmd='tar cf - %s|ssh %s "cd %s;tar xf -"'%(xmlFileName,host,dir)
	else:
		tarpipe_cmd='tar cf - %s|(cd %s;tar xf -)'%(xmlFileName,remotedir)
	try:
		print tarpipe_cmd
		os.system(tarpipe_cmd)
		print "Successfully copied XML report %s to %s"%(xmlFileName,remotedir)
	except Exception,e :
		print "Issues with copying XML report %s to the remote directory %s!\n%s"%(xmlFileName,remotedir,str(e))
		
	
