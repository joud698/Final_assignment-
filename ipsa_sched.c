#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "projdefs.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH                   ( 2 )

/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK           ( 100UL )
#define mainVALUE_SENT_FROM_TIMER          ( 200UL )


/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void * pvParameters );
static void prvQueueSendTask( void * pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );


/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/

/* Priorities for the tasks */
#define PERIODIC_TASK1_PRIORITY 1
#define PERIODIC_TASK2_PRIORITY 2
#define PERIODIC_TASK3_PRIORITY 3
#define PERIODIC_TASK4_PRIORITY 4


/* Task periods in milliseconds */
#define PERIODIC_TASK1_PERIOD pdMS_TO_TICKS(5)
#define PERIODIC_TASK2_PERIOD pdMS_TO_TICKS(20)
#define PERIODIC_TASK3_PERIOD pdMS_TO_TICKS(50)
#define PERIODIC_TASK4_PERIOD pdMS_TO_TICKS(50)



/* Task functions */
void Task1();
void Task2();
void Task3();
void Task4();
void vAperiodicTask();

/*-----------------------------------------------------------*/

void ipsa_sched( void )
{
    
    xTaskCreate(Task1, "PeriodicTask1", configMINIMAL_STACK_SIZE, NULL, PERIODIC_TASK1_PRIORITY, NULL);
    xTaskCreate(Task2, "PeriodicTask2", configMINIMAL_STACK_SIZE, NULL, PERIODIC_TASK2_PRIORITY, NULL);
    xTaskCreate(Task3, "PeriodicTask3", configMINIMAL_STACK_SIZE, NULL, PERIODIC_TASK3_PRIORITY, NULL);
    xTaskCreate(Task4, "PeriodicTask4", configMINIMAL_STACK_SIZE, NULL, PERIODIC_TASK4_PRIORITY, NULL);


    vTaskStartScheduler();
    
    /* The following line should never be reached */
    for(;;);
    
}
/*-----------------------------------------------------------*/


void Task1()
{   
    TickType_t xLastWakeTime = xTaskGetTickCount();


    const TickType_t xDelay = PERIODIC_TASK1_PERIOD;

    for( ; ; )
    {
        ///////////////////////////////////////////
        printf("Working \n");
        ///////////////////////////////////////////
        printf("Période de la tâche 1 : %ld ms\n\n", xDelay);
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
    
    
}

void Task2()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    const TickType_t xDelay = PERIODIC_TASK2_PERIOD;

    for (;;) 
    {
        ///////////////////////////////////////////
        float tempF = 10.0;
        float tempC = (tempF-32)/1.8;
        printf("Temp. en Celsius : %f\n",tempC);
        ///////////////////////////////////////////
        
        printf("Période de la tâche 2 : %ld ms\n\n", xDelay);
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}

void Task3()
{   
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = PERIODIC_TASK3_PERIOD;
    
    for (;;) 
    {
        
        ///////////////////////////////////////////
        long int bigNum1 = 123456789;
        long int bigNum2 = 987654321;
        long int result = bigNum1 * bigNum2;
        printf("Resultat: %ld\n", result);
        ///////////////////////////////////////////
        
        printf("Période de la tâche 3 : %ld ms\n\n", xDelay);
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}

void Task4()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = PERIODIC_TASK4_PERIOD;

    
    int arr[50] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                   41, 42, 43, 44, 45, 46, 47, 48, 49, 50};

    int target = 25; // Élément à rechercher

    int low = 0;
    int high = sizeof(arr) / sizeof(arr[0]) - 1;
    int found = 0; 

    for (;;) 
    {
        ///////////////////////////////////////////
        while (low <= high) 
        {
            int mid = low + (high - low) / 2;

       
            if (arr[mid] == target) 
            {
                found = 1;
                break;
            }
       
            else if (arr[mid] < target)
                low = mid + 1;
            
            else
                high = mid - 1;
        }

        
        if (found)
            printf("Élément %d trouvé dans la liste\n", target);
        else
            printf("Élément %d non trouvé dans la liste\n", target);

    
        low = 0;
        high = sizeof(arr) / sizeof(arr[0]) - 1;
        found = 0;
        ///////////////////////////////////////////

        printf("Période de la tâche 4 : %ld ms\n\n", xDelay);
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}
