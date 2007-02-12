package confdb.gui;

import javax.swing.*;
import javax.swing.tree.*;
import java.awt.*;

import java.util.Enumeration;
import java.util.ArrayList;

import confdb.gui.treetable.*;

import confdb.data.Template;
import confdb.data.Instance;
import confdb.data.Parameter;
import confdb.data.VectorParameter;
import confdb.data.PSetParameter;
import confdb.data.VPSetParameter;

/**
 * ParameterTreeModel
 * ------------------
 * @author Philipp Schieferdecker
 *
 * TreeModel (TreeTableTreeModel) do display a set of parameters.
 */
public class ParameterTreeModel extends AbstractTreeTableTreeModel
{
    //
    // member data
    //
    
    /** column names */
    private static String[] columnNames = { "name",
					    "type",
					    "value",
					    "default",
					    "tracked" };
    
    /** column class types */
    private static Class[] columnTypes = { TreeTableTreeModel.class,
					   String.class,
					   String.class,
					   Boolean.class,
					   Boolean.class };

    /** list of parameters to be displayed */
    private ArrayList<Parameter> parameterList = null;
    
    /** reference to the configuration tree, to notify of changes */
    private JTree configurationTree = null;
    
    /** for orphan parameters, a default template can be set */
    private Template defaultTemplate = null;
    

    //
    // construction
    //

    /** standard constructor */
    ParameterTreeModel()
    {
	super(null);
    }
    
    
    //
    // member functions
    //

    /** TreeModel: number of children of the node */
    public int getChildCount(Object node)
    {
	Object[] children = getChildren(node);
	if (children!=null)
	    return children.length;
	return 0;
    }
    
    /** TreeModel; retreive the i-th child of the node */
    public Object getChild(Object node, int i)
    { 
	return getChildren(node)[i]; 
    }
    
    /** TreeModel: Is the node a leaf? */
    public boolean isLeaf(Object node)
    {
	boolean result = true;
	if (node.equals(root)||
	    node instanceof PSetParameter||
	    node instanceof VPSetParameter) result = false;
	return result;
    }
    
    /** TreeTableTreeModel: get number of table columns */
    public int getColumnCount() { return columnNames.length; }

    /** TreeTableTreeModel: get the name of the i-th table column */
    public String getColumnName(int column) { return columnNames[column]; }

    /** TreeTableTreeModel: get the class of the i-th table column */
    public Class getColumnClass(int column) { return columnTypes[column]; }

    /** TreeTableTreeModel: indicate if the cell is editable */
    public boolean isCellEditable(Object node,int column)
    {
	if (getColumnClass(column) == TreeTableTreeModel.class) return true;
	if (column!=2||node.equals(root)||
	    node instanceof PSetParameter||
	    node instanceof VPSetParameter) return false;
	return true;
    }
    
    /** TreeTableTreeModel: return the value of the i-th table column */
    public Object getValueAt(Object node, int column)
    {
	if (node.equals(root)) return (column==0) ? node.toString() : null;
	
	Parameter p      = (Parameter)node;
	boolean   isPSet = (p instanceof PSetParameter||
			    p instanceof VPSetParameter);

	switch (column) {
	case 0: return p.name();
	case 1: return p.type();
	case 2: return (isPSet) ? null : p.valueAsString();
	case 3: return new Boolean(p.isDefault());
	case 4: return new Boolean(p.isTracked());
	}
	return null;
    }
    
    /** TreeTableTreeModel: set the value of a parameter */
    public void setValueAt(Object value, Object node, int col) {
	if (col!=2) return;
	if (node instanceof Parameter) {
	    Parameter p = (Parameter)node;
	    
	    if (p.parent() == null) {
		String defaultAsString = getDefaultFromTemplate(p);
		p.setValue(value.toString(),defaultAsString);
		fireNodesChanged();
		return;
	    }
	    
	    try {
		String   valueAsString = p.valueAsString();
		Instance instance = (Instance)p.parent();
		Template template = instance.template();
		instance.updateParameter(p.name(),value.toString());
		fireNodesChanged();
	    }
	    catch (Exception e) {
		System.out.println("setValueAt failed: "+e.getMessage());
	    }
	    
	    if (configurationTree!=null) configurationTree.updateUI();
	}
    }

    /** retrieve the children of a Parameter node */
    private Object[] getChildren(Object node)
    {
	if (node.equals(root)) {
	    Object[] children =
		parameterList.toArray(new Parameter[parameterList.size()]);
	    return children;
	}
	if (node instanceof PSetParameter||
	    node instanceof VPSetParameter) {
	    VectorParameter pset = (VectorParameter)node;
	    Object[] children = new Parameter[pset.vectorSize()];
	    for (int i=0;i<pset.vectorSize();i++)
		children[i] = (Parameter)pset.value(i);
	    return children;
	}
	return null;
    }

    /** set the configuration-tree tree model */
    public void setConfigurationTree(JTree configurationTree)
    {
	this.configurationTree = configurationTree;
    }
    
    /** display a new set of parameters */
    public void setParameters(String moduleName,
			      ArrayList<Parameter> parameterList)
    {
	this.parameterList = parameterList;
	root = new String(moduleName);
	Object[] source   = { ParameterTreeModel.this };
	Object[] path     = { root };
	fireTreeStructureChanged(source,path,null,null);
    }
    
   /** some node has changed */
    private void fireNodesChanged()
    {
	Object[] source = { ParameterTreeModel.this };
	Object[] path   = { root };
	fireTreeNodesChanged(source,path,null,null);
    }

    /** set a default template, only considered for orphan parameters */
    public void setDefaultTemplate(Template template)
    {
	defaultTemplate = template;
    }

    /** get the default valueAsString from defaultTemplate */
    private String getDefaultFromTemplate(Parameter p)
    {
	if (defaultTemplate==null) return p.valueAsString();
	int index = parameterList.indexOf(p);
	return defaultTemplate.parameter(index).valueAsString();
    }
    
}
