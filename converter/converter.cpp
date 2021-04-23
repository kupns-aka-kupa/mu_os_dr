#include "converter.hpp"

int toDecimal(string number, int base)
{
    int decimal= 0;
    int size = number.length();
    int ans = 0;

    for(int i = 0; i < size; i++)
    {
        if(number[i] >= 'A')
        {
            ans += (number[i] - 'A' + 10) * pow(base, size - i - 1);
        }
        else
        {
            ans += (number[i] - '0') * pow(base, size - i - 1);
        }
    }
    return ans;
}

char singleDigitPreprocess(int n)
{
    if(n < 10) return n + '0';
    else if(n >= 10)
    {
        return 'A' + (n - 10);
    }
}

string decimalTo(int decimal, int base)
{
    stack<char> remainders;
    if(decimal == 0) return "0";
    while(decimal > 0)
    {
        char rem = singleDigitPreprocess(decimal % base);
        decimal = decimal / base;
        remainders.push(rem);
    }
    string any_base = "";

    while(!remainders.empty())
    {
        any_base += remainders.top();
        remainders.pop();
    }
    return any_base;
}
