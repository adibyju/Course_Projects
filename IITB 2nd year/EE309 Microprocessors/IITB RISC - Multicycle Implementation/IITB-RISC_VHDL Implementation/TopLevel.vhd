library ieee;
use ieee.std_logic_1164.all;

entity processor is
port( clk, rst : in std_logic);
end entity;


architecture arch of processor is 

	
	component Memory is
		port (	
			rst, clk, write_enabler, read_enabler: in std_logic;
			address, write_data : in std_logic_vector (15 downto 0);
			data_out: out std_logic_vector (15 downto 0)
			);
	end component;
	
	component IR is
		port (	
			clk, rst, write_REG: in std_logic;
			inp: in std_logic_vector (15 downto 0);
			outp: out std_logic_vector (15 downto 0)
			);
	end component;

	component RF is
		port (	
			clk, rst, write_Reg, PC_EN: in std_logic;
			RA1, RA2, RA3: in std_logic_vector (2 downto 0);
			write_data ,R7_write_data: in std_logic_vector (15 downto 0);
			outputA, outputB, R7outp: out std_logic_vector (15 downto 0)
			);
	end component;

	component alu is
		port (
			clk: in std_logic;
			op:    in std_logic_vector(1 downto 0);
			a,b:    in std_logic_vector(15 downto 0);
			result: out std_logic_vector(15 downto 0);   
			out_carry, out_zero: out std_logic
		);
	end component;

	component control_system is
		port (	
			clk, rst, z_in, c_in, z2: in std_logic;
			cond: in std_logic_vector(1 downto 0);
			op_code: in std_logic_vector(3 downto 0);
			MR, MW, IR_EN,PC_EN,PC_Mux,RF_EN,RF_A1_Mux,T1_Mux,T2_Mux,T4_Mux,T5_Mux,T5_EN: out std_logic;
			MEM_Addr_Mux,ALU_A_Mux,ALU_B_Mux,ALU_Op,RF_A3_Mux,RF_D3_mux: out std_logic_vector(1 downto 0)
			);
	end component;
	
	component SE6 is
		generic(input_width: integer := 6;
			output_width: integer := 16);
		port(
			input: in std_logic_vector(input_width-1 downto 0);
			output: out std_logic_vector(output_width-1 downto 0));
	end component;

	component SE9 is
		generic(input_width: integer := 9;
			output_width: integer := 16);
		port(
			input: in std_logic_vector(input_width-1 downto 0);
			output: out std_logic_vector(output_width-1 downto 0));
	end component;

	component LS1 is
		port(
			inp: in std_logic_vector(15 downto 0);
			outp: out std_logic_vector(15 downto 0));
	end component;

	component LS7 is
		port(
			inp: in std_logic_vector(15 downto 0);
			outp: out std_logic_vector(15 downto 0));
	end component;

	component PE is
		port (
		  pe_in : in std_logic_vector(7 downto 0);
		  pe_out1 : out std_logic_vector(2 downto 0);
		  pe_out2 : out std_logic_vector(7 downto 0);
		  z2: out std_logic
		) ;
	end component ;

	component T5 is
		port (	
			clk,WE: in std_logic;
			inp: in std_logic_vector (15 downto 0);
			outp: out std_logic_vector (15 downto 0)
			);
	end component;



	component Reg is
		generic(N: integer);
    port (	
		clk: in std_logic;
		inp: in std_logic_vector (N-1 downto 0);
		outp: out std_logic_vector (N-1 downto 0)
	);
	end component;

	component mux4 is
		generic(N: integer);
 port(
 
     A,B,C,D : in std_logic_vector (N-1 downto 0);
     S0,S1: in STD_LOGIC;
     Z: out std_logic_vector (N-1 downto 0)
  );
	end component;

	
	
	component mux2 is
		generic (N: integer);
 port(
 
     A,B : in std_logic_vector (N-1 downto 0);
     S: in STD_LOGIC;
     Z: out std_logic_vector (N-1 downto 0)
  );
	end component;




	signal z2, MR, MW, IR_EN,PC_EN,PC_Mux,RF_EN,RF_A1_Mux,T1_Mux,T2_Mux,T4_Mux,T5_Mux,T5_EN: std_logic;
	signal MEM_Addr_Mux,ALU_A_Mux,ALU_B_Mux,ALU_Op,RF_A3_Mux,RF_D3_mux: std_logic_vector (1 downto 0);
	


	signal CY, ZR : std_logic;
	
	signal X: std_logic_vector (15 downto 0);
	signal PO_1, T6_out: std_logic_vector (2 downto 0);

	signal MEM_D_out, MEM_A: std_logic_vector (15 downto 0);
	signal IR_outp, DR_outp: std_logic_vector (15 downto 0);
	signal PO_2,PE_mux_outp,PEreg_outp: std_logic_vector (7 downto 0);
	signal RF_A3, RF_A1: std_logic_vector (2 downto 0);
	signal RF_WData, R7_WData, RF_outpA, RF_outpB, R7outp, T1_outp_LS1: std_logic_vector (15 downto 0);
	signal T1_inp, T1_outp, T2_inp, T2_outp, T3_outp, IR0_5_SE, IR0_8_SE, IR0_8_SE_LS7: std_logic_vector (15 downto 0);
	signal ALU_inA, ALU_inB, ALU_outp: std_logic_vector (15 downto 0);
	signal T5_out, T5_inp: std_logic_vector (15 downto 0);
	
