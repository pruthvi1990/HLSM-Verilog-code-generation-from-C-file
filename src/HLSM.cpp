
//Code Developed by Pruthvi Gowda & Mukunda M J.. Date:04-Nov-13

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <stack>


using namespace std;
char netlistfile[1000][500];
char inputs[100][20];
char outputs[100][20];
char wires[100][20];
char reg[100][20];
int nofip, nofop,nofw,nofr= 0;
int node = 0;
int nofline = 0;
int startnode = 0; //used in Listr or Listl
int endnode = 0; //used in Listr or Listl
int total_listr_states;
int total_listl_states;

struct equnode{
	char ip1[20];
	char ip2[20];
	char ip3[20];
	char op[20];
	char opr1[20];
	char opr2[20];
	float delay;
	char node_type[10];
	bool status;
	char condition[10];
	int jmpst;
	int cdfg_start_time;     //This the scheduled time of an operation
	int cdfg_end_time;
	int alap_start_time;
	int alap_end_time;
	int state_no_listr;
    bool listr_sch_status;
	int slack_time;
	int listr_start_time;
	int listr_end_time;
	int listr_jump_state;
	bool listr_jmpst_flag;
	int state_no_listl;
    bool listl_sch_status;
    int listl_start_time;
	int listl_end_time;
	int listl_jump_state;
	bool listl_jmpst_flag;
	int listl_label;
};

int getnetlistfile();
void print_equation( int );
int if_while_parse( int, char * );
int chk_cond_validity(char *);
void cdfg_schedule(int, int);
void alap_schedule(int, int, int);
void find_listl_label(int startnode, int endnode);
int check_inopwire(struct equnode *equations);
struct equnode *equations = (struct equnode*) malloc(100*sizeof(struct equnode));

int checkinpvalidity(char *inp);
int checkopvalidity(char *op);
int checkoprvalidity(char *opr);
int inp1validity, inp2validity,inp3validity,oprvalidity,opvalidity = 1;
int no_of_nodes;
int index1;
int check_regop (struct equnode *equations);
list<int> *mul_qty_global = new list<int>[1];
list<int> *add_qty_global = new list<int>[1];
list<int> *log_qty_global = new list<int>[1];
void create_ns_verilog();
void create_listl_verilog(int mul, int add, int log);
void create_listr_verilog(int latency);
int find_listr_states(int startnode, int endnode, int state_no_listr, int latency);
void find_jump_state_listr(int close_brace_node);
void find_jump_state_listl(int close_brace_node);
void print_listr_verilog();
void print_listl_verilog();
int find_listl_states(int startnode, int endnode, int state_no_listl, int mul, int add, int log);

int getnetlistfile()
{
	char line[1000];
    int jumpid=0;
	char *temp;
	char *buff;
	FILE *fp;
	int jump_state;
    int newnode;
    int brace_cnt = 0;
    int cond_validity;
	char node_t[]= {'O', 'W' , 'R'};


	int x=0;

	fp = fopen("test1.c", "r");

	if (!fp) {
		cerr << "Bad netlist File. Exiting . . ." << endl;
		exit (-1); // Exit out of the whole program if the file is bad
	}

	while (!feof(fp))
	{
		int i=0;
		fgets (line,1000,fp);
		if(!strcmp(netlistfile[nofline-1],line)) break;
		while (isspace(line[i])) i++;
//		cout << (int)line[i]<< endl;
	        if (!line[i]) continue;

		// If you encounter an comment line, continue scanning the next


		if (line[i] == '/' && (line[i+1] == '/'))  continue;
        if( (line[0] == '/') && (line[1] == '/'))
            continue;
        else if(strlen(line) == 1)
        {
            continue;
        }
        else
        {
                strcpy(netlistfile[nofline],line);
              //  cout << netlistfile[nofline];
                nofline++;
              //  cout << nofline << endl;
        }

	}

	temp = (char *) calloc(1,20);
	buff = (char *) calloc(1,1000);
	for(int i =0; i<nofline ;i++)
	{
		temp = strtok(netlistfile[i],"  \t\n");


		if((!strcmp(temp,"INPUTS"))||(!strcmp(temp,"OUTPUTS"))||(!strcmp(temp,"WIRES"))||(!strcmp(temp,"REGS")))
		{


		if(!strcmp(temp,"INPUTS"))
			{
			    temp = strtok(NULL," \t\n");
			    if(temp == NULL)
                {
                    cerr << "Invalid inputs definition"  << endl;
                    exit(-1);
                }
                if(strcmp(temp, ":"))
                {
                    cerr << "Invalid inputs definition"  << endl;
                    exit(-1);
                }
                if(!strcmp(temp, ":"))
                {
                    temp =strtok(NULL," \t\n");
                }

				while(temp!= NULL)
				{
					strcpy(inputs[nofip],temp);
                    nofip++;
				//	cout << "Input no. "<< (nofip-1) << " \n " << inputs[nofip-1] << endl;
					temp =strtok(NULL," \t\n");
				}

			}
			else if(!strcmp(temp,"OUTPUTS"))
			{
                temp = strtok(NULL," \t\n");
			    if(temp == NULL)
                {
                    cerr << "Invalid outputs definition"  << endl;
                    exit(-1);
                }
                if(strcmp(temp, ":"))
                {
                    cerr << "Invalid outputs definition"  << endl;
                    exit(-1);
                }
                if(!strcmp(temp, ":"))
                {
                    temp =strtok(NULL," \t\n");
                }


				while(temp != NULL)
				{
					strcpy(outputs[nofop],temp);
                    nofop++;
					temp = strtok(NULL," \t\n");
			//		cout << "Output no. "<< (nofop-1) << " \n " << outputs[nofop-1] << endl;
				}
            }



			else if(!strcmp(temp,"REGS"))
			{
                temp = strtok(NULL," \t\n");
			    if(temp == NULL)
                {
                    cerr << "Invalid regs definition"  << endl;
                    exit(-1);
                }
                if(strcmp(temp, ":"))
                {
                    cerr << "Invalid regs definition"  << endl;
                    exit(-1);
                }
                if(!strcmp(temp, ":"))
                {
                    temp =strtok(NULL," \t\n");
                }
				while(temp != NULL)
				{
					strcpy(reg[nofr],temp);
                    temp = strtok(NULL," \t\n");
					nofr++;
		//		cout << "Reg no. "<< nofr << " is " << reg[nofr-1] << endl;
				}
			}
		}

		else
		{
			while(temp != NULL)
			{
				if( temp == NULL) break;
                if ((!strcmp(temp,"if")) || (!strcmp(temp,"while")))
                    {
                        jump_state = node;
                        brace_cnt = 0;
                        jumpid = 0;
                        if (nofr > 0)
                        {
                            newnode = node+3;
                        }
                        else
                        {
                            newnode = node+2;
                        }
                        strcpy(equations[node].node_type, temp);
                        temp = strtok(NULL," \t");
                        if ((strcmp(temp ,"(") || (temp == NULL)))
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }
                        temp = strtok(NULL," \t");
                        if ( temp == NULL)
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }

                        cond_validity = chk_cond_validity(temp);
                        if( cond_validity == 0)
                        {
                            cerr << "Invalid condition variable in line no. " << node << endl;
                            exit(-1);
                        }
                        strcpy (equations[node].condition,temp);
                        temp = strtok(NULL," \t");

                        if ( temp == NULL)
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }

                        if (strcmp(temp ,")"))
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }

                        temp = strtok(NULL," \t\n");

                        if ( temp == NULL)
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }

                        if (strcmp(temp,"{"))
                        {
                            cerr << "Syntax error : invalid syntax for WHILE/IF" << endl;
                            exit(-1);
                        }

