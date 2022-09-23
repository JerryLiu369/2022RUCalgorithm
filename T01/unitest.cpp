#include "triplet.cpp"
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    Triplet a;
    Triplet b;
    Triplet c;
    InitTriplet(a,1,2,3);
    InitTriplet(b,1,2,3);
    InitTriplet(c,1,2,3);
    ElemType i;
    Min(a, i);
    cout << i<<endl;
    Add(a, b, c);
    cout << a[0] << " " << a[1] << " " << a[2] << endl;
    return 0;
}
