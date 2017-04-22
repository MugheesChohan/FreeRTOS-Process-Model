#include "FreeRTOS.h"
#include "task.h"
#include "processes.h"

static StackType_t stack[100];

static TaskHandle_t taskhandle;
static StaticTask_t tasktcb;

extern void serial_write (char* pString);
extern uint32_t uxGetProcessId (void);

FREERTOS_PROCESS

void vTask(void * pvParameters)
{
    while(1)
    {
        uint32_t pid = uxGetProcessId() + 48;
        serial_write("This is a task of Process ");
        serial_write(&pid);
        serial_write("\n\r");
        vTaskDelay(100);
    }
}

int main()
{
   serial_write("Creating Tasks \n\r");

   taskhandle = xTaskCreateStatic(vTask, "task", 100, 0, 0, stack, &tasktcb);

   return 0;
}
