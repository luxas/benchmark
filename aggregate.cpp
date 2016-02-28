#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>   //for std::vector

using namespace std;


vector<string> split(const string &s, char delim);
bool strcontains(const string &str, const string &match);

int main()
{

	//RESULTS
	int 
		// CPU
		whetstone = 0,
		dhrystone = 0,
		linpacksp = 0,
		linpackdp = 0,
		liverloops = 0,

		// REAL
		compileTime = 0,

		// RAM
		l1flops = 0,
		l2flops = 0,
		ramflops = 0,
		rammips = 0,
		busspeed = 0,

		// TEMP VALUES
		tmp_rammipslo = 0,
		tmp_rammipshi = 0,
		tmp_ramflopslo = 0,
		tmp_ramflopshi = 0,
		tmp_busspeedlo = 0,
		tmp_busspeedhi = 0;

	float 

		// REAL
		tmp_compileTime = 0;


	string line = "";

	ifstream myfile ("benchmarks.log");
	if (myfile.is_open())
	{
	  while (getline (myfile, line))
	  {
	  	// Init an array and remove the first item, the name
	  	vector<string> arr = split(line, ' ');


	  	if(strcontains(line, "WHETSTONE"))
	  	{
	  		whetstone = stoi(arr[1]);
	  	} 
	  	else if (strcontains(line, "DHRYSTONE"))
	  	{
	  		dhrystone = stoi(arr[1]);
	  	}
	  	else if (strcontains(line, "LINPACKSP"))
	  	{
	  		linpacksp = stoi(arr[1]);
	  	}
	  	else if (strcontains(line, "LINPACKDP"))
	  	{
	  		linpackdp = stoi(arr[1]);
	  	}
	  	else if (strcontains(line, "LIVERLOOPS"))
	  	{
	  		liverloops = stoi(arr[1]);
	  	}
	  	else if (strcontains(line, "L1"))
	  	{
	  		int dbl1 = stoi(arr[1]),
	  			sng1 = stoi(arr[2]),
	  			dbl2 = stoi(arr[3]),
	  			sng2 = stoi(arr[4]);

	  		l1flops = dbl1 / 32 + dbl2 / 64 + sng1 / 16 + sng2 / 32;
	  	}
	  	else if (strcontains(line, "L2"))
	  	{
	  		int dbl1 = stoi(arr[1]),
	  			sng1 = stoi(arr[2]),
	  			dbl2 = stoi(arr[3]),
	  			sng2 = stoi(arr[4]);

	  		l2flops = dbl1 / 32 + dbl2 / 64 + sng1 / 16 + sng2 / 32;
	  	}
	  	else if (strcontains(line, "RAMLOW"))
	  	{
	  		int dbl1 = stoi(arr[1]),
	  			sng1 = stoi(arr[2]),
	  			dbl2 = stoi(arr[3]),
	  			sng2 = stoi(arr[4]);

	  		tmp_ramflopslo = dbl1 / 32 + dbl2 / 64 + sng1 / 16 + sng2 / 32;
	  	}
	  	else if (strcontains(line, "RAMHIGH"))
	  	{
	  		int dbl1 = stoi(arr[1]),
	  			sng1 = stoi(arr[2]),
	  			dbl2 = stoi(arr[3]),
	  			sng2 = stoi(arr[4]);

	  		tmp_ramflopshi = dbl1 / 32 + dbl2 / 64 + sng1 / 16 + sng2 / 32;
	  	}
	  	else if (strcontains(line, "MIPSLOW"))
	  	{
	  		int int1 = stoi(arr[1]),
	  			int2 = stoi(arr[2]),
	  			int3 = stoi(arr[3]);

	  		tmp_rammipslo = (int1 * 0.78 + int2 * 0.59 + int3 * 0.688) / 3;
	  	}
	  	else if (strcontains(line, "MIPSHIGH"))
	  	{
	  		int int1 = stoi(arr[1]),
	  			int2 = stoi(arr[2]),
	  			int3 = stoi(arr[3]);

	  		tmp_rammipshi = (int1 * 0.78 + int2 * 0.59 + int3 * 0.688) / 3;
	  	}
	  	else if (strcontains(line, "BUSLOW"))
	  	{
	  		tmp_busspeedlo = stoi(arr[1]);
	  	}
	  	else if (strcontains(line, "BUSHIGH"))
	  	{
			tmp_busspeedhi = stoi(arr[1]);
	  	}
	  	else
	  	{
	  		if (line != "")
	  			cout << "Ignoring line: " + line << endl;
	  	}
	  }
	  myfile.close();
	}
	else cout << "Unable to open file: benchmarks.log";


	ifstream mytime ("compiletime.log");
	if (mytime.is_open())
	{
		while (getline (mytime, line))
	  	{
	  		vector<string> arr = split(line, 'm');
	  		tmp_compileTime += stof(arr[0]) * 60 + stof(arr[1]);
	  	}
	}
	else cout << "Unable to open file: compiletime.log";
	mytime.close();

	// Convert the float to an integer
	compileTime = static_cast<int>(tmp_compileTime);
	if (compileTime <= 0)
		compileTime = 1;

	ramflops = (tmp_ramflopslo + tmp_ramflopshi) / 2;
	rammips = (tmp_rammipslo + tmp_busspeedhi) / 2;
	busspeed = (tmp_busspeedlo + tmp_busspeedhi) / 2;

	//RASPBERRY PI B PERFORMANCE
	int 
		// CPU
		comp_whetstone = 225,
		comp_dhrystone = 871,
		comp_linpacksp = 57,
		comp_linpackdp = 42,
		comp_liverloops = 62,

		// RAM
		comp_l1flops = 92,
		comp_l2flops = 36,
		comp_ramflops = 18,
		comp_rammips = 136,
		comp_busspeed = 685,

		// REAL
		comp_compileTime = 125;


	int 
		// CPU
		res_whetstone = whetstone / (float)comp_whetstone * 100,
		res_dhrystone = dhrystone / (float)comp_dhrystone * 100,
		res_linpacksp = linpacksp / (float)comp_linpacksp * 100,
		res_linpackdp = linpackdp / (float)comp_linpackdp * 100,
		res_liverloops = liverloops / (float)comp_liverloops * 100,

		// RAM
		res_l1flops = l1flops / (float)comp_l1flops * 100,
		res_l2flops = l2flops / (float)comp_l2flops * 100,
		res_ramflops = ramflops / (float)comp_ramflops * 100,
		res_rammips = rammips / (float)comp_rammips * 100,
		res_busspeed = busspeed / (float)comp_busspeed * 100,

		// REAL
		res_compileTime = comp_compileTime / (float)compileTime * 100;


	int res_cpu = (res_whetstone + res_dhrystone + res_linpacksp + res_linpackdp + res_liverloops) / 5,
		res_ram = (res_l1flops + res_l2flops + res_ramflops + res_rammips + res_busspeed) / 5,
		res_real = res_compileTime;

	int overall_result = (res_cpu + res_ram + res_real) / 3;

	ofstream json ("results.json");

	json << "{" << endl;
	json <<	"	\"overall\": \"" + to_string(overall_result) + "\"," << endl;
	json <<	"	\"cpu\": \"" + to_string(res_cpu) + "\"," << endl;
	json <<	"	\"ram\": \"" + to_string(res_ram) + "\"," << endl;
	json <<	"	\"real\": \"" + to_string(res_real) + "\"," << endl;
	json <<	"	\"subPerf\": {" << endl;
	json <<	"		\"floatingPoint_whetstone\": \"" + to_string(res_whetstone) + "\"," << endl;
	json <<	"		\"integerPerf_dhrystone\": \"" + to_string(res_dhrystone) + "\"," << endl;
	json <<	"		\"linearAlgebraSinglePoint_linpack\": \"" + to_string(res_linpacksp) + "\"," << endl;
	json <<	"		\"linearAlgebraDoublePoint_linpack\": \"" + to_string(res_linpackdp) + "\"," << endl;
	json <<	"		\"kernelDoublePrecision_liverloops\": \"" + to_string(res_liverloops) + "\"," << endl;
	json <<	"		\"L1CacheMFLOPS_l1flops\": \"" + to_string(res_l1flops) + "\"," << endl;
	json <<	"		\"L2CacheMFLOPS_l2flops\": \"" + to_string(res_l2flops) + "\"," << endl;
	json <<	"		\"RAMCacheSpeed_ramflops\": \"" + to_string(res_ramflops) + "\"," << endl;
	json <<	"		\"RAMMIPS_rammips\": \"" + to_string(res_rammips) + "\"," << endl;
	json <<	"		\"wordReadingBusSpeed_busspeed\": \"" + to_string(res_busspeed) + "\"," << endl;
	json <<	"		\"compileTimeAllBenchmarks\": \"" + to_string(res_compileTime) + "\"" << endl;
	json <<	"	}," << endl;
	json <<	"	\"rawData\": {" << endl;
	json <<	"		\"floatingPoint_whetstone\": \"" + to_string(whetstone) + "\"," << endl;
	json <<	"		\"integerPerf_dhrystone\": \"" + to_string(dhrystone) + "\"," << endl;
	json <<	"		\"linearAlgebraSinglePoint_linpack\": \"" + to_string(linpacksp) + "\"," << endl;
	json <<	"		\"linearAlgebraDoublePoint_linpack\": \"" + to_string(linpackdp) + "\"," << endl;
	json <<	"		\"kernelDoublePrecision_liverloops\": \"" + to_string(liverloops) + "\"," << endl;
	json <<	"		\"L1CacheMFLOPS_l1flops\": \"" + to_string(l1flops) + "\"," << endl;
	json <<	"		\"L2CacheMFLOPS_l2flops\": \"" + to_string(l2flops) + "\"," << endl;
	json <<	"		\"RAMCacheSpeed_ramflops\": \"" + to_string(ramflops) + "\"," << endl;
	json <<	"		\"RAMMIPS_rammips\": \"" + to_string(rammips) + "\"," << endl;
	json <<	"		\"wordReadingBusSpeed_busspeed\": \"" + to_string(busspeed) + "\"," << endl;
	json <<	"		\"compileTimeAllBenchmarks\": \"" + to_string(compileTime) + "\"" << endl;
	json <<	"	}" << endl;
	json << "}" << endl;
	json.close();

	return 0;
}

//HELPERS
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

bool strcontains(const string &str, const string &match)
{
	return str.find(match) != string::npos;
}
