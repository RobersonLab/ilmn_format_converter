/*
 * main.cpp
 *
 *  Created on: Sep 06, 2013
 *      Author: eli_lab
 */

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

const std::string PROGNAME = "ilmn_format_converter";
const std::string USAGE = "Usage: ilmn_format_converter --inFormat scarf --outFormat fastq --inFile file.txt";
const std::string VERSION = "1.0.0";
std::string inFormat = "scarf";
std::string outFormat = "fastq";
std::string inFile;

void error( std::string const& err )
{
	std::cerr << err << std::endl;
	std::exit(1);
}

void parseOptions( int argCount, char *commands[] )
{
	if (argCount != 7)
	{
		error( "Incorrect number of parameters used!\n" + USAGE );
	}

	int cmdIndex = 0;

	while (++cmdIndex < argCount)
	{
		if (std::strcmp(commands[cmdIndex], "--inFormat") == 0)
		{
			if (++cmdIndex < argCount)
			{
				if (std::strcmp(commands[cmdIndex], "fastq") == 0 || std::strcmp(commands[cmdIndex], "scarf") == 0)
				{
					inFormat = commands[cmdIndex];
				}
				else
				{
					error("--informat [" + std::string(commands[cmdIndex]) + "] not recognized!");
				}
			}
			else
			{
				error( "--inFormat used but no format specified!" );
			}
		}
		else if (std::strcmp(commands[cmdIndex], "--outFormat") == 0)
		{
			if (++cmdIndex < argCount)
			{
				if (std::strcmp(commands[cmdIndex], "fastq") == 0 || std::strcmp(commands[cmdIndex], "scarf") == 0)
				{
					outFormat = commands[cmdIndex];
				}
				else
				{
					error("--outFormat [" + std::string(commands[cmdIndex]) + "] not recognized!");
				}
			}
			else
			{
				error( "--inFormat used but no format specified!" );
			}
		}
		else if (std::strcmp(commands[cmdIndex], "--inFile") == 0)
		{
			if (++cmdIndex < argCount)
			{
				inFile = commands[cmdIndex];
			}
			else
			{
				error( "--inFile used with no value specified!");
			}
		}
		else
		{
			error("Command-line value [" + std::string(commands[cmdIndex]) + "] not recognized!");
		}
	}

	if (inFormat.empty() || outFormat.empty() || inFile.empty())
	{
		error("Please specify --inFormat, --outFormat and --inFile");
	}

	if (inFormat == outFormat)
	{
		error("--inFormat and --outFormat are the same! Nothing to do.");
	}

	std::cerr << std::string("--inFormat: ") << inFormat << std::endl;
	std::cerr << std::string("--outFormat: ") << outFormat << std::endl;
	std::cerr << std::string("--inFile: ") << inFile << std::endl;
}

int main (int argc, char *argv[])
{
	unsigned long long int readCount = 0;
	
	std::cerr << PROGNAME << " v" << VERSION << std::endl;
	
	parseOptions( argc, argv );

	clock_t tstart = std::clock();

	std::ifstream INFH( inFile.c_str(), std::ios::in);
	
	if (!INFH.good())
	{
		error("Trouble opening input file [" + inFile + "]");
	}

	std::string buffer;

	if (inFormat == std::string("scarf"))
	{
		while (!INFH.eof())
		{
			std::getline( INFH, buffer);

			if (!buffer.empty())
			{
				++readCount;

				size_t search_pos;
				search_pos = buffer.find_last_of( std::string(":") );

				if (search_pos == std::string::npos)
				{
					error("Couldn't find colon scarf delimiter!");
				}

				std::string qual = buffer.substr(search_pos+1);

				buffer = buffer.erase(search_pos);

				search_pos = buffer.find_last_of(std::string(":"));

				if (search_pos == std::string::npos)
				{
					error("Couldn't find colon scarf delimiter before qualities!");
				}

				std::string seq = buffer.substr(search_pos+1);
				buffer = buffer.erase(search_pos);

				std::cout << std::string("@") << buffer << std::string("\n") << seq << std::string("\n+\n") << qual << std::string("\n");
				buffer.clear();
			}
		}
	}
	else if (inFormat == std::string("fastq"))
	{
		short int fqLine = 0;
		std::string id;
		std::string seq;

		while (!INFH.eof())
		{
			std::getline( INFH, buffer );

			if (!buffer.empty())
			{
				if (++fqLine == 1)
				{
					id = buffer.substr(1);
				}
				else if (fqLine == 2)
				{
					seq = buffer;
				}
				else if (fqLine == 4)
				{
					++readCount;

					std::cout << id << std::string(":") << seq << std::string(":") << buffer << std::string("\n");

					// reset
					fqLine = 0;
					id.clear();
					seq.clear();
				}

				buffer.clear();
			}
		}
	}

	INFH.close();

	clock_t tend = std::clock();

	std::cerr << readCount << std::string(" reads processed") << std::endl;
	std::cerr << std::fixed;
	std::cerr << std::string("Completed in ") << std::setprecision(3) << (static_cast<float>(tend)-static_cast<float>(tstart)) / CLOCKS_PER_SEC << std::string(" seconds") << std::endl;
	std::exit(0);
}
