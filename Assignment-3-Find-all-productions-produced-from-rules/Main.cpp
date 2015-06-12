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
#include <fstream>

std::ofstream Global_Logging_File; //Used for logging.

	//Stores the production rule (I.e. "S->Abd").
struct Rule{
	std::string Input;		//The input of the rule "S".
	std::string Output;		//The output of the rule "Abd".

		//Default constructor with the rule passed as a argument. Split at the -> and then fill input and output.
	Rule(std::string argument){
				
		unsigned int Split = 0;
		for (unsigned int i = 0; i < argument.size() - 1; i++){ if (argument[i] == '-' && argument[i + 1] == '>'){ Split = i; break; } }

		for (unsigned int i = 0; i < Split; i++){
			Input += argument[i];
		}

		for (unsigned int i = Split + 2; i < argument.size(); i++){
			Output += argument[i];
		}
	}
};


//Takes input of the starting point and then handles recursive tracing of the combinations.
class Combined{

		//The starting point.
	private: std::string Root; 

		//Default constructor.
	public: Combined(std::string Argument){
		Root = Argument;
	}
		//Start the trace.
	public: void Process(std::vector<Rule> &InputRules, int Depth){
		Find_Combined(InputRules, Depth, Root);
	}

	public: void Find_Combined(std::vector<Rule> &InputRules, int Depth, std::string Root_Input, std::string Trace = ""){

		bool found = false; //If we are unable to find a combination then it must be a endpoint.

			//Go though everypoint on the input.
		for (unsigned int i = 0; i < Root_Input.size(); i++){

				//Check it against every rule.
			for (unsigned int g = 0; g < InputRules.size(); g++){
					//Check if the rule matches.
				if (InputRules[g].Input[0] == Root_Input[i]){

					std::string New_Root = ""; //The new output.

					for (unsigned int split = 0; split < i; split++){
						New_Root += Root_Input[split];
					}

						//Handle null 
					if (InputRules[g].Output[0] != '*'){
						New_Root += InputRules[g].Output;
					}

					for (unsigned int split = i + 1; split < Root_Input.size(); split++){
						New_Root += Root_Input[split];
					}

						//Add to trace.
					if (Depth == 0){
						Trace = Root_Input;
						Trace += "->";
						Trace += New_Root;
					}
					else{
						Trace += "->";
						Trace += New_Root;
					}

						//Cut off after 7 levels of depth.
					if (Depth <= 7){
						Find_Combined(InputRules, ++Depth, New_Root, Trace); //Trace the newer stack.
					}
					else{
							//Log the end point.
						std::cout << Trace << std::endl;
						Global_Logging_File << Trace << std::endl;
					}
					found = true;
				}
			}
		}

		if (found == false){
				//Log the end point.
			std::cout << Trace << std::endl;
			Global_Logging_File << Trace << std::endl;
		}
	}
};





int main(){

		//Open logging file.
	Global_Logging_File.open("Output.txt");

		//The list of rules.
	std::vector<Rule> RuleList;

		//Add the rules.
	Rule New1("S->abS");
	Rule New2("S->bcS");
	Rule New3("S->bbS");
	Rule New4("S->a");
	Rule New5("S->cb");
		
	RuleList.push_back(New1);
	RuleList.push_back(New2);
	RuleList.push_back(New3);
	RuleList.push_back(New4);
	RuleList.push_back(New5);

	Combined Ghosts("S"); //Create a instance of the trace class.

		//Process the rule list.
	Ghosts.Process(RuleList, 0);

		//Close logging file.
	Global_Logging_File.close();

	return 0;
}
