//homework
#include<iostream>
using namespace std;

struct node
{
    int data;
    struct node *next;
};

typedef struct node NODE;
typedef struct node* PNODE;

class SinglyCl
{
    public:
        PNODE head;
        PNODE tail;
        int iCount;

        SinglyCl()
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


        void InsertLast(int no)
        { PNODE newn = NULL;

            newn = new NODE;
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

        void InsertAtPos(int no, int ipos)
        {
            PNODE newn = NULL;
            PNODE temp = NULL;
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
                newn = new NODE;
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

        void DeleteFirst()
        {
             PNODE temp = NULL;
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

        void DeleteLast()
        {
            PNODE temp = NULL;
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
        void DeleteAtPos(int ipos)
        {
            PNODE target = NULL;
            PNODE temp = NULL;
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

        void Display()
        {
             do
            {
                cout<<"|"<<head->data<<"|->";
                head = head->next;
            } while (head != tail->next);
            cout<<"\n";
            
        }
        int Count()
        {
            return iCount;
        }
};

int main()
{
    SinglyCl sobj;
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