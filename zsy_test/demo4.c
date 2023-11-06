//===-- demo4.cpp ------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <stdio.h>

#define N 8

int t = 3;
int exec_var[9][N] = {0};
int control_var[9][N] = {0};
int cond[N+1] = {0};

static int EXEC(int stmt, int stmt_id, int iter)
{
  exec_var[stmt_id - 1][iter] = 1;
  return stmt;
}

static int RECORD(int stmt, int stmt_id, int iter)
{
  control_var[stmt_id - 1][iter] = (stmt == 1) ? 1 : 2;
  return stmt;
}

static int GET_RECORD(int stmt_id, int iter)
{
  return control_var[stmt_id - 1][iter];
}

static void print_arr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
    printf("%d, ", arr[i]);
  printf("\n");
}

void zsy_test1(int a[], int b[], int c[])
{
  puts("zsy_test1");
  for (int i = 0; i < N; i++) {
S1:   if (a[i] < 5) {
S2:     if (b[i] / a[i] > 1) {
            goto S4;
        }
      }
S3:   a[i] = b[i];
      goto S8;
S4:   if (a[i] > t) {
S5:         t = (b[i] - a[i]) + t;
      } else {
S6:         t = (a[i] - b[i]) + t;
            goto S7;
S7:         b[i] = a[i];
      }
S8:   c[i] = b[i] + c[i];
  }
}

void zsy_test1_mod1(int a[], int b[], int c[])
{
  puts("zsy_test1_mod1");
  for (int i = 0; i < N; i++) {
S1:   RECORD(a[i] < 5, 1, i);
S2:   if (GET_RECORD(1, i) == 1)
        RECORD(b[i] / a[i] > 1, 2, i);
S3:   if (GET_RECORD(1, i) == 2 || GET_RECORD(2, i) == 2)
        a[i] = b[i];
S4:   if (GET_RECORD(2, i) == 1)
        RECORD(a[i] > t, 4, i);
S5:   if (GET_RECORD(4, i) == 1)
        t = (b[i] - a[i]) + t;
S6:   if (GET_RECORD(4, i) == 2)
        t = (a[i] - b[i]) + t;
S7:   if (GET_RECORD(4, i) == 2)
        b[i] = a[i];
S8:   c[i] = b[i] + c[i];
  }
}

void zsy_test1_mod2(int a[], int b[], int c[])
{
  puts("zsy_test1_mod2");
  for (int i = 0; i < N; i++) {
S1:   RECORD(a[i] < 5, 1, i);
  }
  for (int i = 0; i < N; i++) {
S2:   if (GET_RECORD(1, i) == 1)
        RECORD(b[i] / a[i] > 1, 2, i);
  }
  for (int i = 0; i < N; i++) {
S3:   if (GET_RECORD(1, i) == 2 || GET_RECORD(2, i) == 2)
        a[i] = b[i];
  }
  for (int i = 0; i < N; i++) {
S4:   if (GET_RECORD(2, i) == 1)
        RECORD(a[i] > t, 4, i);
S5:   if (GET_RECORD(4, i) == 1)
        t = (b[i] - a[i]) + t;
S6:   if (GET_RECORD(4, i) == 2)
        t = (a[i] - b[i]) + t;
  }
  for (int i = 0; i < N; i++) {
S7:   if (GET_RECORD(4, i) == 2)
        b[i] = a[i];
  }
  for (int i = 0; i < N; i++) {
S8:   c[i] = b[i] + c[i];
  }
}

void zsy_test2(int a[], int b[], int c[])
{
  puts("zsy_test2");
  for (int i = 0; i < N; i++) {
S1:   if (a[i] < 5) {
S2:     if (b[i] / a[i] > 1) {
            goto S4;
        }
      }
S3:   a[i] = b[i];
      goto S8;
S4:   if (a[i] > t) {
COND_S1:    if (i == 2) break;
S5:         t = (b[i] - a[i]) + t;
      } else {
S6:         t = (a[i] - b[i]) + t;
            goto S7;
S7:         b[i] = a[i];
      }
S8:   c[i] = b[i] + c[i];
  }
}

void zsy_test2_show_ev(int a[], int b[], int c[])
{
  puts("zsy_test2_show_ev");
  for (int i = 0; i < N; i++) {
S1:   if (EXEC(a[i] < 5, 1, i)) {
S2:     if (EXEC(b[i] / a[i] > 1, 2, i)) {
            goto S4;
        }
      }
S3:   EXEC(a[i] = b[i], 3, i);
      goto S8;
S4:   if (EXEC(a[i] > t, 4, i)) {
COND_S1:    if (EXEC(i == 2, 9, i)) break;
S5:     EXEC(t = (b[i] - a[i]) + t, 5, i);
      } else {
S6:     EXEC(t = (a[i] - b[i]) + t, 6, i);
        goto S7;
S7:     EXEC(b[i] = a[i], 7, i);
      }
S8:   EXEC(c[i] = b[i] + c[i], 8, i);
  }
}

