//==============================================================================
// Last updated 30/01/2018
// The code in this file will generate the Cell RS mapping.
//
// MappingInfo is a simple class that stores that the ofdm symbol number
// within the slot and the subcarrier to define an RE that will contain a Cell RS.
// It also stores the slot index (ns) and sequence index (m�) used to extract
// the Cell RS symbol itself.
//
// The class LteCellRs contains a method called mapCellRS.
// Given a set of parameters, this method will populate a 2D MappingInfo vector.
// There is one vector for each port that the Cell RS should be mapped to.
//
// The parameters of this method are:
// cpMode: The Cyclic prefix mode. Can be CP_NORMAL or CP_EXTENDED.
// numRBs: This is the number of RBs for which to generate the Cell RS.
// ns: The slot within the subframe that we will generate the mapping for.
// cellId: This is the Cell ID which is used within the mapping function
// to determine the sub-carrier mapping.
// numPorts: This is the number of ports to generate the mapping for.
//
// The test harness below will call the mapCellRS for 1 subframe (two
// consecutive slots). The output of the mapping vector for each slot
// and port is then displayed.
// Any errors thrown from the mapCellRs are caught at the end.
//==============================================================================
#include <iostream>
#include <vector>

using namespace std;

// =============================================================================
//
// Class Definitions
//
// =============================================================================

//==============================================================================
// CLASS: MappingInfo
// DESCRIPTION: Store the ofdm symbol and subcarrier defining a Cell RS RE.
//==============================================================================
class MappingInfo
{
public:
// Overload << operator
friend std::ostream &operator<<(std::ostream &os,
const MappingInfo &a_mappingInfo);

// Constructor
MappingInfo(unsigned a_ofdmSymbol, unsigned a_subcarrier, unsigned a_ns, unsigned a_mDash) : m_ofdmSymbol(a_ofdmSymbol),
m_subcarrier(a_subcarrier),
m_ns(a_ns),
m_mDash(a_mDash) {}

// Destructor
~MappingInfo() {}

private:
// Make default constructor private so it cannot be called without parameters.
MappingInfo() {}

unsigned m_ofdmSymbol; // OFDM symbol within the slot the RE belongs to.
unsigned m_subcarrier; // Subcarrier the RE belongs to.
unsigned m_ns; // Slot index into sequence.
unsigned m_mDash; // MDash index into sequence
};

//==============================================================================
// CLASS: LteCellRS
// DESCRIPTION: Class used to generate the mapping for Cell RS.
//==============================================================================
class LteCellRS
{
public:
// Constructor
LteCellRS() {}

// Destructor
~LteCellRS() {}

// Define enumerated types for the CP Mode.
enum ECPMode
{
CP_NORMAL = 0,
CP_EXTENDED
};

// Define some useful constants
static const unsigned N_MAX_DL_RB = 110; // Maximum number of RBs used in sequence generation.
static const unsigned SLOTS_PER_SUB_FRAME = 2; // Number of slots in a sub-frame.
static const unsigned SLOTS_PER_RADIO_FRAME = 20; // Number of slots in a radio frame.
static const unsigned MAX_CELLRS_PORTS = 4; // Maximum number of ports that Cell RS can be mapped to.

// Define the number of OFDM Symbols per slot for cyclic prefix
static unsigned OFDM_SYMBOLS_PER_SLOT(ECPMode a_cpMode) { return (a_cpMode == CP_NORMAL ? 7 : 6); }

// Method to generate the which REs the Cell RS will be mapped to.
static void mapCellRS(const ECPMode a_cpMode,
const unsigned a_numRBs,
const unsigned a_ns,
const unsigned a_cellId,
const unsigned a_numPorts,
const unsigned a_A_port,
std::vector<std::vector<MappingInfo>> &a_mappingInfo);

private:
};

//==============================================================================
// FUNCTION: Main
// DESCRIPTION: Calls the LteCellRS mapping class for given parameters.
//==============================================================================
int main(int argc, char *argv[]) // Argument count i.e. the number of command line arguements the program was invoked with.
// Argument vector. A pointer to an array of character strings the contain the arguments.

