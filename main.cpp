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
        gen = mt19937(rd() ^ ((mt19937::result_type) // seed
                              std::chrono::duration_cast<std::chrono::seconds>(
                                  std::chrono::system_clock::now().time_since_epoch())
                                  .count() +
                              (mt19937::result_type)
                                  std::chrono::duration_cast<std::chrono::microseconds>(
                                      std::chrono::high_resolution_clock::now().time_since_epoch())
                                      .count()));
    }

    size_t rand(int max = 2, int min = 0) // exclusive ex: (1 - 0)
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
};

class componentNode
{
public:
    vector<basicNode> basicNodes;
    vector<vector<int>> nodeConnections;
    vector<int> outputIndexes;
    int numInputs;

    void info()
    {
        for (int i = 0; i < basicNodes.size(); i++)
        {
            cout << i << ": ";
            basicNodes[i].info();
        }
        cout << endl;
        // for (int i = 0; i < nodeConnections.size(); i++)
        // {
        //     for (int j = 0; j < nodeConnections[i].size(); j++)
        //     {
        //         cout << nodeConnections[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;
    }

    componentNode()
    {
        for (int i = 0; i < 3; i++)
        {
            basicNode newBasicNode(2);
            basicNodes.push_back(newBasicNode);
        }
        nodeConnections.push_back({-1, -2});
        nodeConnections.push_back({-1, 0});
        nodeConnections.push_back({1, 0});
        outputIndexes.push_back(2);
        numInputs = 2;
    }

    vector<bool> evaluateInputs(vector<bool> inputs)
    {
        vector<bool> nodeOutputs(3, false);
        vector<bool> nodeEvaluated(3, false);
        vector<bool> output;
        for (int i = 0; i < outputIndexes.size(); i++)
        {
            output.push_back(evaluateNode(&inputs, outputIndexes[i], &nodeOutputs, &nodeEvaluated));
        }
        return output;
    }

    bool evaluateNode(vector<bool> *inputs, int index, vector<bool> *nodeOutputs, vector<bool> *nodeEvaluated)
    {
        if (index < 0)
        {
            // cout << "start: " << index << " is " << startingInputs[-index - 1] << endl;
            return (*inputs)[-index - 1];
        }
        if ((*nodeEvaluated)[index])
        {
            // cout << "done: " << index << " is " << (*nodeOutputs)[index] << endl;
            return (*nodeOutputs)[index];
        }
        vector<bool> childReturns;
        for (int i = 0; i < nodeConnections[index].size(); i++)
        {
            childReturns.push_back(evaluateNode(inputs, nodeConnections[index][i], nodeOutputs, nodeEvaluated));
        }
        // cout << "inputs for node " << index << " is ";
        // for (int i = 0; i < childReturns.size(); i++)
        // {
        //     cout << childReturns[i] << " ";
        // }
        // cout << endl;
        (*nodeEvaluated)[index] = true;
        (*nodeOutputs)[index] = basicNodes[index].evaluateInputs(childReturns);
        // cout << "solved: " << index << " is " << (*nodeOutputs)[index] << endl;
        return (*nodeOutputs)[index];
    }
};

int main()
{
    componentNode newComponentNode;
    newComponentNode.info();
    vector<bool> outputs = newComponentNode.evaluateInputs({1, 0});
    for (int i = 0; i < outputs.size(); i++)
    {
        cout << outputs[i] << endl;
    }
}