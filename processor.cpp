#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

int endflag = 0;

int PC = 0;
// this is the Main Memory and the registers
vector<string> MainMemory(400);
vector<int> RegisterFile(32);
// RegisterFile[31] = 5;
/* this is the way the different registers are stored in register file */
//  [0] $zero;
//  [1] $at;
//  [2] $v0;
//  [3] $v1;
//  [4] $a0;
//  [5] $a1;
//  [6] $a2;
//  [7] $a3;
//  [8] $t0;
//  [9] $t1;
//  [10] $t2;
//  [11] $t3;
//  [12] $t4;
//  [13] $t5;
//  [14] $t6;
//  [15] $t7;
//  [16] $s0;
//  [17] $s1;
//  [18] $s2;
//  [19] $s3;
//  [20] $s4;
//  [21] $s5;
//  [22] $s6;
//  [23] $s7;
//  [24] $t8;
//  [25] $t9;
//  [26] $k0;
//  [27] $k1;
//  [28] $gp;
//  [29] $sp;
//  [30] $fp;
//  [31] $ra;

int HI = 0;
int LO = 0;

vector<string> InstructionMemory;

int binTint(string s)
{
    int n = 0;
    int p = 1;
    for (int i = s.size() - 1; i >= 0; i--)
    {
        n += (s[i] - '0') * p;
        p *= 2;
        // cout << n << endl;
    }
    if (s[0] == '1')
        n -= 65536;
    // cout << n;
    return n;
}

void SetMem(int address, int data)
{
    // cout << "setmem called with address ";
    address -= 268500992;
    // cout << address << endl;
    string temp;
    bitset<32> binary(data);
    temp = binary.to_string();
    string a(temp, 0, 8);
    string b(temp, 8, 8);
    string c(temp, 16, 8);
    string d(temp, 24, 8);
    // cout << temp << endl;
    MainMemory[address] = a;
    MainMemory[address + 1] = b;
    MainMemory[address + 2] = c;
    MainMemory[address + 3] = d;
    // cout << "the memory at " << address << " is " << MainMemory[address+4] << endl;
}

void Setbyte(int address, int data)
{
    address -= 26850992;
    string temp;
    bitset<8> binary(data);
    temp = binary.to_string();
    MainMemory[address] = temp;
}

void SetHalf(int address, int data)
{
    address -= 26850992;
    string temp;
    bitset<16> binary(data);
    temp = binary.to_string();
    string a(temp, 0, 8);
    string b(temp, 8, 8);
    MainMemory[address] = a;
    MainMemory[address + 1] = b;
}

int GetMem(int address, int start, int end)
{
    address -= 26850992;
    string s;
    while (start < end)
        s += MainMemory[address + start++];
    int n = stoi(s, 0, 2);
    return n;
}

int syscall()
{
    // cout << "WE ARE DOING A SYSCALL WITH CODE " << RegisterFile[2] << endl;
    switch (RegisterFile[2])
    {
    case 1:
    {
        cout << RegisterFile[4] << endl;
        return 0;
    }
    case 10:
    {
        endflag = 1;
        return 1;
    }
    default:
    {
        cout << "Invalid syscall number";
        return -1;
    }
    }
}

vector<string> InstructionFetch(int PC)
{
    string temp = InstructionMemory[PC];
    string opcode = temp.substr(0, 6);

    if (opcode == "011100") // specifically for mul operation
    {
        string rs = temp.substr(6, 5);
        string rt = temp.substr(11, 5);
        string rd = temp.substr(16, 5);
        string shamt = temp.substr(21, 5);
        string funct = temp.substr(26, 6);
        vector<string> data;
        data.push_back(opcode);
        data.push_back(rs);
        data.push_back(rt);
        data.push_back(rd);
        data.push_back(shamt);
        data.push_back(funct);
        return data;
    }
    // this is the instruction decode for I-type instructions
    if (opcode != "000000" && opcode != "000010" && opcode != "000011")
    {
        string rs = temp.substr(6, 5);
        string rt = temp.substr(11, 5);
        string imm = temp.substr(16, 16);

        vector<string> data;
        data.push_back(opcode);
        data.push_back(rs);
        data.push_back(rt);
        data.push_back(imm);
        return data;
    }
    // this is the instruction decode for R-type instructions
    else if (opcode == "000000")
    {
        string rs = temp.substr(6, 5);
        string rt = temp.substr(11, 5);
        string rd = temp.substr(16, 5);
        string shamt = temp.substr(21, 5);
        string funct = temp.substr(26, 6);

        vector<string> data;
        data.push_back(opcode);
        data.push_back(rs);
        data.push_back(rt);
        data.push_back(rd);
        data.push_back(shamt);
        data.push_back(funct);
        return data;
    }
    // this is the instruction decode for J-type instructions
    else
    {
        string target = temp.substr(6, 26);

        vector<string> data;
        data.push_back(opcode);
        data.push_back(target);
        return data;
    }
}