//Finding Jump state
                        for ( int i= newnode ; i < nofline ; i++)
                        {
                            strcpy(buff, netlistfile[i]);
                            temp = strtok ( buff, " \t\n");

                            if((!strtok(temp, "if" )) || (!strtok(temp, "while" )))
                            {
                                jump_state+=1;
                                brace_cnt+=1;
                            }

                            else if((!strtok(temp, "}" )))
                            {
                                jump_state+=1;
                                brace_cnt-=1;
                            }

                            else
                            {
                                jump_state+=1;
                            }

                            if ( brace_cnt == 0)
                            {
                                jumpid = 1;
                                break;
                            }
                            if ( (i ==  nofline-1) && (brace_cnt != 0))
                            {
                                cerr << "Syntax error in braces...quitting.." << endl;
                                exit(-1);
                            }

                        }
                       if( jumpid == 0)
                       {
                           cerr << " Syntax error in while loop/if statements " << endl;
                           exit(-1);
                       }
                        else if (jumpid == 1)
                        {
                           equations[node].jmpst = jump_state+1;
                           node++;
                           break;
                        }

                    break;
                    }
                   if ((!strcmp(temp,"{")))
                      {
                          cerr << " Syntax error....open brace cannot start in new line ..quitting.." << endl;
                          exit(-1);
                      }

                   if ((!strcmp(temp,"}")))
                       {
                          strcpy (equations[node].node_type,"if/while");
                          node++;
                          break;
                       }
				strcpy((equations[node].op),temp);
				//cout << (equations+node)->op << " no of nodes" << node << endl;
				opvalidity = checkopvalidity(equations[node].op);
                if (opvalidity == 0)
                {
                    cerr << "Invalid op definition in the equation" << endl;
                    exit(-1);
                }

                for (x=0 ; x<(nofop) ; x++)
                {
                    if (!strcmp((equations[node].op) , outputs[x]))
                        {
                            (equations+node)->node_type[0]= node_t[0];
              //              cout << "node_type" << (equations+node)->node_type << endl;
                            break;
                        }
                }


				for (x=0 ; x< nofw; x++)
                {

                    if (!strcmp((equations[node].op) , wires[x]))
                        {
                             (equations+node)->node_type[0]= node_t[1];
 //                            cout << "node_type" << (equations+node)->node_type << endl;
                             break;
                        }
                }



				for (x=0 ; x< nofr ; x++)
                {
                    if  (!strcmp((equations[node].op) , reg[x]))
                        {
                            //strcpy(((equations+node)->op) , operators[0]);
                             (equations+node)->node_type[0]= node_t[2];
                             break;
                        }
                }

				temp = strtok(NULL," \t\n");
				if (temp == NULL)
                {
                    cerr << "Syntax error: Invalid equation definition" << endl;
                }

				if (strcmp(temp,"="))
                {
                    cerr << "Syntax error: Invalid equation definition" << endl;
                    exit(-1);
                }

                if (!strcmp(temp,"="))
                {
                    temp = strtok(NULL," \t\n");
                }

                if (temp == NULL)
                {
                    cerr << "Syntax error: Invalid equation definition" << endl;
                    exit(-1);
                }

				strcpy(((equations+node)->ip1),temp);
				inp1validity = checkinpvalidity((equations+node)->ip1);
                if (inp1validity == 0)

                {
                    cerr << "Syntax error: Invalid ip1 definition in the equation" << endl;
                    exit(-1);
                }


                temp = strtok(NULL," \t\n");
				if (temp == NULL)
                    {
              //      cout<< "node no. " << node << endl << (equations+node)->op << "   " << (equations+node)->ip1 << "   "<< (equations+node)->delay << " " << (equations+node)->node_type <<  endl;
                    node++;
 //                   cout << "node no is " << node << endl;
                    break;
				   }

				if (!strcmp(temp,"?"))
				{
					strcpy(((equations+node)->opr1),temp);
					temp = strtok(NULL," \t\n");
					if (temp == NULL)
                    {
                        cerr << "Syntax error: Invalid equation definition" << endl;
                        break;
                    }
					strcpy(((equations+node)->ip2),temp);
					inp2validity = checkinpvalidity((equations+node)->ip2);
                    if (inp2validity == 0)
                    {
                        cerr << "Syntax error: Invalid ip2 definition in the equation" << endl;
                        exit(-1);
                    }
					temp = strtok(NULL," \t\n");
					if (temp == NULL)
                    {
                       cerr << "Syntax error: Invalid MUX definition in the equation" << endl;
                       exit(-1);
					}
                    if (strcmp(temp, ":"))
                    {
                       cerr << "Syntax error: Invalid MUX definition in the equation" << endl;
                       exit(-1);
					}

					strcpy(((equations+node)->opr2),temp);
					temp = strtok(NULL," \t\n");
					if (temp == NULL)
					{
                       cerr << "Syntax error: Invalid MUX definition in the equation" << endl;
                       exit(-1);
					}
					strcpy(((equations+node)->ip3),temp);
					inp3validity = checkinpvalidity((equations+node)->ip3);
                    if (inp3validity == 0)
                    {
                        cerr << "Syntax error: Invalid ip3 definition in the equation" << endl;
                        exit(-1);
                    }
					temp = strtok(NULL," \t\n");
	//				cout << "node no. "<< node << " \n " << (equations+node)->op << " " << (equations+node)->ip1 << " " <<(equations+node)->opr1 << " " <<(equations+node)->ip2 << (equations+node)->opr2 << "  "<< (equations+node)->ip3 <<  (equations+node)->node_type << "  " << endl;
					node++;
//					 cout << "node no is " << node << endl;
                    if (temp == NULL) break;
                    else
                    {
                        cerr << "Syntax error: Invalid MUX equation definition" << endl;
                    }
				}
				strcpy(((equations+node)->opr1),temp);
				oprvalidity = checkoprvalidity((equations+node)->opr1);
                    if (oprvalidity == 0)
                    {
                        cerr << "Syntax error: Invalid opr definition in the equation" << endl;
                        exit(-1);
                    }
				temp = strtok(NULL," \t\n");
				if (temp == NULL)
                {
                    cerr << "Syntax error: Invalid equation definition" << endl;
                    exit(-1);
                }
				strcpy(((equations+node)->ip2),temp);
				inp2validity = checkinpvalidity((equations+node)->ip2);
                if (inp2validity == 0)
                {
                    cerr << "Syntax error: Invalid ip2 definition in the equation";
                    exit(-1);
                }
				temp = strtok(NULL," \t\n");

				if (!(temp == NULL))
                {
                    cerr << "Syntax error: Invalid equation termination" << endl;
                    exit(-1);
                }

//				cout << "node no. "<< node << "\n" << (equations+node)->op << " " << (equations+node)->ip1 << " " <<(equations+node)->opr1 << " " <<(equations+node)->ip2 << " " <<(equations+node)->opr2 << " "  <<(equations+node)->ip3 << " "<< (equations+node)->delay << "  " <<(equations+node)->node_type << endl;
				node++;

			}

		}

	}
//cout << "no of nodes = " << node << endl ;
no_of_nodes=node;
//	cout << node  << endl;
//	cout << "no of node " << node << endl;

//#################################################################################################################################################################
//To check the duplication of variables in INPUTS , WIRES , REGS


	for (int a=0 ; a<nofip ; a++)
        {
            for (int x=0 ; x<nofop ; x++)
            {
                if ( !(strcmp(inputs[a], outputs[x])))
                {
                    cerr << "\n"<< "Syntax error: Same variabe defined  in INPUTS and OUTPUTS ... Quiting!!!" << "\n "<< endl;
                    exit(-1);
                }
            }
        }

        for (int a=0 ; a<nofip ; a++)
        {
            for (int x=0 ; x<nofw ; x++)
            {
                if ( !(strcmp(inputs[a], wires[x])))
                {
                    cerr << "\n "<< "Syntax error: Same variabe defined  in INPUTS and WIRES ... Quiting!!!" << "\n "<< endl;
                    exit(-1);
                }
            }
        }

        for (int a=0 ; a<nofip ; a++)
        {
            for (int x=0 ; x<nofr ; x++)
            {
                if ( !(strcmp(inputs[a], reg[x])))
                {
                    cerr << "\n" << "Syntax error: Same variabe defined  in INPUTS and REGS ... Quiting!!!" <<"\n" << endl;
                    exit(-1);
                }
            }
        }

        for (int a=0 ; a<nofop ; a++)
        {
            for (int x=0 ; x<nofw ; x++)
            {
                if ( !(strcmp(outputs[a], wires[x])))
                {
                    cerr <<  " \n "<< "Syntax error: Same variabe defined  in OUTPUTS and WIRES ... Quiting!!!" <<"\n" <<endl;
                    exit(-1);
                }
            }
        }

         for (int a=0 ; a<nofop ; a++)
        {
            for (int x=0 ; x<nofw ; x++)
            {
                if ( !(strcmp(outputs[a], reg[x])))
                {
                    cerr << "\n"<< "Syntax error: Same variabe defined  in OUTPUTS and REGS ... Quiting!!!" << "\n"<< endl;
                    exit(-1);
                }
            }
        }

         for (int a=0 ; a<nofw ; a++)
        {
            for (int x=0 ; x<nofr ; x++)
            {
                if ( !(strcmp(wires[a], reg[x])))
                {
	    cerr << "\n" << "Syntax error: Same variabe defined  in WIRES and REGS ... Quiting!!!" << "\n" <<endl;
                    exit(-1);
                }
            }
        }


