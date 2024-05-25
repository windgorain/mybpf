/******************************************************************************
* Copyright (C) LiXingang
* Author:      LiXingang  Version: 1.0  Date: 2012-10-26
* Description: 
* History:     
******************************************************************************/

#ifndef __GETOPT2_UTL_H_
#define __GETOPT2_UTL_H_

#ifdef __cplusplus
    extern "C" {
#endif 

#define GETOPT2_FLAG_HIDE  0x1     
#define GETOPT2_OUT_FLAG_ISSET 0x10000 

typedef struct {
    UINT min;
    UINT max;
}GETOPT2_NUM_RANGE_S;


enum {
    
    GETOPT2_V_NONE = 0,

    
    GETOPT2_V_U32 = 'u',    
    GETOPT2_V_STRING = 's', 
    GETOPT2_V_BOOL = 'b',   
    GETOPT2_V_IP = '4',     
    GETOPT2_V_IP6 = '6',    
    GETOPT2_V_RANGE = 'r',  
    GETOPT2_V_MAC = 'm',    

    
    GETOPT2_V_IP_PREFIX = 256,    
    GETOPT2_V_IP_PORT,            
    GETOPT2_V_IP_PROTOCOL,        
};

typedef struct
{
    char opt_type;    
    char opt_short_name; 
    char *opt_long_name; 
    int value_type;   
    void *value;
    char *help_info;
    unsigned int flag;
}GETOPT2_NODE_S;

int GETOPT2_ParseFromArgv0(UINT uiArgc, CHAR **ppcArgv, INOUT GETOPT2_NODE_S *opts);
int GETOPT2_Parse (UINT uiArgc, CHAR **ppcArgv, GETOPT2_NODE_S *pstNodes);
char * GETOPT2_BuildHelpinfo(GETOPT2_NODE_S *nodes, OUT char *buf, int buf_size);

int GETOPT2_IsOptSetted(GETOPT2_NODE_S *nodes, char short_opt_name, char *long_opt_name);
GETOPT2_NODE_S * GETOPT2_IsMustErr(GETOPT2_NODE_S *opts);
void GETOPT2_PrintHelp(GETOPT2_NODE_S *opts);

#ifdef __cplusplus
    }
#endif 

#endif 


