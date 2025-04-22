//============================================================================
// Name        : LinkedList.cpp
// Author      : Dexter Melton
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <memory>
#include <optional>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // FIXME (1): Initialize housekeeping variables
    //set head and tail equal to nullptr
    head = NULL;
    tail = NULL;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // FIXME (2): Implement append logic
    //Create new node
    //if there is nothing at the head...
            // new node becomes the head and the tail
    //else 
        // make current tail node point to the new node
        // and tail becomes the new node
    //increase size count

    Node* newNode = new Node(bid);

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // FIXME (3): Implement prepend logic
    // Create new node

    // if there is already something at the head...
        // new node points to current head as its next node

    // head now becomes the new node
    //increase size count

    Node* newNode = new Node(bid);
    newNode->next = head;
    head = newNode;

    if (tail == nullptr) {
        tail = head; //List was empty
    }
    size++;

}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // FIXME (4): Implement print logic
    // start at the head

    // while loop over each node looking for a match
        //output current bidID, title, amount and fund
        //set current equal to next

    Node* currNode = head;

    while (currNode != nullptr) {
        cout << currNode->bid.bidId << ": " << currNode->bid.title
            << " | " << currNode->bid.amount << " | " << currNode->bid.fund << endl;
        currNode = currNode->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // FIXME (5): Implement remove logic
    // special case if matching node is the head
        // make head point to the next node in the list
        //decrease size count
        //return

    // start at the head
    // while loop over each node looking for a match
        // if the next node bidID is equal to the current bidID
        	// hold onto the next node temporarily
         // make current node point beyond the next node
         // now free up memory held by temp
         // decrease size count
         //return

    // current node is equal to next node
    
    Node* curr = head;
    Node* prev = nullptr;

    while (curr != nullptr) {
        if (curr->bid.bidId == bidId) {
            if (curr == head) {
                head = head->next;
                if (curr == tail) {
                    tail = nullptr; // head & tail were same node
                }
            }
            else {
                prev->next = curr->next;
                if (curr == tail) {
                    tail = prev;
                }
            }
            delete curr;
            size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    // FIXME (6): Implement search logic

    // special case if matching bid is the head

    // start at the head of the list

    // keep searching until end reached with while loop (current != nullptr)
        // if the current node matches, return current bid
        // else current node is equal to next node

    //(the next two statements will only execute if search item is not found)
        //create new empty bid
        //return empty bid 

    Bid bid;
    Node* cursor = head;

    while (cursor != NULL) {
        if (cursor->bid.bidId == bidId) {
            return cursor->bid;
        }

        cursor = cursor->next;
    }

    return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    string userPath;
    int userChoice;

    //Allow user to enter custom file path if needed
    cout << "(1) Would you like to pass your own file path?\n(2) Use default file?\nEnter choice: ";
    cin >> userChoice;

    if (userChoice == 1) {
        cout << "Please enter your desired file path: ";;
        cin >> userPath;
    }

    string pathToUse = userPath.empty() ? csvPath : userPath;

    // initialize the CSV Parser
    csv::Parser* file = nullptr;

    while (true) { //try catch block to determine if file is valid and allow the user to re-enter path if necessary.
        cout << "Loading CSV file: " << pathToUse << endl;

        try {
            file = new csv::Parser(pathToUse);
            break; //Exit the loop
        }
        catch (csv::Error& e) {
            std::cerr << "Error loading CSV: " << e.what() << std::endl;

            int retryChoice = 0;
            cout << "(1) Retry with a new file path\n(2) Exit CSV loading\nEnter choice: ";
            cin >> retryChoice;

            if (retryChoice == 1) {
                cout << "Enter new file path: ";
                cin >> pathToUse;
            }
            else {
                return; // Exit early
            }
        }
    }

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file->rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = (*file)[i][1];
            bid.title = (*file)[i][0];
            bid.fund = (*file)[i][8];
            bid.amount = strToDouble((*file)[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            cout << "Enter Bid ID: ";
            cin >> bidKey;

            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            cout << "Enter Bid ID: ";
            cin >> bidKey;

            bidList.Remove(bidKey);

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
