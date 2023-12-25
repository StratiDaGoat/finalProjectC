#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

void points();  
int search_player(char *name);
bool users(char *name);
void save_players(char *filename);
void leaderboard();
void load_players(char *filename);
bool ans(char ans);
int plays_game();
void save_leaderboard(char *filename);
bool searchList(int r);


typedef struct Players {
				//Player struct that defines a player: a name and their score (points);
	char name[20];
	int points;

}Players;

void sort_players(Players **arr); 

typedef struct Questions{  //Question struct that defines a Question: a question and an answer, and its possible choices for the user to pick

	char question[100];
	char answer[50];
	char choice1[50];
	char choice2[50];
	char choice3[50];
	char choice4[50];

}Questions;

int qArray[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; //this q array takes in numbers of possible questions numbers that came in and make sure they do not repeat.
Questions qArr[30];  //this is a questions array consisting of the struct Questions, consists of all questions, answers, and answer choices that the user will be receiving
Players **arr; //double pointer array that consists of players that previously played the game and that currently play the game, has the user name and points.
int playerLen; //length of the player array

int main(){
	srand(time(NULL)); //used for a random number generator
	char userName[20]; //variable that takes in the users name
	int userPoints = 0; //a variable that will take in the users points at the end of the game

	load_players("players.txt"); //load players is a function that takes in a file and loads in the players that are on that file to the players arr
	sort_players(arr); //sort players just sorts the players based on their points, from highest points to lowest
	printf("What is your name?\n"); 
	
	if(scanf("%10s", userName) != 1){
		return 2;
	}

	char userTut[4]; //userTut takes in the users answer if they want to see the tutorial

	if(users(userName) == true){  //if statement that checks the function 'users' to see if the user has played the game before, function 'users' checks the player arr to see if the player with that name has played the game before.
		printf("I see that you have played this game before! Would you like to go through the tutorial?\n"); //if the player has played the game before, it asks kindly if they want to go through the tutorial
		printf("Yes\n");
		printf("No\n");
		if(scanf("%s", userTut) != 1){
			return 2;
		}
		userTut[0] = toupper(userTut[0]);
		if(userTut[0] == 'Y'){
			printf("Welcome to my game!");
			printf("This game gives you 10 random multiple choice questions, out of 30 possible questions, which you must answer in 30 seconds or less. The faster you answer, the more points you receive! If it takes you more than 30 seconds to answer the question, you receive 0 points! The questions are going to inform you on the availabilty of safe drinking water. Good luck!\n");
		}
	}else{ //automatically displays the tutorial if the users name was not found in the player arr
			printf("This game gives you 10 random multiple choice questions, out of 30 possible questions, which you must answer in 30 seconds or less. The faster you answer, the more points you receive!If it takes you more than 30 seconds to answer the question, you receive 0 points! The questions are going to inform you on the availabilty of safe drinking water. Good luck!\n");
	}	
	int userNum; //variables
	int index;
	printf("\n");
	while(true){ //infinite while loop of the main menu for the user
		printf("What would you like to do?\n");
		printf("(1) Save & Exit the game.\n");
		printf("(2) Play the game.\n");      //choices for user
		printf("(3) View the leaderboard (Top 5 Scores).\n");

	while( scanf("%d", &userNum) != 1){  //makes sure the user inputs a number and not a character, for some reason, if you put a character, the program would just quit, not save and exit or anything, just quit
		while(getchar() != '\n');

		printf("Invalid input. Please enter a number.\n");
	}

		switch(userNum){  //switch case that takes in a user input on what choice he choice from the above menu, if user choice 1,2, or 3, user will experience what he choice, if he fails to choose 1, 2, or 3 it will give him another chance to press 1 2 or 3.
			case 1:  //case 1 sorts the player arr, saves the leaderboard and the players and then prints that the players were saved and free the arr and promptly exits
				sort_players(arr);
				save_leaderboard("leaderboard.txt");
				save_players("players.txt");
				printf("The players scores were saved.\n");
				for(int i = 0; i < playerLen; i++){
					free(arr[i]);
				}
				free(arr);
				exit(1);

			case 2: //case 2 saves the user points from playing the game and prints that the user with this name scored this many points and then finds the player and saves the points to the player with the corresponding name, if the user did worse than previously, his score does not change and his best score stays
				userPoints = plays_game();
				printf("%s scored %d points!",userName, userPoints);
				index = search_player(userName);
				if(arr[index]->points < userPoints){
					arr[index]->points = userPoints;
				}
				break;

			case 3: //case 3 sorts the list and then shows the top 5 players on the leaderboard
				sort_players(arr);
				leaderboard();
				break;

			default: //default case has the user retry the option if they did not choose 1, 2, or 3
				printf("User did not choose option 1, 2, or 3.Try again...\n");
				break;
		}
	}

}

int plays_game(){  //user plays the game, has all 30 possible questions in here

	qArr[0] = (Questions) {"How many people lacked safe drinking water?", "2.2 Billion people",  "2.2 Million people", "2.7 Billion people", "2.4 Billion people", "2.2 Billion people"};
	qArr[1] = (Questions) {"How many people lacked safe sanitation?", "3.5 Billion people", "3.5 Billion people", "2.7 Billion people", "3.5 Million people", "2.7 Million people"};
	qArr[2] = (Questions) {"How many people lacked basic hand washing facilities?", "2.2 Billion people", "2.2 Million people", "2.2 Billion people", "2.4 Million People", "2.4 Billion people"};
	qArr[3] = (Questions) {"How many people live in water-stressed countries?", "2.4 Billion people", "2.4 Million people", "2.2 Million people", "2.4 Billion people", "2.2 Billion people"};	
	qArr[4] = (Questions) {"How many people in the U.S. lack safe drinking water?", "46 million people", "36 million people", "42 million people", "54 million people", "46 million people"};
	qArr[5] = (Questions) {"What percent of people in Niger lack acces to clean water?", "54 percent", "46 percent", "15 percent", "54 percent", "68 percent"};
	qArr[6] = (Questions) {"What percent of people in Papau New Guinea lack access to clean water?", "55 percent", "64 percent", "55 percent", "50 percent", "43 percent"};
	qArr[7] = (Questions) {"What percent of people in the Democratic Republic of the Congo lack access to clean water?", "54 percent", "65 percent", "57 percent", "54 percent", "48 percent"};
	qArr[8] = (Questions) {"What percent of people in Chad lack access to clean water?", "54 percent", "60 percent", "58 percent", "55 percent", "54 percent"};
	qArr[9] = (Questions) {"What percent of people in Ethiopia lack access to clean water?", "50 percent", "50 percent", "48 percent", "46 percent", "44 percent"};
	qArr[10] = (Questions) {"What percent of people in Eritrea lack access to clean water?", "48 percent", "54 percent", "52 percent", "50 percent", "48 percent"};
	qArr[11] = (Questions) {"What percent of people in Somalia lack access to clean water?", "44 percent", "46 percent", "44 percent", "42 percent", "40 percent"};
	qArr[12] = (Questions) {"What percent of people in Uganda lack access to clean water?", "44 percent", "46 percent", "44 percent", "42 percent", "40 percent"};
	qArr[13] = (Questions) {"What percent of people in Angola lack access to clean water?", "43 percent", "47 percent", "45 percent", "43 percent", "41 percent"};
	qArr[14] = (Questions) {"What percent of people in Mozambique lack access to clean water?", "37 percent", "43 percent", "41 percent", "39 percent", "37 percent"};
	qArr[15] = (Questions) {"How many people in Egypt lack access to clean water?", "7.3 million people", "8.4 million people", "7.3 million people", "84 million people", "73 million people"};
	qArr[16] = (Questions) {"What percent of people in South Africa lack access to clean water?", "19 percent", "21 percent", "19 percent", "17 percent", "15 percent"};
	qArr[17] = (Questions) {"How many people in Nigeria lack access to clean water?", "60 million people", "6.0 million people", "7.3 million people", "60 million people", "73 million people"};
	qArr[18] = (Questions) {"What percent of people in Kenya lack access to clean water?", "43 percent", "49 percent", "47 percent", "45 percent", "43 percent"};
	qArr[19] = (Questions) {"What percent of people in Ghana lack access to clean water?", "8 percent", "12 percent", "10 percent", "8 percent", "45 percent"};
	qArr[20] = (Questions) {"What percent of peoeple in Tanzania lack access to clean water?", "39 percent", "39 percent", "36 percent", "33 percent", "30 percent"};
	qArr[21] = (Questions) {"What percent of people in Mali lack access to clean water?", "20 percent", "26 percent", "23 percent", "20 percent", "17 percent"};
	qArr[22] = (Questions) {"What percent of people in Côte d'Ivoire lack access to clean water?", "50 percent", "59 percent", "56 percent", "53 percent", "50 percent"};
	qArr[23] = (Questions) {"What percent of people in Algeria lack access to clean water?", "28 percent", "34 percent", "30 percent", "28 percent", "24 percent"};
	qArr[24] = (Questions) {"What percent of people in Sudan lack access to clean water?", "40 percent", "45 percent", "40 percent", "35 percent", "30 percent"};
	qArr[25] = (Questions) {"What percent of people in Cameroon lack access to clean water?","25 percent", "35 percent", "30 percent", "25 percent", "20 percent"};
	qArr[26] = (Questions) {"What percent of people in Madagascar lack access to clean water?", "58 percent", "68 percent", "63 percent", "58 percent", "53 percent"};
	qArr[27] = (Questions) {"What percent of people in Libya lack access to clean water?", "40 percent", "70 percent", "60 percent", "50 percent", "40 percent"};
	qArr[28] = (Questions) {"What percent of people in Rwanda lack access to clean water?", "43 percent", "53 percent", "43 percent", "33 percent", "23 percent"};
	qArr[29] = (Questions) {"What percent of people in Burkina Faso lack access to clean water?", "27 percent", "30 percent", "27 percent", "24 percent", "21 percent"};
	int totalPoints = 0;
	int finalPoints = 0;
	double pointMaker = 0.0;
	double timer = 0;
	char userAns;	
	char yCont;
	int r = rand()%30;
	int questCount = 1;

	for(int i = 0; i < 10; i++){ //for loop of size 10 so 10 questions can be displayed
	

		while(true){
			r = rand()%30; //chooses a number at random for the question to display.

			if(searchList(r) == false){ //searchList checks the qArray to see if the number has already been chosen
				break;
			}
		}

		time_t start = time(NULL); //starts timer for user
		printf("\n");
		printf("Question %d:\n", questCount);
		printf("%s\n",qArr[r].question);
		printf("(A) %s\n", qArr[r].choice1);
		printf("(B) %s\n", qArr[r].choice2);
		printf("(C) %s\n", qArr[r].choice3);
		printf("(D) %s\n", qArr[r].choice4);
		questCount++;
		if(scanf(" %c", &userAns) != 1){
			return 2;
		}
		time_t end = time(NULL);  //ends timer
		timer = ((double) (end - start));  //gets how many seconds it took the user to answer the question
		qArray[i] = r; //saves r to the qArray
		userAns = toupper(userAns);
		if(ans(userAns) != true){  //checks if the user input A B C or D, if user did not answer one of those, they get 0 points
			printf("Did not enter a valid option! No points are awarded!\n");
			continue;
		}
		if(((userAns == 'A')&&(strcmp(qArr[r].choice1, qArr[r].answer) == 0))||((userAns =='B')&&(strcmp(qArr[r].choice2, qArr[r].answer) == 0))||((userAns == 'C')&&(strcmp(qArr[r].choice3, qArr[r].answer) == 0))||((userAns == 'D')&&(strcmp(qArr[r].choice4, qArr[r].answer) == 0))){
			if(timer >= 30){   // if it took the user more than 30 seconds to answer the question, then they get no points
				printf("Player was awarded 0 points for going over 30 seconds.\n");
	
			}else{ //if the user did not exceed 30 points, he gets points based on how many seconds it took him, less points the longer it took
					pointMaker = (timer / 30)*1000;
					finalPoints = 1000-pointMaker;
					totalPoints += finalPoints; 
					printf("That is the correct answer! Player is awarded %d points!\n", finalPoints);
					printf("User answered in %.1f seconds and received %d points!\n", timer, totalPoints);
			}
						
		}else{ //anser wrong = no points
				printf("Wrong answer! No points are awarded.\n");
				printf("The right answer is: %s \n", qArr[r].answer);
		}
		if( i != 9){ //asks the user if they ready for the next question, stops after the last question is done
			printf("Press 'y' if you are ready for the next question.\n");
			while(true){
				if(scanf(" %c", &yCont) != 1){
					return 2;
				}
				yCont = tolower(yCont);
	
				if(yCont == 'y'){
					break;
				}else
					printf("You did not press 'y'... Try again.\n");
			}	
		}



	}
	return totalPoints;  //returns users totalPoints

}

bool searchList(int r){ //searches the list to see if the random number 'r' has been randomly chosen again, if it has, display true, if it is not in the list, it displays false

	for(int i = 0; i < 10; i++){
		if(qArray[i] == r){
			return true;
		}
	}
	return false;
}



void sort_players(Players **arr){ //sort players takes in the players arr and sorts it by the users points from highest to lowest

	Players *temp;

	for(int i = 0; i < playerLen-1; i++){
		for(int j = i+1; j < playerLen; j++){
			if((arr[i]->points) < (arr[j]->points)){
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

int search_player(char *name){  //search players has an index of -1 and updates the index of the player in the player arr if it finds the name that corresponds with the name that it took in, if it index is equal to -1, meaning it did not find the player in the list, it adds the player to the list
	int index = -1;
	
	for(int i = 0; i < playerLen; i++){
		if(strlen(name) != strlen(arr[i]->name)){
			continue;
		}
		int length = strlen(name);
		bool same = true;
		for(int j = 0; j < length; j++){
			if(tolower(name[j]) != tolower(arr[i]->name[j])){
				same = false;
				break;
			}
		}
		if(same){
			index = i;
			break;
		}
	}
	if(index == -1){
		Players *p = calloc(1, sizeof(Players));
		playerLen++;
		arr = realloc(arr, playerLen * sizeof(Players*));
		arr[playerLen-1] = p;
		strcpy(arr[playerLen-1]->name, name);
		return playerLen-1;	
	}

	return index;

}
void save_leaderboard(char *filename){  //save leaderboard saves the top 5 scores and the leaderboard layout to the file leaderboard.txt
	FILE *fp;

	fp = fopen(filename, "w");

	if(fp == NULL){
		return;
	}

	fprintf(fp,"       Leaderboard         \n");	
	fprintf(fp,"       Top 5 Scores        \n");
	fprintf(fp,"Rank  Name      Points    \n");
	
	for(int i = 0; i < 5; i++){
		if(feof(fp)){
			break;
		}
		fprintf(fp,"%-4d %-10s %-7d\n", i+1, arr[i]->name, arr[i]->points);
	}

	fclose(fp);
}


void save_players(char *filename){ //save players saves all the players and their points to the file players.txt
	FILE *fp;

	fp = fopen(filename, "w");

	if(fp == NULL){
		return;
	}

	for(int i = 0; i < playerLen; i++){
		fprintf(fp, "%s %d \n", arr[i]->name, arr[i]->points);
	}

	fclose(fp);
}

bool ans(char ans){ //bool ans takes in the users ans and displays true if the user did enter characters A, B, C, or D, displays false if they entered anything else
	ans = toupper(ans);

	if((ans == 'A') || (ans == 'B') || (ans == 'C') || (ans == 'D')){
		return true;
	}else
		return false;
}


bool users(char *name){  //bool users checks if the user is in the player arr, if not, then it displays false, if found, displays true


	for(int i = 0; i < playerLen; i++){
		if(strlen(name) != strlen(arr[i]->name)){
			continue;
		}
		int length = strlen(name);
		bool same = true;
		for(int j = 0; j < length; j++){
			if(tolower(name[j]) != tolower(arr[i]->name[j])){
				same = false;
				break;
			}
		}
		if(same){
			return true;
		}
	}
	return false;
}


void leaderboard(){ //leaderboard is used to display the leaderboard and write it to the file, displays the top 5 players
	FILE *fp;

	fp = fopen("leaderboard.txt", "w");

	if(fp == NULL){
		return;
	}
	
	
	fprintf(fp,"       Leaderboard         \n");	
	printf("       Leaderboard        \n ");
	fprintf(fp,"       Top 5 Scores        \n");
	printf("       Top 5 Scores        \n");
	fprintf(fp,"Rank  Name      Points    \n");
	printf("Rank  Name      Points    \n");
	for(int i = 0; i < 5; i++){
		if(feof(fp)){
			break;
		}
		fprintf(fp,"%-4d %-10s %-7d\n", i+1, arr[i]->name, arr[i]->points);
		printf("%-4d %-10s %-7d\n", i+1, arr[i]->name, arr[i]->points);
	}
	fclose(fp);
}

void load_players(char *filename){ //load players loads previous players that were saved in the players.txt file and adds them to the players arr
	FILE *fp;

	fp = fopen(filename, "r");

	if(fp == NULL){
		return;
	}

	while(true){
		if(feof(fp))
			break;

		Players *p = malloc(sizeof(Players));
		if(fscanf(fp, "%s %d", p->name, &p->points) != 2){
			break;
		}
		playerLen++;
		arr = realloc(arr, playerLen * sizeof(Players*));
		arr[playerLen-1] = p;
	}
	fclose(fp);
}