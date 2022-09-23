#include "predef.h"
#include <stdlib.h>
#define ElemType int

typedef ElemType *Triplet;

Status InitTriplet(Triplet &T, ElemType v1, ElemType v2, ElemType v3);

Status DestroyTriplet(Triplet &T);

Status Get(Triplet T, int i, ElemType &e);

Status Put(Triplet &T, int i, ElemType e);

Status IsAscending(Triplet T);

Status IsDescending(Triplet T);

Status Max(Triplet T, ElemType &e);

Status Min(Triplet T, ElemType &e);

Status Add(Triplet &T, Triplet T1, Triplet T2);

Status Minus(Triplet &T, Triplet T1, Triplet T2);

Status InitTriplet(Triplet &T, ElemType v1, ElemType v2, ElemType v3)
{
    T = (ElemType *)malloc(3 * sizeof(ElemType));
    if (!T)
        exit(OVERFLOW);
    T[0] = v1;
    T[1] = v2;
    T[2] = v3;
    return OK;
}

Status DestroyTriplet(Triplet &T)
{
    free(T);
    T = NULL;
    return OK;
}

Status Get(Triplet T, int i, ElemType &e)
{
    if (i < 1 || i > 3)
        return ERROR;
    e = T[i - 1];
    return OK;
}

Status Put(Triplet &T, int i, ElemType e)
{
    if (i < 1 || i > 3)
        return ERROR;
    T[i - 1] = e;
    return OK;
}

Status IsAscending(Triplet T)
{
    return (T[0] <= T[1]) && (T[1] <= T[2]);
}

Status IsDescending(Triplet T)
{
    return (T[0] >= T[1]) && (T[1] >= T[2]);
}

Status Max(Triplet T, ElemType &e)
{
    e = (T[0] >= T[1]) ? ((T[0] >= T[2]) ? T[0] : T[2]) : ((T[1] >= T[2]) ? T[1] : T[2]);
    return OK;
}

Status Min(Triplet T, ElemType &e)
{
    e = (T[0] <= T[1]) ? ((T[0] <= T[2]) ? T[0] : T[2]) : ((T[1] <= T[2]) ? T[1] : T[2]);
    return OK;
}

Status Add(Triplet &T, Triplet T1, Triplet T2)
{
    T[0] = T1[0] + T2[0];
    T[1] = T1[1] + T2[1];
    T[2] = T1[2] + T2[2];
    return OK;
}

Status Minus(Triplet &T, Triplet T1, Triplet T2)
{
    T[0] = T1[0] - T2[0];
    T[1] = T1[1] - T2[1];
    T[2] = T1[2] - T2[2];
    return OK;
}