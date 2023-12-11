// #include <mbed.h>
// #define wait_ms(x) ThisThread::sleep_for(x)

// BufferedSerial pc(USBTX,USBRX,230400);
// EventQueue queue(16 * EVENTS_EVENT_SIZE);
// Thread thread;
// DigitalOut LED(D13);
// char buffer[8];
// bool state;

// void check2(){
//         pc.read(&buffer,sizeof(8));
// }
// void IRQ(){
//         LED = !LED;
//         //   state = true;
//         queue.call(check2);
// }

// void check(){

//         if(state == true) {
//                 pc.read(&buffer,sizeof(8));
//                 state = false;
//         }
// }

// int main() {
//         LED = 0;
//         printf("Start!!");
//         pc.sigio(&IRQ);
//         pc.set_blocking(false);
//         pc.sync();

//         thread.start(callback(&queue,&EventQueue::dispatch_forever));
//       //   queue.dispatch_forever();
//         while(1) {
//                 printf("hello world : %s\r\n",buffer);
//                 wait_ms(100);
//         }
// }



/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"

// Creates an event bound to the specified event queue
EventQueue queue;
void handler(int count);
Event<void(int)> event1(&queue, handler);
Event<void(int)> event2(&queue, handler);

#define wait_ms(x) ThisThread::sleep_for(x)

BufferedSerial pc(USBTX,USBRX,230400);

void handler(int count)
{
        unsigned time_ms = equeue_tick();
        printf("Timestamp = %d Event = %d \n", time_ms, count);
        return;
}

void post_events(void)
{
        // Events can be posted multiple times and enqueue gracefully until
        // the dispatch function is called. Each event will be processed
        // subject to any delay and period specified. Each time an event has
        // been dispatched it will be re-queued ready for the next dispatch
        // period.
        event1.post(1);  // Event1 with a value of 1
        event1.post(2);  // Event1 with a value of 2
        event1.post(3);  // Event1 with a value of 3

        // Cancel the last event posted ie Event1 with a value of 3
        event1.cancel();

        event1.post(4);  // Event1 with a value of 4

        event2.post(5);  // Event2 with a value of 5

}


// Example  demonstrates the following:
// 1. Post 5 different events to a queue
// 2. Configure the event delay and period for each
// 3. Invoke the dispatcher to dispatch events for a specified period
//
//    | 100ms |    200ms    | 100ms | 100ms |    200ms     |
//            ^             ^       ^       ^              ^
//       Events 1,2,4       ^       ^       ^              ^
//        dispatched        ^       ^       ^              ^
//                          ^       ^       ^              ^
//                   Events 1,2,4   ^       ^              ^
//                    dispatched    ^       ^              ^
//                                  ^       ^              ^
//                               Event 5    ^              ^
//                                          ^              ^
//                                      Event 1,2,4        ^
//                                       dispatched        ^
//                                                         ^
//                                                       Events 1,2,4
//                                                        dispatched
//
// Expected Output
//
// Timestamp = 100 Event = 1
// Timestamp = 127 Event = 2
// Timestamp = 156 Event = 4
// Timestamp = 300 Event = 1
// Timestamp = 327 Event = 2
// Timestamp = 356 Event = 4
// Timestamp = 400 Event = 5
// Timestamp = 500 Event = 1
// Timestamp = 527 Event = 2
// Timestamp = 556 Event = 4
// Timestamp = 700 Event = 1
// Timestamp = 727 Event = 2
// Timestamp = 756 Event = 4

int main()
{
        // Example 1 Dispatch posted events for a specified period
        Thread event_thread;

        // The event can be manually configured for special timing requirements
        // specified in milliseconds (using Chrono durations)
        event1.delay(100ms);   // Starting delay - 100 msec
        event1.period(200ms);  // Delay between each event - 200msec

        event2.delay(400ms);       // Starting delay - 400 msec
        event2.period(non_periodic); // Single non periodic event

        event_thread.start(callback(post_events));

        // Posted events are dispatched in the context of the queue's
        // dispatch function. Note that the EventQueue library has yet to be
        // converted to using Chrono times and thus times are still specified
        // in integer millisecond units.
        // 800 ms will allow the posted events to be dispatched multiple times
        queue.dispatch_for(800ms);

        event_thread.join();
}