#ifndef ASSIGNMENT_CONFIG_H
#define ASSIGNMENT_CONFIG_H

#include <string>
#include <vector>

using namespace std;


struct DirectoryConfig
{
    string tests;
    string actual;
    string results;
};


struct AlgorithmConfig
{
    string name;
    string driver_name;
    DirectoryConfig directories;
};


struct AssignmentConfig
{
    string name;
    string readme;
    string dirname;
    vector<AlgorithmConfig> algorithms;
};

#endif