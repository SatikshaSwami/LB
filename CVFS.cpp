#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<io.h>
using namespace std;

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission; // 1 23
    struct inode *next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode; //1 2 3
    PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

void man(char *name)
{
    if(name == NULL)return;

    if(strcmp(name,"create")== 0)
    {
        cout<<"Description : Used to create new regular file\n";
        cout<<"Usage : create File_name permission\n";
    }
    else if(strcmp(name,"read") == 0)
    {
        cout<<"Description : Used to read data from regular file\n";
        cout<<"Usage : read File_name No_Of_Bytes_To_Read\n";
    }
    else if(strcmp(name,"write") == 0)
    {
        cout<<"Description : Used to write into regular file\n";
        cout<<"Usage : write File_name\n After this enter the data that we want to write\n";
    }
    else if(strcmp(name,"ls") == 0)
    {
        cout<<"Description : Used to list all information of files\n";
        cout<<"Usage : ls\n";
    }
    else if(strcmp(name,"start") == 0)
    {
        cout<<"Description : Used to display information of file\n";
        cout<<"Usage : stat File_name\n";
    }
    else if(strcmp(name,"fstart") == 0)
    {
        cout<<"Description : Used to display information of file\n";
        cout<<"Usage : stat File_Descriptor\n";
    }
    else if(strcmp(name,"truncate") == 0)
    {
        cout<<"Description : Used to remove data from file\n";
        cout<<"Usage : truncate File_name\n";
    }
    else if(strcmp(name,"open") == 0)
    {
        cout<<"Description : Used to open existing file\n";
        cout<<"Usage : open File_name mode\n";
    }
    else if(strcmp(name,"close") == 0)
    {
        cout<<"Description : Used to close opened file\n";
        cout<<"Usage : close File_name\n";
    }
    else if(strcmp(name,"closeall") == 0)
    {
        cout<<"Description : Used to close all opened file\n";
        cout<<"Usage : closeall\n";
    }
    else if(strcmp(name,"lseek") == 0)
    {
        cout<<"Description : Used to change file offset\n";
        cout<<"Usage : lseek File_Name ChangeInOffset StartPoint\n";
    }
    else if(strcmp(name,"rm") == 0)
    {
        cout<<"Description : Used to delete the file\n";
        cout<<"Usage : rm File_Name\n";
    }
    else
    {
        cout<<"ERROR : No manual entry available.\n";
    }
}

void DisplayHelp()
{
    cout<<"ls : To List out all files\n";
    cout<<"clear : To clear console\n";
    cout<<"open : To open the file\n";
    cout<<"close : To close the file\n";
    cout<<"closeall : To close all opened files\n";
    cout<<"read : To read the contents from file\n";
    cout<<"write : To Write contents into file\n";
    cout<<"exit : To Terminate file system\n";
    cout<<"start : To Display information of file using name\n";
    cout<<"fstart : To Display information of file Descriptor\n";
    cout<<"truncate : To remove all data from file\n";
    cout<<"rm : To Delete the file\n";
}

int GetFDFromName(char *name)
{
    int i = 0;

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
        if(strcmp((UFDTArr[i].ptrfiletable ->ptrinode->FileName),name)== 0)
        break;
        i++;
    }

    if(i == 50)   return -1;
    else          return i;
}

PINODE Get_Inode(char *name)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL)
    return NULL;

    while(temp != NULL)
    {
        if(strcmp(name,temp->FileName)== 0)
        break;
        temp = temp ->next;
    }
    return temp;
}

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = new INODE;// newn = (PINODE)malloc(sizeof(INODE));

        newn->LinkCount = 0;
        newn->ReferenceCount = 0;
        newn->FileType = 0;
        newn->FileSize = 0;

        newn->Buffer = NULL;
        newn->next = NULL;

        newn->InodeNumber = i;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
        i++;
    }
    cout<<"DILB created successfully\n";
}

void InitialiseSuperBlock()
{
    int i = 0;
    while(i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }
    SUPERBLOCKobj.TotalInodes = MAXINODE;
    SUPERBLOCKobj.FreeInode = MAXINODE;
}

