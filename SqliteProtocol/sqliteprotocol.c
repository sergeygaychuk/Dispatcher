#include <stdlib.h>
#include <string.h>

typedef struct sp_disposable sp_disposable;
struct sp_disposable {
    void *m_This;
    void (*dispose)(sp_disposable*);
};

void init_sp_disposable(sp_disposable *aObj, void *aThis, void (*aDispose)(sp_disposable*)) {
  aObj->dispose = aDispose;
  aObj->m_This = aThis;
}

void* sp_zalloc(size_t szCount) {
    void *buffer = malloc(szCount);
    if (buffer) {
        memset(buffer, 0, szCount);
    }
    return buffer;
}

void sp_free(void *aBuffer) {
    free(aBuffer);
}

#define NEW(struct_name) (struct_name##_constructor())
#define NEW_ARR(struct_name, length) (struct_name##_constructor(length))
#define DELETE(obj) (obj->m_Disposer.dispose(((sp_disposable*)&obj->m_Disposer)), sp_free(obj))
#define DELETE_ARR(obj, length) \
{ \
  int i = 0; \
  for(i = 0; i < length; ++ i) { \
    obj[i].m_Disposer.dispose((sp_disposable*)&obj[i].m_Disposer); \
  } \
  sp_free(obj); \
}

#define SP_DEVICE_TYPE_UNKNOWN 0xFF
#define SP_DEVICE_TYPE_TEMPERATURE_SENSOR 0x01
#define SP_DEVICE_TYPE_RELAY 0x02

typedef struct sp_device sp_device;
struct sp_device {
    char *m_Address;
    int m_Type;
};

typedef struct sp_device_i sp_device_i;
struct sp_device_i {
    sp_device m_Device;
    sp_disposable m_Disposer;
    void (*set_device)(sp_device_i *aThis, const char *aAddress, int aType);
};

void sp_device_i_destructor(sp_disposable *aThis) {
  sp_device_i *th = (sp_device_i*)aThis->m_This;
  if (th->m_Device.m_Address) {
    sp_free(th->m_Device.m_Address);
  }
}

void sp_device_i_set_device(sp_device_i *aThis, const char *aAddress, int aType) {
  if (aThis && !aThis->m_Device.m_Address) {
    aThis->m_Device.m_Address = (char *)sp_zalloc(sizeof(char) * (strlen(aAddress) + 1));
    strcpy(aThis->m_Device.m_Address, aAddress);
    aThis->m_Device.m_Type = aType;
  }
}

sp_device_i* sp_device_i_constructor(int aLength) {
    int i = 0;
    sp_device_i *obj = (sp_device_i *)sp_zalloc(sizeof(sp_device_i) * aLength);
    if (obj) {
        for (i = 0; i < aLength; ++ i) {
          init_sp_disposable(&obj[i].m_Disposer, &obj[i], sp_device_i_destructor);
          obj[i].set_device = &sp_device_i_set_device;
          obj[i].m_Device.m_Address = 0;
          obj[i].m_Device.m_Type = SP_DEVICE_TYPE_UNKNOWN;
        }
    }
    return obj;
}

typedef struct sp_device_list sp_device_list;
struct sp_device_list {
    sp_disposable m_Disposer;
    int m_Length;
    sp_device_i *m_Devices;
    void (*init_device_list)(sp_device_list *aThis, int aLength);
    void (*set_device)(sp_device_list *aThis, int aIdx, const char *aAddress, int aType);
};

void sp_device_list_destructor(sp_disposable* aThis) {
  sp_device_list *th = (sp_device_list *)aThis->m_This;
  if (th->m_Devices) {
    DELETE_ARR(th->m_Devices, th->m_Length);
    th->m_Length = 0;
    th->m_Devices = 0;
  }
}

void sp_device_list_init_device_list(sp_device_list *aThis, int aLength) {
  if (!aThis->m_Devices) {
    aThis->m_Length = aLength;
    aThis->m_Devices = NEW_ARR(sp_device_i, aLength);
  }
}

void sp_device_list_set_device(sp_device_list *aThis, int aIdx, const char *aAddress, int aType) {
  if (aThis->m_Devices && aThis->m_Length > aIdx) {
    aThis->m_Devices[aIdx].set_device(&aThis->m_Devices[aIdx], aAddress, aType);
  }
}

sp_device_list *sp_device_list_constructor() {
    sp_device_list *obj = (sp_device_list *)sp_zalloc(sizeof(sp_device_list));
    if (obj) {
        init_sp_disposable(&obj->m_Disposer, obj, sp_device_list_destructor);
        obj->init_device_list = &sp_device_list_init_device_list;
        obj->set_device = &sp_device_list_set_device;
        obj->m_Devices = 0;
        obj->m_Length = 0;
    }
    return obj;
}

//Return count of devices
//Return pointer to device list
//For free device list call sp_close_device_list
int sp_get_device_list(sp_device_list **aList) {
    if (!aList) return -1;
    *aList = NEW(sp_device_list);
    if (!*aList) return -1;

    //TEST FILL
    (*aList)->init_device_list(*aList, 2);
    (*aList)->set_device(*aList, 0, "AAFF1234", SP_DEVICE_TYPE_TEMPERATURE_SENSOR);
    (*aList)->set_device(*aList, 1, "1234AAFF", SP_DEVICE_TYPE_RELAY);
    //TEST FILL
    return (*aList)->m_Length;
}

//return device on specific position
//return null if error
sp_device* sp_get_device_from_list(sp_device_list *aList, int aPosition) {
    if (!aList || aList->m_Length <= aPosition || aPosition < 0)
        return 0;
    return &aList->m_Devices[aPosition].m_Device;
}

//close device list
void sp_close_device_list(sp_device_list *aList) {
    if (aList) {
        DELETE(aList);
    }
}
