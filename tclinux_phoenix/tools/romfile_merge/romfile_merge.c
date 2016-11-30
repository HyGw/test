
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "romfile_merge.h"
#include "mxml.h"
#define FAIL -1
/*______________________________________________________________________________
**	isNode
**
**	descriptions:
**				 	Check the name is used for node.
**
**	parameters:
**		name: 		Specify the element of name.
**
**	return:
**		Success: 	1
**		Otherwise: 	0
**____________________________________________________________________________
*/
int
isNode(const char *name){
	return 1;
}/* end isNode */

/*______________________________________________________________________________
**	whitespace_cb
**
**	descriptions:
**		Let the mxmlSaveFile() function know when to insert newlines and tabs...
**	parameters:
**		None
**	return:
**		Success:  newlines
**		Otherwise: Null
**____________________________________________________________________________
*/

/*
 * 'whitespace_cb()' - Let the mxmlSaveFile() function know when to insert
 *                     newlines and tabs...
 */

const char *				/* O - Whitespace string or NULL */
whitespace_cb(mxml_node_t *node,	/* I - Element node */
              int         where)	/* I - Open or close tag? */
{
	mxml_node_t	*parent= NULL;	/* Parent node */
	int		level;			/* Indentation level */
	const char	*name= NULL;			/* Name of element */
	static const char *tabs = "\t\t\t\t\t\t\t\t";
					/* Tabs for indentation */

	/*
	* We can conditionally break to a new line before or after any element.
	* These are just common HTML elements...
	*/

	name = node->value.element.name;
	if((where  == MXML_WS_BEFORE_OPEN)
	|| (isNode(name) && (where ==MXML_WS_BEFORE_CLOSE))){
		for (level = -1, parent = node->parent;
		     parent;
		 level ++, parent = parent->parent);

		if (level > 8){
			level = 8;
		}
		else if (level < 0){
			level = 0;
		}
		return (tabs + 8 - level);
	}
	else if ((where == MXML_WS_AFTER_CLOSE)
	||(isNode(name) && where == MXML_WS_AFTER_OPEN)){
		return ("\n");
	}
	else if (where == MXML_WS_AFTER_OPEN && !node->child){
		return ("\n");
	}

	/*
	* Return NULL for no added whitespace...
	*/
	return (NULL);
}/* end whitespace_cb */