void zsy_test2_mod1(int a[], int b[], int c[])
{
  puts("zsy_test2_mod1");
  for (int i = 0; i < N; i++) {
S1:   RECORD(a[i] < 5, 1, i);
S2:   if (GET_RECORD(1, i) == 1)
        RECORD(b[i] / a[i] > 1, 2, i);
S3:   if (GET_RECORD(1, i) == 2 || GET_RECORD(2, i) == 2)
        a[i] = b[i];
S4:   if (GET_RECORD(2, i) == 1)
        RECORD(a[i] > t, 4, i);
COND_S1:
      if (GET_RECORD(4, i) == 1)
        if (i == 2) break;
S5:   if (GET_RECORD(4, i) == 1)
        t = (b[i] - a[i]) + t;
S6:   if (GET_RECORD(4, i) == 2)
        t = (a[i] - b[i]) + t;
S7:   if (GET_RECORD(4, i) == 2)
        b[i] = a[i];
S8:   c[i] = b[i] + c[i];
  }
}


void zsy_test2_mod2(int a[], int b[], int c[])
{
  puts("zsy_test2_mod2");
  cond[0] = 1;
  for (int i = 0; i < N; i++) {
S1:   if (cond[i])
        RECORD(a[i] < 5, 1, i);
S2:   if (cond[i] && (GET_RECORD(1, i) == 1))
        RECORD(b[i] / a[i] > 1, 2, i);
S3:   if (cond[i] && (GET_RECORD(1, i) == 2 || GET_RECORD(2, i) == 2))
        a[i] = b[i];
S4:   if (cond[i] && (GET_RECORD(2, i) == 1))
        RECORD(a[i] > t, 4, i);
COND_S1:
      int ei = 1;
      if (cond[i] && (GET_RECORD(4, i)) == 1) {
        ei = !(i == 2);
      }
      cond[i + 1] = cond[i] && ei; 

S5:   if (cond[i + 1] && (GET_RECORD(4, i) == 1))
        t = (b[i] - a[i]) + t;
S6:   if (cond[i + 1] && (GET_RECORD(4, i) == 2))
        t = (a[i] - b[i]) + t;
S7:   if (cond[i + 1] && (GET_RECORD(4, i) == 2))
        b[i] = a[i];
S8:   if (cond[i + 1])
        c[i] = b[i] + c[i];
  }
}

void zsy_test2_mod3(int a[], int b[], int c[])
{
  puts("zsy_test2_mod3");
  cond[0] = 1;
  for (int i = 0; i < N; i++) {
S1:   if (cond[i])
        RECORD(a[i] < 5, 1, i);
S2:   if (cond[i] && (GET_RECORD(1, i) == 1))
        RECORD(b[i] / a[i] > 1, 2, i);
S3:   if (cond[i] && (GET_RECORD(1, i) == 2 || GET_RECORD(2, i) == 2))
        a[i] = b[i];
S4:   if (cond[i] && (GET_RECORD(2, i) == 1))
        RECORD(a[i] > t, 4, i);
COND_S1_1:
      int ei = 1;
COND_S1_2:
      if (cond[i] && (GET_RECORD(4, i)) == 1) {
        ei = !(i == 2);
      }
COND_S1_3:
      cond[i + 1] = cond[i] && ei; 

S5:   if (cond[i + 1] && (GET_RECORD(4, i) == 1))
        t = (b[i] - a[i]) + t;
S6:   if (cond[i + 1] && (GET_RECORD(4, i) == 2))
        t = (a[i] - b[i]) + t;
  }
  for (int i = 0; i < N; i++) {
S7:   if (cond[i + 1] && (GET_RECORD(4, i) == 2))
        b[i] = a[i];
  }
  for (int i = 0; i < N; i++) {
S8:   if (cond[i + 1])
        c[i] = b[i] + c[i];
  }
}

int main()
{
  int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int b[] = {2, 4, 6, 8, 10, 11, 13, 16};
  int c[] = {1, 3, 5, 7, 9, 11, 13, 15};

  zsy_test2_mod2(a, b, c);

  printf("c[]:\t");
  print_arr(c, N);
  for (int i = 0; i < 9; i++) {
    printf("ev[S%d]:\t", i + 1);
    print_arr(exec_var[i], N);
  }

  return 0;
}
