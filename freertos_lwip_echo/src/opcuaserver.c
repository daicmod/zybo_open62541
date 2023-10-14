/*
 * opcuaserver.c
 *
 *  Created on: 2023/10/07
 *      Author: daicm
 */
#define OPEN62541_FEERTOS_USE_OWN_MEM
#include "open62541.h"

void opcua_thread(void *arg){

        //The default 64KB of memory for sending and receicing buffer caused problems to many users. With the code below, they are reduced to ~16KB
        UA_UInt32 sendBufferSize = 8192;       //64 KB was too much for my platform
        UA_UInt32 recvBufferSize = 8192;       //64 KB was too much for my platform
        UA_UInt16 portNumber = 4840;

        UA_Server* mUaServer = UA_Server_new();
        UA_ServerConfig *uaServerConfig = UA_Server_getConfig(mUaServer);
        UA_ServerConfig_setMinimalCustomBuffer(uaServerConfig, portNumber, 0, sendBufferSize, recvBufferSize);

        //VERY IMPORTANT: Set the hostname with your IP before starting the server
        UA_ServerConfig_setCustomHostname(uaServerConfig, UA_STRING("192.168.3.36"));

        //The rest is the same as the example

        UA_Boolean running = true;

        // add a variable node to the adresspace
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        UA_Int32 myInteger = 42;
        UA_Variant_setScalarCopy(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
        attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");
        attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "the.answer");
        UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "the answer");
        UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        UA_Server_addVariableNode(mUaServer, myIntegerNodeId, parentNodeId,
                                                              parentReferenceNodeId, myIntegerName,
                                                              UA_NODEID_NULL, attr, NULL, NULL);

        /* allocations on the heap need to be freed */
        UA_VariableAttributes_clear(&attr);
        UA_NodeId_clear(&myIntegerNodeId);
        UA_QualifiedName_clear(&myIntegerName);

        UA_StatusCode retval = UA_Server_run(mUaServer, &running);
        UA_Server_delete(mUaServer);
}

void vApplicationMallocFailedHook(){
        for(;;){
                vTaskDelay(pdMS_TO_TICKS(1000));
        }
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ){
        for(;;){
                vTaskDelay(pdMS_TO_TICKS(1000));
        }
}
