/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - convert to FreeRTOS
 *******************************************************************************/

// #include "ql_uart.h"
// #include "ql_stdlib.h"
#include "MQTTRTOS.h"

int ThreadStart(Thread* thread, void (*fn)(void*), void* arg)
{
	int rc = 0;
    // TODO: TASK

	return rc;
}


void MutexInit(Mutex* mutex)
{
	mutex->sem = Ql_OS_CreateMutex();
}

int MutexLock(Mutex* mutex)
{
	return Ql_OS_TakeMutex(mutex->sem, 5000);
}

int MutexUnlock(Mutex* mutex)
{
	return Ql_OS_GiveMutex(mutex->sem);
}


void TimerInit(Timer* timer)
{
	timer->xTicksToWait = 0;
    timer->xTimeOut = 0;
}

char TimerIsExpired(Timer* timer)
{
	uint32_t now = Ql_OS_GetTaskTickCount();
	return timer->xTicksToWait + timer->xTimeOut <= now;
    // char buf[100];
    // Ql_snprintf(buf, "[TimerIsExpired] xTicksToWait=%d, xTimeOut=%d\n", timer->xTicksToWait, timer->xTimeOut);
    // Ql_UART_Write((Enum_SerialPort)(UART_PORT0), (u8*)(buf), Ql_strlen((const char *)(buf)));
}


void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
	timer->xTicksToWait = Ql_OS_GetTaskTickCount();
    timer->xTimeOut = timeout / 10 + ((timeout % 10)?1:0);
}


void TimerCountdown(Timer* timer, unsigned int timeout)
{
	timer->xTicksToWait = Ql_OS_GetTaskTickCount();
    timer->xTimeOut = timeout * 100;
}


int TimerLeftMS(Timer* timer)
{
	uint32_t now = Ql_OS_GetTaskTickCount();
	return (timer->xTicksToWait + timer->xTimeOut <= now)? 0: (timer->xTicksToWait + timer->xTimeOut - now)*10;
}

extern int MqttSample_RecvPkt(void *arg, unsigned char* buffer, int len, int timeout_ms);
int RTOS_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    int recvLen = 0;
    recvLen = MqttSample_RecvPkt(n, buffer, len, timeout_ms);

	return recvLen;
}

extern int MqttSample_SendPkt(void *arg, unsigned char* buffer, int len, int timeout_ms);
int RTOS_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    MqttSample_SendPkt(n, buffer, len, timeout_ms);
	return len;
}


void RTOS_disconnect(Network* n)
{
    // none
}


void NetworkInit(Network* n)
{
	n->my_socket = 0;
	n->mqttread = RTOS_read;
	n->mqttwrite = RTOS_write;
	n->disconnect = RTOS_disconnect;
}


int NetworkConnect(Network* n, char* addr, int port)
{
    return 0;
}

