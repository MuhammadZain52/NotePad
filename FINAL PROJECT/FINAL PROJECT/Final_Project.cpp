#include <iostream>
#include <conio.h> 
#include <windows.h>
#include <fstream>
using namespace std;

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct node
{
	char letter;
	node*next;
	node*prev;
	node*up;
	node*down;
};

class TwoDDLLADT
{
	node * first, *cursor;

public:
	TwoDDLLADT();  //Constructor
	~TwoDDLLADT();  //Destructor
	void Insert(char c);
	void Print();
	void Save();
	void Load();
	void Delete(); //delete a character wherever the pointer cursor is pointing.

	void Up();
	void Down();
	void Left();
	void Right(); // functions control the movement of the pointer cursor

};

void TwoDDLLADT::Insert(char c) //Insert in LinkList
{
	node  *temp = new node;
	temp->letter = c;
	temp->next = temp->prev = temp->up = temp->down = 0;
	if (first != 0)
	{
		node *row = first;
		node *col = first;

		while (row != 0)
		{
			while (col->letter != 13)
			{
				if (col->next == 0)
					break;

				if (col == cursor)
					break;
				col = col->next;
			}

			if (col->letter == 13)
			{
				if (col == cursor)
					break;

				if (row->down != 0)
				{
					col = row->down;
					row = col;
				}
				else
					break;
			}
			else
				break;
		}
		if (col->letter == 13)
		{
			if (col == cursor)
			{
				temp->next = col;
				col->prev = temp;
				row->down = temp;
				temp->up = row;
			}
		}

		else if (col->letter != 13)
		{
			col->next = temp;
			temp->prev = col;
		}
		else
		{
			row->down = temp;
			temp->up = row;
		}
	}
	else
		first = temp;
	cursor = temp;

}
void TwoDDLLADT::Print() // (Simple Printing of Link List)
{
	node*r = first;
	node*c = first;
	while (!r)
	{
		while (!c)
		{
			cout << c->letter;
			c = c->next;
		}
		r = r->down;
		c = r;
	}
}

void TwoDDLLADT::Delete() //delete a character wherever the pointer cursor is pointing.
{
	if (!cursor)
	{
		node*curr = cursor;
		while (!curr->next)
		{
			curr->letter=curr->next->letter;
			curr = curr->next;
		}
	}
}


//Save and Load
void TwoDDLLADT::Save()   //File handling (Console to File)
{
	ofstream fout;
	fout.open("save.txt");	
	node*r = first;
	node*c = first;
	while (!r)
	{
		while (!c)
		{
			fout << c->letter;
			c = c->next;
		}
		r = r->down;
		c = r;
	}
	fout.close();
}
void TwoDDLLADT::Load()   // File Handling (File to Console)
{
	char letter;
	fstream fin;
	fin.open("save.txt");

	while (!fin.eof()) {
		fin >> letter;
		Insert(letter);
	}
	fin.close();
	Print();
}
//Change the Position of cursor
void TwoDDLLADT::Up()
{

}
void TwoDDLLADT::Down()
{

}
void TwoDDLLADT::Left()
{}
void TwoDDLLADT::Right()
{}

//Constructor or Destructor
TwoDDLLADT::TwoDDLLADT()
{
	first = 0;
	cursor = 0;
}
TwoDDLLADT ::~TwoDDLLADT()
{
}


void main()
{
	TwoDDLLADT notepad;
	int x = 0;
	int y = 0;
	while (1)
	{
		char c = _getch();
		if (c == -32)
		{
			c = _getch();
			if (c == 'H')
			{
				if (x != 0 && y != 0)
				{
					notepad.Up();
					gotoxy(x, y++);
				}
			}


			if (c == 'K')
			{
				notepad.Left();
				gotoxy(x, y);
			}


			if (c == 'M')
			{
				notepad.Right();
				gotoxy(x++, y);
			}


			if (c == 'P')
			{
				notepad.Down();
				gotoxy(x, y--);
			}

		}

		/*else if (c == 8)
		notepad.Delete() ;
		else if (c == 19)
		notepad.Save() ;
		else if (c == 76)
		notepad.Load() ;*/
		else
		{
			system("pause");
			c= _getch();
			notepad.Insert(c);
			system("pause");
			system("cls");
			system("pause");
			notepad.Print();
			system("pause");
		}
	}
}