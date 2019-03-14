#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <ctype.h>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_Sequence;
	static bool processGenome(string &curSequence); //checks for validity and fixes capitalization

};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_Sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
	string curTitle;
	string curSequence;
	string temp;
	genomes.clear();

	getline(genomeSource, temp);
	if (temp.size() <= 1 || temp[0] != '>' || !genomeSource)
		return false;
	else
		curTitle = temp;
	curTitle.erase(0, 1);
	int line = 1;
	do
	{
		line++;
		getline(genomeSource, temp);
		if (temp.size() > 0 && temp[0] == '>') //Start of new Genome detected
		{
			if (temp.size() == 1 || curSequence.size() == 0 || !genomeSource)
				return false;
			temp.erase(0, 1); //remove the '>' symbol

			//Process curSequence to check for validity and correct capitalization
			if (!processGenome(curSequence))
				return false;
			genomes.push_back(Genome::Genome(curTitle, curSequence));
			curTitle = temp;
			curSequence = "";


		}
		else if (temp.size() > 0)
		{
			curSequence += temp;
		}
		else if (!genomeSource)
			break;
		else
			return false;

	} while (genomeSource); //Should end loop once end of file is reached?

	if (!processGenome(curSequence))
	{
		return false;
	}
	genomes.push_back(Genome::Genome(curTitle, curSequence));
	return true;
}

int GenomeImpl::length() const
{
    return m_Sequence.size();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
	return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (!((position + length) <= m_Sequence.size() && position >= 0 && length >= 0)) //If the extracted string is invalid, do nothing and return false
	{
		return false; 
	}
	fragment = "";
	for (int i = position; i < (position + length); i++)
	{
		fragment += m_Sequence[i];
	}
	return true;

   
}

bool GenomeImpl::processGenome(string &curSequence)
{
	for (int i = 0; i < curSequence.size(); i++)
	{
		switch (curSequence[i])
		{
		case 'a':
		case 'c':
		case 't':
		case 'g':
		case 'n': curSequence[i] = toupper(curSequence[i]);
			break;
		case 'A':
		case 'C':
		case 'T':
		case 'G':
		case 'N': break;

		default: return false;
		}
	}
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