//#####################################################################################################################################################

	if (nofip == 0)
	{
		cerr << "Syntax error.Inputs not defined in netlist...quitting !!!!!" << endl;
		exit(-1);
	}

	if (nofop == 0)
	{
		cerr << "Syntax error.Outputs not defined in netlist...quitting !!!!!" << endl;
		exit(-1);
	}

//	 cout << "node no is " << node << endl;


if (!(check_inopwire(equations)))
    {
        cerr << "Syntax error: Unidentified INPUT, OUTPUT , WIRE in netlist file.......!!!!" << endl;
        exit(-1);
    }


if (!check_regop(equations))
        {
            cerr << "Errror!!" << endl;
	    exit(-1);
        }



//	cout << "No of Inputs " << nofip << endl;
//	cout << "No of outputs " << nofop << endl;
//	cout << "No of wires " << nofw << endl;
//	cout << "No of Regs " << nofr << endl;
//	cout << "No of nodes " << node << endl << "\n" << "\n";

	return 1;
}

int check_regop (struct equnode *equations)

{
//cout << "this loop executing" << endl;
   char operators[][3] = {"!" , "+" , "-", "*", ">" , "<" , "==", "?", "<<", ">>" ,0 };
   for(int z=0 ; z<no_of_nodes ; z++)
   {

            for (int j=0 ; j<nofop; j++ )
			{
				if(!strcmp((equations[z].op) , outputs[j]) )
					{
						for (int x=0 ; x<10 ; x++)
						{
						if(!strcmp((equations[z].opr1) , operators[x]) )
							{
							cerr << "Syntax error: Invalid entry in netlist file....!!" << endl;
							exit(-1);
							}
						}
                    }
            }

   }
return 1;
}

int check_inopwire(struct equnode *equations)
{
        for(int z=0 ; z<no_of_nodes ; z++)
        {
            for (int j=0 ; j<nofip; j++ )
            {

            if(!strcmp((equations[z].op) , inputs[j]) )
            {
             cerr << "Syntax error : INPUT is  declared as OUTPUT...............!!!!" << endl;
             exit(-1);
            }

            }
        }

            for(int z=0 ; z<no_of_nodes ; z++)
        {

            for (int m=0 ; m<nofop; m++)
             {

            if (!(strcmp((equations[z].ip1) , outputs[m])))
                {
                cerr << "Syntax error: OUTPUT is declared as input.............!!!!" << endl;
                exit(-1);
                }

            if (!(strcmp((equations[z].ip2) , outputs[m])))
            {

                cerr << "Syntax error: OUTPUT is declared as input.............!!!!" << endl;
                exit(-1);

            }

             if (!(strcmp((equations[z].ip3) , outputs[m])))
            {

                cerr << "Syntax error: OUTPUT is declared as input.............!!!!" << endl;
                exit(-1);

            }



             }

        }

        return 1;
}


int checkinpvalidity(char *inp)
{
int inpflag = 0;
   for (int k=0; k<nofip; k++)
   {
       if(!strcmp(inp, inputs[k]))
       {
           inpflag = 1;
           return 1;
       }
   }
   for (int l=0; l<nofw; l++)
   {
       if(!strcmp(inp, wires[l]))
       {
           inpflag = 1;
           return 1;
       }
   }
   for (int m=0; m<nofr; m++)
   {
       if(!strcmp(inp, reg[m]))
       {
           inpflag = 1;
           return 1;
       }
   }
   if(!inpflag) return 0;
}

int checkopvalidity(char *op)
{
int opflag = 0;
   for (int k=0; k<nofop; k++)
   {
       if(!strcmp(op, outputs[k]))
       {
           opflag = 1;
           return 1;
       }
   }
   for (int l=0; l<nofw; l++)
   {
       if(!strcmp(op, wires[l]))
       {
           opflag = 1;
           return 1;
       }
   }
   for (int m=0; m<nofr; m++)
   {
       if(!strcmp(op, reg[m]))
       {
           opflag = 1;
           return 1;
       }
   }
   if(!opflag) return 0;
}

int checkoprvalidity(char *opr)
{
    int oprflag = 0;
    if((!strcmp(opr,"+")) || (!strcmp(opr,"-")) || (!strcmp(opr,"*")) || (!strcmp(opr,"<")) ||(!strcmp(opr,">")) ||(!strcmp(opr,"==")) ||(!strcmp(opr,"<<")) ||(!strcmp(opr,">>")) ||(!strcmp(opr,"?")))
    {
       oprflag = 1;
       return 1;
    }
    if(!oprflag) return 0;
}

