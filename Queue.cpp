#include<iostream>
using namespace std;

template <class T>
struct node
{
    T data;
    struct node<T> *next;
};

template <class T>
class queue
{
    public:
        struct node<T> * head;
        int iCount;

        queue();
        void enqueue(T no);   // InsertLast
        int dequeue();   // DeleteFirst
        void Disply();
        int Count();
};

template <class T>
queue<T> :: queue()
    {
        head = NULL;
        iCount = 0;
    }

    template <class T>
    void queue<T> :: enqueue(T no)   // InsertLast
    {
        struct node<T> * newn = NULL;
        struct node <T>* temp = NULL;

        newn = new node<T>;
        newn->data = no;
        newn->next = NULL;

        if(head == NULL)
        {
            head = newn;
        }
        else
        {
            temp = head;
            while(temp->next != NULL)
            {
                temp = temp -> next;
            }
            temp->next = newn;
        }
        iCount++;
    }

    template <class T>
    int queue<T> :: dequeue()   // DeleteFirst
    {
        int no = -1;
        struct node<T> * temp = NULL;

        if(head == NULL)
        {
            cout<<"Queue is empty\n";
            return -1;
        }
        else if(head->next == NULL)
        {
            no = head->data;
            delete head;
            head = NULL;
        }
        else
        {
            no = head->data;
            temp = head;
            head = head->next;
            delete temp;
        }
        iCount--;
        return no;
    }

    template <class T>
    void queue<T> :: Disply()
    {
        struct node<T> * temp = head;
        
        cout<<"Elements of queue are : \n";

        while(temp != NULL)
        {
            cout<<"| "<<temp->data<<" |\n";
            temp = temp -> next;
        }
    }

    template <class T>
    int queue<T> :: Count()
    {
        return iCount;
    }
int main()
{
    queue <int>qobj;
    int iRet = 0;

    qobj.enqueue(101);
    qobj.enqueue(51);
    qobj.enqueue(21);
    qobj.enqueue(11);
    
    qobj.Disply();
    iRet = qobj.Count();
    cout<<"Number of elements are : "<<iRet<<"\n";

    iRet = qobj.dequeue();
    cout<<"Removed element is : "<<iRet<<"\n";

    iRet = qobj.dequeue();
    cout<<"Removed element is : "<<iRet<<"\n";
    
    qobj.Disply();
    iRet = qobj.Count();
    cout<<"Number of elements are : "<<iRet<<"\n";

    return 0;
}