begin

	--control_system
	control_systemport : control_system port map (clk, rst, ZR, CY, z2,
    IR_outp(1 downto 0),
    IR_outp(15 downto 12),
	MR, MW, IR_EN,PC_EN,PC_Mux,RF_EN,RF_A1_Mux,T1_Mux,T2_Mux,T4_Mux,T5_Mux,T5_EN,
	MEM_Addr_Mux,ALU_A_Mux,ALU_B_Mux,ALU_Op,RF_A3_Mux,RF_D3_mux);
	-- 2to1muxes
	A1mux: mux2 generic map(3) port map (IR_outp(11 downto 9), PO_1, RF_A1_Mux, RF_A1); --"000" = T6
	PE_inpMux: mux2 generic map(8) port map(PO_2,IR_outp(7 downto 0),T4_Mux,PE_mux_outp);
	R7_WDataMux:mux2 generic map(16) port map (ALU_outp, T2_outp, PC_Mux, R7_WData);
	T1_inpMux:mux2 generic map(16) port map (RF_outpA, RF_outpB, T1_Mux, T1_inp);
	T2_inpMux:mux2 generic map(16) port map (IR0_5_SE, RF_outpB, T2_Mux, T2_inp);
	Addr_incrMux:mux2 generic map(16) port map (RF_outpA, ALU_outp, T5_Mux, T5_inp);
	-- 4to1muxes
	A3mux: mux4 generic map(3) 	port map (IR_outp(11 downto 9), IR_outp(8 downto 6), IR_outp(5 downto 3), T6_out, RF_A3_Mux(1), RF_A3_Mux(0), RF_A3);
	MemAddrMux: mux4 generic map(16) port map (T5_out, R7outp ,T3_outp , X, MEM_Addr_Mux(1), MEM_Addr_Mux(0), MEM_A);
	RF_WDataMux: mux4 generic map(16) port map (DR_outp, R7outp, IR0_8_SE_LS7, T3_outp, RF_D3_mux(1), RF_D3_mux(0), RF_WData);
	ALU_AMux: mux4 generic map(16) port map (T1_outp, T5_out, R7outp, T1_outp_LS1, ALU_A_Mux(1), ALU_A_Mux(0), ALU_inA);
	ALU_BMux: mux4 generic map(16) port map (T2_outp, IR0_5_SE, IR0_8_SE, "0000000000000001", ALU_B_Mux(1), ALU_B_Mux(0), ALU_inB);
	--memory
	Memport : Memory port map (rst, clk, MW, MR, MEM_A, T1_outp, MEM_D_out);
	--registerfile
	RFport : RF port map (clk, rst, RF_EN, PC_EN, RF_A1, IR_outp(8 downto 6), RF_A3, RF_WData, R7_WData, RF_outpA, RF_outpB, R7outp);
	--ALU
	ALUport : ALU port map (clk, ALU_Op, ALU_inA, ALU_inB, ALU_outp, CY, ZR);
	--Priority Encoder
	pe_encoder_port: PE port map(PEreg_outp, PO_1, PO_2, z2);
	
	LS1_port: LS1 port map(T1_outp,T1_outp_LS1);
	LS7_port: LS7 port map(IR0_8_SE,IR0_8_SE_LS7);
	Instr0_5SE :SE6 port map(IR_outp(5 downto 0),IR0_5_SE);
	Instr0_8SE :SE9 port map(IR_outp(8 downto 0),IR0_8_SE);
	
	IRport : IR port map (clk, rst, IR_EN, MEM_D_out, IR_outp);

	DRport : Reg generic map(16)
	port map (clk, MEM_D_out, DR_outp);
	RegAport : Reg generic map(16) port map (clk, T1_inp, T1_outp);
	RegBport : Reg generic map(16) port map (clk, T2_inp, T2_outp);
	RegALUport : Reg generic map(16) port map (clk, ALU_outp, T3_outp);
	
	Addr_inc_port : T5 port map (clk, T5_EN, T5_inp, T5_out);
	
	PE_inpRegport : Reg generic map(8) port map(clk, PE_mux_outp, PEreg_outp);
	PE_outpRegport : Reg generic map(3) port map(clk, PO_1, T6_out);

	




	







	-- RF inputs






	-- ALU inps

	


	-- RegALU inps



	
end architecture;