/*______________________________________________________________________________
**	load_def_romfile
**
**	descriptions:
**		To parser default romfile.cfg
**
**	parameters:
**		file: 			Specify the file path.
**
**	return:
**		Success: 	root of mxml_node_t address
**		Otherwise: 	NULL
**____________________________________________________________________________
*/
mxml_node_t *
load_romfile(char *file){
	FILE *fp=NULL;
	mxml_node_t *tree=NULL;
	char buf[128];
	fp = fopen(file,"r");
	if(fp == NULL){
		fprintf(stderr, OPEN_FAIL_MSG, file, strerror(errno));
		return NULL;
	}

	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	if(tree == NULL){
		fprintf(stderr, DEF_ROMFILE_ERR_MSG);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return tree;
}



int 
getOptionFromConfigFile(char* opt,char* configFile)
{
        FILE *fp =NULL;
        char line[512] = {0};
        int rev = 0;
	char *ptr = NULL;

        fp = fopen(configFile,"r");
         if(opt[0] == '!')
        {
                rev = 1;
               opt++;
        }
        //printf("configFile:%s\n",configFile);
#ifdef DEBUG
        printf("rev:%d, macro:%s\n",rev,opt);
#endif
        if(fp == NULL)
        {
                printf("error opening configure file!\n");
                exit(1);
        }

        while(fgets (line, sizeof(line), fp) )
        {
        	   ptr = strstr(line,opt);
			   
                  if(ptr && strstr(ptr+strlen(opt), "y"))
                  {
                        fclose(fp);
                       // printf("SUCESS\n");
                        if(rev == 0)
                                return 1;
                        else
                                return 0;
                  }
                memset(line, 0, sizeof(line));
        }

        fclose(fp);
//      printf("not found\n");
        if(rev == 0)
                return 0;
        else
                return 1;
}

int 
parseOption(char *nodeName, char *fileOpt, char *fileConfig)
{
	FILE *fpOpt = NULL;
	char line[1024] = {0};
	char option[8][64];
	char compileOpt[256]= {0};
	int n, found = 0;
	int i;
	char *ptmp = NULL;
	char tmpopt[34] = {0};
	int index = 0;
	int mode = 0;
	int ret = 0;
	int lineNum = 0;

	fpOpt = fopen(fileOpt, "r");

	if(fpOpt == NULL)
	{
		printf("error opening file:%s for reading\n", fileOpt);
		exit(1);
	}
	while(fgets (line, sizeof(line) -1, fpOpt) )
	{
		memset(option, 0, sizeof(option) );
		memset(compileOpt, 0, sizeof(compileOpt));
		n = sscanf(line, "%s : %s %s %s %s %s %s %s %s", compileOpt,  
			 option[0],option[1],option[2],option[3],option[4],option[5],option[6],option[7]);
#ifdef DEBUG
		printf("\ncompileOpt = %s\n", compileOpt);
		printf("option[0] = %s\n",option[0]);
		printf("option[1] = %s\n",option[1]);
		printf("option[2] = %s\n",option[2]);
		printf("option[3] = %s\n",option[3]);
		printf("option[4] = %s\n",option[4]);
		printf("option[5] = %s\n",option[5]);
		printf("option[6] = %s\n",option[6]);
		printf("option[7] = %s\n",option[7]);
#endif
		/* the definition without node name, skip this invalid line */
		if(n < 2)
		{
			printf("Invalid line:%d in config file:%s, n = %d !\n", lineNum, fileOpt, n);
			continue;
		}
#ifdef DEBUG
		printf("n = %d\n", n);
#endif
		/*find the node name in the line */
		for(i = 0; i < n - 1; i++)
		{
			if(!strcmp(nodeName, option[i]) )
			{
				found = 1;
				break;
			}
#ifdef DEBUG
			printf("option[%d] = %s\n", i, option[i]);
#endif
		}

		/* in case of node is found, parse the compileoption*/
		if(found ==1)
		{
			/* when || is found in the compile option string, parse each of them*/
#ifdef DEBUG
			printf("compileOpt = %s\n", &compileOpt[index]);
#endif
			while( (ptmp = strstr(&compileOpt[index], "||")) != NULL)
			{
#ifdef DEBUG
				printf("ptmp = %s\n", ptmp);
#endif
				mode = 1;
				//ret = 0;
			
				memset(tmpopt, 0, sizeof(tmpopt));	
				strncpy(tmpopt, &compileOpt[index], ptmp - compileOpt - index);
				index = ptmp - compileOpt + 2;
#ifdef DEBUG
				 printf("ptmp = %s, tmpopt = %s\n", ptmp, tmpopt);
#endif
				//ret = ret || (!!getOptionFromConfigFile( tmpopt, fileConfig)) ;	
				 ret =  !!getOptionFromConfigFile( tmpopt, fileConfig);
				if(ret)
					goto out;
			}

			/* when && is found in the compile option string, parse each of them*/
			if(mode == 0)
			{
				while( (ptmp = strstr(&compileOpt[index], "&&")) != NULL)
				{
#ifdef DEBUG
					printf("ptmp = %s\n", ptmp);	
#endif
					mode = 2;
					//ret = 1;
					memset(tmpopt, 0, sizeof(tmpopt));	
					strncpy(tmpopt, &compileOpt[index], ptmp - compileOpt - index);
					index = ptmp - compileOpt + 2;

#ifdef DEBUG
					printf("ptmp = %s, tmpopt = %s\n", ptmp, tmpopt);
#endif
					//ret = ret && (!!getOptionFromConfigFile( tmpopt, fileConfig)) ;	
					ret = !!getOptionFromConfigFile( tmpopt, fileConfig) ;
					if(!ret)
						goto out;
				}
			}

			/*parse the last compile option, according to defferent mode*/
			switch (mode)
			{
				case 0:
					ret = getOptionFromConfigFile(compileOpt, fileConfig);
					break;
				case 1:
					//ret = ret || (!!getOptionFromConfigFile(&compileOpt[index], fileConfig) );
					ret = !!getOptionFromConfigFile(&compileOpt[index], fileConfig) ;
#ifdef DEBUG
					printf("case 1: &compileOpt[index] = %s\n", &compileOpt[index]);
#endif
					break;
				case 2:
#ifdef DEBUG
					printf("case 2: &compileOpt[index] = %s\n", &compileOpt[index]);
#endif
					//ret = ret && (!!getOptionFromConfigFile(&compileOpt[index], fileConfig) );
					ret = !!getOptionFromConfigFile(&compileOpt[index], fileConfig) ;
			}

			fclose(fpOpt);
#ifdef DEBUG
			printf("111nodeName = %s, ret = %d, mode = %d!\n", nodeName, ret, mode);
#endif
			return ret;
		}

		memset(line, 0, sizeof(line));
		lineNum++;
	}

out:
	fclose(fpOpt);
#ifdef DEBUG
	printf("222nodeName = %s, ret = 1!\n", nodeName);
#endif
	return 1;
	
}
mxml_node_t *
merge_romfile_tree (mxml_node_t *tree_main, mxml_node_t *tree_custom,char *fileOpt, char *fileConfig)
{
	mxml_node_t *custom_node = NULL, *main_node = NULL;
	mxml_node_t *tmp_node = NULL;
	FILE *fp = NULL;

	if(tree_main == NULL && tree_custom == NULL)
		return NULL;
	else if(tree_main == NULL  && tree_custom != NULL)
		return tree_custom;
	else if(tree_main != NULL  && tree_custom == NULL)
		return tree_main;
	custom_node = mxmlWalkNext(tree_custom, tree_custom, MXML_DESCEND);
#ifdef DEBUG
	if(custom_node->type == MXML_ELEMENT)
		printf("customer_node->value.element.name:%d\n", custom_node->value);
#endif
	while(custom_node)
	{
		if(custom_node->type == MXML_ELEMENT)
		{
#ifdef DEBUG
			printf("customer_node->value.element.name:%s\n", custom_node->value.element.name);
#endif
		}
		else
			goto this;

#ifdef DEBUG
		printf("name:%s\n", custom_node->value.element.name);
#endif
		main_node = mxmlFindElement(tree_main, tree_main, custom_node->value.element.name, NULL, NULL, MXML_DESCEND);
		if(main_node)
		{
#ifdef DEBUG
			printf("main name is:%s\n", main_node->value.element.name);
#endif
			mxmlDelete(main_node);
		}
		tmp_node = custom_node;
		custom_node = mxmlWalkNext(tmp_node, tree_custom/*parantNode*/, MXML_NO_DESCEND);

		mxmlRemove(tmp_node);
		mxmlAdd(tree_main, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, tmp_node);
		continue;
this:
		custom_node = mxmlWalkNext(custom_node, tree_custom/*parantNode*/, MXML_NO_DESCEND);
	}
	main_node = mxmlWalkNext(tree_main, tree_main, MXML_DESCEND);
#ifdef DEBUG
        if(main_node->type == MXML_ELEMENT)
                printf("main_node->value.element.name:%d\n", main_node->value);
#endif
        while(main_node)
        {
                int optFound = 0;
                if(main_node->type == MXML_ELEMENT)
                {
#ifdef DEBUG
                        printf("main_node->value.element.name:%s\n", main_node->value.element.name);
#endif
                }
                else
                        goto that;

                optFound =  parseOption(main_node->value.element.name, fileOpt, fileConfig);
#ifdef DEBUG
		if(optFound)
                	printf("name:%s found!\n", main_node->value.element.name);
		else
			printf("name:%s not found!\n", main_node->value.element.name);
#endif
                if(optFound == 0)
                {
                	tmp_node = main_node;
                	main_node = mxmlWalkNext(tmp_node, tree_main/*parantNode*/, MXML_NO_DESCEND);
			mxmlDelete(tmp_node);
			continue;
		}

that:
                main_node = mxmlWalkNext(main_node, tree_main/*parantNode*/, MXML_NO_DESCEND);
        }
	return tree_main;
}

int 
main(int argc, char* argv[])
{
	mxml_node_t *tree_main = NULL, *tree_custom = NULL;
	mxml_node_t *tree_save = NULL;
	FILE *fp= NULL;
	int retval = 0;

	tree_main = load_romfile(argv[1]);
	tree_custom = load_romfile(argv[2]);

	tree_save = merge_romfile_tree( tree_main, tree_custom, argv[4], argv[5] );

	fp = fopen(argv[3], "w");
	if(fp == NULL || tree_save == NULL){
		if(fp)
			fclose(fp);
		return FAIL;
	}
	else{
		retval = mxmlSaveFile(tree_save, fp,  whitespace_cb);
		fclose(fp);
	}

	return retval;
		
}
