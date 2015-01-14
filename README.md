# HLSM-Verilog-code-generation-from-C-file

In this assignment, we will design and implement a high-level synthesis tool capable of creating a synthesizable 
high-level state machine (HLSM) description in Verilog from a C-like sequential program supporting conditional 
and loop constructs while providing various scheduling alternatives. 

Your program must be capable of utilizing commandline arguments specifying the location of the input and output files, 
several option flags indicating which scheduling algorithm should be utilized in synthesizing the C-like sequential 
program input, as well as a latency constraint when appropriate. The following provides an example of the acceptable 
commandline arguments:

>hlsyn -ns cfile verilogfile

>hlsyn -listl cfile verilogfile mul add logic

>hlsyn -listr cfile verilogfile latency

>hlsyn -help

## Verilog Code Generation

    >The resulting high-level state machine should include an initial Wait state and a final Final state used for controlling the execution of the resulting hardware description.
    The Wait state will wait for the Start input to be 1 before proceeding to execute the synthesized statements from the C-like sequential program.
    After the HLSM has executed the synthesized statements from the C-like sequential program, the Final state will assert the Done output to 1 for one cycle before returning to the Wait state.
    All inputs and outputs should be included in the model declaration in the order specified within the C-like sequential program.
    All input, outputs, and internal registers declared within the C-like sequential program are assumed to be 32-bit vectors.
    All outputs and internal registers should be declared as reg variables/outputs in the Verilog description.
    A single State register should be declared as a reg vector using the fewest number of bits possible to implement the resulting HLSM.
    The HLSM description should consist of a single procedure sensitive only to posedge Clk.
    Upon a reset indicated by the Rst input, all outputs and internal registers should be set to 0 and the State register should be assigned to the Wait state.
    The generated Verilog code must be synthesizable using Xilinx ISE 13.2. 
    
## Scheduling Algorithms

 program must support the scheduling options described below. The scheduling algorithm will be utilized to schedule all 
 C-like sequential statements, but will not be utilized to synthesize the conditional and loop statements themselves.

> No Scheduling Algorithm ("-ns")

If the "-ns" commandline option is specified, your program should create a one procedure high-level state machine description (HLSM) that executes one sequential statement defined within the C-like sequential per clock cycle.

    All operations are handled by one "black box" resource (includes, multiplier, adder/subtractor, and logic/logical operations)
    All resources are assumed to have a 1 cycle delay. 


> List_L ("-listl")

If the "-listl" commandline option is specified, you program should create a one procedure high-level state machine 
description (HLSM) such that all sequential statements, including those contained within each conditional or loop statement,
are scheduled using the List_L scheduling algorithm, such that:

    Available resource types include, multiplier, adder/subtractor, and logic/logical.
    The multiplier resource only supports multiplication.
    The adder/subtractor resource supports addition and subtraction.
    The logic/logical resource type supports all other operations, including comparisons, shift, and ternary operators.
    Multiplier resources are assumed to have a 2 cycle delay, all other resources are assumed to have a 1 cycle delay.
    The user should be able to specify the resource constraints for each resource type using commandline options. 


> List_R ("-listr") 

If the "-listr" commandline option is specified, you program should create a one procedure high-level state machine
description (HLSM) such that all sequential statements, including those contained within each conditional or loop statement,
are scheduled  using the List_R scheduling algorithm, such that:

    Available resource types include, multiplier, adder/subtractor, and logic/logical.
    The multiplier resource only supports multiplication.
    The adder/subtractor resource supports addition and subtraction.
    The logic/logical resource type supports all other operations, including comparisons, shift, and ternary operators.
    Multiplier resources are assumed to have a 2 cycle delay, all other resources are assumed to have a 1 cycle delay.
    The user should be able to specify the latency constraint as a commandline option. 
