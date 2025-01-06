#include<iostream>
using namespace std;

template<class T>
struct node
{
    T data;
    struct node *next;
    struct node *prev;
};

template<class T>
class DoublyCL
{
    public:
        struct node<T>* head;
        struct node<T>* tail;
        int iCount;

        DoublyCL();
        
        void InsertFirst(T no);
        void InsertLast(T no);
        void InsertAtPos(T no, int ipos);
        void DeleteAtPos(int ipos);
        void DeleteFirst();
        void DeleteLast();
        void Display();
        int Count();      
};

template<class T>
DoublyCL<T>::DoublyCL()
    {
        head = NULL;
        tail = NULL;
        iCount = 0;
    }

template<class T>
    void DoublyCL<T>:: InsertFirst(T no)
    {
        struct node<T>* newn = NULL;

        newn = new node<T>;
        newn->data = no;
        newn->next = NULL;
        newn->prev = NULL;

        if(head == NULL && tail == NULL)
        {
            head = newn;
            tail = newn;
        }
        else
        {
            newn->next = head;
            head->prev = newn;
            head = newn;

            tail->next = head;
            head->prev = tail;
        }
        iCount++;
    }

template<class T>
    void DoublyCL<T>:: InsertLast(T no)
    {
        struct node<T>* newn = NULL;

        newn = new node<T>;
        newn->data = no;
        newn->next = NULL;
        newn->prev = NULL;

        if(head == NULL && tail == NULL)
        {
            head = newn;
            tail = newn;
        }
        else
        {
            tail->next = newn;
            newn->prev = tail;

            tail = newn;

            tail->next = head;
            head->prev = tail;
        }
        iCount++;
    }

template<class T>
    void DoublyCL<T>:: InsertAtPos(T no, int ipos)
    {
        struct node<T>* newn = NULL;
        struct node<T>* temp = NULL;
        int CountNode = 0;
        int i = 0;

        CountNode = Count();

        if(ipos < 1 || ipos > CountNode + 1)
        {
            cout<<"Ivalid position";
            return;
        }
        if(ipos == 1)
        {
            InsertFirst(no);
        }
        else if(ipos == 1)
        {
            InsertLast(no);
        }
        else
        {
            newn = new node<T>;
            newn->data = no;
            newn->next = NULL;
            newn->prev = NULL;

            temp = head;

            for(i =1; i < ipos -1;i++)
            {
                temp = temp->next;
            }
            newn->next = temp->next;
            temp->next->prev = newn;

            temp->next = newn;
            newn->prev = temp;
        }
        iCount++;
    }


template<class T>
    void DoublyCL<T>:: DeleteFirst()
    {
        if(head == NULL && tail == NULL)
        {
            return;
        }
        else if(head == tail)
        {
            free(head);
            head = NULL;
            tail = NULL;
        }
        else
        {
            head = head->next;
            free(tail->next);

            tail->next = head;
            head->prev = tail;
        }
        iCount--;
    }

    template<class T>
    void DoublyCL<T> :: DeleteLast()
    {
            if(head == NULL && tail == NULL)
        {
            return;
        }
        else if(head == tail)
        {
            free(head);
            head = NULL;
            tail = NULL;
        }
        else
        {
            tail = tail->prev;
            free(tail->next);

            tail->next = head;
            head->prev = tail;
        }
        iCount--;
    }

template<class T>
    void DoublyCL<T>:: DeleteAtPos(int ipos)
    {
        struct node<T>* target = NULL;
        struct node<T>* temp = NULL;
        int CountNode = 0;
        int i = 0;

        CountNode = Count();

        if(ipos < 1 || ipos > CountNode + 1)
        {
            cout<<"Ivalid position";
            return;
        }
        if(ipos == 1)
        {
            DeleteFirst();
        }
        else if(ipos == 1)
        {
            DeleteLast();
        }
        else
        {
            temp = head;

            for(i =1; i < ipos -1;i++)
            {
                temp = temp->next;
            }
            target = temp->next;
            temp->next = target->next;
            temp->next->prev = temp;
        }
        iCount--;
    }

template<class T>
     void DoublyCL<T> :: Display()
    {
        do{
            cout<<"|"<<head->data<<"|<=>";
            head = head->next;
        }while(head != tail->next);
        cout<<"\n";
    }

    template<class T>
    int DoublyCL<T>:: Count()
    {
        return iCount;
    }
    

int main()
{
    DoublyCL <int>dobj;
    int iRet = 0;

    dobj.InsertFirst(51);
    dobj.InsertFirst(21);
    dobj.InsertFirst(11);

    dobj.InsertLast(101);
    dobj.InsertLast(111);
    dobj.InsertLast(121);

    dobj.InsertAtPos(75,4);
    
    dobj.Display();
    iRet = dobj.Count();
    cout<<"Number of elements are : "<<iRet<<"\n";

    dobj.DeleteAtPos(4);

    dobj.Display();
    iRet = dobj.Count();
    cout<<"Number of elements are : "<<iRet<<"\n";

    dobj.DeleteFirst();
    dobj.DeleteLast();

     dobj.Display();
    iRet = dobj.Count();
    cout<<"Number of elements are : "<<iRet<<"\n";

}