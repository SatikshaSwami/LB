#include<iostream>
using namespace std;

struct node
{
    int data;
    struct node *next;
    struct node *prev;
};

typedef struct node NODE;
typedef struct node* PNODE;

class DoublyCL
{
    public:
        PNODE head;
        PNODE tail;
        int iCount;

        DoublyCL()
        {
            head = NULL;
            tail = NULL;
            iCount = 0;
        }

        void InsertFirst(int no)
        {
            PNODE newn = NULL;

            newn = new NODE;
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

        void InsertLast(int no)
        {
            PNODE newn = NULL;

            newn = new NODE;
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

        void InsertAtPos(int no, int ipos)
        {
            PNODE newn = NULL;
            PNODE temp = NULL;
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
                newn = new NODE;
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


        void DeleteFirst()
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
        void DeleteLast()
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

        void Display()
        {
            do{
                cout<<"|"<<head->data<<"|<=>";
                head = head->next;
            }while(head != tail->next);
            cout<<"\n";
        }
        int Count()
        {
            return iCount;
        }
        
        void DeleteAtPos(int ipos)
        {
            PNODE target = NULL;
            PNODE temp = NULL;
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
};

int main()
{
    DoublyCL dobj;
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