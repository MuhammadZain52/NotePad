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

struct point 
{
	int x , y ;
};

struct node
{
	char letter ;
	node *next ;
	node *prev ;
	node *up ;
	node *down ;
};

class TwoDDLLADT
{
private:
	node * first ; 
	node *cursor ;

	//Helping Functions For Helping Insertion && Deletion
	void search (node *&row , node *&col) ;
	void Linker(node *&temp , node*&temp1) ;
	void zeroRowDown(node *&temp) ;
	void zeroRowUp(node *&temp) ;

	//Helping Function For Inseertion
	void insertionFromStart(node *temp) ;
	void insertionFromRight(node *temp) ;
	void insertionFromCenter(node *temp) ;

	//Helping Functions from Deletion
	void deleteFromStart() ;
	void deleteFromRight() ;
	void deleteFromCenter() ;

	//Helping Function For Destructor
	void clear() ;

public:
	//Point which maintain the load function
	point p ;

	TwoDDLLADT() ;//Constructor
	~TwoDDLLADT() ; //destructor	

	void Insert(char c); 
	void Print();
	void Save();
	void Load();
	bool Delete(); //delete a character wherever the pointer cursor is pointing.

	bool Up(); 
	bool Down(); 
	bool Left();
	bool Right(); // functions control the movement of the pointer cursor


	void shiftPress(TwoDDLLADT&);
	void cut(TwoDDLLADT&, node*, node*);
	void Paste(TwoDDLLADT&);
	 	
};

//Helping Function For Destructor
void TwoDDLLADT :: clear()
{
	if (first != 0)
	{
		cursor = first ;
		node  *temp ;
		node * temp1 = cursor->down ;

		while (temp1 != 0)
		{
			temp1 = cursor->down ;

			while (first != 0)
			{
				temp = first ;
				first = first->next ;
				delete[] temp ;
			
			}
			if(temp1 != 0)
			{
				cursor = temp1 ;
				first = cursor ;
			}
		
		}
		first = 0 ;
		cursor = 0 ;
	}
}

//Constructor or Destructor
TwoDDLLADT :: TwoDDLLADT()
{
	first = 0 ;
	cursor = 0 ;
}
TwoDDLLADT ::~TwoDDLLADT()
{
	if (first != 0)
		clear() ;
}

//Save and Load
void TwoDDLLADT :: Save()   //File handling (Console to File)
{
	ofstream fout;
	fout.open("save.txt");	
	node*r = first;
	node*c = first;
	while (r != 0)
	{
		while (c != 0)
		{
			if (c->letter == 13)
				fout << '-' ;
			else
				fout << c->letter;
			c = c->next;
		}
		fout << endl ;
		r = r->down;
		c = r;
	}
	fout.close();
}
void TwoDDLLADT :: Load()   // File Handling (File to Console)
{
	p.x = 0 ;
	p.y = 0 ;
	clear() ;
	char letter ;
	fstream fin;
	fin.open("save.txt");

	while (!fin.eof()) 
	{
		fin.get(letter) ;
		if (letter != '\n')
		{
			if (letter == '-')
				Insert(13) ;
			else
				Insert(letter);

			//Maintain the point after loading
			if (letter== '-')
			{
				p.y++ ;
				p.x= 0;
			}
			else
				p.x++ ;
			}			
	}
	fin.close();
	Print();
}

//Printing
void TwoDDLLADT :: Print() // (Simple Printing of Link List)
{
	if (first != 0)
	{
		node *row = first ;
		node *col = first ;
		while (row != 0)
		{
			while (col != 0)
			{
				if (col->letter == 13)
				{
					cout << endl ;
					break ;
				}
				
				cout << col->letter ;
				col = col->next ;
			}

			col = row->down ;
			row = col ;
		
		}
	}
}

//Helping Functions For Helping Insertion && Deletion
void TwoDDLLADT :: zeroRowDown(node *&temp)
{
	while (temp != 0)
	{
		temp->down = 0 ;
		temp = temp->next ;
	}
}
void TwoDDLLADT :: zeroRowUp(node *&temp)
{
	while (temp != 0)
	{
		temp->up = 0 ;
		temp = temp->next ;
	}
}
void TwoDDLLADT :: Linker(node *&temp , node *&temp1)
{
	while (temp != 0 && temp1 != 0) //Linking of last two rows
	{
		temp->down = temp1 ;
		temp1->up = temp ;
		temp1 = temp1->next ;
		temp = temp->next ;
	}
}
void TwoDDLLADT :: search (node *&row , node *&col)
{
	while (row != 0)
	{
		while ( col != cursor  )
		{
			if (col->next == 0 )
				break ;
			col = col->next ;
		}

		if (col == cursor)
			break ;

		if(row->down != 0)
		{
			col = row->down ;
			row = col ;
		}
		else
			break ;
	}
}

