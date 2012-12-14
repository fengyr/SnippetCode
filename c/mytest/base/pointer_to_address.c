/*
 * =====================================================================================
 *
 *       Filename:  pointer_to_address.c
 *
 *    Description:  测试指向地址的指针
 *
 *        Version:  
 *        Created:  十月 31, 2012
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

void test_pointer_to_address()
{
    static int a[] = {1, 2, 3, 4, 5};
    
    // a为int数组，
    // &a为a数组的地址，类型为int (*a)[]，指向"类型为5个int型的数组"的指针。
    // 因此，"+1"就表示偏移的大小为"5*sizeof(int)"，
    // &a+1正好是偏移到了a[4]后面的第一个起始字节，
    // (int*)(&a+1)将该字节转换成int*，再-1，即是指向a[4]的地址，内容为5。
    int *ptr1 = (int*) (&a + 1) - 1;    // 1
    int *ptr2 = (int*) (&a + 1) - 2;    // 2
    int *ptr3 = (int*) (&a + 1) - 3;    // 3
    int *ptr4 = (int*) (&a + 1) - 4;    // 4
    int *ptr5 = (int*) (&a + 1) - 5;    // 5
    printf("ptr1 = %d, ptr2 = %d, ptr3 = %d, ptr4 = %d, ptr5 = %d\n", *ptr1, *ptr2, *ptr3, *ptr4, *ptr5);
}
