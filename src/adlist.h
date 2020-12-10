/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */
/* List 、scan和hscan 迭代器命令、*/
/**
 *  数据结构 : 双向链表，但不是环链
 *
 */
typedef struct listNode {
    /* 前指针 */
    struct listNode *prev;
    /* 后指针 */
    struct listNode *next;
    void *value;
} listNode;

/* 迭代器 */
typedef struct listIter {
    listNode *next;
    /* 用于区分是反向迭代还是正向迭代  参考 ： AL_START_HEAD 为从头节点开始*/
    int direction;
} listIter;

/**
 * list 数据结构 ->  客户端的list
 * 有头指针和尾指针
 * 1. 为什么需要list ?
 *     管理链表更加方便，并且保存链头尾节点，又因为是双向链表所以更利于查询
 *
 */
typedef struct list {
    listNode *head;
    listNode *tail;

    /**
     * 节点复制函数
     * @param ptr
     * @return
     */
    void *(*dup)(void *ptr);
    /**
     * 节点释放函数
     * @param ptr
     */
    void (*free)(void *ptr);
    /**
     * 节点对比函数
     * @param ptr
     * @param key
     * @return
     */
    int (*match)(void *ptr, void *key);
    /**
     * 当前链表的节点数
     */
    unsigned long len;
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
/* 创建一个空的list */
list *listCreate(void);
/* 将list空间释放 */
void listRelease(list *list);
/* 将链表的所有节点指向null */
void listEmpty(list *list);
/* 将指定值添加到链表头部 */
list *listAddNodeHead(list *list, void *value);
/* 将指定值添加到链表尾部 */
list *listAddNodeTail(list *list, void *value);
/* 指定位置插入一个节点 */
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
/* 删除指定节点 */
void listDelNode(list *list, listNode *node);
/*将list 转化为迭代器 */
listIter *listGetIterator(list *list, int direction);
/* 获取迭代器的下一个元素 */
listNode *listNext(listIter *iter);
/* 释放链表 */
void listReleaseIterator(listIter *iter);
/* 复制一份list的副本 */
list *listDup(list *orig);
/* list中搜索指定值 */
listNode *listSearchKey(list *list, void *key);
/* 获取指定下标的值 0开始 */
listNode *listIndex(list *list, long index);
/* 指定list 为迭代器的下一个指针 */
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
/* 将尾节点移到头部 */
void listRotateTailToHead(list *list);
void listRotateHeadToTail(list *list);
/* 拼接两个list */
void listJoin(list *l, list *o);

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
