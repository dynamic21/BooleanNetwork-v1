#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;

class basicNode
{
public:
    vector<bool> outputTable;

    basicNode(int numInputs)
    {
        outputTable = vector<bool>(pow(2, numInputs));
        for (int i = 0; i < outputTable.size(); i++)
        {
            outputTable[i] = rand() % 2;
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
        return outputTable[sum];
    }

    void mutate()
    {
        int index = rand() % outputTable.size();
        outputTable[index] = !outputTable[index];
    }

    int getDecompressedTable()
    {
        int sum = 0;
        int power = 1;
        for (int i = 0; i < outputTable.size(); i++)
        {
            if (outputTable[i])
            {
                sum += power;
            }
            power *= 2;
        }
        return sum;
    }
};

class componentNode
{
public:
    int outputNode;
    vector<basicNode> basicNodes;
    vector<vector<int>> connections;

    componentNode()
    {
        basicNode newBasicNode(3);
        basicNodes.push_back(newBasicNode);
        outputNode = 0;
        connections.push_back({-1, -2});
    }

    bool evaluateInputs(vector<bool> inputs)
    {
        return basicNodes[outputNode].evaluateInputs(inputs);
    }

    void mutate()
    {
        basicNodes[outputNode].mutate();
    }
};

// int main()
// {
//     srand(time(NULL));
//     int totalCycles = 0;
//     for (int k = 0; k < 1000; k++)
//     {
//         componentNode keptComponentNode;
//         while (keptComponentNode.basicNodes[0].getDecompressedTable() != 150)
//         {
//             totalCycles++;
//             componentNode newComponentNode;
//             newComponentNode = keptComponentNode;
//             newComponentNode.mutate();
//             bool one = rand() % 2;
//             bool two = rand() % 2;
//             bool three = rand() % 2;
//             bool outcome = one ^ two ^ three;
//             bool outputOne = newComponentNode.evaluateInputs({one, two, three});
//             bool outputTwo = keptComponentNode.evaluateInputs({one, two, three});
//             if (outputOne != outputTwo && outputOne == outcome)
//             {
//                 keptComponentNode = newComponentNode;
//             }
//         }
//     }
//     cout << "It took a total of " << totalCycles << " cycles" << endl;
// }

int main()
{
    srand(time(NULL));
    int totalCycles = 0;
    for (int k = 0; k < 1000; k++)
    {
        componentNode keptComponentNode;
        while (keptComponentNode.basicNodes[0].getDecompressedTable() != 150)
        {
            totalCycles += 8;
            int scoreOne = 0;
            int scoreTwo = 0;
            componentNode newComponentNode;
            newComponentNode = keptComponentNode;
            newComponentNode.mutate();
            int j = 8;
            while (j--)
            {
                bool one = j % 2 > 0;
                bool two = j % 4 > 1;
                bool three = j % 8 > 3;
                bool outcome = one ^ two ^ three;
                bool outputOne = newComponentNode.evaluateInputs({one, two, three});
                bool outputTwo = keptComponentNode.evaluateInputs({one, two, three});
                scoreOne += outputOne == outcome;
                scoreTwo += outputTwo == outcome;
            }
            if (scoreOne > scoreTwo)
            {
                keptComponentNode = newComponentNode;
            }
        }
    }
    cout << "It took a total of " << totalCycles << " cycles" << endl;
}