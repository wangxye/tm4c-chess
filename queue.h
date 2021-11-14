/*
 * queue.h
 *
 *  Created on: 2021年1月3日
 *      Author: wxy
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_NUM 100

typedef qizi QElemType;

typedef struct QNode //节点结构
{
	QElemType data;
	int num;
} QNode, *QueuePtr;

int length = 0;

int len = 0;

struct QNode qn[MAX_NUM];

struct QNode Undoqn[MAX_NUM];

void Analyse() {
	QElemType value;
	int count, i;
	for (i = 0; i < length; i++) {
		move(qn[i].data, qn[i].num);
	}
	restartqueue();
}

void restartqueue(void) {
	length = 0;
	len = 0;
}

void requeue(void) {
	if (len > 0) {
		len--;
		move(Undoqn[len].data, Undoqn[len].num);
		b = !b;
	}
}

void Enqueue(QElemType *e, int count) {
	(qn[length]).data = *e;
	(qn[length]).num = count;
	length++;
}

void Undoqueue(QElemType *e, int count) {
	(Undoqn[len]).data = *e;
	(Undoqn[len]).num = count;
	len++;
}

#endif /* QUEUE_H_ */
