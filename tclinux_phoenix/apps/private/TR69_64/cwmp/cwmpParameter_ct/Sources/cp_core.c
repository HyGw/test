#include "Global_res.h"
//#include "AsEngine.h"
#include "cp_core.h"
#include "cp_dynamicnodetable.h"

//#define DEBUG_REGISTERNODE

/*
    Description:
    The DeviceNodeAlloc routine is called to alloc resource for TR069 node

    Inputs:
        IndexStr:       - node name
        type:            - node type, see enum NodeType
        attrFlag:        - node attribute setting flag, see enum NodeAttrType
        nodeFunc	  - node function

    Returns:
        theResult:
            node point      - no error
            NULL              - alloc resource for TR069 node failed
*/
DeviceNodePtr DeviceNodeAlloc(char *IndexStr, NodeType type, NodeAttrType attrFlag, NodeFunc *nodeFunc)
{
	DeviceNodePtr	 deviceNode = NULL;

	if(IndexStr == NULL)
	{
		tc_printf("DeviceNodeAlloc: Node name ptr or nodeFunc ptr is NULL \n");
		return NULL;
	}
	 if ((deviceNode = os_malloc(sizeof(DeviceNode))) == NULL){
		tc_printf("malloc fail in DeviceNodeAllocObjectW!\n");	
		return NULL;		
	 }	 

	memset(deviceNode, 0, sizeof(DeviceNode));
	if(strlen(IndexStr) > PARAMETER_NAME_LEN-1)
	{
		tc_printf("DeviceNodeAlloc: Node name length greater than %d \n", PARAMETER_NAME_LEN-1);
		os_free(deviceNode);
		return NULL;
	}
	
	strcpy( deviceNode->name,IndexStr);
	deviceNode->flag = attrFlag;
	if(type != ObjectR)
	{
		if(nodeFunc == NULL)
		{
			tc_printf("DeviceNodeAlloc:Node Type is not Object ReadOnly But NodeFunc ptr is NULL \n");
			os_free(deviceNode);
			return NULL;
		}
		deviceNode->addObject = nodeFunc->addObject;
		deviceNode->deleteObject= nodeFunc->deleteObject;
		deviceNode->getValueInfo = nodeFunc->getValueInfo;
		deviceNode->getAttribute = nodeFunc->getAttribute;
		deviceNode->setValueInfo = nodeFunc->setValueInfo;
		deviceNode->setAttribute = nodeFunc->setAttribute;
	}
	switch(type)
	{
		case ObjectR:
			deviceNode->type = CWMP_OBJECT;
			deviceNode->accessType = ReadOnly;	
			break;
		case ObjectW:
			deviceNode->type = CWMP_OBJECT;
			deviceNode->accessType = Writable;				
			break;			
		case StrW:
			deviceNode->type = CWMP_STRING;
			deviceNode->accessType = Writable;	
			break;
		case StrR:
			deviceNode->type = CWMP_STRING;
			deviceNode->accessType = ReadOnly;	
			break;		
		case BooleanW:
			deviceNode->type = CWMP_BOOLEAN;
			deviceNode->accessType = Writable;	
			break;		
		case BooleanR:
			deviceNode->type = CWMP_BOOLEAN;
			deviceNode->accessType = ReadOnly;	
			break;
		case UnsignIntW:
			deviceNode->type = CWMP_UNSIGNEDINT;
			deviceNode->accessType = Writable;	
			break;
		case UnsignIntR:
			deviceNode->type = CWMP_UNSIGNEDINT;
			deviceNode->accessType = ReadOnly;	
			break;
		case IntR:
			deviceNode->type = CWMP_INTEGER;
			deviceNode->accessType = ReadOnly;	
			break;
		case IntW:
			deviceNode->type = CWMP_INTEGER;
			deviceNode->accessType = Writable;	
			break;
		case DateTimeTypeW:
			deviceNode->type = CWMP_DATE_TIME;
			deviceNode->accessType = Writable;	
			break;
		case DateTimeTypeR:
			deviceNode->type = CWMP_DATE_TIME;
			deviceNode->accessType = ReadOnly;	
			break;	
		#ifdef DHCP_PROFILE
		case Base64W:
			deviceNode->type = CWMP_BASE64;
			deviceNode->accessType = Writable;	
			break;	
		case Base64R:
			deviceNode->type = CWMP_BASE64;
			deviceNode->accessType = ReadOnly;	
			break;		
		#endif
		default:
			tc_printf("DeviceNodeAlloc: Enter default case, we will not register node [%s]\n", IndexStr);
			os_free(deviceNode);
			return NULL;
			break;
	}						
	return deviceNode;
} 


