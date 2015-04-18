/*
*		Written by David Harkins

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


#include <iostream> //The library for console interfacing. 
#include <fstream> //Library for file interfacing.

//Global debug variables.
std::ofstream DebugLog;

//The function to find the GCD
int Divide(int divisor, int dividend){

	//Error handling.
	if (DebugLog.is_open() == false){ 
		std::cout << "Error - File is not opened." << std::endl;
		return -1; 
	}	
	
	DebugLog << "--------------------------------------------------------------------------------------\n"; //Log it.
	DebugLog << "Starting new GCD, divisor is " << divisor << " and the dividend is " << dividend << ".\n"; //Log it.


	//force the divisor is smaller then the dividend.
	if (divisor > dividend){
		//We switch them.
		

		int Reg = divisor; //Hold the variable.

		divisor = dividend;		
		dividend = Reg;
				
		DebugLog << "Switching the divisor and dividend.\n"; //Log it.
	}
		

	int remainder = -1; //Holds the remainder for mod.

	int Debug_Loop_Count = 1; //Debugging, contains how many times the loop has cycled.

	while(remainder != 0){
		remainder = dividend % divisor; // dividend MOD divisor = remainder.
		
		if (remainder != 0){
			//continue with the search. Move the divisor to the dividend so we can divide it by the remainder.
			dividend = divisor;
			divisor = remainder;
		}

		DebugLog << "Cycle " << Debug_Loop_Count << ": " << dividend << " Mod " << divisor << " = " << remainder << ".\n"; //Log it.

		Debug_Loop_Count++; //Increase the cycle count.
	}
	
	return divisor; //Return the GCD
}

//A simple function to call GCD and log the results.
void GCD(int divisor, int dividend){

	//Find the GCD.
	int GCD = Divide(divisor, dividend);

	//Log it.
	DebugLog << "The GCD must be " << GCD << ".\n"; //Log it.
}

int main(){

	//Open the file for debug output.
	DebugLog.open("DebugLog.txt");

	//Calculate the GCD from the work sheet.
	GCD(116, 84);
	GCD(85, 65);
	GCD(72, 26);
	GCD(72, 25);

	//Close the file.
	DebugLog.close(); 

	return 0;
}
