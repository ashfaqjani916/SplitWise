#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

int main()
{
    //variable declarations
    string name;
    double amount;
    int sum,len;

    unordered_map<string, double> debts; // Amount owed by each person
    unordered_map<string, vector<pair<string, double>>> graph; // Adjacency list
    vector<string> sortedNodes; // Topologically sorted nodes
    unordered_map<string, int> inDegree; // In-degree of each node
    queue<string> q; // Queue used in topological sort

    int option = 0;
    do
    {   
        cout << "******** MAIN MENU ********" << endl;
        cout << " 1. Add expense... "<< endl;
        cout << " 2. Settle the expenses..."<< endl;
        cout << " 3. Exit....." << endl;

        cout << "Enter your option: ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            // Add expense to group
            sum = 0;
            cout << "Enter the number of people in the group: ";
            cin >> len;
            cout << "\n\nPlease Input the names of the people in the group.\n If they have paid any amount please Enter zero.......\n\n\n" << endl;
            for (int i = 0; i < len; i++)
            {
                cout << "Enter the name of person " << i + 1 << ": ";
                cin >> name;
                cout << "Enter the amount paid by " << name << ": ";
                cin >> amount;
                sum += amount;
               debts[name] += amount; 
            }

            if(sum>0)
            {
                double each = sum/len;
                for(const auto &entry: debts)
                {
                    const string &person = entry.first;
                    debts[person] -= each;
                }
            }
            cout << "\n\n................Expenses added successfully................\n\n" << endl;
            break;
        }
        case 2:
        {
            // we are initializing the graph here
            for (const auto &entry : debts)
            {
                const string &person = entry.first;
                graph[person] = {}; // Initialize adjacency list for each person
            }

            for (const auto &entry : debts)
            {
                const string &creditor = entry.first;
                double amount = entry.second;

                for (const auto &entry : debts)
                {
                    const string &debtor = entry.first;

                    if (creditor != debtor && amount > 0.0 && debts[debtor] < 0.0)
                    {
                        graph[creditor].emplace_back(debtor, amount);
                    }
                }
            }

            // using topological sort

            sortedNodes.clear();
            inDegree.clear();
            for (const auto &entry : graph)
            {
                const string &person = entry.first;
                inDegree[person] = 0;
            }

            for (const auto &entry : graph)
            {
                for (const auto &edge : entry.second)
                {
                    const string &debtor = edge.first;
                    inDegree[debtor]++;
                }
            }

            queue<string> emptyQueue;
            q.swap(emptyQueue);
            for (const auto &entry : inDegree)
            {
                if (entry.second == 0)
                {
                    q.push(entry.first);
                }
            }

            while (!q.empty())
            {
                const string &person = q.front();
                q.pop();
                sortedNodes.push_back(person);

                for (const auto &edge : graph[person])
                {
                    const string &debtor = edge.first;
                    inDegree[debtor]--;
                    if (inDegree[debtor] == 0)
                    {
                        q.push(debtor);
                    }
                }
            }

            for (const auto &creditor : sortedNodes)
            {
                for (const auto &edge : graph[creditor])
                {
                    const string &debtor = edge.first;
                    double amount = min(debts[creditor], abs(debts[debtor]));

                    if (amount > 0.0)
                    {   
                        cout <<"\n"<< debtor << " pays " << creditor << ": $" << fixed << setprecision(2) << amount << endl;
                        debts[creditor] -= amount;
                        debts[debtor] += amount;
                    }
                }
            }
            cout << "\n\n................Expenses settled successfully................\n\n" << endl;
            break;
        }

        case 3:
            cout << "Exiting the program. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid option. Please try again." << endl;
            break;
        }

    } while (option != 3);

    return 0;
}