/*
    Description:
    The RegisterNodeFromTable routine is called to regist TR069 node from node Table

    Inputs:
        parent:                  - parent node for Node registed
        pNodeStruct:          - node table which want to registe

    Returns:
        theResult:
            parent node point      - no error
            NULL                       - regist TR069 node failed
*/
DeviceNodePtr RegisterNodeFromTable(DeviceNodePtr parent, CwmpParameterStruct *pNodeStruct)
{
	int i = 0;
	DeviceNodePtr temp = NULL;
	if(pNodeStruct == NULL)
	{
		tc_printf("RegisterNodeFromTable: Input parameter Invalid, we will return\n");
		return NULL;
	}
#ifdef	DEBUG_REGISTERNODE
	tc_printf("RegisterNodeFromTable: parent is [%08x] pNodeStruct is [%08x]\n", parent, pNodeStruct);
#endif

	/*Parent is NULL, we init Top Tree Node*/
	if(parent == NULL )
	{
		if(pNodeStruct->name == NULL)
		{
			tc_printf("RegisterNodeFromTable: Top Tree Node is NULL\n");
			return NULL;
		}

		temp = DeviceNodeAlloc(pNodeStruct->name, pNodeStruct->type, pNodeStruct->attrFlag, pNodeStruct->func);
		if(temp == NULL)
		{
			tc_printf("RegisterNodeFromTable: temp is NULL, we will not register node [%s]\n", pNodeStruct->name);
			return NULL;
		}
		
#ifdef	DEBUG_REGISTERNODE
		tc_printf("RegisterNodeFromTable: Top Node name is [%s] Node Ptr is [%08x]\n", pNodeStruct->name, temp);
#endif

		/*Object Readonly or Writable Node, we call Special Register Node function, we also Register next Level Node*/
		if(pNodeStruct->type == ObjectR || pNodeStruct->type == ObjectW)
		{
			if(pNodeStruct->next != NULL)
			{
				if(RegisterNodeFromTable(temp, pNodeStruct->next) == NULL)
				{
					tc_printf("RegisterNodeFromTable: ObjectR Node Failed, we will not register node [%s]\n", pNodeStruct->name);
					goto Error_Handle_ROOT;
				}
			}
			
			if(pNodeStruct->func != NULL && pNodeStruct->func->dynFunc != NULL)
			{
				if(pNodeStruct->func->dynFunc(temp) != 0)
				{
					tc_printf("RegisterNodeFromTable: Object Node Dynamic Function executeFailed, we will not register node [%s]\n", pNodeStruct->name);
					goto Error_Handle_ROOT;
				}
			}
		}
#if 0
		if(pNodeStruct->type == ObjectR)
		{
			if(RegisterNodeFromTable(temp, pNodeStruct->next) == NULL)
			{
				cwmpNodeFree(temp);
				temp = NULL;
			}		
		}
#endif
	/*Return Top Tree Node*/
	return temp;
Error_Handle_ROOT:
#ifdef	DEBUG_REGISTERNODE
	tc_printf("RegisterNodeFromTable: ERROR Handle node name is [%s]\n", pNodeStruct->name);
#endif
	cwmpNodeFree(temp);
	temp = NULL;
	return NULL;
	}

	/*Parent is not NULL, we init Object or Leaf node*/
	while(pNodeStruct)
	{
		temp = NULL;
		/*End Table, we will return*/
		if(pNodeStruct->name == NULL)
		{
			break;
		}

		/*Alloc Resource foe New Node*/
		temp = DeviceNodeAlloc(pNodeStruct->name, pNodeStruct->type, pNodeStruct->attrFlag, pNodeStruct->func);
		if(temp == NULL)
		{
			tc_printf("RegisterNodeFromTable: temp is NULL, we will not register node [%s]\n", pNodeStruct->name);
			goto Error_Handle;
		}
		
#ifdef	DEBUG_REGISTERNODE
		tc_printf("RegisterNodeFromTable: Node name is [%s] Node Ptr is [%08x]\n", pNodeStruct->name, temp);
#endif

		/*Append New node to Parent Node*/
		cwmpNodeAppendChild(parent, temp);

		/*Object Readonly or Writable Node, we call Special Register Node function, we also Register next Level Node*/
		if(pNodeStruct->type == ObjectR || pNodeStruct->type == ObjectW)
		{
			if(pNodeStruct->func != NULL && pNodeStruct->func->dynFunc != NULL)
			{
				if(pNodeStruct->func->dynFunc(temp) != 0)
				{
					tc_printf("RegisterNodeFromTable: Object Node Dynamic Function executeFailed, we will not register node [%s]\n", pNodeStruct->name);
					goto Error_Handle;
				}
			}

			if(pNodeStruct->next != NULL)
			{
				if(RegisterNodeFromTable(temp, pNodeStruct->next) == NULL)
				{
					tc_printf("RegisterNodeFromTable: ObjectR Node Failed, we will not register node [%s]\n", pNodeStruct->name);
					goto Error_Handle;
				}
			}
		}

#if 0
		/*Object Writable Node, we call Special Register Node function*/
		if(pNodeStruct->type == ObjectW)
		{
			if(pNodeStruct->func->dynFunc != NULL)
			{
				if(pNodeStruct->func->dynFunc(temp) != 0)
				{
					tc_printf("RegisterNodeFromTable: ObjectW Node Failed, we will not register node [%s]\n", pNodeStruct->name);
					goto Error_Handle;
				}
			}
			else
			{
				tc_printf("RegisterNodeFromTable: ObjectW Node Dynamic function is NULL, we will not register node [%s]\n", pNodeStruct->name);
				goto Error_Handle;
			}
		}
#endif
		pNodeStruct++;
	
	}
	return parent;
	
Error_Handle:
#ifdef	DEBUG_REGISTERNODE
	tc_printf("RegisterNodeFromTable: ERROR Handle node name is [%s]\n", pNodeStruct->name);
#endif
	cwmpNodeFree(parent->firstChild);
	parent->firstChild = NULL;
	return NULL;	
}