void create_ns_verilog()
{
        ofstream verilog_file;
        verilog_file.open ("verilog_ns_opfile.v");

        verilog_file <<"// ------------------------------------------------------------------------------------------------- "<< endl;
        verilog_file <<" // Copyright (c) 2013 Pruthvi & Mukunda MJ , University of Arizona" <<endl;
        verilog_file <<" // Permission to copy is granted provided that this header remains" <<endl;
        verilog_file <<" // intact. This software is provided with no warranties" << endl;
        verilog_file <<" // -------------------------------------------------------------------------------------------------" << endl;

        verilog_file << "module" << " " << "HLSM (Clk, Rst, Start, Done" ;
        for (int i = 0; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i];
        }

        for (int j = 0; j< nofop ; j++)
        {
            verilog_file << "," << outputs[j];
        }
        verilog_file << ");"<< "\n" <<endl;

        verilog_file << "input Clk, Rst, Start;"<< endl;
        verilog_file << "output reg Done ;" << endl;
        verilog_file << "input[31:0]" << " ";
        verilog_file << inputs[0] ;
        for (int i = 1; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i]  ;
        }
        verilog_file << ";"<< endl;

        verilog_file << "output reg [31:0]" << " ";
        verilog_file << outputs[0] ;
        for (int i = 1; i< nofop ; i++)
        {
            verilog_file << "," << outputs[i] ;
        }
        verilog_file << ";"<< endl;

         if ( nofr > 0)
        {
            verilog_file << "reg[31:0]" << " ";
            verilog_file << reg[0];
            for (int i = 1; i< nofr ; i++)
            {
                verilog_file << "," << reg[i] ;
            }
            verilog_file << ";"<< "\n" << endl;
        }



        verilog_file << "reg [5:0]" << " " << "state" << " " << ";" << endl;




        verilog_file << "parameter" << " " << "start_wait" << " " << "=" << " " << "0" << ";" << endl;

        for (int i=1 ; i <= node ; i++)
        {
            verilog_file << "parameter" << " " << "state" <<"_" << i << " " << "=" << " " << i << " ;" << endl;
        }
        verilog_file << "parameter" << " " << "Final = " << (node+1) << " ;" << endl;
        verilog_file<< "\n" << endl;

        verilog_file << "always@" << " " << "(posedge Clk)" << " begin" << endl;
        verilog_file << "if" << " " << "(" << " " << "Rst" << " " << "==" << "1" << " " << ")" << " " << "begin" << endl;
        verilog_file << "state" << " " << "<="<< " " << "start_wait" << " " << ";"<< endl;

        verilog_file << "end" << endl;

        verilog_file<<endl;

        verilog_file<< "else begin" << endl;
        verilog_file<< "case( state )" << endl;

        verilog_file<< "start_wait : begin" << endl;
        verilog_file << "if(start == 1) begin "<<endl;
        verilog_file << "state <= state_1 ;" << endl;
        verilog_file << "end" << endl;

        verilog_file<< "else begin" << endl;
        verilog_file << "state <= start_wait ;" << endl;
        verilog_file << "end" << endl;
        verilog_file << "end" << endl;


        for(int i=0; i< node ; i++ )
        {
                if ((strcmp(equations[i].node_type , "if")) && (strcmp(equations[i].node_type , "while") && (strcmp(equations[i].node_type , "if/while"))))
//Code to be executed if the statements are not if or while
                {
                    verilog_file << "state_" << (i+1) << " : begin" << endl;
                    if( (equations[i].node_type[0] == 'O'))
                    {
                        verilog_file << equations[i].op <<" = " << equations[i].ip1 << ";" << endl;
                        if ( (i+1) < node )
                        {
                            verilog_file << "state <= state_" << (i+2) << ";" << endl; verilog_file << "end" << endl;
                            continue;
                        }
                        else
                        {
                            verilog_file << "state <= Final ;" << endl; verilog_file << "end" << endl;
                            continue;
                        }

                    }

                    else if(!strcmp(equations[i].opr1,"+")){ verilog_file <<equations[i].op << " = " << equations[i].ip1 << " + " << equations[i].ip2  << ";" << endl;}

                    else if(!strcmp(equations[i].opr1,"-")){ verilog_file <<equations[i].op << " = " << equations[i].ip1 << " - " << equations[i].ip2  << ";" <<endl;}

                    else if(!strcmp(equations[i].opr1,"*")){ verilog_file <<equations[i].op << " = " << equations[i].ip1 << " * " << equations[i].ip2  << ";"<<endl;}

                    else if(!strcmp(equations[i].opr1,">")) { verilog_file <<equations[i].op << " = " << equations[i].ip1 << " > " << equations[i].ip2  << ";" <<endl;}

                    else if(!strcmp(equations[i].opr1,"<")) { verilog_file <<equations[i].op << " = " << equations[i].ip1 << " < " << equations[i].ip2  << ";" <<endl;}

                    else if(!strcmp(equations[i].opr1,"==")) { verilog_file <<equations[i].op << " = " << equations[i].ip1 << " == " << equations[i].ip2  << ";" <<endl;}

                    else if(!strcmp(equations[i].opr1,"?")) {verilog_file << equations[i].op << " = " <<  equations[i].ip1 << " ? " << equations[i].ip2 << " : " << equations[i].ip3 << ";" <<endl;}

                    else if(!strcmp(equations[i].opr1,"<<")) { verilog_file <<equations[i].op << " = " << equations[i].ip1 << " << " << equations[i].ip2  << ";" << endl;}

                    else { verilog_file <<equations[i].op << " = " << equations[i].ip1 << " >> " << equations[i].ip2  <<";" <<endl;}

                    if ( (i+1) < node )
                    {
                        verilog_file << "state <= state_" << (i+2) << ";" <<endl; verilog_file << "end" << endl;
                        continue;
                    }
                    else
                    {
                        verilog_file << "state <= start_wait" << ";" << endl; verilog_file << "end" << endl;
                        continue;
                    }

                }

                else if (!strcmp(equations[i].node_type , "while") )
                {
                     verilog_file << "state_" << (i+1) << " : begin" << endl;
                     verilog_file << "while (" << equations[i].condition << " ) begin " <<endl;
                     verilog_file << "state <= state_" << (i+2) << " ;" << endl;
                     verilog_file << "end" << endl;

                     verilog_file << "if (" << equations[i].condition << " == 0 ) begin " << endl;
                     verilog_file << "state <= state_" << (equations[i].jmpst) << " ;" <<endl;
                     verilog_file << "end" << endl;
                     verilog_file << "end" << endl;

                     verilog_file << "state_" << ((equations[i].jmpst)-1) << " : begin" << endl;
                     verilog_file << "state <= state_" << (i+1) << " ;" << endl;
                     verilog_file << "end" << endl;

                }

                else if (!strcmp(equations[i].node_type , "if") )
                {
                     verilog_file << "state_" << (i+1) << " : begin" << endl;
                     verilog_file << "if (" << equations[i].condition <<" != 0 )" << " begin " << endl;
                     verilog_file << "state <= state_" << (i+2) << " ;" << endl;
                     verilog_file << "end" << endl;

                     verilog_file << "else begin" << endl;
                     verilog_file << "state <= state_" << ((equations[i].jmpst)-1) << " ;" <<endl;
                     verilog_file << "end" << endl;
                     verilog_file << "end" << endl;

                     verilog_file << "state_" << ((equations[i].jmpst)-1) << " : begin" << endl;
                     verilog_file << "state <= state_" << equations[i].jmpst << " ;" << endl;
                     verilog_file << "end" << endl;

                }

        }
        verilog_file << "Final : begin" << endl << "Done <= 1 " << endl << "state <= start_wait ;" << endl << "end" << endl;
        verilog_file << "endcase" << endl << "end " << endl << "end" << endl << "endmodule" <<endl;


}

 void create_listr_verilog(int latency)
 {
     int state_no_listr=1;
        for (int i = 0; i<node ; i++)
        {
            if(!strcmp(equations[i].node_type,"if") || !strcmp(equations[i].node_type,"while") || !strcmp(equations[i].node_type,"if/while"))
            {
                if(!strcmp(equations[i].node_type,"if/while"))
                {
                     find_jump_state_listr(state_no_listr);
                }
                startnode = i+1;
                equations[i].state_no_listr = state_no_listr;
                state_no_listr = state_no_listr+1;
                continue;
            }
            if( !strcmp(equations[i+1].node_type,"if") || !strcmp(equations[i+1].node_type,"while") || !strcmp(equations[i+1].node_type,"if/while"))
            {
                endnode = i;
                cdfg_schedule(startnode, endnode);
                alap_schedule(startnode, endnode, latency);
                state_no_listr = find_listr_states(startnode, endnode, state_no_listr,latency);
                continue;
            }
            if( i == node-1)
            {
                endnode = i;
                cdfg_schedule(startnode, endnode);
                alap_schedule(startnode, endnode, latency);
                state_no_listr = find_listr_states(startnode, endnode, state_no_listr, latency);
            }
        }
        total_listr_states = state_no_listr-1;
        print_listr_verilog();

 }


void cdfg_schedule(int startnode, int endnode)
{
   list<int> *dep = new list<int>[100];
   for (int i= startnode; i<=endnode ;i++)
   {
       for(int j=startnode; j<=endnode ; j++)
        {
            if(j == i) continue;
            if(!strcmp(equations[i].ip1,equations[j].op) || !strcmp(equations[i].ip2,equations[j].op) || !strcmp(equations[i].ip3,equations[j].op))
            {
                dep[i].push_back(j);
            }
        }
   }

    for(int i = 0; i< node ; i++)
    {
        list<int>::iterator j;
        for (j = dep[i].begin(); j != dep[i].end(); ++j)
        {
          cout << *j << " ";
        }
    cout << endl;
    }

        for (int i= startnode; i<=endnode ;i++)
        {
            equations[i].cdfg_start_time = 1;
            if( !strcmp(equations[i].opr1,"*"))
            {
                equations[i].cdfg_end_time = 2;
            }
            else
            {
                equations[i].cdfg_end_time = 1;
            }
        }
    cout<< "cdfg" << endl;
   for (int i= startnode; i<=endnode ;i++)
   {
        int end_time = 1;
        list<int>::iterator j;
        for (j = dep[i].begin(); j != dep[i].end(); ++j)
        {
          if ( equations[*j].cdfg_end_time >  end_time)
          {
            end_time = equations[*j].cdfg_end_time;
          }
          equations[i].cdfg_start_time = end_time+1;
          equations[i].cdfg_end_time = end_time+1;
          if( !strcmp(equations[i].opr1,"*"))
          {
            equations[i].cdfg_end_time = end_time+2;
          }
        }

        cout << "node" << i << " ";
        cout <<  equations[i].cdfg_start_time << " ";
        cout <<  equations[i].cdfg_end_time << endl;
   }
}

void alap_schedule(int startnode, int endnode, int latency)
{
   list<int> *dep = new list<int>[100];
   for (int i= startnode; i<=endnode ;i++)
   {
       for(int j=startnode; j<=endnode ; j++)
        {
            if(j == i) continue;
            if(!strcmp(equations[i].ip1,equations[j].op) || !strcmp(equations[i].ip2,equations[j].op) || !strcmp(equations[i].ip3,equations[j].op))
            {
                dep[i].push_back(j);
            }
        }
   }
        for (int i= startnode; i<=endnode ;i++)
        {
            equations[i].alap_end_time = latency;
            if( !strcmp(equations[i].opr1,"*"))
            {
                equations[i].alap_start_time = latency - 1;
            }
            else
            {
                equations[i].alap_start_time = latency;
            }
        }
   cout<< "alap" << endl;
   for (int i= endnode; i>=startnode ;i--)
   {
        list<int>::iterator j;
        for (j = dep[i].begin(); j != dep[i].end(); ++j)
        {
            equations[*j].alap_end_time = equations[i].alap_start_time -1;
            if( !strcmp(equations[*j].opr1,"*"))
            {
                equations[*j].alap_start_time = equations[*j].alap_end_time-1;
            }
            else
            {
                equations[*j].alap_start_time = equations[*j].alap_end_time;
            }
        }
        cout << "node" << i << " ";
        cout <<  equations[i].alap_start_time << " ";
        cout <<  equations[i].alap_end_time << endl;
   }
}

