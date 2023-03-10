/**
* IERG2080 Introduction to Systems Programming
*
* I declare that the assignment here submitted is original
* except for source material explicitly acknowledged,
* and that the same or closely related material has not been
* previously submitted for another course.
* I also acknowledge that I am aware of University policy and
* regulations on honesty in academic work, and of the disciplinary
* guidelines and procedures applicable to breaches of such
* policy and regulations, as contained in the website.
*
* University Guideline on Academic Honesty:
* http://www.cuhk.edu.hk/policy/academichonesty/
*
* Student Name : Chan Sum Pui <fill in yourself>
* Student ID   : 1155176283 <fill in yourself>
* Class/Section: IERG2080 <fill in yourself>
* Date         : 10/12/2022 <fill in yourself>
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

enum suit {red , green , blue , yellow , empty};

char transfer_color;
int transfer_size;

typedef struct Card{
	enum suit color;//red green blue yellow wild empty;
	int size;
	struct Card* next;
}Card;

Card* Create_dummy_card(){
	Card* tmp = (Card*)malloc(sizeof(tmp));
	tmp->color=empty;
	tmp->size=0;
	tmp->next=tmp;
	return tmp;
}

Card* DrawCard(Card* player_deck , char color , int size){
	Card* tmp = (Card*)malloc(sizeof(tmp));
	tmp->size=size;
	if(color=='R')tmp->color=red;
	else if(color=='G')tmp->color=green;
	else if(color=='B')tmp->color=blue;
	else if(color=='Y')tmp->color=yellow;
	tmp->next=player_deck;
	return tmp;
}

int jumpin(Card* player_deck , char color , int size){
	Card* tmp=player_deck;
	while(tmp->next!=tmp){
		if(tmp->size==size){
			if(color=='R'&&tmp->color==red){
				tmp=tmp->next;
				return 1;
			}
			else if(color=='G'&&tmp->color==green){
				tmp=tmp->next;
				return 1;
			}
			else if(color=='B'&&tmp->color==blue){
				tmp=tmp->next;
				return 1;
			}
			else if(color=='Y'&&tmp->color==yellow){
				tmp=tmp->next;
				return 1;
			}
		}
		tmp=tmp->next;
	}
	return 0;
}

int have_card(Card* player_deck , char color , int size){
	Card* tmp=player_deck;
	while(tmp->next!=tmp){
		if(tmp->size==size){
			if(tmp->color==red)transfer_color='R';

			tmp=tmp->next;
			return 1;
		}
		else if(color=='R'&&tmp->color==red){
			transfer_size=tmp->size;
			tmp=tmp->next;
			return 2;
		}
		else if(color=='B'&&tmp->color==blue){
			transfer_size=tmp->size;
			tmp=tmp->next;
			return 3;
		}
		else if(color=='G'&&tmp->color==green){
			transfer_size=tmp->size;
			tmp=tmp->next;
			return 4;
		}
		else if(color=='Y'&&tmp->color==yellow){
			transfer_size=tmp->size;
			tmp=tmp->next;
			return 5;
		}
	}
	return 0;
}


int main(int argc, char **argv){
	int number_of_player=0;
	int game_continue=1;
	Card** player_deck = (Card**)malloc(sizeof(Card)*120);
	player_deck[0]=Create_dummy_card();
	player_deck[1]=Create_dummy_card();
	

	FILE* original_hand=fopen(argv[1] , "r");
	if(!original_hand) return 0;

	fscanf(original_hand , "%d" , &number_of_player);//assume one only
	int player_no_;
	fscanf(original_hand , "%d" , &player_no_);//assume one only
	char c;
	int d;
	while(fscanf(original_hand , "%c%d " , &c , &d)!=EOF){
		player_deck[1]=DrawCard(player_deck[1] , c , d);
	}
	fclose(original_hand);


	FILE* Carddeck = fopen(argv[2] , "r");
	if(!Carddeck){
		return 0;
	}

	while(fscanf(Carddeck , "%c%d " , &c , &d)!=EOF){
		player_deck[0]=DrawCard(player_deck[0] , c , d);
	}
	fclose(Carddeck);


	FILE* table=fopen(argv[3] , "r");
	if(!table){
		return 0;
	}

	int tmp;
	fscanf(table , "%d %c%d " , &tmp , &c , &d);

	fclose(table);

	table=fopen(argv[3] , "a");
	if(!table){
		return 0;
	}

	fprintf(table , "1 R1\n");

	while(player_deck[0]->next!=player_deck[0]){
		if(player_deck[0]->color==red){
			fprintf(table , "1 R%d\n" , player_deck[0]->size);
		}
		else if(player_deck[0]->color==green){
			fprintf(table , "1 G%d\n" , player_deck[0]->size);
		}
		else if(player_deck[0]->color==blue){
			fprintf(table , "1 B%d\n" , player_deck[0]->size);
		}
		else if(player_deck[0]->color==yellow){
			fprintf(table , "1 Y%d\n" , player_deck[0]->size);
		}
		player_deck[0]=player_deck[0]->next;
	}
	
	fclose(table);


	FILE* output=fopen("output.txt" , "w");
	fprintf(output , "WIN:1\n1 NULL\n");
	fclose(output);
	return 0;
}

