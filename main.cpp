#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#include <chrono>

using std::abs;
using std::clock;
using std::cout;
using std::endl;
using std::mt19937;
using std::random_device;
using std::vector;

int binaryToDecimal(vector<bool> inputs)
{
    int sum = 0;
    int power = 1;
    for (int i = 0; i < inputs.size(); i++)
    {
        if (inputs[i])
        {
            sum += power;
        }
        power *= 2;
    }
    return sum;
}

vector<bool> decimalToBinary(int input)
{
    vector<bool> outputs;
    int power = 1;
    while (input >= power)
    {
        outputs.push_back(input % (2 * power) >= power);
        power *= 2;
    }
    return outputs;
}

class random
{
public:
    mt19937 gen;

    random()
    {
        random_device rd;
        gen = mt19937(rd() ^ ((std::mt19937::result_type) // seed
                              std::chrono::duration_cast<std::chrono::seconds>(
                                  std::chrono::system_clock::now().time_since_epoch())
                                  .count() +
                              (std::mt19937::result_type)
                                  std::chrono::duration_cast<std::chrono::microseconds>(
                                      std::chrono::high_resolution_clock::now().time_since_epoch())
                                      .count()));
    }

    size_t rand(int max = 2, int min = 0) // exclusive
    {
        std::mt19937::result_type n;
        int range = (max - min);
        while ((n = gen()) > std::mt19937::max() - (std::mt19937::max() - range + 1) % range)
        {
        }
        return n % range + min;
    }
};

random newRandom;

class basicNode
{
public:
    vector<bool> outputTable;

    basicNode(int numInputs)
    {
        outputTable = vector<bool>(pow(2, numInputs));
        for (int i = 0; i < outputTable.size(); i++)
        {
            outputTable[i] = newRandom.rand();
        }
    }

    void info()
    {
        for (int i = 0; i < outputTable.size(); i++)
        {
            cout << outputTable[i] << " ";
        }
        cout << endl;
    }

    bool evaluateInputs(vector<bool> inputs)
    {
        return outputTable[binaryToDecimal(inputs)];
    }

    vector<vector<bool>> backpropagate(vector<bool> inputs, bool goal)
    {
        int index = binaryToDecimal(inputs);
        if (outputTable[index] == goal)
        {
            return {};
        }
        outputTable[index] = goal;
        vector<vector<bool>> outputs;
        for (int i = 0; i < outputTable.size(); i++)
        {
            if (outputTable[i])
            {
                outputs.push_back(decimalToBinary(i));
            }
        }
        return outputs;
    }
};

int main()
{
    int totalCycles = 0;
    for (int k = 0; k < 1; k++)
    {
        basicNode newBasicNode(3);
        while (binaryToDecimal(newBasicNode.outputTable) != 150)
        {
            totalCycles++;
            bool one = newRandom.rand();
            bool two = newRandom.rand();
            bool three = newRandom.rand();
            bool outcome = one ^ two ^ three;
            bool output = true;
            bool stateOne = output == outcome;
            bool stateTwo = output != outcome;
            newBasicNode.backpropagate({one, two, three}, (stateOne) ? true : false);
        }
    }
    cout << "It took a total of " << totalCycles << " cycles" << endl;
}