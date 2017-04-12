// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
// 

#include "common.h"
#include "gcenv.h"
#include "gchandletableimpl.h"
#include "objecthandle.h"

IGCHandleTable* CreateGCHandleTable()
{
    return new(nothrow) GCHandleTable();
}

bool GCHandleTable::Initialize()
{
    return Ref_Initialize();
}

void GCHandleTable::Shutdown()
{
    Ref_Shutdown();
}

void* GCHandleTable::GetGlobalHandleTable()
{
    return (void*)g_HandleTableMap.pBuckets[0];
}

void* GCHandleTable::GetNewHandleTable(void* context)
{
    return (void*)::Ref_CreateHandleTableBucket(ADIndex((uintptr_t)context));
}

void* GCHandleTable::GetHandleContext(OBJECTHANDLE handle)
{
    return (void*)((uintptr_t)::HndGetHandleTableADIndex(::HndGetHandleTable(handle)).m_dwIndex);
}

OBJECTHANDLE GCHandleTable::CreateHandleOfType(void* table, Object* object, int type)
{
    return ::HndCreateHandle((HHANDLETABLE)table, type, ObjectToOBJECTREF(object));
}

OBJECTHANDLE GCHandleTable::CreateGlobalHandleOfType(Object* object, int type)
{
    return ::HndCreateHandle(g_HandleTableMap.pBuckets[0]->pTable[GetCurrentThreadHomeHeapNumber()], type, ObjectToOBJECTREF(object)); 
}

OBJECTHANDLE GCHandleTable::CreateHandleWithExtraInfo(void* table, Object* object, int type, void* pExtraInfo)
{
    return ::HndCreateHandle((HHANDLETABLE)table, type, ObjectToOBJECTREF(object), reinterpret_cast<uintptr_t>(pExtraInfo));
}

OBJECTHANDLE GCHandleTable::CreateDependentHandle(void* table, Object* primary, Object* secondary)
{
    OBJECTHANDLE handle = ::HndCreateHandle((HHANDLETABLE)table, HNDTYPE_DEPENDENT, ObjectToOBJECTREF(primary));
    ::SetDependentHandleSecondary(handle, ObjectToOBJECTREF(secondary));

    return handle;
}

OBJECTHANDLE GCHandleTable::CreateDuplicateHandle(OBJECTHANDLE handle)
{
    return ::HndCreateHandle(HndGetHandleTable(handle), HNDTYPE_DEFAULT, ::HndFetchHandle(handle));
}

void GCHandleTable::DestroyHandleOfType(OBJECTHANDLE handle, int type)
{
    ::HndDestroyHandle(::HndGetHandleTable(handle), type, handle);
}

void GCHandleTable::DestroyHandleOfUnknownType(OBJECTHANDLE handle)
{
    ::HndDestroyHandleOfUnknownType(::HndGetHandleTable(handle), handle);
}

void* GCHandleTable::GetExtraInfoFromHandle(OBJECTHANDLE handle)
{
    return (void*)::HndGetHandleExtraInfo(handle);
}