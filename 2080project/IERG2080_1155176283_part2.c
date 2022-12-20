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
#include <string.h>

int transfer_player_number;
int transfer_size;
char trannsfer_color;

typedef struct Card{
	char color;
	int size;
	struct Card* next;
}Card;

Card* Create_dummy_card(){
	Card* tmp=(Card*)malloc(sizeof(Card));
	tmp->color='0';
	tmp->size=-1;
	tmp->next=tmp;
	return tmp;
}

Card* AddCard(Card* player_deck , char color , int size){
	Card* tmp=(Card*)malloc(sizeof(Card));
	tmp->size=size;
	tmp->color=color;
	tmp->next=player_deck;
	return tmp;
}

void print_hand(Card* deck){
	FILE* output = fopen("output.txt" , "w");
	while(deck!=deck->next){
		fprintf(output , "%c%d " , deck->color , deck->size);
		deck=deck->next;
	}
}

int jumpin(Card* player_deck , char color , int size){
	printf("jumpin called\n");
	Card* tmp=player_deck;
	while(tmp->next!=tmp){
		if(tmp->size==size){
			return 1;
		}
		tmp=tmp->next;
	}
	return 0;
}

int have_card(Card* player_deck , char color , int size){
	Card* tmp=player_deck;
	Card* previous = NULL;
	while(tmp->next!=tmp){
		if(tmp->size==size){
			trannsfer_color=tmp->color;
			free(tmp);
			return 1;
		}
		else if(tmp->color==color){
			transfer_size=tmp->size;
			free(tmp);
			return 2;
		}
		previous = tmp;
		tmp=tmp->next;
	}
	return 0;
}

Card* remove_card(Card* deck , char c , int d){
	Card *current , *previous;
	current=deck;
	previous=Create_dummy_card();
	while(current->next!=current){
		if(current->color==c && current->size==d){
			if(current==deck){
				deck=deck->next;
				free(current);
				current=deck;
			}
			else{
				previous->next=current->next;
				free(current);
				current = previous->next;
			}
		}
		else{
			previous=current;
			current=current->next;
		}
	}
	return deck;
}

int main(int argc, char **argv){
	int number_of_player=0;
	int game_continue=1;
	Card** player_deck = (Card**)malloc(sizeof(Card)*120);
	
	FILE* original_hand=fopen(argv[1] , "r");
	if(!original_hand){
		return 0;
	}
	fscanf(original_hand , "%d" , &number_of_player);
	int player_no;
	char str[300];
	char c;
	int d;
	for(int i=0;i<=number_of_player+1;i++){//memory allocate
		player_deck[i]=Create_dummy_card();
		printf("address of deck[%d]: %d\n" , i , player_deck[i]);
	}
	for(int i=1;i<=number_of_player;i++){
		fscanf(original_hand , "%d " , &player_no);
		fgets(str , 300 , original_hand);
		for(int j=0;j<strlen(str);j++){//read player hand
			if(str[j]>='A'&&str[j]<='Z'){
				player_deck[i]=AddCard(player_deck[i] , str[j] , str[j+1]-'0');
			}
			if(str[j]=='\n')break;
		}
	}

	fclose(original_hand);
	FILE* Carddeck = fopen(argv[2] , "r");
	if(!Carddeck){
		return 0;
	}
	while(fscanf(Carddeck , "%c%d " , &c , &d)!=EOF){//read deck
		player_deck[0]=AddCard(player_deck[0] , c , d);
	}
	while(player_deck[0]->next!=player_deck[0]){//reverse deck
		player_deck[number_of_player+1] = AddCard(player_deck[number_of_player+1] , player_deck[0]->color , player_deck[0]->size);
		player_deck[0]=player_deck[0]->next;
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
	//game progress
	int cnt=0;
	int winner=0;
	while(game_continue){
		int current_player = (cnt%number_of_player)+1;
		printf("%d\n" , current_player);
		for(int i=1;i<=number_of_player;i++){
			if(jumpin(player_deck[i] , c , d)==1){
				printf("%d jumpined\n" , i);
				player_deck[i]=remove_card(player_deck[i] , c ,d);
				fprintf(table , "%d %c%d\n" , i , c , d);
				cnt = i;
				current_player=(cnt%number_of_player)+1;
			}
		}
		if(have_card(player_deck[current_player] , c , d)==1){
			c=trannsfer_color;
			player_deck[current_player]=remove_card(player_deck[current_player] , c ,d);
			fprintf(table , "%d %c%d\n" , current_player , c , d);
		}
		else if(have_card(player_deck[current_player] , c , d)==2){
			d=transfer_size;
			player_deck[current_player]=remove_card(player_deck[current_player] , c ,d);
			fprintf(table , "%d %c%d\n" , current_player , c , d);
		}
		else{
			printf("draw crad\n");
			fprintf(table , "%d add\n" , current_player);
			AddCard(player_deck[current_player] , player_deck[number_of_player+1]->color , player_deck[0]->size);
			player_deck[number_of_player+1]=player_deck[number_of_player+1]->next;
		}
		for(int i=1;i<=number_of_player;i++){
			if(player_deck[i]==Create_dummy_card()){
				printf("has winner\n");
				game_continue=0;
				winner=i;
				break;
			}
		}
		cnt++;
	} 
	
	fclose(table);
	FILE* output=fopen("output.txt" , "w");
	fprintf(output , "WIN:%d\n" , winner);
	fclose(output);
	for(int i=1;i<=number_of_player;i++){
		if(i!=winner){
			FILE* output=fopen("output.txt" , "w");
			fprintf(output , "%d " , i);
			fclose(output);
			print_hand(player_deck[i]);
		}
		
		else{
			FILE* output=fopen("output.txt" , "w");
			fprintf(output , "%d NULL" , winner);
			fclose(output);
		} 
	}
	fclose(output);
	return 0;
}