//Helping Function For Inseertion
void TwoDDLLADT :: insertionFromStart(node *temp) 
{
	if (cursor != 0)
	{
		if (cursor->prev == 0)
		{
			if (cursor->up == 0)
			{
				if (cursor->down == 0)
				{
					if (cursor->next == 0)  
					{
						if (cursor->letter == 13)
						{
							cursor->down = temp ;
							temp->up = cursor ;
							cursor = cursor->down ;
						}
						else
						{
							cursor->next = temp ;
							temp->prev = cursor ;
							cursor = cursor->next ;
						}
					}
					else 
					{
						cursor->next->prev = temp ;  
						temp->next = cursor->next ;
						cursor->next = temp ;
						temp->prev = cursor ;
						cursor = cursor->next ;

						if (cursor->letter == 13)
						{
							node *nextcursor = cursor->next ;
							cursor->next = 0 ;
							nextcursor->prev = 0;

							node *tempcursor = cursor ;
							while(tempcursor->prev != 0)
								tempcursor = tempcursor->prev ;

							Linker(tempcursor,nextcursor) ;

							if (nextcursor != 0)
								zeroRowUp(nextcursor) ;

							else if (tempcursor != 0)
								zeroRowDown(tempcursor) ;
						}
					}
				}
				else
				{
					if (cursor->next == 0)
					{
						if (cursor->letter == 13  && temp->letter != 13)
						{
							node *downcursor = cursor->down ;
							node *tempcursor = downcursor->down ;
							temp->next = downcursor ;
							downcursor->prev =  temp ;
							cursor->down = temp ;
							temp->up = cursor ;
							cursor = temp ;
							if (downcursor != 0)
							{
								Linker(temp,tempcursor) ;

								if (temp != 0)
									zeroRowDown(temp) ;
			
								else if (tempcursor != 0)
									zeroRowUp(tempcursor);
							}
						}
						else if (temp->letter == 13 && cursor->letter == 13)
						{
							node *downcursor = cursor->down ;
							cursor->down = temp ;
							temp->up = cursor ;
							downcursor->up = temp ;
							temp->down = downcursor ;
							cursor = temp ;
						}
					}
					else 
					{
						if (temp->letter != 13)
						{
							node *downcursor = cursor->down ;
							cursor->next->prev = temp ;  
							temp->next = cursor->next ;
							cursor->next = temp ;
							temp->prev = cursor ;

							node *tempcursor = cursor ;

							Linker(tempcursor , downcursor) ;

							if (tempcursor != 0)
								zeroRowUp(tempcursor) ;

							else if (downcursor != 0)
								zeroRowDown(downcursor) ;
						
							cursor = cursor->next ;
						}
						else if (temp->letter == 13)
						{
							node *downcursor = cursor->down ;
							node *nextcursor = cursor->next ;

							if(nextcursor != 0)
							{
								zeroRowDown(nextcursor) ;
								nextcursor = cursor->next ;
								nextcursor->prev = 0 ;
							}
							if (downcursor != 0)
							{
								zeroRowUp(downcursor) ;
								downcursor = cursor->down ;
							}
							node *tempcursor1 =  cursor ;
							cursor->next = 0 ;
							cursor->next = temp ;
							temp->prev = cursor ;
							cursor = temp ;
							node *tempcursor = nextcursor ;
							
							Linker(tempcursor1 , nextcursor) ;

							if (tempcursor1 != 0)
								zeroRowDown(tempcursor1) ;
			
							else if (nextcursor != 0)
								zeroRowUp(nextcursor);

							nextcursor = tempcursor ;

							if (downcursor != 0)
							{
								Linker(nextcursor , downcursor) ;
								if (downcursor != 0)
									zeroRowUp(downcursor) ;
			
								else if (nextcursor != 0)
									zeroRowDown(nextcursor);
							}
						}
					} 
				}
			}
			else
			{
				if(cursor->down == 0)
				{
					if(cursor->next == 0)
					{
						if(cursor->letter == 13)
						{
							cursor->down = temp ;
							temp->up = cursor ;
							cursor = temp ;	
						}
						else
						{
							cursor->next = temp ;
							temp->prev = cursor ;
							node * upcursor = cursor->up ;
							upcursor = upcursor->next ;
							if(upcursor != 0)
							{
								upcursor->down = temp ;
								temp->up = upcursor ;
							}
							cursor = temp ;
						}
					}
					else 
					{
						if (temp->letter != 13)
						{
							cursor->next->prev = temp ;  
							temp->next = cursor->next ;
							cursor->next = temp ;
							temp->prev = cursor ;

							node *upcursor = cursor->up ;
							node *temp1 = cursor ;
				
							Linker(upcursor , temp1) ;
				
							if (temp1 != 0)
								zeroRowUp(temp1) ;

							else if (upcursor != 0)
								zeroRowDown(upcursor) ;
					
							cursor = cursor->next ;
						}	
						else if(temp->letter == 13)
						{
							node *nextcursor = cursor->next ;
							node *upcursor = nextcursor->up ;

							if(upcursor != 0)
								zeroRowDown(upcursor) ;

							upcursor = nextcursor->up ;

							if(nextcursor != 0)
								zeroRowUp(nextcursor);

							nextcursor = cursor->next ;
							nextcursor->prev = 0 ;

							cursor->next = 0 ;
							cursor->next = temp ;
							temp->prev = cursor ;
							if(upcursor != 0)
							{
								upcursor->down = temp ;
								temp->up = upcursor ;
							}

							node *tempcursor = cursor ;

							Linker(tempcursor, nextcursor) ;

							if (tempcursor != 0)
								zeroRowDown(tempcursor) ;
		
							else if (nextcursor != 0)
								zeroRowUp(nextcursor);

							cursor = temp ;

						}
					}
				}
				else
				{
					if(cursor->next == 0) 
					{
						if (temp->letter ==13 && cursor->letter ==13)
						{
							node *downcursor = cursor->down ;
							cursor->down = temp ;
							temp->up = cursor ;
							downcursor->up = temp ;
							temp->down = downcursor ;
							cursor = temp ;
						}
						else if(cursor->letter == 13)
						{
							node *downcursor = cursor->down ;
							temp->next = downcursor ;
							downcursor->prev =temp ;
							cursor->down = temp ;
							temp->up = cursor ;
							cursor = temp ;
						}
					}
					else
					{
						node *upcursor = cursor->up ;
						node *downcursor = cursor->down ;
						node *nextcursor = cursor->next ;

						if(temp->letter == 13)
						{
							if (upcursor != 0)
							{
								upcursor = upcursor->next ;
								if (upcursor != 0)
								{
									zeroRowDown(upcursor) ;
								}
								upcursor = cursor->up ;
								if (upcursor != 0)
									upcursor = upcursor->next ;
							}
							if (downcursor != 0)
							{
								downcursor = downcursor->next ;
								if (downcursor != 0)
								{
									zeroRowUp(downcursor) ;
								}
								downcursor = cursor->down ;
							}

							cursor->next = 0 ;
							cursor->next = temp ;
							temp->prev = cursor ;
							if (upcursor != 0)
							{
								upcursor->down = temp ;
								temp->up = upcursor ;
							}
							
							node *tempcursor = nextcursor ;
							tempcursor->prev = 0 ;
							if (tempcursor != 0)
								zeroRowUp(tempcursor) ;
							
							tempcursor = nextcursor ;
							if (tempcursor != 0)
								zeroRowDown(tempcursor) ;

							tempcursor = cursor ;
							node *tempcursor1 = nextcursor ;
							Linker(tempcursor , nextcursor) ;

							if (tempcursor != 0)
								zeroRowDown(tempcursor) ;

							else if (nextcursor != 0)
								zeroRowUp(nextcursor) ;

							nextcursor = tempcursor1 ;
							Linker(nextcursor , downcursor) ;

							if (nextcursor != 0)
								zeroRowDown(nextcursor) ;

							else if (downcursor != 0)
								zeroRowUp(downcursor) ;

							cursor = temp ;
						}
						else
						{
							cursor->next->prev = temp ;  
							temp->next = cursor->next ;
							cursor->next = temp ;
							temp->prev = cursor ;
							node *temp1 = cursor ;
							if (upcursor != 0)
							{
								Linker(upcursor , temp1) ;
						
								if (temp1 != 0)
									zeroRowUp(temp1) ;
								else 
									if (upcursor != 0)
									zeroRowDown(upcursor) ;
							}
							if (downcursor != 0)
							{
								Linker(temp ,downcursor) ;

								if (temp != 0)
									zeroRowDown(temp) ;
							
								else if (downcursor != 0)
									zeroRowUp(downcursor);
							}
							cursor= cursor->next ;
						}
					}
				}
			}
		}
	}
	else
	{
		if (temp->letter != 13)
		{
			node *downcursor = first->down ;
			temp->next = first ;
			first->prev = temp ;
			first = temp ;
			cursor = temp ;
			if (downcursor != 0)
			{
				Linker(temp, downcursor) ;

				if (temp != 0)
					zeroRowDown(temp) ;

				else if (downcursor != 0)
					zeroRowUp(downcursor) ;	
			}
		}
		else
		{
			temp->down = first ;
			first->up = temp ;
			first = temp ;
			cursor = temp ;
		}
	}
}
void TwoDDLLADT :: insertionFromRight(node *temp)
{
	node *row = first ;
	node *col = first ;
	node *prevrow = 0 ;

	while (row != 0)  //search the last node
	{
		while ( col->letter != 13  )
		{
			if (col->next == 0 || cursor == col)
				break ;
			col = col->next ;
		}
		if (col == cursor)
			break ;

		if (col->letter == 13)
		{
			if(row->down != 0)
			{
				prevrow = row ;
				col = row->down ;
				row = col ;
			}
			else
				break ;
		}
		else
			break ;
	}
	
	if (row->down != 0 && cursor->letter == 13) //insert character if we move cursor up & down
	{
		if (temp->letter == 13)
		{
			node *tempcursor = cursor ;

			while(tempcursor->prev != 0)
				tempcursor = tempcursor->prev ;

			node *downcursor = tempcursor->down ;
			if (downcursor != 0)
			{
				downcursor = downcursor->next ;
				if (downcursor != 0)
				{
					zeroRowUp(downcursor) ;
				}
				downcursor = tempcursor->down ;
			}
			node *upcursor = tempcursor ;

			if(tempcursor != 0)
			{
				tempcursor = tempcursor->next ;
				if (tempcursor != 0)
					zeroRowDown(tempcursor) ;
			}
			tempcursor = upcursor ;
			
			tempcursor->down = temp ;
			temp->up = tempcursor ;
			downcursor->up = temp ;
			temp->down = downcursor ;

			cursor = temp ;
		}
		else
		{
			node *tempcursor = cursor ;

			while(tempcursor->prev != 0)
				tempcursor = tempcursor->prev ;

			node *insertioncursor = tempcursor->down ;

			node *downcursor = insertioncursor->down ;

			temp->next = insertioncursor ;
			insertioncursor->prev = temp ;

			cursor = temp ;
			node *temp1 = temp ;

			Linker(tempcursor, temp) ;

			if (tempcursor != 0)
				zeroRowUp(tempcursor) ;
			
			else if (temp != 0)
				zeroRowDown(temp) ;

			temp = temp1 ;
			if (downcursor != 0)
			{
				Linker(temp , downcursor) ;
				if (temp != 0)
				zeroRowUp(temp) ;
			
				else if (downcursor != 0)
					zeroRowDown(downcursor) ;
			}
		}
	}
	else	
	{
		if (col->letter != 13) //Insert the character with pressing enter
		{
			col->next = temp ;
			temp->prev = col ;
		}
		else   //Insert the character after pressing enter
		{
			row->down = temp ;
			temp->up = row ;
		}

		if (prevrow != 0)
			Linker(prevrow , row) ;
	
		cursor = temp ;
	}
}
void TwoDDLLADT :: insertionFromCenter(node *temp)
{
	node *upcursor = cursor->up ;
	node *downcursor = cursor->down ;
	node * tempcursor  ;
	

	if (temp->letter != 13)
	{
		cursor->next->prev = temp ;  
		temp->next = cursor->next ;
		cursor->next = temp ;
		temp->prev = cursor ;

		if (upcursor != 0)
		{
			tempcursor = upcursor ;
			zeroRowDown(tempcursor) ;
		}
		if (downcursor != 0)
		{
			tempcursor = downcursor ;
			zeroRowUp(tempcursor) ;
		}

		node *temp1 = cursor ;
		if (upcursor != 0)
		{
			Linker(upcursor , temp1) ;
		
			if (temp1 != 0)
				zeroRowUp(temp1) ;

			else if (upcursor != 0)
				zeroRowDown(upcursor) ;		
		}
		temp = cursor ;
		if (downcursor != 0)
		{
			Linker(temp ,downcursor) ;

			if (temp != 0)
				zeroRowDown(temp) ;
			
			else if (downcursor != 0)
				zeroRowUp(downcursor);
		}
		
	}
	else if (temp->letter == 13)  // If we insert enter key in the middle of the line
	{
		if(upcursor !=0 )
		{
			upcursor = upcursor->next ;
			tempcursor = upcursor ;
			zeroRowDown(tempcursor) ;
		}

		if(downcursor != 0)	
		{
			downcursor = downcursor->next ;
			while(downcursor->prev != 0)
				downcursor = downcursor->prev ;

			tempcursor = downcursor ;
			zeroRowUp(tempcursor) ;
		}

		node *nextcursor = cursor->next ;
		nextcursor->prev = 0 ;
		tempcursor = nextcursor ;
		while(tempcursor != 0)
		{
			tempcursor->up = 0 ;
			tempcursor->down = 0 ;
			tempcursor = tempcursor->next ;
		}

		node *startline = cursor ;

		cursor->next = temp ;
		temp->prev = cursor ;

		while(startline->prev != 0)
			startline = startline->prev ;

		node *downstartline = startline->down ;
		if (downstartline != 0)
		{
			tempcursor = downcursor ;
			zeroRowUp(tempcursor) ;
		}

		tempcursor = nextcursor ;
		Linker(startline, nextcursor) ;

		if (startline != 0)
			zeroRowDown(startline) ;
			
		else if (nextcursor != 0)
			zeroRowUp(nextcursor);

		nextcursor = tempcursor ;
		if (downstartline != 0)
		{
			Linker( nextcursor , downstartline) ;

			if (nextcursor != 0)
				zeroRowDown(nextcursor) ;
			else if (downstartline != 0)
				zeroRowUp(downstartline);
		}
	}
	cursor = cursor->next ;
}

