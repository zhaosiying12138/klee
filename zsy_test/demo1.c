//===-- demo1.cpp ------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <stdio.h>

#define N 8

void zsy_test(int a[], int b[], int c[])
{
  for (int i = 0; i < N; i++) {
S1:    c[i + 1] = a[i] + b[i];
S2:    c[i] = c[i] * c[i];
S3:    c[i] = c[i] + 13;
  }
}

// void zsy_test2(int a[], int b[], int c[])
// {
//   for (int i = 0; i < N; i++) {
//     c[i + 1] = a[i] + b[i];
//   }
//   for (int i = 0; i < N; i++) {
//     c[i] = c[i] * c[i];
//   }
//   for (int i = 0; i < N; i++) {
//     c[i] = c[i] + 13;
//   }
// }

void print_arr(int arr[], int len)
{
  for (int i = 0; i < len; i++)
    printf("%d, ", arr[i]);
  printf("\n");
}

int main()
{
  int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int b[] = {2, 4, 6, 8, 10, 12, 14, 16};
  int c1[N + 1] = {0};
  // int c2[N + 1] = {0};

  zsy_test(a, b, c1);
  // zsy_test2(a, b, c2);

  print_arr(c1, N + 1);
  // print_arr(c2, N + 1);

  //13, 22, 49, 94, 157, 238, 337, 454, 24,

  return 0;
}
