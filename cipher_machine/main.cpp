#include <cstdlib>
#include <iostream>
#include "cipher_machine.h"
#include "utils.h"

using namespace std;

int main()
{
    rand_plain_text();
    cipher_machine cm;
    cm.encrypt();
    cm.decrypt();
    return 0;
}