//Insertion
void TwoDDLLADT :: Insert(char c)
{
	node *temp = new node ;
	temp->letter = c ;
	temp->prev = temp->next = temp->up  = temp->down = 0 ;
	if (first != 0)
	{
		if (cursor == 0)
		{
			insertionFromStart(temp) ;
		}
		else if (cursor->prev == 0 )
		{
			insertionFromStart(temp) ;
		}
		else if (cursor->next == 0)
		{
			insertionFromRight(temp) ;
		}
		else if (cursor->prev != 0 && cursor->next != 0)
		{
			insertionFromCenter(temp) ;
		}
	}
	else
	{
		first = temp ;
		cursor = temp ;
	}
}

//Helping Function For Deletion
void TwoDDLLADT :: deleteFromStart()
{
	if (cursor->prev == 0 )
	{
		if ( cursor->up == 0)
		{
			if (cursor->down == 0)
			{
				if (cursor->next == 0)
				{
					delete cursor ;
					first = 0 ;
					cursor = 0 ;
				}
				else
				{
					first = first->next ;
					delete cursor ;
					first->prev = 0 ;
					cursor = 0 ;
				}
			}
			else
			{
				if (cursor->next == 0)
				{
					first = first->down ;
					delete cursor ;
					first->up = 0 ;
					cursor = 0 ;
				}
				else
				{
					node *cursordown = cursor->down ;

					first = first->next ;
					delete cursor ;
					first->prev = 0 ;
					cursor = 0 ;

					node *tempcursor = first ;
					Linker(tempcursor,cursordown) ;

					if(tempcursor != 0)
						zeroRowDown(tempcursor) ;
					else if(cursordown != 0)
						zeroRowUp(cursordown) ;
				}
			}

			//set cursor position
			p.x = 0 ;
			p.y = 0 ;
		}
		else
		{
			if (cursor->down == 0)
			{
				node *upcursor = cursor->up ;

				if (cursor->next == 0)
				{
					delete cursor ;
					upcursor->down = 0 ;
					
					while (upcursor->next != 0)
						upcursor = upcursor->next ;
					
					cursor = upcursor ;
				}
				else
				{
					node *nextcursor = cursor->next ;
					delete cursor ;
					nextcursor->prev = 0 ;
					
					node *tempcursor = upcursor ;
					while(upcursor->next != 0)
						upcursor = upcursor->next ;

					cursor = upcursor ;
					upcursor = tempcursor ;

					Linker(upcursor , nextcursor) ;
					if (upcursor != 0)
						zeroRowDown(upcursor) ;
					else if (nextcursor != 0)
						zeroRowUp(nextcursor) ;
				}
				//position cursor
				p.x = 0 ;
			}
			else
			{
				node *upcursor = cursor->up ;
				node *downcursor = cursor->down ;
				node *nextcursor = cursor->next ;

				if (cursor->next == 0)
				{
					delete cursor ;

					node *tempcursor = upcursor ;

					while (upcursor->next != 0)
						upcursor = upcursor->next ;
					
					cursor = upcursor ;	
					upcursor = tempcursor ;
				
					Linker(upcursor , downcursor) ;
					if (upcursor != 0)
						zeroRowDown(upcursor) ;
					else if (downcursor != 0)
						zeroRowUp(downcursor) ;

					//position cursor
					p.y-- ;
				}
				else
				{
					delete cursor ;
					nextcursor->prev = 0 ;
					
					node *tempcursor = upcursor ;

					while (upcursor->next != 0)
						upcursor = upcursor->next ;
					
					cursor = upcursor ;	
					upcursor = tempcursor ;

					tempcursor = nextcursor ;
					Linker(upcursor, nextcursor) ;
					if (upcursor != 0)
						zeroRowDown(upcursor) ;
					else if (nextcursor != 0)
						zeroRowUp(nextcursor) ;

					nextcursor = tempcursor ;

					Linker(nextcursor, downcursor) ;
					if (nextcursor != 0)
						zeroRowDown(nextcursor) ;
					else if (downcursor != 0)
						zeroRowUp(downcursor) ;

					//postion cursor
					p.x = 0 ;
				}
			}
		}
	}
}
void TwoDDLLADT :: deleteFromRight()
{
	/*if(cursor->next == 0 && cursor->down != 0 && cursor->prev != 0)
	{
		node *downcursor = cursor->down ;
		downcursor->up = 0 ;
		cursor = cursor->prev ;
		delete cursor->next ;
		cursor->next = 0 ;
		if (cursor->up != 0)
		{
			node *upcursor = cursor->up ;
			upcursor->down = 0 ;
		}
		
	}
	else *///if (cursor->next == 0 && cursor->down == 0 &&cursor->prev != 0 )
	//{
		if(cursor->letter == 13)
		{
			p.y-- ;
			p.x = 0 ;
			cursor = cursor->prev ;
			delete cursor->next ;
			cursor->next = 0 ;

			node *tempcursor = cursor ;
			node *precursor ;
			while(tempcursor != 0)
			{
				precursor = tempcursor ;
				tempcursor = tempcursor->prev; 
				p.x++ ;
			}

			if (precursor->down != 0)
			{
				node *nextcursor = precursor->down ;
				if (nextcursor->down != 0)
				{
					node *downcursor = nextcursor->down ;
				
					tempcursor = nextcursor ;
					while (nextcursor != 0)
					{
						nextcursor->up = 0 ;
						nextcursor->down = 0;
						nextcursor = nextcursor->next ;
					}
					nextcursor = tempcursor ;
					cursor->next = nextcursor ;
					nextcursor->prev = cursor ;
					if (downcursor != 0)
					{
						Linker(precursor, downcursor);
						if (precursor != 0)
							zeroRowDown(precursor) ;
						else if (downcursor != 0)
							zeroRowUp(downcursor) ;
					}
				}
			}
		}
		else if (cursor->up == 0)
		{
			cursor = cursor->prev ;
			delete cursor->next ;
			cursor->next = 0 ;

			//For Cursor
			p.x-- ;
		}
		else
		{
			node *temp = cursor->up ;
			temp->down = 0 ;
			cursor = cursor->prev;
			delete cursor->next ;
			cursor->next = 0 ;

			//For Cursor
			p.x-- ;
		}

	//}
}		
void TwoDDLLADT :: deleteFromCenter()
{
	if (cursor->prev != 0 && cursor->next != 0)
	{
		if (cursor->down == 0)
		{
			node *prevcursor = cursor->prev ;
			node *nextcursor = cursor->next ;

			if (cursor->up == 0)
			{
				delete cursor ;
				prevcursor->next = nextcursor ;
				nextcursor->prev = prevcursor ;
				cursor = prevcursor ;
			}
			else
			{
				node *upcursor = cursor->up ;
				
				delete cursor ;
				prevcursor->next = nextcursor ;
				nextcursor->prev = prevcursor ;
				cursor = prevcursor ;

				Linker(upcursor , nextcursor) ;

				if(upcursor != 0)
					zeroRowDown(upcursor) ;
				else if(nextcursor != 0)
					zeroRowUp(nextcursor) ;
			}
		}
		else
		{
			node *prevcursor = cursor->prev ;
			node *nextcursor = cursor->next ;	
			node *downcursor = cursor->down ;
			node *upcursor = cursor->up ;

			if (cursor->up == 0)
			{
				delete cursor ;
				prevcursor->next = nextcursor ;
				nextcursor->prev = prevcursor ;
				cursor = prevcursor ;

				Linker(nextcursor , downcursor) ;

				if(downcursor != 0)
					zeroRowUp(downcursor) ;
				else if(nextcursor != 0)
					zeroRowDown(nextcursor) ;
			}
			else
			{
				delete cursor ;
				prevcursor->next = nextcursor ;
				nextcursor->prev = prevcursor ;
				cursor = prevcursor ;

				node *tempcursor = nextcursor ;
				Linker(upcursor , nextcursor) ;

				if(upcursor != 0)
					zeroRowDown(upcursor) ;
				else if(nextcursor != 0)
					zeroRowUp(nextcursor) ;

				nextcursor = tempcursor ;

				Linker(nextcursor, downcursor) ;
				if(downcursor != 0)
					zeroRowUp(downcursor) ;
				else if(nextcursor != 0)
					zeroRowDown(nextcursor) ;
			}
		}
	}
	//Cursor Position
	p.x-- ;
}

