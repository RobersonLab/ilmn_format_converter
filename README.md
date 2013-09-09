ilmn_format_converter
=====================

## Purpose
This program was designed to rapidly interconvert Illumina SCARF and FASTQ formats. There is a substantial speed improvement (important in some of my reprocessing efforts) by moving this to a C++ program over the Perl / Python alternatives. Log messages print to the error stream and the output prints to standard output. See the included license file for terms of use.

**VERY IMPORTANT WARNING** This interconversion can only safely be performed on files with Illumina quality offset (QUAL+64).

## Compilation

### Linux
Edit the Makefile to include any necessary changes, such as -m64 and -mtune parameters. It already compiles at -O3 and -Wall warnings.

    make
    make install

### Windows
This software can be compiled with a properly setup Eclipse or other IDE. Otherwise it can be compiled manually.

    g++ -o ilmn_format_converter -O3 main.cpp

## Running
Viable formats for in and out are only 'scarf' and 'fastq', all lowercase.

    ilmn_format_converter --inFormat scarf --outFormat fastq --inFile myScarf.txt 1>output.fq 2>scarf2fq.log
	
    ilmn_format_converter --inFormat fastq --outFormat scarf --inFile output.fq 1>remadeScarf.txt 2>fq2scarf.log
