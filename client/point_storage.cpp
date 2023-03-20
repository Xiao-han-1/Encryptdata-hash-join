#include <algorithm>
#include <string>
#include <iostream>
#include <libpq-fe.h>

using namespace std;

struct Node { int data; Node* next; };

// Function to insert a node at the beginning of a Linked List 
void insertAtBeginning(struct Node** head_ref, int new_data) { 
// Allocate the new node 
Node* new_node = new Node();

// Put in the data 
new_node->data = new_data; 

// Make next of new node as head 
new_node->next = (*head_ref); 

// Move the head to point to the new node 
(*head_ref) = new_node; 
}

// Function to store the linked list into the PostgreSQL database 
void storeInDatabase(struct Node* head) { 
    PGconn *conn = PQconnectdb("hostaddr=127.0.0.1 port=5432 dbname=mydb user=postgres password=mypassword"); 
    if (PQstatus(conn) == CONNECTION_BAD)
     { cout << "Connection to database failed \n"; PQfinish(conn); }
     else { cout << "Connection to database successful \n";

    // Create a table in the database
    string sqlCreateTable = "CREATE TABLE LinkedList (data INTEGER)";
    PGresult *res = PQexec(conn, sqlCreateTable.c_str());
    if (PQresultStatus(res) == PGRES_COMMAND_OK)
        cout << "Table created successfully \n";
    else
        cout << "Table creation failed \n";

    // Insert values in the table
    Node* curr = head;
    while (curr != NULL)
    {
        // Create an insert sql query
        string sqlInsert = "INSERT INTO LinkedList (data) VALUES (" + to_string(curr->data) + ")";
        res = PQexec(conn, sqlInsert.c_str());
        curr = curr->next;
    }

    // Close the connection
    PQfinish(conn);
}
}

int main() { // Create a sample linked list 
Node* head = NULL; 
insertAtBeginning(&head, 6); 
insertAtBeginning(&head, 5); 
insertAtBeginning(&head, 4); 
insertAtBeginning(&head, 3); 
insertAtBeginning(&head, 2); 
insertAtBeginning(&head, 1);

// Store the linked list in the PostgreSQL database
storeInDatabase(head);

return 0;
}
//Create an inverted index of the data stored in a database

map<string, vector> invertedIndex;

// Connect to the database 
sqlite3 *db;

// Execute query to get all data 
sqlite3_exec(db, "SELECT * FROM data", [](void *data, int numCols, char **colValues, char **colNames) {

// Create an inverted index of the data
 for(int i = 0; i < numCols; i++) { invertedIndex[colNames[i]].push_back(atoi(colValues[i])); }

}, nullptr, nullptr);

