/*
*				Written by David Harkins

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <vector>

#include <math.h>       /* pow */

#include <fstream> //Library for file interfacing.

//Global debug variables.
std::ofstream DebugLog;

class Swanberg_2{

	void ConvertToIntStack(std::string Number, std::vector<int> &Stack, int &decimalplace){

		for (int i = 0; i < Number.size(); i++){
			if (Number[i] == '.'){ decimalplace = i; }
			else{
				Stack.push_back(((int)Number[i] - '0'));
			}
		}
	}

	void Output_In_Root(int Doubled, int third_term, std::vector<int> Answer, int current){
	
		if (Doubled == '*'){
			//first term
			DebugLog << "(X * X) <= Current Value\n";
			DebugLog << "(" << third_term << "*" << third_term << ") <= " << current << "\n";
		}else{
			DebugLog << "((Doubled + X) * X) <= Current Value\n";
			DebugLog << "((" << Doubled << " + " << third_term << ")*" << third_term << ") <= " << current << "\n";
		}		
	}

public:
	void SquareRoot(std::string Number){

		DebugLog << "--------------------------------------------------------------------------------------\n"; //Log it.
		DebugLog << "Find the square root of " << Number << "\n";
		
		int Accuracy = 5; //5 is about the limit of int. Because of the growth of the numbers so I just truncate at 5.

		std::vector<int> Answer; //The square root.

		int decimalplace = -1; //If the input has a decimal place.
		std::vector<int> Stack; //Input stack.		

		ConvertToIntStack(Number, Stack, decimalplace); //Convert std::string to int.

		int StackSize = Stack.size();

		int Current = 0; //The current value of the decimal
		int Starting_Index = 0; //The offset based on if there is a even amount of digits.

		//Find if there is a even amount of terms.
		if (Stack.size() % 2 == 0){
			//Even
			Current = (Stack[0] * 10) + (Stack[1]); //Take the first two terms. 
			Starting_Index = 2;
		}else{
			//Odd
			Current = (Stack[0]); //Take the first term.
			Starting_Index = 1;
		}
		
		
		//Find X * X <= Current
		int bottom = 0; //The storage X for X * X

		for (int g = 0; g <= Current; g++){
			if ((g * g) <= Current){
				bottom = g;
			}else{
				break;
			}
		}

		Answer.push_back(bottom); //Add to the stack.

		Output_In_Root('*', bottom, Answer, Current); //Log it.
		
		//Subtract the X^2 from the current value.
		Current = Current - (bottom * bottom);

		//Fix the stack if the input is too small.
		if (Starting_Index >= Stack.size()){
			Stack.push_back(0);
			Stack.push_back(0);
		}

		//Update current to the next one to divide out.
		//Move the current value over 2 decimal places and put the next two terms in.
		Current = (Current * 100) + (Stack[Starting_Index] * 10) + (Stack[Starting_Index + 1]);

		//The first answer we double.
		int Doubled = bottom * 2;

		//This holds the next term x for (Doubled + x) * x.
		int third_term = 0;
		

		for (int Accuracy_Loop = 0; Accuracy_Loop < Accuracy; Accuracy_Loop++){
			
			third_term = 0; //Reset

			//find the number to add to the doubled.
			for (int i = 0; i < 9; i++){
				if ((((Doubled * 10) + i) * i) <= Current){
					third_term = i;
				}else{
					break;
				}
			}

			Answer.push_back(third_term); //Add to the stack.


			Output_In_Root(Doubled, third_term, Answer, Current); //Log it.

			//Subtract it
			Current = Current - (((Doubled * 10) + third_term) * third_term);

			Starting_Index += 2; //Move two down the stack.
			//Fix the stack.
			if (Starting_Index >= Stack.size()){
				Stack.push_back(0);
				Stack.push_back(0);
			}

			//Update current to the next one to divide out.
			Current = (Current * 100) + (Stack[Starting_Index] * 10) + (Stack[Starting_Index + 1]);

			//Create the doubled.
			Doubled = 0; //Reset
			int Power_Count = 0;

			for (int i = (Answer.size() - 1); i >= 0; i--){
				Doubled += (Answer[i] * pow(10, (Answer.size() - (i + 1)))); //The part of the answer + 10E(decimal place)
			}

			Doubled *= 2; //Double it.
		}

		//Drop the zeros from the stack.
		for (int i = Stack.size() - 1; i > 0; i--){
			if (Stack[i] == 0){
				Stack.erase(Stack.begin() + i);
			}else{
				break;
			}
		}

		//Drop the zeros from the answer.
		for (int i = Answer.size() - 1; i > 0; i--){
			if (Answer[i] == 0){
				Answer.erase(Answer.begin() + i);
			}
			else{
				break;
			}
		}

		int point = -1; //Where the decimal point goes.

		if (decimalplace == -1){

			//Place the decimal point
			if (StackSize % 2 == 0){
				point = StackSize / 2;
			}
			else{
				point = (StackSize + 1) / 2;
			}
		}
		else{
			if (decimalplace % 2 == 0){
			point = decimalplace / 2;
			}
			else{
				point = (decimalplace + 1) / 2;
			}
		}

		//special case where input < 1
		if (decimalplace == 1 && Stack[0] == 0){
			point = 1;
		}
			

		DebugLog << "Answer: "; //Log it.
		//Print the answer to the log.
		for (int i = 0; i < Answer.size(); i++){
			if (i == (point)){
				DebugLog << '.'; //Log it.
			}

			DebugLog << Answer[i]; //Log it.
		}
		DebugLog << "\n\n";
	}

};


int main(){

	DebugLog.open("DebugLog.txt");

	DebugLog << "-------- Written by David Harkins --------\n\n"; //Log it.
	DebugLog << "Please note: If a answer seems to be truncated it is because the value grew to quickly so it not able to continue with the limitations of int.\n"; //Log it.

	Swanberg_2 a;

	a.SquareRoot("152.2756");
	a.SquareRoot("2");
	a.SquareRoot("69");

	//Close the file.
	DebugLog.close();

	return 0;
}
