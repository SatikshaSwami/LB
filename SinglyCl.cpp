#include<iostream>
using namespace std;

template <class T>
struct node
{
    T data;
    struct node *next;
};

template <class T>
class SinglyCl
{
    public:
        struct node<T> * head;
        struct node<T> * tail;
        int iCount;

        SinglyCl();
        void InsertFirst(T no);
        void InsertLast(T no);
        void InsertAtPos(T no, int ipos);
        void DeleteFirst();
        void DeleteLast();
        void DeleteAtPos(int ipos);
        void Display();
        int Count();
       
};

template <class T>
SinglyCl<T>::SinglyCl()
    {
        head = NULL;
        tail = NULL;
        iCount = 0;
    }

template <class T>
    void SinglyCl<T> :: InsertFirst(T no)
    {
        struct node<T> * newn = NULL;

        newn = new node<T>;
        newn->data = no;
        newn->next = NULL;

        if(head == NULL && tail == NULL)
        {
            head = newn;
            tail = newn;
        }
        else
        {
            newn->next = head;
            head = newn;

            tail ->next = head; 
        }
        iCount++;
    }

    template <class T>
    void SinglyCl<T>:: InsertLast(T no)
    {
         struct node<T> * newn = NULL;

        newn = new node<T>;
        newn->data = no;
        newn->next = NULL;

        if(head == NULL && tail == NULL)
        {
            head = newn;
            tail = newn;
        }
        else
        {
            tail ->next = newn;
            tail = newn;

            tail ->next = head; 
        }
        iCount++;
        }

    template <class T>
    void SinglyCl<T>:: InsertAtPos(T no, int ipos)
    {
        struct node<T> * newn = NULL;
        struct node<T> * temp = NULL;
        int CountNode = 0;
        int i = 0;

        CountNode = Count();
        if(ipos < 1 || ipos > CountNode +1)
        {
            cout<<"invalid position\n";
            return;
        }
        if(ipos == 1)
        {
            InsertFirst(no);
        }
        else if(ipos == CountNode +1)
        {
            InsertLast(no);
        }
        else
        {
            newn = new node<T>;
            newn->data = no;
            newn->next = NULL;

            temp = head;

            for(i = 1; i < ipos -1; i++)
            {
                temp = temp->next;
            }
            newn->next = temp->next;
            temp->next = newn;
        }
        iCount++;
    }

    template <class T>
    void SinglyCl<T>:: DeleteFirst()
    {
            struct node<T> * temp = NULL;
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
            temp = head;

            head = head->next;
            free(temp);

            tail ->next = head;
        }
        iCount--;
    }

    template <class T>
    void SinglyCl<T>:: DeleteLast()
    {
        struct node<T> * temp = NULL;
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
            temp = head;

            while(temp->next != tail)
            {
                temp = temp->next;
            }
            free(temp->next);
            tail = temp;

            tail ->next = head;

        }
        iCount--;
    }

    template <class T>
    void SinglyCl<T>:: DeleteAtPos(int ipos)
    {
        struct node<T> * target = NULL;
        struct node<T> * temp = NULL;
        int CountNode = 0;
        int i = 0;

        CountNode = Count();
        if(ipos < 1 || ipos > CountNode +1)
        {
            cout<<"invalid position\n";
            return;
        }
        if(ipos == 1)
        {
            DeleteFirst();
        }
        else if(ipos == CountNode +1)
        {
            DeleteLast();
        }
        else
        {
            temp = head;

            for(i = 1; i < ipos -1; i++)
            {
                temp = temp->next;
            }
            target = temp->next;
            temp->next = target->next;
            free(target);
        }
        iCount--;
    }

    template <class T>
    void SinglyCl<T>:: Display()
    {
            do
        {
            cout<<"|"<<head->data<<"|->";
            head = head->next;
        } while (head != tail->next);
        cout<<"\n";
        
    }

    template <class T>
    int SinglyCl<T>:: Count()
    {
        return iCount;
    }

int main()
{
    SinglyCl <int>sobj;
    int iRet = 0;

    sobj.InsertFirst(51);
    sobj.InsertFirst(21);
    sobj.InsertFirst(11);

    sobj.InsertLast(101);
    sobj.InsertLast(111);
    sobj.InsertLast(121);

    sobj.InsertAtPos(75,4);

    sobj.Display();
    iRet = sobj.Count();

    cout<<"Number of elements are :"<<iRet<<"\n";

    sobj.DeleteAtPos(4);

    sobj.Display();
    iRet = sobj.Count();

    cout<<"Number of elements are :"<<iRet<<"\n";

    sobj.DeleteFirst();
    sobj.DeleteLast();

    sobj.Display();
    iRet = sobj.Count();

    cout<<"Number of elements are :"<<iRet<<"\n";


}