//Deletion
bool TwoDDLLADT :: Delete()
{
	if (first != 0)
	{
		if (cursor != 0)
		{
			if(cursor->prev == 0)
				deleteFromStart() ;
			else if(cursor->next == 0)
				deleteFromRight() ;
			else if(cursor->prev != 0 && cursor->next != 0)
				deleteFromCenter() ;

			return true ;
		}
	}

	return false ;
}

//Position
bool TwoDDLLADT:: Up()
{
	if (first != 0)
	{
		if(cursor != 0)
		{
			if (cursor->up != 0 && cursor->letter != 13)
			{
				if (cursor->up->letter != 13)
				{
					cursor = cursor->up ;
					return true ;
				}
				return false ;
			}
			else if (cursor->letter == 13 && cursor->next == 0) //&& cursor->down == 0)
			{
				if (cursor->up != 0)
				{
					if (cursor->up->letter == 13)
					{
						cursor=cursor->up ;
					}
					else
					{
						cursor = cursor->up ;
						while (cursor->next != 0)
						{
							cursor = cursor->next ;
						}
					}
				}
				else
				{
					while (cursor->prev != 0)
						cursor = cursor->prev ;

					if (cursor->up != 0)
					{
						cursor=cursor->up ;
						while (cursor->next != 0)
							cursor = cursor->next ;
					}
					else
						cursor = 0 ;
	
				}
					return true ;
			}
			else if (cursor->letter == 13)
			{
				node *tempcursor = cursor ;
				while(tempcursor->prev != 0)
					tempcursor = tempcursor->prev ;
				if(tempcursor->up == 0)
				{
					cursor = 0 ;
					return true ;
				}
				else if (tempcursor->up != 0)
				{
					tempcursor = tempcursor->up ;
					while(tempcursor->next != 0)
						tempcursor = tempcursor->next ;

					if (tempcursor->letter == 13)
					{
						cursor = tempcursor ;
						return true ;
					}
				}
				return false ;
			}
		}
	}
	return false ;
}
bool TwoDDLLADT :: Down()
{
	if (first != 0 && cursor != 0)
	{
		if (/*cursor->prev != 0 && */cursor->down != 0 && cursor->letter != 13)
		{
			cursor = cursor->down ;
			return true ;
		}
		else
		{
			if (cursor == 0)
			{
				node * tempcursor = first ;
				while (tempcursor->next != 0)
				{
					tempcursor = tempcursor->next ;
				}
				if (tempcursor->letter == 13 )
				{
					cursor = tempcursor ;
					return true ;
				}
				else
					return false;
			}
			else
			{
				if (cursor->letter == 13)
				{
					if (cursor->down)
					{
						if (cursor->down->letter == 13)
						{
							return false ;
						}
					}
					else
					{
						node *tempcursor = cursor ;
						while(tempcursor->prev != 0)
							tempcursor = tempcursor->prev ;

						if (tempcursor->down != 0)
						{
							tempcursor = tempcursor->down ;
							while(tempcursor->next != 0)
							{
								tempcursor = tempcursor->next ;
							}
							if (tempcursor->letter == 13)
							{
								cursor = tempcursor ;
								return true ;
							}
						}
					}
				}
			}		
		}
	}
	else if (cursor == 0)
	{
		node * tempcursor = first ;
		while (tempcursor->next != 0)
		{
			tempcursor = tempcursor->next ;
		}
		if (tempcursor->letter == 13 )
		{
			cursor = tempcursor ;
			return true ;
		}
		else
			return false;
	}
	else if (cursor->prev == 0)
	{
		node * tempcursor = cursor ;
		while(tempcursor->next != 0)
			tempcursor= tempcursor->next ;
		if (tempcursor->letter == 13 )
		{
			cursor = tempcursor ;
			return true ;
		}
		else
			return false;
	}
	else
	{
		if (cursor->letter == 13)
		{
			node * tempcursor = cursor ;
			while(tempcursor->prev != 0)
				tempcursor = tempcursor->prev ;
			if(tempcursor->down != 0)
			{
				tempcursor = tempcursor->down ;
				while(tempcursor->next != 0)
					tempcursor = tempcursor->next ;
				if (tempcursor->letter == 13 )
				{
					cursor = tempcursor ;
					return true ;
				}
				else
					return false;
			}
			else
				return false ;
		}
	}
	return false ;
}
bool TwoDDLLADT::Left()
{
	if (first != 0 && cursor!= 0)
	{
		if (cursor->prev != 0 && cursor->letter != 13)
		{
			cursor = cursor->prev ;
			return true ;
		}
		else
		{
			if (cursor->prev == 0)
			{
				if (cursor->up != 0)
				{
					node *tempcursor =cursor->up ;
					while(tempcursor->next != 0)
						tempcursor= tempcursor->next ;

					if (tempcursor->letter == 13)
					{
						cursor = tempcursor ;
						return true ;
					}
					else
						return false ;
				}
				else
				{
					if (cursor == first && first->letter != 13)
					{
						cursor = 0 ;
						return true ;
					}
				}
					return false ;
			}
		}
	}
	return false ;
}
bool TwoDDLLADT::Right()
{
	if( first != 0)
	{
		if(cursor != 0)
		{
			if (cursor->next != 0 && cursor->letter != 13)
			{
				if (cursor->next->letter != 13)
				{
					cursor = cursor->next ;
					return true ;
				}
			}
			else if (cursor->letter == 13)
			{
				node  *tempcursor = cursor ;
				while (tempcursor->prev != 0)
					tempcursor = tempcursor->prev ;
				
				if (tempcursor->down == 0)
					return false ;
				else if(tempcursor->down->letter == 13)
					return false ;
				else
				{
					tempcursor = tempcursor->down ;
					cursor = tempcursor ;
					return true ;
				}
			}
		}
		else
		{
			if (cursor == 0)
			{
				cursor = first ;
				return true ;
			}
			else if (cursor->letter == 13)
			{
				node  *tempcursor = cursor ;
				while (tempcursor->prev != 0)
					tempcursor = tempcursor->prev ;
	
				if (tempcursor->down == 0)
					return false ;
				else
				{
					tempcursor = tempcursor->down ;
					cursor = tempcursor ;
					return true ;
				}
			}
		}
	}
	return false ;
}

