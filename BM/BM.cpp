#include <iostream>
using namespace std;
void B_M(int a, int nn)
{
    int i, j, k, n0;
    int cnum = 1, mnum, l[15], d = 0, dis1, temp[15];
    temp[0] = cnum;
    for (i = 0; i < nn; i++)
    {
        if (a >> i & 1)
        {
            n0 = i;
            d = d ^ ((a >> i & 1) << i);
            l[i + 1] = n0 + 1;
            break;
        }
        else
        {
            l[i + 1] = 0;
            temp[i + 1] = 1;
        }
    }
    cnum = cnum ^ ((1 << n0) + 1);
    temp[n0 + 1] = cnum;
    mnum = n0 - 1;
    for (i = n0 + 1; i < nn; i++)
    {
        dis1 = a >> i & 1;
        for (j = 1; j <= l[i]; j++)
            dis1 = dis1 ^ ((temp[i] >> j & 1) & ((a >> i) - j & 1));
        d = d ^ (dis1 << i);
        if (dis1 == 0)
        {
            temp[i + 1] = temp[i];
            l[i + 1] = l[i];
        }
        else
        {
            if (l[i] < (i + 1 - l[i]))
                l[i + 1] = i + 1 - l[i];
            else
                l[i + 1] = l[i];
            if (l[i] > l[i - 1])
                mnum = i - 1;
            temp[i + 1] = temp[i] ^ (temp[mnum] << (i - mnum));
        }
    }
    cout << "The LFSR of a(" << nn << ")=(";
    for (i = 0; i < nn; i++)
        cout << (a >> i & 1);
    cout << ") is " << endl
         << "f(x)=1";
    for (j = 1; j <= nn; j++)
        if (temp[nn] >> j & 1)
            cout << "+"
                 << "x^" << j;
    cout << endl;
}
/*
int main()
{
    int a = 0, nn = 10;
    for (int i = 0; i < nn; i++)
    {

        a = a ^ ((rand() % 2) << i);
    }
    B_M(a, nn);
    return 0;
}
*/