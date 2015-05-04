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


#include <iostream> //The library for console interfacing. 
#include <fstream> //Library for file interfacing.

//Global debug variables.
std::ofstream DebugLog;

//Basically I messed around with the algorithm to figure out how to enter the data forwards
//I noticed that it repeats so STRUCTURE TIME BABE!

struct Mitchell{
	//(Term1 - Multiplier * (Term2))

	//Pointer to link
	Mitchell *Term2_Pointer = NULL;

	//The values
	int Term1 = -1;
	int Term2 = -1;

	//Neat.
	int Multiplier = -1;

	void Push(int arg_Term1, int arg_Term2, int arg_Multiplier){
		//Check if the structure has been used or not.
		if (Term1 == -1){
			Term1 = arg_Term1;
			Term2 = arg_Term2;

			Multiplier = arg_Multiplier;
		}
		else{
			//The structure is already full so we create a new one.
			if (Term2_Pointer == NULL){ Term2_Pointer = new Mitchell; }

			//Push the values into the new structure.
			Term2_Pointer->Push(arg_Term1, arg_Term2, arg_Multiplier);
		}
	}

	//Tracing will result in two numbers but they are not complete.
	//The numbers need to be reduced and then a negative assigned.
	void Validate(int &X, int &Y, int divisor, int dividend){

		//Reduce.
		if (X % divisor == 0){
			X = X / divisor;
			Y = Y / dividend;
		}else{
			X = X / dividend;
			Y = Y / divisor;
		}
		DebugLog << "Reduced: X = " << X << " and Y = " << Y << "\n"; //Log it.

		//Fixing a bug where both can be divided by the divisor.
		if (((X * divisor) - (Y * dividend)) > ((Y * divisor) - (X * dividend))){
			int debug = 0;

			int temp = X;
			X = Y;
			Y = temp;

		}

		//Choose the sign.
		//if( AX - BY > 0)
		if (((X * divisor) - (Y * dividend)) > 0){
			Y *= -1;
		}
		else{
			X *= -1;
		}

		DebugLog << "Sign change: X = " << X << " and Y = " << Y << "\n"; //Log it.
	}

	//Trace the stack and solve X and Y (version two)
	int Trace_2(int &X, int &Y, int divisor, int dividend, bool first = false){

		//X and Y flip signs.
		int Switchcase = 0;

		//Go to the end of the stack.
		if (Term2_Pointer->Term2_Pointer != NULL){
			Switchcase = Term2_Pointer->Trace_2(X, Y, divisor, dividend);
		}

		//Are we the first term?
		if (X == 0){
			X += Term1;
			Y += Term2 * Multiplier;

			return 0;
		}

		//Do some amazing math based on signs.
		if (Switchcase == 0){

			//Less then amazing logging.
			DebugLog << "Switchcase 0: Y = " << Y << " / " << Term2_Pointer->Term2 << "\n"; //Log it.
			DebugLog << "Switchcase 0: X += " << Y << " * " << Multiplier << " * " << Term2 << "\n"; //Log it.
			DebugLog << "Switchcase 0: Y = " << Y << " * " << Term1 << "\n"; //Log it.

			Y = Y / Term2_Pointer->Term2;
			X += Y * Multiplier * Term2;
			Y = Y * Term1;

			DebugLog << "Switchcase 0: Y = " << Y << " and X = " << X << "\n"; //Log it.

			//If done, validate.
			if (first){ Validate(X, Y, divisor, dividend); }

			return 1;
		}
		else{

			//Less then amazing logging.
			DebugLog << "Switchcase 1: X = " << X << " / " << Term2_Pointer->Term2 << "\n"; //Log it.
			DebugLog << "Switchcase 1: Y += " << X << " * " << Multiplier << " * " << Term2 << "\n"; //Log it.
			DebugLog << "Switchcase 1: X = " << X << " * " << Term1 << "\n"; //Log it.

			X = X / Term2_Pointer->Term2;
			Y += X * Multiplier * Term2;
			X = X * Term1;

			DebugLog << "Switchcase 1: Y = " << Y << " and X =" << X << "\n"; //Log it.

			//If done, validate.
			if (first){ Validate(X, Y, divisor, dividend); }

			return 0;
		}

		return 0;
	}


	~Mitchell(){
		//Clean up.
		delete Term2_Pointer;
	}
};

//The function to find the GCD
int Divide(int divisor, int dividend, Mitchell &TraceStack){

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

	int Debug_Loop_Count = 0; //Debugging, contains how many times the loop has cycled.

	while (remainder != 0){

		remainder = dividend % divisor; // dividend MOD divisor = remainder.

		DebugLog << "Cycle " << Debug_Loop_Count << ": " << dividend << " Mod " << divisor << " = " << remainder << ".\n"; //Log it.

		//Mitchell structure: Set the values in.

		TraceStack.Push(dividend, divisor, (dividend / divisor));

		if (remainder != 0){
			//continue with the search. Move the divisor to the dividend so we can divide it by the remainder.
			dividend = divisor;
			divisor = remainder;


		}

		Debug_Loop_Count++; //Increase the cycle count.
	}

	return divisor; //Return the GCD
}

//A simple function to call GCD and log the results.
void GCD(int divisor, int dividend){

	Mitchell TraceStack; //The structure for us to trace back to X and Y.

	//Find the GCD.
	int GCD = Divide(divisor, dividend, TraceStack);

	//Log it.
	DebugLog << "The GCD must be " << GCD << ".\n"; //Log it.

	//Back trace Mitchell to find ax + by = d.
	int X = 0;
	int Y = 0;

	DebugLog << "Starting new trace to find " << divisor << "X + " << dividend << "Y = " << GCD << "\n"; //Log it.

	//Trace to find X and Y.
	TraceStack.Trace_2(X, Y, divisor, dividend, true);

	//Log it.
	DebugLog << "Trace Complete: (" << divisor << ")(" << X << ") + (" << dividend << ")(" << Y << ") = " << GCD << "\n"; //Log it.
}

int main(){

	std::cout << "Written by David Harkins.";

	//Open the file for debug output.
	DebugLog.open("DebugLog.txt");

	DebugLog << "-------- Written by David Harkins --------\n\n"; //Log it.

	//Calculate the GCD from the work sheet.
	GCD(116, 84);
	GCD(85, 65);
	GCD(72, 26);
	GCD(72, 25);

	//Close the file.
	DebugLog.close();

	return 0;
}