int check_str_isint(char *psrc)
{
	int num = 0;
	char *p = NULL;
	if(psrc == NULL)
		return -1;
	p = psrc;
	while((*p) != '\0')
	{
		if((*p) >= '0' && (*p) <= '9')
		{
			num *= 10;
			num += (*p) - '0';
			p++;
		}
		else
		{
			/*String contain no 0-9 character*/
			return -1;
		}
	}
	return num;
	
}

/*
    Description:
    The RegisterNodeDynamic routine is called to regist TR069 node from node Table

    Inputs:
        parentNodeName     - parent node name for Node registed
        pNodeStruct           - node table which want to registe
        Flag			    - Add Dynamic Node or Static Node

    Returns:
        theResult:
            0     - no error
            -1   - regist TR069 node failed
*/
int RegisterNodeDynamic(DeviceNodePtr root, char *parentNodeName, CwmpParameterStruct *pNodeStruct, NodeAddType Flag)
{
	DeviceNodePtr rootptr = NULL;
	DeviceNodePtr nodeadd = NULL;
	DeviceNodePtr child = NULL;
	if((root == NULL) || (parentNodeName == NULL) || (pNodeStruct == NULL) )
	{
		tc_printf("RegisterNodeDynamic:  Input parameter Invalid, we will return\n");
		return -1;

	}
	rootptr = root;

	/*Search Parent node by name*/
	cwmpSearchDeviceNodeByName(parentNodeName, rootptr, &nodeadd);
	if(nodeadd == NULL)
	{
		tc_printf("RegisterNodeDynamic: parentNode [%s] can not find, we will return\n", parentNodeName);
		return -1;
	}
#ifdef	DEBUG_REGISTERNODE
	tc_printf("*******************RegisterNodeDynamic: Register Begin*****************\n");
#endif			
	if(nodeadd->type == CWMP_OBJECT)
	{
		/*Static node add*/
		if(Static == Flag)
		{
			if(RegisterNodeFromTable(nodeadd, pNodeStruct) == NULL)
			{
				tc_printf("RegisterNodeDynamic: add Static Node to parentNode [%s] failed\n", parentNodeName);
			}
		}
		/*Dynamic node add*/
		else
		{
			/*Add NodeTable to .1, .2 .3 ...*/
			child = nodeadd->firstChild;
			while(child != NULL)
			{
				/*check node name is 1,2,3,... and node type is object*/
				if( (check_str_isint(child->name) > 0) && (child->type == CWMP_OBJECT))
				{
					if(RegisterNodeFromTable(child, pNodeStruct) == NULL)
					{
						tc_printf("RegisterNodeDynamic: add Static Node to parentNode [%s] failed\n", parentNodeName);
					}
				}
				child = child->nextSibling;
			}
		}
	}
	else
	{
		tc_printf("cwmp_add_node: Parent Node type is not object parentNode [%s] Type is[%d]\n", parentNodeName, nodeadd->type);
	}
#ifdef	DEBUG_REGISTERNODE
	tc_printf("*******************RegisterNodeDynamic: Register End*****************\n");
#endif
	return 0;

}

