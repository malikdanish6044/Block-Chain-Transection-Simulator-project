#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <limits>
using namespace std;

// Hash function (DJB2)
string generateHash(const string& data)
{
    unsigned long long hash = 5381;
    for (char c : data) {
        hash = ((hash << 5) + hash) + c;
    }

    stringstream ss;
    ss << hex << hash;
    return ss.str();
}
 
struct Block {
    int index;
    string sender;
    string receiver;
    double amount;
    time_t timestamp;
    string prevHash;
    string currHash;

    Block* next;
};

class Blockchain {
private:
    Block* head;
    int blockCount;

public:
    Blockchain() {
        head = nullptr;
        blockCount = 0;
    }

    void addBlock(string sender, string receiver, double amount)
    {
        Block* newBlock = new Block();
        newBlock->index = blockCount + 1;
        newBlock->sender = sender;
        newBlock->receiver = receiver;
        newBlock->amount = amount;
        newBlock->timestamp = time(0);

        if (head == nullptr)
            newBlock->prevHash = "NULL";
        else
            newBlock->prevHash = head->currHash;

        string data = to_string(newBlock->index) + sender + receiver + to_string(amount)
                      + newBlock->prevHash + to_string(newBlock->timestamp);

        newBlock->currHash = generateHash(data);
        newBlock->next = head;
        head = newBlock;

        blockCount++;
        cout << "\nNEW BLOCK  ADDED  SUCCESSFULLY!\n";
    }

    void displayBlockchain()
    {
        if (head == nullptr) {
            cout << "\nBlockchain is empty.\n";
            return;
        }

        Block* temp = head;
        while (temp != nullptr) {
            cout << "\n**** BLOCK " << temp->index << " ****";
            cout << "\nSender: " << temp->sender;
            cout << "\nReceiver: " << temp->receiver;
            cout << "\nAmount: " << temp->amount;
            cout << "\nTimestamp: " << ctime(&temp->timestamp);
            cout << "Prev Hash: " << temp->prevHash;
            cout << "\nCurr Hash: " << temp->currHash;
            cout << "\n###############\n";
            temp = temp->next;
        }
    }
    void deleteBlock(int index)
    {
        if (head == nullptr) {
            cout << "Blockchain is empty.\n";
            return;
        }

        Block* temp = head;
        Block* prev = nullptr;

        while (temp != nullptr && temp->index != index) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Block not found.\n";
            return;
        }

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;
        cout << "\n✔ Block deleted successfully!\n";
    }

    void searchBySender(string sender)
    {
        Block* temp = head;
        bool found = false;

        while (temp != nullptr) {
            if (temp->sender == sender) {
                cout << "\nFound in Block " << temp->index;
                cout << "\nReceiver: " << temp->receiver;
                cout << "\nAmount: " << temp->amount << endl;
                found = true;
            }
            temp = temp->next;
        }

        if (!found)
            cout << "\nNo transactions found for this sender.\n";
    }

    // View block by index
    void viewBlockDetails(int index)
    {
        Block* temp = head;

        while (temp != nullptr) {
            if (temp->index == index) {
                cout << "\n****BLOCK DETAILS ****";
                cout << "\nSender: " << temp->sender;
                cout << "\nReceiver: " << temp->receiver;
                cout << "\nAmount: " << temp->amount;
                cout << "\nTimestamp: " << ctime(&temp->timestamp);
                cout << "Prev Hash: " << temp->prevHash;
                cout << "\nCurr Hash: " << temp->currHash;
                cout << "\n###############\n";
                return;
            }
            temp = temp->next;
        }

        cout << "Block not found.\n";
    }
};

double getValidAmount()
{
    double amount;
    while (true) {
        cout << "Amount (must be > 0): ";

        if (cin >> amount && amount > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return amount;
        }

        cout << "ERROR! Please enter a valid numeric amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
int main()
{
    Blockchain bc;
    int choice;

    while (true) {
        cout << "\n~~~~~ BLOCKCHAIN MENU ~~~~~";
        cout << "\n1. Add New Block";
        cout << "\n2. Display Blockchain";
        cout << "\n3. Delete Block";
        cout << "\n4. Search Transactions by Sender";
        cout << "\n5. View Block Details";
        cout << "\n6. Exit";
        cout << "\nEnter your choice (1-6): ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer

        if (choice == 1) {
            string sender, receiver;

            cout << "Sender: ";
            getline(cin, sender);

            cout << "Receiver: ";
            getline(cin, receiver);

            double amount = getValidAmount();

            bc.addBlock(sender, receiver, amount);
        }
        else if (choice == 2) {
            bc.displayBlockchain();
        }
        else if (choice == 3) {
            int index;
            cout << "Enter block index to delete: ";
            cin >> index;
            bc.deleteBlock(index);
        }
        else if (choice == 4) {
            string name;
            cout << "Enter sender name: ";
            getline(cin, name);
            bc.searchBySender(name);
        }
        else if (choice == 5) {
            int index;
            cout << "Enter block index: ";
            cin >> index;
            bc.viewBlockDetails(index);
        }
        else if (choice == 6) {
            cout << "Exiting program...\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
