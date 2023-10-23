//===-- demo2.cpp ------------------------------------------------------===//
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
int exec_var[8][N] = {0};

int EXEC(int stmt, int stmt_id, int iter)
{
  exec_var[stmt_id - 1][iter] = 1;
  return stmt;
}

void zsy_test(int a[], int b[], int c[])
{
  for (int i = 0; i < N; i++) {
S1:   if (a[i] < 5) {
S2:     if (b[i] / a[i] > 1) {
            goto S4;
        }
      }
S3:   a[i] = b[i];
      goto S8;
S4:   if (a[i] > t) {
S5:     t = (b[i] - a[i]) + t;
      } else {
S6:     t = (a[i] - b[i]) + t;
        goto S7;
S7:     b[i] = a[i];
      }
S8:   c[i] = b[i] + c[i];
  }
}

void zsy_test2(int a[], int b[], int c[])
{
  for (int i = 0; i < N; i++) {
S1:   if (EXEC(a[i] < 5, 1, i)) {
S2:     if (EXEC(b[i] / a[i] > 1, 2, i)) {
            goto S4;
        }
      }
S3:   EXEC(a[i] = b[i], 3, i);
      goto S8;
S4:   if (EXEC(a[i] > t, 4, i)) {
S5:     EXEC(t = (b[i] - a[i]) + t, 5, i);
      } else {
S6:     EXEC(t = (a[i] - b[i]) + t, 6, i);
        goto S7;
S7:     EXEC(b[i] = a[i], 7, i);
      }
S8:   EXEC(c[i] = b[i] + c[i], 8, i);
  }
}

void print_arr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
    printf("%d, ", arr[i]);
  printf("\n");
}

int main()
{
  int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int b[] = {2, 4, 6, 8, 10, 11, 13, 16};
  int c[] = {1, 3, 5, 7, 9, 11, 13, 15};

  zsy_test2(a, b, c);

  printf("c[]:\t");
  print_arr(c, N);
  for (int i = 0; i < 8; i++) {
    printf("ev[S%d]:\t", i + 1);
    print_arr(exec_var[i], N);
  }

  return 0;
}
