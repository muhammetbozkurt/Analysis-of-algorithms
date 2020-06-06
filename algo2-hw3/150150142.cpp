/*
Muhammet Bozkurt
150150142
*/


/*
SuiteScheduler suites("file_name"); reading data and prepare data

suites.opt(); this method use knapsack algo to optimize test suite selection problem
results of this mothod keeps in res atribute of object

TestSuite suite(...); it is not used in main explicitly however  when creation phase of TestSuite
object it is called and espacially prepare frequencies.

suite.new_order() firstly calls prioritize_most_significant method to put test case with highest 
coverage at the top of the vector. after that, ordered_sequence_cretor and ordered_sequence (vector) is created.
at the end find all distance values relative to first one and sort them

*/
#include "SuiteScheduler.h"

using namespace std;

void part1_result_printer(SuiteScheduler &suites);
void part2_result_printer(SuiteScheduler& suites);

int main(int argc, char const *argv[]){

    SuiteScheduler suites(argv[1]);    
    part1_result_printer(suites);
    part2_result_printer(suites);
    
    return 0;
}

void part1_result_printer(SuiteScheduler &suites){
    cout<<"\t\tPart 1"<<endl;
    
    time_t start, end;
    start = clock();
    
    suites.opt();

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    suites.printRes();
    cout << "Time : " << fixed << time_taken << setprecision(5)<<endl;
    cout<<"\n*******************************\n"<<endl;
}

void part2_result_printer(SuiteScheduler& suites){
    int result_size;
    double total_time = 0;
    time_t start, end;
    result_size = suites.result.size();
    cout<<"\t\tPart 2"<<endl<<endl;
    for(int i=0;i<result_size;i++){
        TestSuite temp = suites.result[i];// just to make it more readable
        cout<<endl<<"Old Order: "<<temp.get_testId()<<endl<<"\t";
        for(int k =0; k<temp.freqs.size(); k++){
            for(int j=0; j<temp.freqs[k].size();j++)
                cout<<temp.freqs[k][j]<<" ";
            cout<<endl<<"\t";
        }
        start = clock();
        temp.new_order();
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout<<endl << "Time for ordering test cases of "<<temp.get_testId()<<": " << fixed << time_taken << setprecision(5)<<endl;

        cout<<endl<<"New Order: "<<temp.get_testId()<<endl<<"\t";
        for(int k =0; k<temp.freqs.size(); k++){
            for(int j=0; j<temp.freqs[k].size();j++)
                cout<<temp.freqs[k][j]<<" ";
            cout<<endl<<"\t";
        total_time += time_taken;
        }
        
    }
    cout<<endl << "Total Time of ordering test cases of all selected test suites: " << fixed << total_time << setprecision(5)<<endl;
}
