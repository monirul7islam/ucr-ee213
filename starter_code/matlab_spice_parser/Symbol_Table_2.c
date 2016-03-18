/**
	Symbol_Table.c
	For the undergraduate EDA course offered at 
	Dept of Micro/Nano-electronics

	(c) Guoyong Shi, 
	All rights reserved, 2006-2015
	Shanghai Jiao Tong University

	If you modify the code, please keep the above copyright info.
*/

#ifdef MATLAB
#include "mex.h"
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "Symbol_Table.h"

// Electric type of a node variable
#define SP_NOTYPE 	0
#define SP_VOLTAGE 	1
#define SP_CURRENT 	2

#define LINE_INFO   "\n[" << __FILE__ << " : line " << __LINE__ << " : " << __FUNCTION__ << "()] - "
#define PRINT_LINE_INFO 	printf("\n[%s : line %d : %s()]\n", __FILE__, __LINE__, __FUNCTION__);


Node_Entry **NodeTable;
int NodeTableSize;

Device_Entry **DeviceTable;
int DeviceTableSize;


void Init_Symbol_Tables()
{
    NodeTableSize = 0;
    DeviceTableSize = 0;
    
    DeviceTable = (Device_Entry **) malloc(sizeof(Device_Entry*));
    NodeTable = (Node_Entry **) malloc(sizeof(Node_Entry*));
}


void Destroy_Symbol_Table()
{
	Delete_Node_Table();
	Delete_Device_Table();
}


void Delete_Node_Table()
{
    Node_Entry* cur = *NodeTable;
    while(cur != NULL){
        Node_Entry* next = cur->next;
        free(cur);
        cur = next;
    }
}

void Delete_Device_Table()
{
    Device_Entry* cur = *DeviceTable;
    while(cur != NULL){
        Device_Entry* next = cur->next;
        // Free Nodes list for device
        Node_Entry* n = *(cur->nodelist);
        while(n != NULL){
            Node_Entry* nx = n->next;
            free(n);
            n = nx;
        }
        free(cur->nodelist);
        free(cur);
        cur = next;
    }
}

void Delete_Node_Entry(const char *name)
{
}

void Delete_Device_Entry(const char *name)
{
	// NOT IMPLEMENTED!
}

Node_Entry* Lookup_Node_Entry(const char *name)
{
    if(*name=='0') return NULL; // Should not look for ground node

    Node_Entry* cur = *NodeTable;
    
    while(cur != NULL){
        if( strcmp(cur->name,name) == 0){ // Found match
            return cur;
        }
        cur = cur->next; 
    }
    
    return  NULL; // No match
}


Device_Entry* Lookup_Device_Entry(const char *name)
{
	return NULL;
}


Node_Entry* Insert_Node_Entry(const char *name)
{
    Node_Entry* node = Lookup_Node_Entry(name);
    if(node != NULL) return NULL; //Already exists in table
    
    // Make new entry
    Node_Entry* nodeEntry = malloc(sizeof(Node_Entry));
    nodeEntry->index = NodeTableSize;
    nodeEntry->name = name;
    nodeEntry->next = *NodeTable;
    
    *NodeTable = nodeEntry;
    ++NodeTableSize;
    
    return nodeEntry;
}


Device_Entry* Insert_Device_Entry(const char *name,  const int numnodes, 
				Node_Entry **nodelist, const double value)
{
    Device_Entry* dev = malloc(sizeof(Device_Entry));
    dev->name = name;
    dev->numnodes = numnodes;
    dev->nodelist = nodelist;
    dev->value = value;
    dev->next = *DeviceTable;
    
    *DeviceTable = dev;
    ++DeviceTableSize;
    
    
    // Update index of the nodes
    Node_Entry* node = *nodelist;
    while(node != NULL){
        if(*(node->name) == '0'){ // Set index for ground node as -1
            node->index = -1;
        }else{
            Node_Entry* found = Lookup_Node_Entry(node->name);
            if(found == NULL){ //Node appears the first time, insert new node
                Node_Entry* newNode = Insert_Node_Entry(node->name);
                node->index = newNode->index;
            }else{ //Node exists, copy index
                node->index = found->index;
            }
        }
        
        node = node->next;
    }
    
    return dev;
}


void Print_Node_Table2file()
{
    FILE *file=fopen("nodeIndex.csv","w");
    //printf("\nNumber of Nodes: %d", NodeTableSize);
    if(NodeTableSize >0) {
        //printf("\nList of Nodes:\n");
        
        Node_Entry* cur = *NodeTable;
        while (cur != NULL) {
            fprintf(file,"%s,%d\n",cur->name,cur->index);
            cur = cur->next;
        }
        //printf("\n");
    }
    fclose(file);
}

void Print_Node_Table()
{
    printf("\nNumber of Nodes: %d", NodeTableSize);
    if(NodeTableSize >0) {
        printf("\nList of Nodes:");
        
        Node_Entry* cur = *NodeTable;
        while (cur != NULL) {
            printf("  %s(%d)",cur->name,cur->index);
            cur = cur->next;
        }
        printf("\n");
    }
}


void Print_Device_Table()
{
    printf("\nNumber of Devices: %d", DeviceTableSize);
    if(DeviceTableSize >0) {
        printf("\nList of Devices:");
        
        Device_Entry* cur = *DeviceTable;
        while (cur != NULL) {
            printf("  %s",cur->name);
            cur = cur->next;
        }
        printf("\n");
    }
}


int NameHash(const char *name,  int tsize)
{
	const char *s; 
	register int i = 0; 

	// In case no name
	if (!name)	return (0);
	
	for (s = name; *s; s++) {
		i += (unsigned) *s; 
	}
	
	return (i % tsize);
}