int find_listr_states(int startnode, int endnode, int state_no_listr, int latency)
{
   int local_mul_qty = 0;
   int local_add_qty = 0;
   int local_log_qty = 0;
   int mul_qty = 0;
   int add_qty = 0;
   int log_qty = 0;
   int buffer_mul = 1;
   int buffer_add = 1;
   int buffer_log = 1;

//-------------------------Finding depenancy list---------------------//
   list<int> *dep = new list<int>[100];
   for (int i= startnode; i<=endnode ;i++)
   {
       for(int j=startnode; j<=endnode ; j++)
        {
            if(j == i) continue;
            if(!strcmp(equations[i].ip1,equations[j].op) || !strcmp(equations[i].ip2,equations[j].op) || !strcmp(equations[i].ip3,equations[j].op))
            {
                dep[i].push_back(j);
            }
        }
   }
//-------------------------End Finding depenancy list---------------------//

//-------------------------Initializing slack asn schedule status---------------------//
   for(int j= startnode; j<=endnode ; j++)
   {
        equations[j].listr_sch_status = 0;
        equations[j].listr_end_time = latency;
   }

//-------------------------End Initializing slack asn schedule status---------------------//

   list<int> *U_mul = new list<int>[1];
   list<int> *U_add = new list<int>[1];
   list<int> *U_log = new list<int>[1];
   list<int> *U_reg = new list<int>[1];

   for(int l=1 ; l<= latency ; l++)
   {

        for(int j= startnode; j<=endnode ; j++)
        {
            equations[j].slack_time = equations[j].alap_start_time-l;
        }
 //-------------------------- Finding U array------------------------------------------//
       for(int k= startnode; k<=endnode ; k++)
       {
          int schedule = 1;
          if(equations[k].listr_sch_status == 0)
          {
              if(equations[k].cdfg_start_time <= l)
              {
                 if( !strcmp(equations[k].opr1, "*") )
                 {
                    list<int>::iterator u;
                    for (u = dep[k].begin(); u != dep[k].end(); ++u)
                    {
                        if( (equations[*u].listr_sch_status == 0) || (equations[*u].listr_end_time > l))
                        {
                            schedule = 0;
                        }
                    }
                    if (schedule == 1)
                    {
                        U_mul[0].push_back(k);
                    }
                 }
                 else if( !strcmp(equations[k].opr1, "+") || !strcmp(equations[k].opr1, "-"))
                 {
                    list<int>::iterator v;
                    for (v = dep[k].begin(); v != dep[k].end(); ++v)
                    {
                        if( (equations[*v].listr_sch_status == 0) || (equations[*v].listr_end_time >= l))
                        {
                            schedule = 0;
                        }
                    }
                    if (schedule == 1)
                    {
                        U_add[0].push_back(k);
                    }
                 }
                 else if( !strcmp(equations[k].opr1, ">") || !strcmp(equations[k].opr1 ,"<") || !strcmp(equations[k].opr1,"==") || !strcmp(equations[k].opr1 ,"?") || !strcmp(equations[k].opr1 ,">>") || !strcmp(equations[k].opr1 ,"<<"))
                 {
                    list<int>::iterator x;
                    for (x = dep[k].begin(); x != dep[k].end(); ++x)
                    {
                        if( (equations[*x].listr_sch_status == 0) || (equations[*x].listr_end_time >= l))
                        {
                            schedule = 0;
                        }
                    }
                    if (schedule == 1)
                    {
                        U_log[0].push_back(k);
                    }
                 }
                 else
                 {
                    list<int>::iterator w;
                    for (w = dep[k].begin(); w != dep[k].end(); ++w)
                    {
                        if((equations[*w].listr_sch_status == 0) || (equations[*w].listr_end_time >= l))
                        {
                            schedule = 0;
                        }
                    }
                    if (schedule == 1)
                    {
                        U_reg[0].push_back(k);
                    }
                 }
              }
          }
       }

          list<int>::iterator m;
          cout << "U_mul " ;
          for (m = U_mul[0].begin(); m != U_mul[0].end(); ++m)
          {
              cout << *m << " ";
          }
          cout << endl;

          list<int>::iterator n;
          cout << "U_add " ;
          for (n = U_add[0].begin(); n != U_add[0].end(); ++n)
          {
              cout << *n << " ";
          }
          cout << endl;

          list<int>::iterator s;
          cout << "U_log " ;
          for (s = U_log[0].begin(); s != U_log[0].end(); ++s)
          {
              cout << *s << " ";
          }
          cout << endl;

          list<int>::iterator t;
          cout << "U_reg " ;
          for (t = U_reg[0].begin(); t != U_reg[0].end(); ++t)
          {
              cout << *t << " ";
          }
          cout << endl;

          if ((U_mul[0].size()== 0) && (U_add[0].size()== 0) && (U_log[0].size()==0) && (U_reg[0].size()==0))
          {
              state_no_listr = state_no_listr + l - 1;
              mul_qty_global[0].push_back(mul_qty);
              add_qty_global[0].push_back(add_qty);
              log_qty_global[0].push_back(log_qty);
              return state_no_listr;
          }
//-------------------------- End Fining U array------------------------------------------//

//--------------------------Find S array-------------------------------------------------//
        //---S array for Mul----//
        //list<int>::iterator m;
        for (m = U_mul[0].begin(); m != U_mul[0].end(); ++m)
        {
           if (equations[*m].slack_time == 0)
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l+1;
               equations[*m].listr_sch_status = 1;
               local_mul_qty=local_mul_qty+1;
               buffer_mul = buffer_mul-1;
           }
        }
         for (m = U_mul[0].begin(); m != U_mul[0].end(); ++m)
        {
           if ((equations[*m].slack_time != 0) && (buffer_mul > 0))
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l+1;
               equations[*m].listr_sch_status = 1;
               buffer_mul = buffer_mul-1;
           }
        }
        if (mul_qty<local_mul_qty)
        {
            mul_qty = local_mul_qty;
            buffer_mul = local_mul_qty;
        }
         local_mul_qty = 0;
         //------end S arrray for mul---//

         //-----S array for adder----//
        //list<int>::iterator m;
        for (m = U_add[0].begin(); m != U_add[0].end(); ++m)
        {
           if (equations[*m].slack_time == 0)
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l;
               equations[*m].listr_sch_status = 1;
               local_add_qty=local_add_qty+1;
               buffer_add = buffer_add-1;
           }
        }
        for (m = U_add[0].begin(); m != U_add[0].end(); ++m)
        {
           if ((equations[*m].slack_time != 0) && (buffer_add > 0))
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l;
               equations[*m].listr_sch_status = 1;
               buffer_add = buffer_add-1;
               local_add_qty=local_add_qty+1;
           }
        }
        if (add_qty<local_add_qty)
        {
            add_qty = local_add_qty;
            buffer_add = local_add_qty;
        }
         local_add_qty = 0;

        //-------end s array for adder-----//

         //-----S array for logical units----//
        //list<int>::iterator m;
        for (m = U_log[0].begin(); m != U_log[0].end(); ++m)
        {
           if (equations[*m].slack_time == 0)
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l;
               equations[*m].listr_sch_status = 1;
               local_log_qty=local_log_qty+1;
               buffer_log = buffer_log-1;
           }
        }
        for (m = U_log[0].begin(); m != U_log[0].end(); ++m)
        {
           if ((equations[*m].slack_time != 0) && (buffer_log > 0))
           {
               equations[*m].state_no_listr = state_no_listr+l-1;
               equations[*m].listr_start_time = l;
               equations[*m].listr_end_time = l;
               equations[*m].listr_sch_status = 1;
               buffer_log = buffer_log-1;
               local_log_qty=local_log_qty+1;
           }
        }
        if (log_qty<local_log_qty)
        {
            log_qty = local_log_qty;
            buffer_log = local_log_qty;
        }
         local_log_qty = 0;

        //-------end s array for logical unit-----//

         //-------s array for reg-----//
        for (t = U_reg[0].begin(); t != U_reg[0].end(); ++t)
        {
               equations[*t].state_no_listr = state_no_listr+l-1;
               equations[*t].listr_start_time = l;
               equations[*t].listr_end_time = l;
               equations[*t].listr_sch_status = 1;
        }
        //-------end s array for reg-----//

      //----Print s array ---------//
         cout << endl <<" Time slot " << l << endl;
         for( int i = startnode; i <= endnode ; i++)
         {
             if((equations[i].listr_sch_status == 1) && (equations[i].listr_start_time == l))
             {
                 cout << "node " << i << " state number " <<equations[i].state_no_listr << endl ;
             }
         }
      //------end print s array------//
      U_mul[0].clear();
      U_add[0].clear();
      U_log[0].clear();
      U_reg[0].clear();
   }
   state_no_listr = state_no_listr + latency;
   mul_qty_global[0].push_back(mul_qty);
   add_qty_global[0].push_back(add_qty);
   log_qty_global[0].push_back(log_qty);
   return state_no_listr;
}

void find_jump_state_listr(int close_brace_state)
{
    for( int i = close_brace_state; i >=0 ; i--)
    {
        if (!strcmp(equations[i].node_type, "if") || !strcmp(equations[i].node_type, "while"))
            {
                if (equations[i].listr_jmpst_flag == 0)
                {
                    equations[i].listr_jump_state = close_brace_state;
                    equations[i].listr_jmpst_flag = 1;
                    break;
                }
            }
    }
}

