#include "pch.h"
using namespace std;
struct berResults
{
    double tot; 
    double err; 
    float ber;  
    clock_t t1; 
    clock_t t2; 
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2);
void createFile1(const string name, const int count, const char value);
berResults calculateBer(string fpath1, string fpath2);
void printResult(berResults results);

int main(int argc, char* argv[])
{
    string fpath1; 
    string fpath2; 
    berResults results;

    openLog("log.log"); 
    if (argc != 3) 
    {
        saveLog("User did not provide path to the files.");
        saveLog("Creating the test files for user...");

        bool powtorzenie = true;


        while (powtorzenie)
        {
            
            printf("\n, wybierz od 1 do 3  \n\n1.Test 1 \n2.Test 2 \n3.Test 3 \n\n 4. Wyjscie ");
            int wybierz;
            cin >> wybierz;
            switch (wybierz) {

            case 1:
            {
                

                createFile1("test1_file1.bin", 100, 0x55);
                createFile1("test1_file2.bin", 100, 0x55);
                results = calculateBer("test1_file1.bin", "test1_file2.bin");
                printResult(results);
                break;
            }
            
            case 2:
            {
                createFile1("test2_file1.bin", 100, 0xFF);
                createFile1("test2_file2.bin", 100, 0xFE);
                results = calculateBer("test2_file1.bin", "test2_file2.bin");
                printResult(results);
                break;
            }

            case 3:
            {
                
                createFile1("test3_file1.bin", 400000000, 0x55);
                createFile1("test3_file2.bin", 400000000, 0x50);
                results = calculateBer("test3_file1.bin", "test3_file2.bin");
                printResult(results);
                break;
            }

            case 0:
            {
                
                powtorzenie = false;
                break;
            }
            }
        }

        saveLog("Test files are prepared");
        saveLog("Re-run with correct arguments ie: ./task_iv_ber.exe test1_file1.bin test1_file2.bin");
    }
    else 
    {
        fpath1 = argv[1];
        fpath2 = argv[2];

        saveLog("Processing test files");
        results = calculateBer(fpath1, fpath2);
        printResult(results);
    }

    closeLog();
    return 0;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2; 
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}


void createFile1(const  string name, const int count, const char value)
{
    fstream f;
    f.open(name.c_str(), ios::binary | ios::out);
    for (int i = 0; i < count; i++)
    {
        f.write((char*)&value, 1);
    }
    f.close();
}


berResults calculateBer(string fpath1, string fpath2)
{
    fstream f1, f2; 
    berResults results;
    results.t1 = 0;
    results.t2 = 0;
    results.ber = 0;
    results.err = 0;
    results.tot = 0;

    saveLog("Calculating BER...");
    f1.open(fpath1.c_str(), ios::binary | ios::in);
    f2.open(fpath2.c_str(), ios::binary | ios::in);
    char a = 0x00;
    char b = 0x00;
    results.t1 = clock();

    while (!f1.eof())
    {
        f1 >> a; 
        f2 >> b; 
        if (!f1.eof()) 
        {
            results.err += hammingDistance(a, b); 
            results.tot += 8; 
        }
    }

    results.ber = (float)results.err / results.tot; 
    results.t2 = clock();
    saveLog("BER calculations are done");
    return results; 
}

void printResult(berResults results)
{
    stringstream message;
    message << "Results are: " << endl;
    message << "BER: " << results.ber << endl;
    message << "Tot: " << results.tot << endl;
    message << "Err: " << results.err << endl;
    message << "Calc time: " << ((float)results.t2 - results.t1) / CLOCKS_PER_SEC << " sec " << endl;
    saveLog(message.str());
}