/*
    Description:
    The check_child routine is check node which name is childNodeName is child of node which name is parentNodeName

    Inputs:
        parentNodeName   - node name assumed as parent node 
        childNodeName    - node name assumed as child node 

    Returns:
        theResult:
            0    - node which name is childNodeName is not a child
            1    - node which name is childNodeName is a child of node which name is parentNodeName
*/
int check_child(char *parentNodeName, char *childNodeName)
{
	int lenParent = 0;
	int lenChild = 0;
	if((parentNodeName == NULL) || (childNodeName == NULL))
	{
		return 1;
	}
	lenParent = strlen(parentNodeName);
	lenChild = strlen(childNodeName);
	if(lenParent > lenChild)
	{
		return 0;
	}
	if(strncmp(parentNodeName, childNodeName, lenParent) == 0)
	{
		return 1;
	}
	return 0;	
}

/*
    Description:
    The InitDynamicNode routine is called to regist TR069 Dynamic node from Dynamic node Table

    Inputs:
        root     		- root node tree ptr for Node registed
		FreeNodeName    - Node name which node reinit

    Returns:
        theResult:
            0     - no error
            -1   - regist TR069 node failed
*/
int InitDynamicNode(DeviceNodePtr root, char *FreeNodeName)
{
	int i = 0;
	CwmpDynamicNodeStruct *table = DynamicNode_table;
	if(table == NULL)
	{
		tc_printf("InitDynamicNode: table is NULL!\n");
		return -1;
	}
	for(i = 0 ;; i++){
		if(table[i].name == NULL){
			break;
		}
		if(check_child(FreeNodeName, table[i].name))
		{
			if(RegisterNodeDynamic(root, table[i].name, (CwmpParameterStruct *)table[i].nodetable, table[i].type) != 0)
			{
				tc_printf("InitDynamicNode: table[%d] init failed! parent Node name is [&s]\n", i, table[i].name);
			}
		}
		else
		{
		#ifdef	DEBUG_REGISTERNODE
			if(FreeNodeName != NULL)
			{
				tc_printf("InitDynamicNode: table[%d] init failed! parent Node name is [%s] is not child of FreeNodeName [%s]\n", i, table[i].name, FreeNodeName);
			}
		#endif		
		}
	}
	return 0;
}

/***********************************************
**	this function is used for register node like 
**	*.1.
**	*.2.
**	*.3.
**	...
/***********************************************/
int RegisterDynNodeFromIndex( DeviceNodePtr deviceNode, CwmpParameterStruct *pNodeStruct, int NUM )
{
	int i,rtnCode = -1;
	char nodeIndex[10];

	memset( nodeIndex, 0, sizeof(nodeIndex) );

	/*check input parameter*/
	if( deviceNode == NULL ) {
		tc_printf( " ==>RegisterDynNodeFromIndex(): deviceNode should not be NULL.\n" );
		return rtnCode;
	}

	if( pNodeStruct == NULL ) {
		tc_printf( " ==>RegisterDynNodeFromIndex(): pNodeStruct should not be NULL.\n" );
		return rtnCode;
	}

	if( NUM < 0 ) {
		tc_printf( " ==>RegisterDynNodeFromIndex(): NUM should not be smaller than 0.\n" );
		return rtnCode;
	}
	
	for ( i = 0; i < NUM; i++ ) {
		sprintf( nodeIndex, "%d", i + 1 );
		pNodeStruct[0].name = nodeIndex;

		if ( RegisterNodeFromTable( deviceNode, pNodeStruct ) == NULL ) {
			
			/*register fail*/
			tc_printf( " ==>RegisterDynNodeFromIndex(): call RegisterNodeFromTable fail.\n" );
			return rtnCode;
		}
	}

	rtnCode = 0;
	return rtnCode;
}

#if 0
int cwmpInitParameterTree()
{
	DeviceNodePtr top = NULL;
	

}
#endif