{
if (argc != 2)
{
cout << "A test number needs to be passed";
exit(1);
}

try
{

// Set the test parameters depending on the test numberDefine the input parameters

// Reference Scenario
LteCellRS::ECPMode cpMode = LteCellRS::CP_NORMAL;

unsigned startSlot = 0, numRBs = 0, cellId = 0, numPorts = 0;
switch (argv[1][0])
{

case '0':
// Reference scenario. Should produce the mapping
// defined in 36.211 V11.6.0 Figure 6.10.1.2-1 for one antenna port.
cpMode = LteCellRS::CP_NORMAL;
startSlot = 0, numRBs = 6, cellId = 0, numPorts = 4;
break;
case '1':
//
cpMode = LteCellRS::CP_EXTENDED;
startSlot = 2, numRBs = 1, cellId = 2, numPorts = 4;
break;
case '2':
cpMode = LteCellRS::CP_NORMAL;
startSlot = 12, numRBs = 15, cellId = 3, numPorts = 2;
break;
case '3':
cpMode = LteCellRS::CP_NORMAL;
startSlot = 22, numRBs = 1, cellId = 0, numPorts = 1;
break;
case '4':
cpMode = LteCellRS::CP_NORMAL;
startSlot = 0, numRBs = 125, cellId = 0, numPorts = 1;
break;
case '5':
cpMode = LteCellRS::CP_NORMAL;
startSlot = 0, numRBs = 1, cellId = 0, numPorts = 6;
break;

default:
throw out_of_range("Invalid test number.");
break;
}

// Instantiate a LteCellRS class
LteCellRS lteCellRS;

// Make a 2D vector to store the output mapping.
// The first dimension is for the port.
// The second is for the REs that will be mapped to Cell RS for this port.
// This will be filled in by the call to mapCellRS below.
vector<vector<MappingInfo>> mappingInfo; // Port RE

// Generate the mapping for two consecutive slots.
string cpModeStr = cpMode == LteCellRS::CP_NORMAL ? "CP Normal" : "CP EXTENDED";
cout << cpModeStr.c_str() << ", Num RBs = " << numRBs << ", Cell ID = " << cellId << ", Num Ports = " << numPorts << endl;

// Make sure we start on an even subframe.
if (startSlot % LteCellRS::SLOTS_PER_SUB_FRAME != 0)
{
throw out_of_range("Start slot must be even.");
}

//Checking input for errors

//Check For RBs
if (lteCellRS.N_MAX_DL_RB < numRBs)
{
throw out_of_range(" Maximum Number of Resource Block Exceeded");
}
//Check For Ports
if (lteCellRS.MAX_CELLRS_PORTS < numPorts)
{
throw out_of_range(" Maximum Number of Ports Exceeded");
}

for (unsigned a_A_port = 0; a_A_port <= numPorts - 1; a_A_port++) //added this for loop , to get the port numbers right.
{
// Loop through each slot in the subframe.
for (unsigned indSlot = 0; indSlot < LteCellRS::SLOTS_PER_SUB_FRAME; ++indSlot)
{
unsigned ns = startSlot + indSlot;

cout << "Slot In Radio Frame " << ns << endl;
cout << "=====================" << endl;

// Generate the mapping
lteCellRS.mapCellRS(cpMode, numRBs, ns, cellId, numPorts, a_A_port, mappingInfo);

// Output the results

for (unsigned indPort = 0; indPort < mappingInfo.size(); ++indPort)
{
cout << "Port " << a_A_port << endl; //replaced "indPort" with "a_A_ports" as per my simplicity to understand
cout << "======" << endl;
for (unsigned indRe = 0; indRe < mappingInfo.at(indPort).size(); ++indRe)
{
cout << mappingInfo.at(indPort).at(indRe);
}
cout << endl;
}
}
}
}

// Catch any errors.
catch (const exception &error)
{

cerr << error.what() << endl
<< endl;
return 1;
}

catch (...)
{
std::cout << "Unknown exception\n";
return 2;
}

return 0;
}

// =============================================================================
//
// Implementation of class methods.
//
// =============================================================================

//==============================================================================
// FUNCTION: <<
// DESCRIPTION: Overload << operator.
//==============================================================================
ostream &operator<<(std::ostream &os, const MappingInfo &a_mappingInfo)
{
os << "MappingInfo: ";
os << " l = " << a_mappingInfo.m_ofdmSymbol << " ";
os << " k = " << a_mappingInfo.m_subcarrier << " ";
os << " ns = " << a_mappingInfo.m_ns << " ";
os << " mDash = " << a_mappingInfo.m_mDash << " ";
os << endl;

return os;
}

//==============================================================================
// FUNCTION: mapCellRS
// DESCRIPTION: Get the mapping information for mapping the RS to the REs,
// for slot a_ns.
// Fills in the a_mappingInfo parameter. This has one
// vector for each port.
//==============================================================================
void LteCellRS::mapCellRS(
const ECPMode a_cpMode, // CP Mode
const unsigned a_numRBs, // Number of RBs to generate mapping data for
const unsigned a_ns, // Slot number in the radio frame
const unsigned a_cellId, // Cell Id
const unsigned a_numPorts, // Number of antenna ports to generate mapping data for.
const unsigned a_A_port,
vector<vector<MappingInfo>> &a_mappingInfo // RS Mapping information [port][RE]
)
{
unsigned k, m, v, vshift = a_cellId % 6, m_dash, logical_port = a_numPorts - 1;
vector<MappingInfo> vMapinfo;
a_mappingInfo.clear();
vector<unsigned> l_dash;
vMapinfo.clear();
if (a_A_port >= 2)
{
l_dash.clear();
unsigned temp = 1;
l_dash.push_back(temp);
}
else
{
unsigned l_limit = LteCellRS::OFDM_SYMBOLS_PER_SLOT(a_cpMode) - 3;
l_dash.clear();
unsigned temp = 0;
l_dash.push_back(temp);
l_dash.push_back(l_limit);
}

for (unsigned m = 0; m <= (2 * a_numRBs) - 1; m++)
{
unsigned new_l;
for (vector<unsigned>::iterator itr = l_dash.begin(); itr != l_dash.end(); itr++)
{
new_l = *itr;

if (a_A_port == 0 && (new_l == 0)) //p==0,l==0
{

v = 0;
}
else if (a_A_port == 0 && new_l != 0) //p==0,l!=0
{
v = 3;
}
else if ((a_A_port == 1) && (new_l == 0)) // p==1,l==0
{

v = 3;
}
else if (((a_A_port == 1) && (new_l != 0))) //p==1, l!=0
{

v = 0;
}
else if (a_A_port == 2) //p==2
{

v = 3 * (a_ns % 2);
}
else if (a_A_port == 3) //p==3
{
v = 3 + (3 * (a_ns % 2));
}
else
{
/ code /
}

k = 6 * m + (v + vshift) % 6;
m_dash = m + LteCellRS::N_MAX_DL_RB - a_numRBs;
MappingInfo MappingInfo(new_l, k, a_ns, m_dash);
vMapinfo.push_back(MappingInfo);
}
}
//m loop end
a_mappingInfo.push_back(vMapinfo);
}
//EOF