#include "triplet.cpp"
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    Triplet a;
    Triplet b;
    Triplet c;
    InitTriplet(a, 1, 2, 3);
    InitTriplet(b, 1, 2, 3);
    InitTriplet(c, 1, 2, 3);
    ElemType i;
    Get(a, 1, i);
    cout << (i == 1) << endl;
    Put(a, 1, 1);
    cout << (a[0] == 1) << endl;
    cout << (IsAscending(a)==1) << endl;
    cout << (IsDescending(a)==0) << endl;
    Max(a, i);
    cout << (i == 3) << endl;
    Min(a, i);
    cout << (i == 1) << endl;
    Add(a, b, c);
    cout << ((a[0] == 2) && (a[1] == 4) && (a[2] == 6)) << endl;
    Minus(a, b, c);
    cout << ((a[0] == 0) && (a[1] == 0) && (a[2] == 0)) << endl;
    DestroyTriplet(a);
    DestroyTriplet(b);
    DestroyTriplet(c);
    return 0;
}
