//===-- demo3.cpp ------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

void zsy_test(int cond)
{
	int a;

    for (int i = 0; i < 10; i++) {
L1:
        a = 0;
        a = a + 2;
        if (cond < 10)
            goto L2;
        else
            goto L3;
L2:
        a = a + 3;
        if (cond < 20)
            goto L4;
        else
            goto L5;
L3:
        if (cond < 30)
            goto L5;
        else
            goto L7;
L4:
        a = a + 4;
        goto L6;
L5:
        a = a + 5;
        goto L6;
L6:
        a = a + 7;
        goto L7;
L7:
	    a = a + 8;
    }

}

int main()
{
  int cond;

  zsy_test(cond);

  return 0;
}