void print_listr_verilog()
{
        ofstream verilog_file;
        verilog_file.open("verilog_listr_opfile.v");

        verilog_file <<"// ------------------------------------------------------------------------------------------------- "<< endl;
        verilog_file <<" // Copyright (c) 2013 Pruthvi & Mukunda MJ , University of Arizona" <<endl;
        verilog_file <<" // Permission to copy is granted provided that this header remains" <<endl;
        verilog_file <<" // intact. This software is provided with no warranties" << endl;
        verilog_file <<" // -------------------------------------------------------------------------------------------------" << endl;

        verilog_file << "module" << " " << "HLSM (Clk,Rst,Start,Done" ;
        for (int i = 0; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i];
        }

        for (int j = 0; j< nofop ; j++)
        {
            verilog_file << "," << outputs[j];
        }
        verilog_file << ");"<< "\n" <<endl;

        verilog_file << "input Clk, Rst, Start;"<< endl;
        verilog_file << "output reg Done;" << endl;
        verilog_file << "input[31:0]" << " ";
        verilog_file << inputs[0] ;
        for (int i = 1; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i]  ;
        }
        verilog_file << ";"<< endl;

        verilog_file << "output reg [31:0] " << " ";
        verilog_file << outputs[0] ;
        for (int i = 1; i< nofop ; i++)
        {
            verilog_file << "," << outputs[i] ;
        }
        verilog_file << ";"<< endl;

         if ( nofr > 0)
        {
            verilog_file << "reg[31:0]" << " ";
            verilog_file << reg[0];
            for (int i = 1; i< nofr ; i++)
            {
                verilog_file << "," << reg[i] ;
            }
            verilog_file << ";"<< "\n" << endl;
        }



        verilog_file << "reg [5:0]" << " " << "state" << " " << ";" << endl;

        verilog_file << "parameter" << " " << "start_wait" << " " << "=" << " " << "0" << ";" << endl;

        for (int i=1 ; i <= total_listr_states ; i++)
        {
            verilog_file << "parameter" << " " << "state" <<"_" << i << " " << "=" << " " << i << " ;" << endl;
        }

        verilog_file << "parameter" << " " << "Final = "<< (total_listr_states+1) << " ;" << endl;
        verilog_file<< "\n" << endl;

        verilog_file << "always@" << " " << "(posedge Clk)" << " begin" << endl;
        verilog_file << "if" << " " << "(" << " " << "Rst" << " " << "==" << "1" << " " << ")" << " " << "begin" << endl;
        verilog_file << "state" << " " << "<="<< " " << "start_wait" << " " << ";"<< endl;
        verilog_file << "Done <= 0;" << endl;


        verilog_file << "end" << endl;

        verilog_file<<endl;

        verilog_file<< "else begin" << endl;
        verilog_file<< "case( state )" << endl;

        verilog_file<< "start_wait : begin" << endl;
        verilog_file << "if(start == 1) begin "<<endl;
        verilog_file << "state <= state_1 ;" << endl;
        verilog_file << "end" << endl;

        verilog_file<< "else begin" << endl;
        verilog_file << "state <= start_wait ;" << endl;
        verilog_file << "end" << endl;
        verilog_file << "end" << endl;


     bool print_flag = 0;

     for(int j=1; j<= total_listr_states ; j++ )
        {

            for(int i=0 ; i< node ; i++)
            {
                if( equations[i].state_no_listr == j)
                {
                    if (!strcmp(equations[i].node_type , "while") )
                    {
                         verilog_file << "state_" << j << " : begin" << endl;
                         verilog_file << "while (" << equations[i].condition << " )  begin " <<endl;
                         verilog_file << "state <= state_" << (equations[i].state_no_listr+1) << " ;" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "if (" << equations[i].condition << " == 0 ) begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listr_jump_state+1) << " ;" <<endl;
                         verilog_file << "end" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "state_" << equations[i].listr_jump_state << " : begin" << endl;
                         verilog_file << "state <= state_" << equations[i].state_no_listr << " ;" << endl;
                         verilog_file << "end" << endl;
                         print_flag = 1;
                         break;
                    }

                     else if (!strcmp(equations[i].node_type , "if") )
                    {
                         verilog_file << "state_" << j << " : begin" << endl;
                         verilog_file << "if (" << equations[i].condition <<" != 0 )" << " begin " << endl;
                         verilog_file << "state <= state_" << (equations[i].state_no_listr+1) << " ;" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "else begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listr_jump_state) << " ;" <<endl;
                         verilog_file << "end" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "state_" << equations[i].listr_jump_state << " : begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listr_jump_state+1) << " ;" << endl;
                         verilog_file << "end" << endl;
                         print_flag = 1;
                         break;
                    }

                     else if (!strcmp(equations[i].node_type , "if/while") )
                    {
                        print_flag = 1;
                        break;
                    }
                }
            }

            if (print_flag == 0)
            {
                verilog_file << "state_" << j << " : begin" << endl;
                for(int i=0 ; i< node ; i++)
                {
                    if( equations[i].state_no_listr == j)
                    {
                        if( (equations[i].node_type[0] == 'O'))
                        {
                            verilog_file << equations[i].op <<" = " << equations[i].ip1 << ";" << endl;
                        }

                        else if(!strcmp(equations[i].opr1,"+")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " + " << equations[i].ip2  << ";" << endl;}

                        else if(!strcmp(equations[i].opr1,"-")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " - " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"*")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " * " << equations[i].ip2  << ";"<<endl;}

                        else if(!strcmp(equations[i].opr1,">")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " > " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"<")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " < " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"==")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " == " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"?")) {verilog_file << equations[i].op << " <= " <<  equations[i].ip1 << " ? " << equations[i].ip2 << " : " << equations[i].ip3 << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"<<")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " << " << equations[i].ip2  << ";" << endl;}

                        else { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " >> " << equations[i].ip2  <<";" <<endl;}

                    }

                }
                if ( j< total_listr_states)
                {
                    verilog_file << "state = state_" << j+1 << ";" << endl << "end" <<endl;
                }
            }
            print_flag = 0;
            if( j == total_listr_states)
            {
                verilog_file << "state = Final" << ";" << endl << "end" << endl;
                continue;
            }

    }
        verilog_file << "Final: begin" << endl << "Done <= 1" << endl << " state <= start_wait" << endl << "end" <<endl;
        verilog_file << "endcase" << endl << "end" << endl << "end" <<endl << "endmodule" ;

}

int chk_cond_validity(char *cond_dummy)
{
    return 1;  //Code to be written to check if the coddition inside while or If is valid or not. This is done by checking
                // if the condition variable is in reg field or not.
}
 void create_listl_verilog(int mul, int add, int log)
 {
     int state_no_listl=1;
        for (int i = 0; i<node ; i++)
        {
            if(!strcmp(equations[i].node_type,"if") || !strcmp(equations[i].node_type,"while") || !strcmp(equations[i].node_type,"if/while"))
            {
                if(!strcmp(equations[i].node_type,"if/while"))
                {
                     find_jump_state_listl(state_no_listl);
                }
                startnode = i+1;
                equations[i].state_no_listl = state_no_listl;
                state_no_listl = state_no_listl+1;
                continue;
            }
            if( !strcmp(equations[i+1].node_type,"if") || !strcmp(equations[i+1].node_type,"while") || !strcmp(equations[i+1].node_type,"if/while"))
            {
                endnode = i;
                cdfg_schedule(startnode, endnode);
                find_listl_label(startnode, endnode);
                state_no_listl = find_listl_states(startnode, endnode, state_no_listl,mul, add, log);
                continue;
            }
            if( i == node-1)
            {
                endnode = i;
                cdfg_schedule(startnode, endnode);
                find_listl_label(startnode, endnode);
                state_no_listl = find_listl_states(startnode, endnode, state_no_listl, mul, add, log);
            }
        }
        total_listl_states = state_no_listl-1;
        print_listl_verilog();
 }

 void find_jump_state_listl(int close_brace_state)
{
    for( int i = close_brace_state; i >=0 ; i--)
    {
        if (!strcmp(equations[i].node_type, "if") || !strcmp(equations[i].node_type, "while"))
            {
                if (equations[i].listl_jmpst_flag == 0)
                {
                    equations[i].listl_jump_state = close_brace_state;
                    equations[i].listl_jmpst_flag = 1;
                    break;
                }
            }
    }
}

