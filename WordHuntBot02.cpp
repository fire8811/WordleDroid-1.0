//DRAFT 02: GUESSES CORRECTLY IN ~6 TRIES OR LESS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <array>

using namespace std;

vector<string> words; //word list of 5 letter words
vector<char> correctChars; //correct letters that are found but in the wrong spot
vector<char> wrongChars; //letters that the target word does not have
char perfectChars[5] = {'_', '_', '_', '_', '_'}; //letters that are in the right spot, starting as blank spaces
vector<string> wrongWords;


bool checkIfWord(string wordToCheck);
string randomWord();
bool checkUniqueChar(char letter, vector<char> chars);
bool allLettersFound();
void tossBadLetters(string word);
bool isPerfectCharsBlank();
bool hasCorrectChars(string word);
bool hasWrongChars(string word);

int main(int argc, char *argv[]) {
	srand(time(0));
	string fileWord;
	string target;
	string guessWord;
	int attempts = 0;
	
	ifstream ReadFile("word_list.txt");
	
	while(getline(ReadFile, fileWord)){ //insert contents of word list into vector words
		words.push_back(fileWord);
	}
	
	
	cout << "Enter target word: " << endl;
	cin >> target;
	
	if(!checkIfWord(target)) cout << "Not in word list! Restart Program!" << endl; //check if target word is in the word list
	
	
	//now for the meat of the program
	
	
	guessWord = randomWord(); //initial guess by the computer (random word from word list)
	cout << "Initial Guess: " << guessWord << endl;
	
	
	while(!(allLettersFound())){ //loop until the guessed word matches target word
		
		if(attempts > 0) { 
			cout << "Now Trying: ";
			guessWord = randomWord(); //guess new word after the first attempt
			cout << guessWord << endl;
		}
		
		for(int i = 0; i < target.size(); i++){ //iterate through the target's letters...
			for(int j = 0; j < guessWord.size(); j++){ //...as well as the guess's letters to find match
				
				if(target[i] == guessWord[j]){ //if match is found
					if(checkUniqueChar(target[i], correctChars)) correctChars.push_back(target[i]); //add correct letter to correctChars if not already added
					if(i == j) perfectChars[i] = guessWord[i]; //if letter is in correct spot add it to it's spot in perfectChars
				}
			}
		}
		
		tossBadLetters(guessWord); //record all letters that are not in target word (so they can be ignored in subsequent guesses)
		
		
		
		cout << "Progress: " << endl;
		
		if(correctChars.empty()){ //if no correct letters have been found:
			cout << "None. Trying new word..." << endl;
		}
		
		else{ //correct letters have been found!
			cout << "Found correct letters "; 
			for(int i = 0; i < correctChars.size(); i++){
				cout << correctChars.at(i) << " "; //print them out
			}
			cout << endl;
		
			cout << "Letters in correct spot: ";
			for (int i = 0; i < 5; i++){
				cout << perfectChars[i] << " ";
			}
			cout << endl;
		}
		
		
		attempts = attempts + 1;
		cout << "number of attempts: " << attempts << endl;
	}
}

//FUNCTIONS 

bool checkIfWord(string wordToCheck){ //check if an input is in the word list (vector words)
	for (int i = 0; i < words.size(); i++) { //loop thru word list and compare wordToCheck to find match
		if(words.at(i) == wordToCheck){
			return true;
		}
	}
	return false; //no match found
}


string randomWord(){ //use random integer to pick out of vector words and return it
	int wordNum = rand() % words.size(); 
	string newWord = words.at(wordNum);
	//cout << "NEW WORD " << newWord << endl;
	
	if(hasWrongChars(newWord)) newWord = randomWord(); //if guess has any letters marked as incorrect, guess a new word

	if(correctChars.size() >= 1){ 
		//cout << "CHAR > 1" << endl;
		if(!isPerfectCharsBlank()){
			//cout << "PCHARS NOT BLANK" << endl;
			for(int i = 0; i < 5; i++){
				if((perfectChars[i] != '_') && (perfectChars[i] != newWord[i])){ //if the newWord does not have a letter in the same spot as a perfect letter, guess a new word
					//cout << "MISSING PERFECT CHAR " << newWord << endl;
					newWord = randomWord();  
				}
			}
		}
		
		if(!hasCorrectChars(newWord)){ //if newWord doesn't have all correct characters guess a new word
			//cout << "MISSING ALL CORRECT CHARS " << newWord << endl;
			newWord = randomWord();
		}
	}
	
	return newWord;
}

bool checkUniqueChar(char letter, vector<char> chars){ //check to see if a letter is not already in the correct char list
	for (int i = 0; i < chars.size(); i++){
		if (letter == chars.at(i)){ //char already in vector, return false to ignore
			return false;
		}
	}
	return true; //letter is unique
}

bool allLettersFound(){ //check if all correct letters have been found
	for(int i = 0; i < 5; i++){
		if(perfectChars[i] == '_') return false;
	}
	
	return true;
}
	
bool isPerfectCharsBlank(){
	for (int i = 0; i < 5; i++){
		if(perfectChars[i] != '_'){
			return false;
		}
	}
	
	return true;
}

bool hasCorrectChars(string word){ //checks if a word has the required characters that are correct
	bool letterMatchFound = false;
	
	for(int i = 0; i < correctChars.size(); i++){ //iterate through correctChars
		letterMatchFound = false; //each step the flag for a correct match is set to false for each letter/char
		for(int j = 0; j < 5; j++){
			if(correctChars.at(i) == word[j]){ //if a match is found it's switched to true
				letterMatchFound = true;
			}
		}
		if (!letterMatchFound){ //if at the end of looping through letters of word no match is found break out of loop and return false (word doesn't have a correct letter)
			break;
		}
	}
	
	return letterMatchFound;
}

void tossBadLetters(string word){
	bool letterMatch = false;
	
	for(int i = 0; i < word.size(); i++){
		for(int j = 0; j < correctChars.size(); j++){
			if(word[i] == correctChars.at(j)){ //if letter in word is a correct letter in the correct vector
				letterMatch = true; //flag it as a match
			}
		}
		
		if(!letterMatch) wrongChars.push_back(word[i]); //if no match is flagged add that letter in word to the bad letter vector
		letterMatch = false; //reset flag
	}
	
	/* scout << "WRONG CHARS: ";
	for(int i = 0; i < wrongChars.size(); i++){
		cout << wrongChars.at(i) << " ";
	}
	cout << endl; */
	
}

bool hasWrongChars(string word){ //see if a word has any letters that are marked incorrect
	
	for(int i = 0; i < word.size(); i++){
		for(int j = 0; j < wrongChars.size(); j++){
			
			if (word[i] == wrongChars.at(j)){
				return true;
			} 
		}
	}
	return false;
}