void TwoDDLLADT::cut(TwoDDLLADT& copyList, node*t1, node*t2)
{
	if (first != 0)
	{
		node *row = first;
		node *col = first;
		while (row != 0)
		{
			while (col != 0)
			{
				if (col == t1 || col == t2)
					break;
				col = col->next;
			}
			if (col == t1 || col == t2)
				break;
			col = row->down;
			row = col;
		}
		while (row != 0)
		{
			if (col == t1)
			{
				while (col != t2)
				{
					//insert all letters in linked list passed in perimeter
					//ddelete them 
				}
			}
			else
			{
				while (col != t1)
				{
					//insert all letters in linked list passed in perimeter
					//ddelete them 
				}
			}
		}
	}
}
void TwoDDLLADT::shiftPress(TwoDDLLADT& copyList)
{
	node* temp1 = 0, *temp2 = 0;
	temp1 = cursor;
	temp2 = cursor;
	while (1)
		temp2 = cursor;
	//if ()//insert check for control x
		cut(copyList,temp1,temp2);
}
void Paste(TwoDDLLADT&)
{
	//copy letters from passed linked list and insert at the position of cursor
}



void main()
{
	TwoDDLLADT notepad;
	TwoDDLLADT copyList;//pass this whenever called shiftPress function and paste function called
	int x = 0 , y = 0 ;
	while(1)
	{
		char c = _getch();
		if(c == -32)
		{
			c= _getch();
			gotoxy(x,y) ;
			if(c == 'H')
			{
				if (notepad.Up()) 
				{
					y-- ;
					gotoxy(x,y) ;			
				}
			}
		
			if(c == 'K')
			{
				if(notepad.Left()) 
				{
					x-- ;
					gotoxy(x,y) ;				
				}
			}

			if(c == 'M')
			{
				if(notepad.Right()) 
				{
					x++ ;
					gotoxy(x,y) ;
				}	
			}
				
			if(c == 'P')
			{
				if(notepad.Down()) 
				{
					y++ ;
					gotoxy(x,y) ;
				}	
			}

			if( c == 'S') // ASCII of delete
			{
				notepad.p.x = x ;
				notepad.p.y = y ;
				if(notepad.Delete())
					system ("cls") ;
			
				x = notepad.p.x ;
				y = notepad.p.y  ;
				notepad.Print();
				gotoxy(x,y) ;
			}
		}

		else if (c==19) //ASCII of Ctrl+S
			notepad.Save() ;
		
		else if (c==12) //ASCII of Ctrl+L
		{
			system("cls") ;
			notepad.Load() ;

			//retrive the points from loading
			x = notepad.p.x  ; 
			y= notepad.p.y  ;
			gotoxy(x,y) ;
		}

		else if(c == 8)
		{
			notepad.p.x = x ;
			notepad.p.y = y ;
			if(notepad.Delete())
				system ("cls") ;
			
			x = notepad.p.x ;
			y = notepad.p.y  ;
			notepad.Print();
			gotoxy(x,y) ;
		}
			
		else if (c == 17) // ASCII of Ctrl+Q
			break ;
		
		else
		{
			
 			notepad.Insert(c);
			system ("cls") ;
			notepad.Print();
			if (c==13)
			{
				y++ ;
				x= 0;
			}
			else
				x++ ;
			gotoxy(x,y) ;
		}
	}
}