void find_listl_label(int startnode, int endnode)
{
   for (int i= startnode ; i<= endnode ; i++)
   {
        equations[i].listl_label = 1;
   }

   list<int> *dep = new list<int>[100];
   for (int i= startnode; i<=endnode ;i++)
   {
       for(int j=startnode; j<=endnode ; j++)
        {
            if(j == i) continue;
            if(!strcmp(equations[i].ip1,equations[j].op) || !strcmp(equations[i].ip2,equations[j].op) || !strcmp(equations[i].ip3,equations[j].op))
            {
                dep[i].push_back(j);
            }
        }
   }

   for (int i= endnode ; i>= startnode ; i--)
   {
       list<int>::iterator j;
       for (j = dep[i].begin(); j != dep[i].end(); ++j)
       {
           if (equations[*j].listl_label <= equations[i].listl_label )
              {
                  equations[*j].listl_label = equations[i].listl_label+1 ;
              }
       }
   }
   for (int i= startnode ; i<= endnode ; i++)
   {
        cout << "listl_label for node " << i << " is " << equations[i].listl_label << endl;
   }

}

int find_listl_states(int startnode, int endnode, int state_no_listl, int mul, int add, int log)
{
   int latency = 1;
   int infinity = 1000;
   int buffer_mul = mul;
   int buffer_add = add;
   int buffer_log = log;
   int max_mul_label = 1;
   int max_add_label = 1;
   int max_log_label = 1;
   int max_mul_node ;
   int max_add_node ;
   int max_log_node ;
   bool all_scheduled = 0;
   bool sch_flag = 0;

   //-------------------------Finding depenancy list---------------------//
   list<int> *dep = new list<int>[100];
   for (int i= startnode; i<=endnode ;i++)
   {
       for(int j=startnode; j<=endnode ; j++)
        {
            if(j == i) continue;
            if(!strcmp(equations[i].ip1,equations[j].op) || !strcmp(equations[i].ip2,equations[j].op) || !strcmp(equations[i].ip3,equations[j].op))
            {
                dep[i].push_back(j);
            }
        }
   }
  //-------------------------End Finding depenancy list---------------------//


//-------------------------Initializing schedule status---------------------//
   for(int j= startnode; j<=endnode ; j++)
   {
        equations[j].listl_sch_status = 0;
        equations[j].listl_end_time = infinity;
   }

//-------------------------End Initializing schedule status---------------------//

   list<int> *U_mul = new list<int>[1];
   list<int> *U_add = new list<int>[1];
   list<int> *U_log = new list<int>[1];
   list<int> *U_reg = new list<int>[1];


   while(!all_scheduled)
   {

//-------------------------- Start Fining U array------------------------------------------//
           for(int k= startnode; k<=endnode ; k++)
           {
              int schedule = 1;
              if(equations[k].listl_sch_status == 0)
              {
                  if(equations[k].cdfg_start_time <= latency)
                  {
                     if( !strcmp(equations[k].opr1, "*") )
                     {
                        list<int>::iterator u;
                        for (u = dep[k].begin(); u != dep[k].end(); ++u)
                        {
                            if( (equations[*u].listl_sch_status == 0) || (equations[*u].listl_end_time > latency))
                            {
                                schedule = 0;
                            }
                        }
                        if (schedule == 1)
                        {
                            U_mul[0].push_back(k);
                        }
                     }
                     else if( !strcmp(equations[k].opr1, "+") || !strcmp(equations[k].opr1, "-"))
                     {
                        list<int>::iterator v;
                        for (v = dep[k].begin(); v != dep[k].end(); ++v)
                        {
                            if( (equations[*v].listl_sch_status == 0) || (equations[*v].listl_end_time >= latency))
                            {
                                schedule = 0;
                            }
                        }
                        if (schedule == 1)
                        {
                            U_add[0].push_back(k);
                        }
                     }
                     else if( !strcmp(equations[k].opr1, ">") || !strcmp(equations[k].opr1 ,"<") || !strcmp(equations[k].opr1,"==") || !strcmp(equations[k].opr1 ,"?") || !strcmp(equations[k].opr1 ,">>") || !strcmp(equations[k].opr1 ,"<<"))
                     {
                        list<int>::iterator x;
                        for (x = dep[k].begin(); x != dep[k].end(); ++x)
                        {
                            if( (equations[*x].listl_sch_status == 0) || (equations[*x].listl_end_time >= latency))
                            {
                                schedule = 0;
                            }
                        }
                        if (schedule == 1)
                        {
                            U_log[0].push_back(k);
                        }
                     }
                     else
                     {
                        list<int>::iterator w;
                        for (w = dep[k].begin(); w != dep[k].end(); ++w)
                        {
                            if((equations[*w].listl_sch_status == 0) || (equations[*w].listl_end_time >= latency))
                            {
                                schedule = 0;
                            }
                        }
                        if (schedule == 1)
                        {
                            U_reg[0].push_back(k);
                        }
                     }
                  }
              }
           }

          list<int>::iterator m;
          cout << "U_mul " ;
          for (m = U_mul[0].begin(); m != U_mul[0].end(); ++m)
          {
              cout << *m << " ";
          }
          cout << endl;

          list<int>::iterator n;
          cout << "U_add " ;
          for (n = U_add[0].begin(); n != U_add[0].end(); ++n)
          {
              cout << *n << " ";
          }
          cout << endl;

          list<int>::iterator s;
          cout << "U_log " ;
          for (s = U_log[0].begin(); s != U_log[0].end(); ++s)
          {
              cout << *s << " ";
          }
          cout << endl;

          list<int>::iterator t;
          cout << "U_reg " ;
          for (t = U_reg[0].begin(); t != U_reg[0].end(); ++t)
          {
              cout << *t << " ";
          }
          cout << endl;

          if ((U_mul[0].size()== 0) && (U_add[0].size()== 0) && (U_log[0].size()==0) && (U_reg[0].size()==0))
          {
              state_no_listl = state_no_listl + latency -1;
              return state_no_listl;
          }

//-------------------------- End Fining U array------------------------------------------//

//--------------------------Find S array-------------------------------------------------//
        list<int>::iterator p;
        //---S array for Mul----//
        for (m = U_mul[0].begin(); m != U_mul[0].end(); ++m)
        {
           for (p = U_mul[0].begin(); p != U_mul[0].end(); ++p)
           {
                if (equations[*p].listl_sch_status == 1) continue;
                if ( max_mul_label <= equations[*p].listl_label)
                {
                    max_mul_label = equations[*p].listl_label;
                    max_mul_node = *p;
                }
           }

           equations[max_mul_node].state_no_listl = state_no_listl+latency-1;
           equations[max_mul_node].listl_start_time = latency;
           equations[max_mul_node].listl_end_time = latency+1;
           equations[max_mul_node].listl_sch_status = 1;
           buffer_mul = buffer_mul-1;

           max_mul_label = 1;
           if (buffer_mul == 0)
           {
               break;
           }

        }
        buffer_mul = mul;
         //------end S arrray for mul---//

         //---S array for Add----//
        for (m = U_add[0].begin(); m != U_add[0].end(); ++m)
        {
           for (p = U_add[0].begin(); p != U_add[0].end(); ++p)
           {
                if (equations[*p].listl_sch_status == 1) continue;
                if ( max_add_label <= equations[*p].listl_label)
                {
                    max_add_label = equations[*p].listl_label;
                    max_add_node = *p;
                }
           }

           equations[max_add_node].state_no_listl = state_no_listl+latency-1;
           equations[max_add_node].listl_start_time = latency;
           equations[max_add_node].listl_end_time = latency;
           equations[max_add_node].listl_sch_status = 1;
           buffer_add = buffer_add-1;

           max_add_label = 1;
           if (buffer_add == 0)
           {
               break;
           }

        }
        buffer_add = add;
         //------end S arrray for add---//

          //---S array for log----//
        for (m = U_log[0].begin(); m != U_log[0].end(); ++m)
        {
           for (p = U_log[0].begin(); p != U_log[0].end(); ++p)
           {
                if (equations[*p].listl_sch_status == 1) continue;
                if ( max_log_label <= equations[*p].listl_label)
                {
                    max_log_label = equations[*p].listl_label;
                    max_log_node = *p;
                }
           }

           equations[max_log_node].state_no_listl = state_no_listl+latency-1;
           equations[max_log_node].listl_start_time = latency;
           equations[max_log_node].listl_end_time = latency;
           equations[max_log_node].listl_sch_status = 1;
           buffer_log = buffer_log-1;

           max_log_label = 1;
           if (buffer_log == 0)
           {
               break;
           }
        }
        buffer_log = log;
         //------end S arrray for log---//

//-------s array for reg-----//
        for (t = U_reg[0].begin(); t != U_reg[0].end(); ++t)
        {
               equations[*t].state_no_listl = state_no_listl+latency-1;
               equations[*t].listl_start_time = latency;
               equations[*t].listl_end_time = latency;
               equations[*t].listl_sch_status = 1;
        }
//-------end s array for reg-----//

       for(int i = startnode ; i<= endnode ; i++)
       {
           if(equations[i].listl_sch_status == 0)
           {
                sch_flag = 1;
           }

           if( sch_flag = 1) all_scheduled = 0;
           else all_scheduled = 1;
           sch_flag = 0;
       }
//----Print s array ---------//
         cout << endl <<" Time slot " << latency << endl;
         for( int i = startnode; i <= endnode ; i++)
         {
             if((equations[i].listl_sch_status == 1) && (equations[i].listl_start_time == latency))
             {
                 cout << "node " << i << " state number , st time , end time is " << equations[i].listl_start_time << " " << equations[i].listl_end_time  << " " << equations[i].state_no_listl << endl ;
             }
         }
//------end print s array------//

      U_mul[0].clear();
      U_add[0].clear();
      U_log[0].clear();
      U_reg[0].clear();
      latency++;

    }
    state_no_listl = state_no_listl + latency -1;
    return state_no_listl;
}

