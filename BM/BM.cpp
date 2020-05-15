#include <iostream>
using namespace std;
void B_M(int a, int nn)
{
    int i, j, k, n0;
    int c = 1, m, l[15], d = 0, d1, cc[15];
    cc[0] = c;
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
            cc[i + 1] = 1;
        }
    }
    c = c ^ ((1 << n0) + 1);
    cc[n0 + 1] = c;
    m = n0 - 1;
    for (i = n0 + 1; i < nn; i++)
    {
        d1 = a >> i & 1;
        for (j = 1; j <= l[i]; j++)
            d1 = d1 ^ ((cc[i] >> j & 1) & ((a >> i) - j & 1));
        d = d ^ (d1 << i);
        if (d1 == 0)
        {
            cc[i + 1] = cc[i];
            l[i + 1] = l[i];
        }
        else
        {
            if (l[i] < (i + 1 - l[i]))
                l[i + 1] = i + 1 - l[i];
            else
                l[i + 1] = l[i];
            if (l[i] > l[i - 1])
                m = i - 1;
            cc[i + 1] = cc[i] ^ (cc[m] << (i - m));
        }
    }
    cout << "The LFSR of a(" << nn << ")=(";
    for (i = 0; i < nn; i++)
        cout << (a >> i & 1);
    cout << ") is " << endl
         << "f(x)=1";
    for (j = 1; j <= nn; j++)
        if (cc[nn] >> j & 1)
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