int CreateFile(char *name, int permission)
{
    int i = 0;
    PINODE temp = head;

    if((name == NULL) || (permission == 0) || (permission > 3))
    return -1;

    if(SUPERBLOCKobj.FreeInode == 0)
    return -2;

    (SUPERBLOCKobj.FreeInode)--;

    if(Get_Inode(name) != NULL)
    return -3;

    while(temp != NULL)
    {
        if(temp ->FileType == 0)
        break;
        temp = temp->next;
    }

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
        break;

        i++;
    }

   UFDTArr[i].ptrfiletable = new FILETABLE; //(PFILETABLE)malloc(sizeof(FILETABLE));

   UFDTArr[i].ptrfiletable ->count = 1;
   UFDTArr[i].ptrfiletable ->mode = permission;
   UFDTArr[i].ptrfiletable ->readoffset = 0;
   UFDTArr[i].ptrfiletable ->writeoffset = 0;

   UFDTArr[i].ptrfiletable ->ptrinode = temp;

   strcpy(UFDTArr[i].ptrfiletable ->ptrinode ->FileName,name);
   UFDTArr[i].ptrfiletable ->ptrinode ->FileType = REGULAR;
   UFDTArr[i].ptrfiletable ->ptrinode ->ReferenceCount = 1;
   UFDTArr[i].ptrfiletable ->ptrinode ->LinkCount = 1;
   UFDTArr[i].ptrfiletable ->ptrinode ->FileSize = MAXFILESIZE;
   UFDTArr[i].ptrfiletable ->ptrinode ->FileActualSize = 0;
   UFDTArr[i].ptrfiletable ->ptrinode ->permission = permission;
   UFDTArr[i].ptrfiletable ->ptrinode ->Buffer =(char*)malloc(MAXFILESIZE);

   return i;
}

//rm_File("Demo.txt")
int rm_File(char * name)
{
    int fd = 0;

    fd = GetFDFromName(name);
    if(fd == -1)
    return -1;

    (UFDTArr[fd].ptrfiletable ->ptrinode ->LinkCount)--;

    if(UFDTArr[fd].ptrfiletable ->ptrinode ->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable ->ptrinode ->FileType = 0;
        //free (UFDTArr[fd].ptrfiletable ->ptrinode ->Buffer);
        free(UFDTArr[fd].ptrfiletable);
    }
    UFDTArr[fd].ptrfiletable = NULL;
    (SUPERBLOCKobj.FreeInode)++;
}

int ReadFile(int fd, char *arr, int isize)
{
    int read_size = 0;

    if(UFDTArr[fd].ptrfiletable == NULL)   return -1;

    if(UFDTArr[fd].ptrfiletable ->mode != READ && UFDTArr[fd].ptrfiletable ->mode != READ +WRITE)
    return -2;

    if(UFDTArr[fd].ptrfiletable ->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable ->ptrinode->permission != READ +WRITE) return -2;

    if(UFDTArr[fd].ptrfiletable ->readoffset == UFDTArr[fd].ptrfiletable ->ptrinode->FileActualSize) 
    return -3;

    if(UFDTArr[fd].ptrfiletable ->ptrinode->FileType != REGULAR)  return -4;

    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);

    if(read_size < isize)
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),read_size);

        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }
    else
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);

        (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;
    }
    return isize; 
}

int WriteFile(int fd, char *arr, int isize)
{
    if(((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ + WRITE))return -1;

    if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ + WRITE))return -1;

    if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)return -2;

    if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)  return -3;

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;

    return isize;
}

int OpenFile(char *name, int mode)
{
    int i = 0;
    PINODE temp = NULL;

    if(name == NULL || mode <= 0)
    return -1;

    temp = Get_Inode(name);
    if(temp == NULL)
    return -2;

    if(temp->permission < mode)
    return -3;

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
        break;
       i++; 
    }

    UFDTArr[i].ptrfiletable = new FILETABLE;//(PFILETABLE)malloc(sizeof(FILETABLE));
    if(UFDTArr[i].ptrfiletable == NULL)  return -1;
    UFDTArr[i].ptrfiletable->count =1;
    UFDTArr[i].ptrfiletable->mode = mode;

    if(mode == READ + WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    else if(mode == READ)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
    }
    else if(mode == WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    UFDTArr[i].ptrfiletable->ptrinode = temp;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

    return i;
}

void CLoseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
   (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

int CloseFileByName(char *name)
{
    int i = 0;
    i = GetFDFromName(name);

    if(i == -1)
    return -1;

    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

    return 0;
}

void CloseAllFile()
{
    int i = 0;
    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            break;
        }
        i++;
    }
}