void print_listl_verilog()
{
        ofstream verilog_file;
        verilog_file.open("verilog_listl_opfile.v");

        verilog_file <<"// ------------------------------------------------------------------------------------------------- "<< endl;
        verilog_file <<" // Copyright (c) 2013 Pruthvi & Mukunda MJ , University of Arizona" <<endl;
        verilog_file <<" // Permission to copy is granted provided that this header remains" <<endl;
        verilog_file <<" // intact. This software is provided with no warranties" << endl;
        verilog_file <<" // -------------------------------------------------------------------------------------------------" << endl;

        verilog_file << "module" << " " << "HLSM (Clk,Rst,Start,Done" ;
        for (int i = 0; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i];
        }

        for (int j = 0; j< nofop ; j++)
        {
            verilog_file << "," << outputs[j];
        }
        verilog_file << ");"<< "\n" <<endl;

        verilog_file << "input Clk, Rst, Start;"<< endl;
        verilog_file << "output reg Done;" << endl;
        verilog_file << "input[31:0]" << " ";
        verilog_file << inputs[0] ;
        for (int i = 1; i< nofip ; i++)
        {
            verilog_file << "," << inputs[i]  ;
        }
        verilog_file << ";"<< endl;

        verilog_file << "output reg [31:0] " << " ";
        verilog_file << outputs[0] ;
        for (int i = 1; i< nofop ; i++)
        {
            verilog_file << "," << outputs[i] ;
        }
        verilog_file << ";"<< endl;

         if ( nofr > 0)
        {
            verilog_file << "reg[31:0]" << " ";
            verilog_file << reg[0];
            for (int i = 1; i< nofr ; i++)
            {
                verilog_file << "," << reg[i] ;
            }
            verilog_file << ";"<< "\n" << endl;
        }



        verilog_file << "reg [5:0]" << " " << "state" << " " << ";" << endl;

        verilog_file << "parameter" << " " << "start_wait" << " " << "=" << " " << "0" << ";" << endl;

        for (int i=1 ; i <= total_listl_states ; i++)
        {
            verilog_file << "parameter" << " " << "state" <<"_" << i << " " << "=" << " " << i << " ;" << endl;
        }

        verilog_file << "parameter" << " " << "Final = "<< (total_listl_states+1) << " ;" << endl;
        verilog_file<< "\n" << endl;

        verilog_file << "always@" << " " << "(posedge Clk)" << " begin" << endl;
        verilog_file << "if" << " " << "(" << " " << "Rst" << " " << "==" << "1" << " " << ")" << " " << "begin" << endl;
        verilog_file << "state" << " " << "<="<< " " << "start_wait" << " " << ";"<< endl;
        verilog_file << "Done <= 0;" << endl;


        verilog_file << "end" << endl;

        verilog_file<<endl;

        verilog_file<< "else begin" << endl;
        verilog_file<< "case( state )" << endl;

        verilog_file<< "start_wait : begin" << endl;
        verilog_file << "if(start == 1) begin "<<endl;
        verilog_file << "state <= state_1 ;" << endl;
        verilog_file << "end" << endl;

        verilog_file<< "else begin" << endl;
        verilog_file << "state <= start_wait ;" << endl;
        verilog_file << "end" << endl;
        verilog_file << "end" << endl;


     bool print_flag = 0;

     for(int j=1; j<= total_listl_states ; j++ )
        {

            for(int i=0 ; i< node ; i++)
            {
                if( equations[i].state_no_listl == j)
                {
                    if (!strcmp(equations[i].node_type , "while") )
                    {
                         verilog_file << "state_" << j << " : begin" << endl;
                         verilog_file << "while (" << equations[i].condition << " )  begin " <<endl;
                         verilog_file << "state <= state_" << (equations[i].state_no_listl+1) << " ;" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "if (" << equations[i].condition << " == 0 ) begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listl_jump_state+1) << " ;" <<endl;
                         verilog_file << "end" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "state_" << equations[i].listl_jump_state << " : begin" << endl;
                         verilog_file << "state <= state_" << equations[i].state_no_listl << " ;" << endl;
                         verilog_file << "end" << endl;
                         print_flag = 1;
                         break;
                    }

                     else if (!strcmp(equations[i].node_type , "if") )
                    {
                         verilog_file << "state_" << j << " : begin" << endl;
                         verilog_file << "if (" << equations[i].condition <<" != 0 )" << " begin " << endl;
                         verilog_file << "state <= state_" << (equations[i].state_no_listl+1) << " ;" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "else begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listl_jump_state) << " ;" <<endl;
                         verilog_file << "end" << endl;
                         verilog_file << "end" << endl;

                         verilog_file << "state_" << equations[i].listl_jump_state << " : begin" << endl;
                         verilog_file << "state <= state_" << (equations[i].listl_jump_state+1) << " ;" << endl;
                         verilog_file << "end" << endl;
                         print_flag = 1;
                         break;
                    }

                     else if (!strcmp(equations[i].node_type , "if/while") )
                    {
                        print_flag = 1;
                        break;
                    }
                }
            }

            if (print_flag == 0)
            {
                verilog_file << "state_" << j << " : begin" << endl;
                for(int i=0 ; i< node ; i++)
                {
                    if( equations[i].state_no_listl == j)
                    {
                        if( (equations[i].node_type[0] == 'O'))
                        {
                            verilog_file << equations[i].op <<" = " << equations[i].ip1 << ";" << endl;
                        }

                        else if(!strcmp(equations[i].opr1,"+")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " + " << equations[i].ip2  << ";" << endl;}

                        else if(!strcmp(equations[i].opr1,"-")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " - " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"*")){ verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " * " << equations[i].ip2  << ";"<<endl;}

                        else if(!strcmp(equations[i].opr1,">")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " > " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"<")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " < " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"==")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " == " << equations[i].ip2  << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"?")) {verilog_file << equations[i].op << " <= " <<  equations[i].ip1 << " ? " << equations[i].ip2 << " : " << equations[i].ip3 << ";" <<endl;}

                        else if(!strcmp(equations[i].opr1,"<<")) { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " << " << equations[i].ip2  << ";" << endl;}

                        else { verilog_file <<equations[i].op << " <= " << equations[i].ip1 << " >> " << equations[i].ip2  <<";" <<endl;}

                    }

                }
                if ( j< total_listl_states)
                {
                    verilog_file << "state = state_" << j+1 << ";" << endl << "end" <<endl;
                }
            }
            print_flag = 0;
            if( j == total_listl_states)
            {
                verilog_file << "state = Final" << ";" << endl << "end" << endl;
                continue;
            }

    }
        verilog_file << "Final: begin" << endl << "Done <= 1" << endl << "state = start_wait" << endl << "end" <<endl;
        verilog_file << "endcase" << endl << "end" << endl << "end" <<endl << "endmodule" ;

}

int main(int argc , char** argv)
{
	char algo[10] = "-listl";
	int latency = 3;
	int mul = 2;
	int add = 2;
	int log = 2;
	int netfile = getnetlistfile();
	if (!netfile)
	  {
		cerr << "Error in Parsing netlist File" << endl;
		exit(-1);
	  }
    if(!strcmp(algo, "-ns"))
    {
        create_ns_verilog();
    }
     else if(!strcmp(algo, "-listl"))
    {
        create_listl_verilog(mul,add,log);
    }
    else if(!strcmp(algo, "-listr"))
    {
        create_listr_verilog(latency);
    }

return 0;

}

Conversation opened. 1 read message.
Displaying Version 17 ns,ListR,Listl.txt.
