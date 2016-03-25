/*
UVa Online Judge
732 - Anagrams by Stack

Author: Ponson Sun
Email: ponsonsun@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define  MAX_STRLEN  1024
#define  BUF_SIZE  1025

typedef struct  {
    char* rstStr;
    void *next;
}sResultStrList;

sResultStrList grstList,  *gpRstList, *gptmpRstList;

void  StackProcess(int paramInputRemainLen, char* paramOutputStr, int paramOutputStrLen, char* paramStackBuf,  int paramStackDepth, char* paramIoListStr, int paramIoListStrLen);
void ClearResultList();
void PrintResultList() ;
void PrintOneLine(char* str) ;

char gInputStr[BUF_SIZE], ganswerStr[BUF_SIZE];
int gInputStrLen, gansStrLen;

int main(int argc, char* argv[])
{
        bool endOfFile = false;
        bool isFirstBlockProcess = true;

        int sr;

        while (endOfFile == false)
        {
            /* Read First line for input string. */
            sr = scanf("%s", gInputStr) ;
            if (sr != EOF)
            {
                 gInputStrLen = strlen(gInputStr);
                 if (gInputStr[gInputStrLen-1] == '\n') {
                     gInputStr[gInputStrLen-1] = '\0';
                     gInputStrLen--;
                 }
                 /* printf("%s\n", gInputStr); */
            }
            else {
                endOfFile = true;
                break;
            }

            /* Read Second line for output string. */
            if (scanf("%s", ganswerStr) != EOF)
            {
                gansStrLen = strlen(ganswerStr);
                 if (ganswerStr[gansStrLen-1] == '\n') {
                     ganswerStr[gansStrLen-1] = '\0';
                     gansStrLen--;
                 }
                /* printf("%s\n", ganswerStr); */
            }
            else {
                endOfFile = true;
                break;
            }
/*
            if (isFirstBlockProcess == false)
                printf("]\n");
            else
                isFirstBlockProcess = false;
*/
            printf("[\n");
            StackProcess(gInputStrLen, NULL, 0, NULL, 0, NULL, 0);
            PrintResultList();
            ClearResultList();
            printf("]\n");
        }
        /* The last one block. Not need to add LF */
    return 0;
}

void  StackProcess(int paramInputRemainLen, char* paramOutputStr, int paramOutputStrLen, char* paramStackBuf,  int paramStackDepth, char* paramIoListStr, int paramIoListStrLen)
{
    char ioListStr[MAX_STRLEN*2+1];/* Result buffer */
    char stackBuf[BUF_SIZE];
    char outputStr[BUF_SIZE];
    int stackDepth, inputRemainLen, outputStrLen, ioListStrLen, inputIdx, ansIdx;

    inputRemainLen = paramInputRemainLen;
    stackDepth = paramStackDepth;
    outputStrLen = paramOutputStrLen;
    ioListStrLen = paramIoListStrLen;
    if (paramStackBuf)
        memcpy(stackBuf, paramStackBuf, stackDepth);
    stackBuf[stackDepth] = '\0';

    if (paramIoListStr)
        memcpy(ioListStr, paramIoListStr, ioListStrLen);
    ioListStr[ioListStrLen] = '\0';

    if (paramOutputStr)
        memcpy(outputStr, paramOutputStr, outputStrLen);
    outputStr[outputStrLen] = '\0';

     /* If inputRemain Len > 0, it means can to 'i' action. */
     if (inputRemainLen > 0) {
             inputIdx = gInputStrLen - inputRemainLen;
             stackBuf[stackDepth] = gInputStr[inputIdx]; /* Push */
             ioListStr[ioListStrLen] = 'i';
             inputRemainLen--;
             stackDepth++;
             ioListStrLen++;
             StackProcess(inputRemainLen, outputStr, outputStrLen, stackBuf,  stackDepth, ioListStr, ioListStrLen);
             /* Rollback 'i' action for testing the next action 'o' can do or not. */
             stackDepth--;
             stackBuf[stackDepth] = '\0';
             inputRemainLen++;
             ioListStrLen--;
             ioListStr[ioListStrLen] = '\0';
     }

     /* If stack is not empty, it means can do 'o' action. */
      if (stackDepth > 0) {
          ansIdx = ioListStrLen - (gInputStrLen - inputRemainLen);
          if (stackBuf[stackDepth - 1] == ganswerStr[ansIdx]) {
              stackDepth--;
              outputStr[outputStrLen] = stackBuf[stackDepth];
              outputStrLen++;
              stackBuf[stackDepth] = '\0'; /* POP */
              ioListStr[ioListStrLen] = 'o';
              ioListStrLen++;
              StackProcess(inputRemainLen, outputStr, outputStrLen, stackBuf,  stackDepth, ioListStr, ioListStrLen);
          }
      }
     /* If inputRemainLen == 0 and stack is empty, then check the result. */
     if (inputRemainLen == 0 && stackDepth == 0) {
         bool isFoundTheSame;

         /* gpRstList = &grstList; */
         gpRstList = &grstList;
         isFoundTheSame = false;

         /* The first one,  it doesn't need to malloc. */
         if (gpRstList == &grstList)  {
             if (gpRstList->rstStr == NULL)
                 isFoundTheSame = false;
             else if (strcmp(gpRstList->rstStr, ioListStr) == 0)
                 isFoundTheSame = true;
             else
                 isFoundTheSame = false;
         }

         while (gpRstList->next) {
             gptmpRstList = gpRstList->next;
             if (strcmp(gptmpRstList->rstStr, ioListStr) == 0) {
                 isFoundTheSame = true;
                 break;
             }
             else {
                gpRstList = gptmpRstList;
             }
         }

         if (isFoundTheSame == false)  {

             if (grstList.next == NULL && grstList.rstStr == NULL) {
                 grstList.rstStr = malloc(strlen(ioListStr) + 1);
                 strcpy(grstList.rstStr, ioListStr);
             }
             else {
                 gptmpRstList = malloc(sizeof(grstList));
                 gptmpRstList->rstStr = malloc(strlen(ioListStr) +1);
                 strcpy(gptmpRstList->rstStr, ioListStr);
                 gptmpRstList->next = NULL;
                 gpRstList->next = gptmpRstList;
             }
         }
  }
}

void ClearResultList()
{
    sResultStrList *pRstList, *pTmpRstList;

    pRstList = &grstList;

    if (pRstList->rstStr) {
        free(pRstList->rstStr);
        pRstList->rstStr = NULL;
    }
    pRstList = pRstList->next;
    while (pRstList) {
        if (pRstList->rstStr)
            free(pRstList->rstStr);
        pTmpRstList = pRstList->next;
        free(pRstList);
        pRstList = pTmpRstList;
    }
    grstList.next = NULL;
}

void PrintResultList()
{
    sResultStrList *pRstList, *pTmpRstList;

    pRstList = &grstList;
     if (pRstList->rstStr) {
        PrintOneLine(pRstList->rstStr);
    }
    else
        return;

    while (pRstList->next) {
        pTmpRstList = pRstList->next;
        PrintOneLine(pTmpRstList->rstStr);
        pRstList = pTmpRstList;
    }
}

void PrintOneLine(char* str)
{
    int i, len;

    len = strlen(str);

    for (i=0; i < len; i++) {
        if (i == len - 1)
            printf("%c\n", str[i]);
        else
            printf("%c ", str[i]);
    }
}