int LseekFile(int fd, int size, int from)
{
    if((fd < 0) || (from > 2))  return -1;
    if(UFDTArr[fd].ptrfiletable == NULL) return -1;

    if((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
    {
        if(from == CURRENT)
        {
           if(((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)  return -1;
           if(((UFDTArr[fd].ptrfiletable->readoffset) +size) < 0) return -1;
           (UFDTArr[fd].ptrfiletable->readoffset) == (UFDTArr[fd].ptrfiletable->readoffset) + size;
        }
        else if(from == START)
        {
            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))  return -1;
            if(size < 0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if(from == END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
              return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    else if(UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if(from == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE)   return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)   return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
            (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
        }
        else if(from == START)
        {
            if(size > MAXFILESIZE)  return -1;
            if(size < 0)  return -1;
            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
            (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = size;
        }
        else if(from == END)
        {
            if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
            return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0) return -1;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
}

void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if(SUPERBLOCKobj.FreeInode == MAXINODE)
    {
        cout<<"Error : There are no files\n";
        return;
    }

    cout<<"\nFile Name\tInode number\tFile size\tLink count\n";
    cout<<"-------------------------------------------------------\n";
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            cout<<temp->FileName<<"\t"<<temp->InodeNumber<<"\t"<<temp->FileActualSize<<"\t"<<temp->LinkCount<<"\n"; //printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber, temp->FileActualSize,temp->LinkCount);
        }
        temp = temp->next;
    }
    cout<<"---------------------------------------------------------\n";
}

int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;

    if(fd < 0)  return -1;

    if(UFDTArr[fd].ptrfiletable == NULL)  return -2;

    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    cout<<"\n--------- Statical Information about file-----------\n";
    cout<<"File name :"<<temp->FileName<<"\n";
    cout<<"Inode Number :"<<temp->InodeNumber<<"\n";
    cout<<"File size :"<<temp->FileSize<<"\n";
    cout<<"Actual File size :"<<temp->FileActualSize<<"\n";
    cout<<"Link count :"<<temp->LinkCount<<"\n";
    cout<<"Reference count :"<<temp->ReferenceCount<<"\n";

    if(temp->permission == 1)
    cout<<"File Permission :Read only\n";
    else if(temp->permission == 2)
    cout<<"File Permission : Write\n";
    else if(temp->permission == 3)
    cout<<"File Permission : Read & Write\n";
    cout<<"---------------------------------------------\n";

    return 0;
}

int stat_file(char *name)
{
   PINODE temp = head;
   int i = 0; 

   if(name == NULL) return -1;

   while(temp != NULL)
   {
        if(strcmp(name,temp->FileName) == 0)
        break;
        temp = temp->next;
   }

   if(temp == NULL) return -2;

   cout<<"\n-----------Statistical Information about file------------\n";
   cout<<"Fie name :"<<temp->FileName<<"\n";
   cout<<"Inode Number :"<<temp->InodeNumber<<"\n";
   cout<<"File size :"<<temp->FileSize<<"\n";
   cout<<"Actual File size:"<<temp->FileActualSize<<"\n";
   cout<<"Link count:"<<temp->LinkCount<<"\n";
   cout<<"Reference count:"<<temp->ReferenceCount<<"\n";

   if(temp->permission == 1)
   cout<<"File Permission : Read only\n";
    else if(temp->permission == 2)
    cout<<"File Permission : Write\n";
    else if(temp->permission == 3)
    cout<<"File Permission : Read & Write\n";
    cout<<"---------------------------------------------\n";

    return 0;
}

int truncate_File(char *name)
{
    int fd = GetFDFromName(name);
    if(fd == -1)
    return -1;

    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}

int main()
{
    char *ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];

    InitialiseSuperBlock();
    CreateDILB();

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        cout<<"\n Marvellous VFS : >";

        fgets(str,80,stdin); //scanf("%d[^ '\n']s",str);

        count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);

        if(count == 1)
        {
            if(strcmp(command[0],"ls") == 0)
            {
                ls_file();
            }
            else if(strcmp(command[0],"closeall") == 0)
            {
                CloseAllFile();
                cout<<"All files closed successfully\n";
                continue;
            }
            else if(strcmp(command[0],"clear") == 0)
            {
                system("cls");
                continue;
            }
            else if(strcmp(command[0],"help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0],"exit") == 0)
            {
                cout<<"Terminating the Marvellous Virtual File System\n";
                break;
            }
             else
            {
                cout<<"\n Error : Command not found !!!\n";
                continue;
            }
        }
        else if(count == 2)
        {
            if(strcmp(command[0],"stat") == 0)
            {
                ret = stat_file(command[1]);
                if(ret == -1)
                cout<<"ERROR : Incorrect parameters\n";
                if(ret == -2)
                cout<<"ERROR : There is no such file\n";
                continue;  
            }
            else if(strcmp(command[0],"fstat") == 0)
             {
                ret = fstat_file(atoi(command[1]));
                if(ret == -1)
                cout<<"ERROR : Incorrect parameters\n";
                if(ret == -2)
                cout<<"ERROR : There is no such file\n";
                continue;  
            }
            else if(strcmp(command[0],"close") == 0)
             {
                ret = CloseFileByName(command[1]);
                if(ret == -1)
                cout<<"ERROR : There is no such file\n";
                continue;  
            }
            else if(strcmp(command[0],"rm") == 0)
             {
                ret = rm_File(command[1]);
                if(ret == -1)
                cout<<"ERROR : There is no such file\n";
                continue;  
            }
            else if(strcmp(command[0],"man") == 0)
             {
               man(command[1]);
             }
             else if(strcmp(command[0],"write") == 0)
             {
                fd = GetFDFromName(command[1]);
                 if(fd == -1)
                 {
                   cout<<"ERROR : Incorrect parameters\n";
                  continue;
                 }
               
             cout<<"Enter the data :\n";
             cin>>arr; // scanf("%[^\n]",arr);

             ret = strlen(arr);
              if(ret == 0)
              {
                cout<<"ERROR : Incorrect parameters\n";
                continue;
              }
               ret = WriteFile(fd,arr,ret);
              if(ret == -1)
                cout<<"ERROR : Permission denied\n";
              if(ret == -2)
                cout<<"ERROR : There is no sufficient memory to write\n";
             if(fd == -3)
                cout<<"ERROR : It is not regular file\n";   
            }
        else if(strcmp(command[0],"truncate") == 0)
        {
            ret = truncate_File(command[1]);
             if(ret == -1)
            cout<<"ERROR : Incorrect parameters\n";
        }
        else
        {
            cout<<"\nERROR : Command not found !!!\n";
            continue;
 
        }
    }
    else if(count == 3)
    {
        if(strcmp(command[0],"create") == 0)
        {
            ret = CreateFile(command[1],atoi(command[2]));
            if(ret >= 0)
            cout<<"File is successfully created with file descriptor"<<ret;
            if(ret == -1)
            cout<<"ERROR : Incorrect parameters\n";
            if(ret == -2)
            cout<<"ERROR : There is no inodes\n";
            if(ret == -3)
            cout<<"ERROR : File already exists\n";
            if(ret == -4)
            cout<<"ERROR : Memory allocation failure\n";
            continue;
        }
        else if(strcmp(command[0],"open") == 0)
        {
            ret = OpenFile(command[1],atoi(command[2]));
            if(ret >= 0)
            cout<<"File is successfully opened with file descriptor"<<ret;
            if(ret == -1)
            cout<<"ERROR : Incorrect parameters\n";
            if(ret == -2)
            cout<<"ERROR : File not present\n";
            if(ret == -3)
            cout<<"ERROR : Permission denied\n";
            continue;
        }
        else if(strcmp(command[0],"read") == 0)
        {
            fd = GetFDFromName(command[1]);
            if(ret == -1)
            {
                cout<<"ERROR : Incorrect parameters\n";
            continue;
            }
             ptr = new char(atoi(command[2])+1); //(char *)malloc(sizeof(atoi(command[2])) +1);

            if(ptr == NULL)
            {
                cout<<"ERROR : Memory allocation failure\n";
                continue;
            }
            ret = ReadFile(fd,ptr,atoi(command[2]));
            
            if(ret == -1)
            cout<<"ERROR : File not existing\n";
            if(ret == -2)
            cout<<"ERROR : Permission denied\n";
            if(ret == -3)
            cout<<"ERROR : Reached at end of file\n";
            if(ret == -4)
            cout<<"ERROR : It is not regular file\n";
            if(ret == 0)
            cout<<"ERROR : File empty\n";
            
            if(ret > 0)
            {
                write(2,ptr,ret);
            }
            continue;  
        }
        else
        {
            cout<<"\n ERROR : Command not found !!!\n";
            continue;
        }
    }
    else if(count == 4)
    {
        if(strcmp(command[0],"lseek") == 0)
        {
            fd = GetFDFromName(command[1]);
            if(fd == -1)
            {
                cout<<"Error : Incorrect parameter\n";
                continue;
            }
            ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
             if(ret == -1)
            {
                cout<<"Error : Unable to perform lseek\n";  
            }
        }
        else
        {
            cout<<"Error : Command not found!!!\n";
                continue;
        }
    }
    else
    {
        cout<<"Error : Command not found!!!\n";
            continue;
    }
}
    return 0;
}