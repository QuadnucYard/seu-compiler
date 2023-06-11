struct node
{
int data;
struct node *link;
};

void main()
{
int a=111,b=2,c=3,will,wish,num;
struct node *ptr,*ptr2,*result,*temp;
void add(struct node **,int );
struct node * search(struct node *);
void display(struct node *);
void invert(struct node *);
void del(struct node *,int);
struct node * concat(struct node *,struct node *);
ptr=NULL;
ptr2=NULL;
result=NULL;		//result for storing the result of concatenation
will=1;

while(will==1)
{
printf("Main Menu \n1. Add element\n2.Delete element\n3.Search element\n4Linked List concatenation\n5.Invert linked list\n6. Display elements \nPlease enter the choice");
scanf("%d",&wish);
switch(wish)
{
case 1:
	printf("Enter the element you want to add   ");
	scanf("%d",&num);
	add(&ptr,num);
	display(ptr);
	break;
case 2:
	printf("Enter the element to delete ");
	scanf("%d",&num);
	del(ptr,num);
	break;
case 3:
	printf("Now demonstrating search ");
	temp = search(ptr);
	printf("Address of first occurence is  %u ",temp);
	break;
case 4:
	/* Inputs given internally for demo only */
	printf(" Now demonstrating linked list concatenation\nPress any key to continue...");
	add(&ptr2,2);
	add(&ptr2,4);
	add(&ptr2,6);
	printf("Displaying second Linked List");
	display(ptr2);
	result = concat(ptr,ptr2);
	printf("Now Displaying the result of concatenation");
	display(result);
	break;
case 5:

	printf("Inverting the list ...\nPress any key to continue...");
	invert(ptr);
	break;
case 6:
	display(ptr);
	break;
default:
	printf("Illegal choice");
}
printf("DO you want to continue ( press 1 for yes ");
scanf("%d",&will);
}	//end of while
}


void add(struct node **q,int num)
{
struct node *temp;
temp = *q;
if(*q==NULL)
{
	*q=malloc(sizeof(struct node));
	temp = *q;
}
else
{
	while((temp->link)!=NULL)
	{
		temp=temp->link;
	}
	temp->link = malloc(sizeof(struct node));
	temp=temp->link;
}
temp->data = num;
temp->link  = NULL;
}

void display(struct node *pt)
{

while(pt!=NULL)
{

	printf("Data : %d",pt->data);
	printf("Link : %d",pt->link);
	pt=pt->link;
}
}


void invert(struct node *ptr)
{

struct node  *p,*q,*r;
p=ptr;
q=NULL;

while(p!=NULL)
{
	r=q;
	q=p;
	p=p->link;
	q->link=r;
}
ptr = q;
display(ptr);
}


//	CONCATENATION OF LINKED LISTS

struct node * concat(struct node *p,struct node *q)
{
struct node *x,*r;


if (p==NULL)
r=q;

if (q==NULL)
r=p;
else
{
      x=p;
      r=x;
      while(x->link!=NULL)
	 x=x->link;
      x->link=q;
}
    return(r);
}


// SEARCHING AN ELEMENT IN THE LINKED LIST
// THIS FUNCTION FINDS THE FIRST OCCURENCE OF
// A DATA AND RETURNS A POINTER TO ITS ADDRESS

struct node * search(struct node *p)
{
struct node *temp;
int num;
temp = p;
printf("Enter the data that you want to search    ");
scanf("%d",&num);
printf(" Link of temp %u", temp->link);
while(temp->link!=NULL)
	{
	printf(" In while ");
	if(temp->data == num)
	return(temp);
	temp=temp->link;
	}
return(NULL);
}



// DELETING DATA FROM THE LINKED LIST//

void del(struct node *p,int num)
{

struct node *temp,*x;
temp=p;
x= NULL;

while (temp->link !=NULL)
{
if(temp->data == num)
{
	if (x==NULL)
	{
	p = temp->link;
	free(temp);
	return;
	}
	else
	{
	x->link = temp->link;
	free(temp);
	return;
	}
}                  //end of outer if
x=temp;
temp=temp->link;
}	//end of while
printf(" No such entry to delete ");
}	//end of fn.