vector<int> InstructionDecode(vector<string> data)
{
    // this is the instruction decode for R-type instructions
    if (data.size() == 6)
    {
        int opcode = stoi(data[0], 0, 2);
        int rs = stoi(data[1], 0, 2);
        int rt = stoi(data[2], 0, 2);
        int rd = stoi(data[3], 0, 2);
        int shamt = stoi(data[4], 0, 2);
        int funct = stoi(data[5], 0, 2);

        int srcval = RegisterFile[rs];
        int tgtval = RegisterFile[rt];

        vector<int> data;
        data.push_back(opcode);
        data.push_back(srcval);
        data.push_back(tgtval);
        data.push_back(rd);
        data.push_back(shamt);
        data.push_back(funct);
        return data;
    }
    // this is the instruction decode for I-type instructions
    else if (data.size() == 4)
    {
        int opcode = stoi(data[0], 0, 2);
        int rs = stoi(data[1], 0, 2);
        int rt = stoi(data[2], 0, 2);
        int imm = binTint(data[3]);

        int srcval = RegisterFile[rs];
        int tgtval = RegisterFile[rt];

        vector<int> data;
        data.push_back(opcode);
        data.push_back(srcval);
        data.push_back(tgtval);
        data.push_back(imm);
        return data;
    }
    else
    {
        string temp(data[1], 16, 16);
        int opcode = stoi(data[0], 0, 2);
        int target = stoi(temp, 0, 2);

        vector<int> data;
        data.push_back(opcode);
        data.push_back(target);
        return data;
    }
}

pair<int, int> Execute(vector<int> data)
{
    // this is the execute for R-type instructions
    if (data.size() == 6)
    {
        int opcode = data[0];
        int srcval = data[1];
        int tgtval = data[2];
        // int destreg = data[3];
        int shamt = data[4];
        int funct = data[5];

        if (opcode == 28)
            return {1, srcval * tgtval};

        switch (funct)
        {
        case 0:
        {
            while (shamt--)
                tgtval *= 2;
            return {1, tgtval};
        }
        case 2:
        {
            while (shamt--)
                tgtval /= 2;
            return {1, tgtval};
        }
        case 4:
        {
            while (srcval--)
                tgtval *= 2;
            return {1, tgtval};
        }
        case 6:
        {
            while (srcval--)
                tgtval /= 2;
            return {1, tgtval};
        }
        case 8:
            return {-1, srcval}; // for the jr instruction

        case 12:
        {
            syscall();
            return {0, 0};
        }
        case 16:
            return {1, HI};

        case 17:
        {
            HI = srcval;
            return {0, 0};
        }
        case 18:
            return {1, LO};

        case 19:
        {
            LO = srcval;
            return {0, 0};
        }
        case 24:
        {
            int temp = srcval * tgtval;
            int t = pow(2, 32);
            LO = temp % t;
            HI = temp / t;
            return {0, 0};
        }
        case 26:
        {
            LO = srcval / tgtval;
            HI = srcval % tgtval;
            return {0, 0};
        }
        case 32:
            return {1, srcval + tgtval};
        case 33:
            return {1, srcval + tgtval};

        case 34:
            return {1, srcval - tgtval};

        case 36:
            return {1, srcval & tgtval};

        case 37:
            return {1, srcval | tgtval};

        case 38:
            return {1, srcval ^ tgtval};

        case 39:
            return {1, ~(srcval | tgtval)};

        case 42:
            return {1, srcval < tgtval};

        default:
        {
            cout << "Error: Invalid Function Code -Rtype" << endl;
            return {0, 0};
        }
        }
    }
    // now we will do the excecution for I-type instructions
    else if (data.size() == 4)
    {
        int opcode = data[0];
        int srcval = data[1];
        int tgtval = data[2];
        int imm = data[3];

        switch (opcode)
        {
        case 1:
        { // check this for line number error
            if (tgtval == 0)
            {
                if (imm < 0)
                    return {-2, imm};
                else
                    return {0, 0};
            }
            else if (tgtval == RegisterFile[1])
            {
                if (imm > 0)
                    return {-2, imm};
                else
                    return {0, 0};
            }
            else
                return {0, 0};
        }
        case 4:
        {
            if (tgtval == srcval)
                return {-2, imm};
            else
                return {0, 0};
        }
        case 5:
        {
            if (tgtval != srcval)
                return {-2, imm};
            else
                return {0, 0};
        }
        case 6:
        {
            if (srcval <= 0)
                return {-2, imm};
            else
                return {0, 0};
        }
        case 7:
        {
            if (srcval > 0)
                return {-2, imm};
            else
                return {0, 0};
        }
        case 8:
            return {2, srcval + imm};

        case 9:
            return {2, srcval + imm};

        case 10:
            return {2, srcval < imm};

        case 12:
            return {2, srcval & imm};

        case 13:
            return {2, srcval | imm};

        case 14:
            return {2, srcval ^ imm};

        case 15:
        {
            int temp = imm * pow(2, 16);
            return {2, temp};
        }
        case 32:
            return {10, srcval + imm};

        case 33:
            return {15, srcval + imm};

        case 35:
            return {2, srcval + imm};

        case 40:
            return {11, srcval + imm};

        case 41:
            return {16, srcval + imm};

        case 43:

            return {3, srcval + imm};

        default:
        {
            cout << "Error: Invalid Function Code -Itype" << endl;
            return {0, 0};
        }
        }
    }
    // finally we do the jump instruction
    else
    {
        int opcode = data[0];
        int target = data[1];
        if (opcode == 2)
        {
            // cout << "jumping to " << target-1 << endl;
            return {-1, target - 1};
        }
        else
        {
            cout << "Error: Invalid jump Code" << endl;
            return {0, 0};
        }
    }
}

