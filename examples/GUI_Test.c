#include "GUI_Test.h"
#include "GUI.h"
#include "GUIDemo.h"


static void test_thread(void *param)
{
    CRC_HandleTypeDef   CrcHandle;
    CrcHandle.Instance = CRC; 
    HAL_CRC_Init(&CrcHandle);
    GUI_Init();

    
    GUIDEMO_Main();
}

static int gui_test_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("gui_test",
                           test_thread, RT_NULL,
                           2048,
                           RT_THREAD_PRIORITY_MAX - 2, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return RT_EOK;
}
INIT_APP_EXPORT(gui_test_init);