pair<int, int> MemAcess(pair<int, int> data, vector<int> prevdata)
{
    switch (data.first)
    {
    case -2:
    {
        PC += data.second;
        return {0, 0};
    }
    case -1:
    {
        PC = data.second;
        return {0, 0};
    }
    case 0:
        return {0, 0};
    case 1:
        return {1, data.second};
    case 2:
        return {1, data.second};

    case 3:
    {
        SetMem(data.second, prevdata[2]);
        return {0, 0};
    }
    case 10:
    {
        return {1, GetMem(data.second, 0, 1)};
    }
    case 11:
    {
        Setbyte(data.second, prevdata[2]);
        return {0, 0};
    }
    case 15:
        return {1, GetMem(data.second, 0, 2)};

    case 16:
    {
        SetHalf(data.second, prevdata[2]);
        return {0, 0};
    }

    default:
    {
        cout << "Error: Invalid Memory Access" << endl;
        return {0, 0};
    }
    }
}

void WriteBack(int reg, int data)
{
    RegisterFile[reg] = data;
    // cout << "data of val " << data << " written to register " << reg << endl;
}

int main()
{
    // RegisterFile[31] = 5;
    for (int i = 0; i < 400; i++)
        MainMemory[i].resize(8);

    ifstream input("MIPS_ISA/factorial.txt");
    if (!input.is_open())
    {
        cout << "error opening file \n";
        return 1;
    }

    string line;
    while (getline(input, line, '\n'))
        InstructionMemory.push_back(line);

    // storing the instructions as byte addressable memory
    vector<string> ByteAddressableMemory;
    for (int i = 0; i < InstructionMemory.size(); i++)
        for (int j = 0; j < InstructionMemory[i].length(); j += 8)
            ByteAddressableMemory.push_back(InstructionMemory[i].substr(j, 8));

    PC = 0;

    // for (int i = 0; i < InstructionMemory.size(); i++)
    //     cout << "This is the string in index " << i << " : " << InstructionMemory[i] << endl; // testing the input file
    int count = 0;
    while (PC < InstructionMemory.size() && count < 1000)
    {
        // cout << "this is the current PC val " << PC << endl;
        string instruction = InstructionMemory[PC];
        vector<string> data = InstructionFetch(PC);
        // for (string i: data) cout << i << ' ';
        // cout << "  :these are the results of IF" << endl;

        vector<int> regvals = InstructionDecode(data);
        // for (int i: regvals) cout << i << ' ';
        // cout << "  :these are the results of ID" << endl;

        pair<int, int> exe = Execute(regvals);
        // cout << exe.first << ' ' << exe.second << ' ';
        // cout << "  :these are the results of EX" << endl;

        pair<int, int> mem = MemAcess(exe, regvals);
        // cout << mem.first << ' ' << mem.second << ' ';
        // cout << "  :thes are the results of MEM" << endl;

        // cout << "this is the value of  ra " << RegisterFile[31] << endl;
        // cout << "this is the value of  s0 " << RegisterFile[16] << endl;
        // cout << "this is the value of  ans " << RegisterFile[9] << endl;
        if (mem.first == 1)
        {
            if (exe.first == 1)
                WriteBack(stoi(data[3], 0, 2), mem.second);
            else if (exe.first == 2)
                WriteBack(stoi(data[2], 0, 2), mem.second);
        }

        // cout << endl
        //      << endl;
        PC++;
        if (endflag == 1)
            return 0;
        count++;
    }

    input.close();
    return 0;
}