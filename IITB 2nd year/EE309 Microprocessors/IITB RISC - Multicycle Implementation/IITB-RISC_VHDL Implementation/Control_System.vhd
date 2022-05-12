library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity control_system is
port (	
	clk, rst, z_in, c_in, z2: in std_logic;
    cond: in std_logic_vector(1 downto 0);
    op_code: in std_logic_vector(3 downto 0);
	MR, MW, IR_EN,PC_EN,PC_Mux,RF_EN,RF_A1_Mux,T1_Mux,T2_Mux,T4_Mux,T5_Mux,T5_EN: out std_logic;
	MEM_Addr_Mux,ALU_A_Mux,ALU_B_Mux,ALU_Op,RF_A3_Mux,RF_D3_mux: out std_logic_vector(1 downto 0)
	);
end entity;

architecture control_system_logic of control_system is

type FsmState is (S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,S17,S18,S19,S20,S21,S22,S23,S24);
signal Q: FsmState;
signal z,c: std_logic;
begin
-- signal Q:std_logic_vector (4 downto 0):= "00000";
-- signal NQ:std_logic_vector (4 downto 0):= "00000";



	state_machine:process(clk)
	variable NQ: FsmState;

	begin
		if rising_edge(clk) then
			if rst ='1' then
				Q<=S0;
				NQ:=S0;
			else
				if Q=S0 then
					NQ:=S1;

				elsif Q=S1 then
					if op_code="0000" then--ADI
						NQ:=S6;
					elsif op_code="0100" then--LHI
						NQ:=S8;
					elsif op_code="0111" or op_code="0101" then--LW or SW
						NQ:=S9;
					elsif op_code="1001" or op_code="1010" then--JAL or JLR
						NQ:=S14;
					elsif op_code="1100" or op_code="1101" then--LM or SM
						NQ:=S20;
					elsif op_code="0001" then--ADD ADZ ADC ADL
						if cond="00" then--ADD
							NQ:=S2;
						elsif cond="01" then--ADZ
							if z='1' then
								NQ:=S2;
							else
								NQ:=S0;
							end if;
						elsif cond="10" then--ADC
							if c='1' then
								NQ:=S2;
							else
								NQ:=S0;
							end if;
						elsif cond="11" then--ADL
								NQ:=S2;
						end if;
					elsif op_code="0010" then--NDU NDZ NDC
						if cond="00" then--NDU
							NQ:=S2;
						elsif cond="01" then--NDZ
							if z='1' then
								NQ:=S2;
							else
								NQ:=S0;
							end if;
						elsif cond="10" then--NDC
							if c='1' then
								NQ:=S2;
							else
								NQ:=S0;
							end if;
						end if;
					else
						NQ:=S2;
					end if;

				elsif Q=S2 then
					if op_code="0010" or op_code="1000" then--NDU,NDC,NDZ or BEQ
						NQ:=S7;
					elsif op_code="1010" then--JLR
						NQ:=S16;
					elsif op_code="1011" then--JRI
						NQ:=S17;
					else
						NQ:=S3;
					end if;
				elsif Q=S3 then
					--modify c,z
					if op_code="0000" then --ADI
						NQ:=S5;
					else
						NQ:=S4;
					end if;
				elsif Q=S4 then
					NQ:=S0;
				elsif Q=S5 then
					NQ:=S0;
				elsif Q=S6 then
					NQ:=S3;
				elsif Q=S7 then
					--modify z
					--assert enters ifs
					if op_code="0101" then--LW
						NQ:=S10;
					elsif op_code="0111" then--SW
						NQ:=S11;
					elsif op_code="1000" then--BEQ
						if z_in='1' then
							NQ:=S12;
						else
							NQ:=S0;
						end if;
					elsif op_code="0010" then --NDU,NDC,NDZ
						NQ:=S4;
					end if;
				elsif Q=S8 then
					NQ:=S0;
				elsif Q=S9 then
					if op_code="0101" then
						NQ:=S11;
					else
						NQ:=S7;
					end if;
				elsif Q=S10 then
					NQ:=S18;
				elsif Q=S11 then
					NQ:=S19;
				elsif Q=S12 then
					NQ:=S13;
				elsif Q=S13 then
					NQ:=S0;
				elsif Q=S14 then
					--assert enters ifs
					if op_code="1001" then
						NQ:=S15;
					elsif op_code="1010" then
						NQ:=S2;
					end if;
				elsif Q=S15 then
					NQ:=S0;
				elsif Q=S16 then
					NQ:=S0;
				elsif Q=S17 then
					NQ:=S0;
				elsif Q=S18 then
					NQ:=S0;
				elsif Q=S19 then
					NQ:=S0;
				elsif Q=S20 then
					if op_code="1100" then--LM
						NQ:=S21;
					elsif op_code="1101" then--SM
						NQ:=S23;
					end if;
				elsif Q=S21 then
					if z2 ='0' then
						NQ:=S22;
					else
						NQ:=S0;
					end if;
				elsif Q=S22 then
					if z2 ='0' then
						NQ:=S22;
					else
						NQ:=S0;
					end if;
				elsif Q=S23 then
					if z2 ='0' then
						NQ:=S24;
					else
						NQ:=S0;
					end if;
				elsif Q=S24 then
					if z2 ='0' then
						NQ:=S24;
					else
						NQ:=S0;
					end if;	
				end if;
			end if;--rst
			Q<=NQ;
		end if;--clk
	end process;

	process(clk)
	begin
		
		if Q=S0 then
			--Read instr from Mem
			MW<='0';
			MR<='1';
			MEM_Addr_Mux<="01";--R7
			IR_EN<='1';

			--store R7+1 in R7
			PC_EN<='1';
			ALU_A_Mux<="10";--R7
			ALU_B_Mux<="11";--+1
			ALU_Op<="10";--add
			PC_Mux<='0';--ALUout
			
			RF_EN<='0';

			T5_EN<='1';

		elsif Q=S1 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";

		elsif Q=S2 then
			--RegRead
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='1';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		
		elsif Q=S3 then
			--ALUadd
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='1';
			if cond="11" then
				ALU_A_Mux<="11";
			else
				ALU_A_Mux<="00";
			end if;
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
			z<=z_in;
			c<=c_in;
		
		elsif Q=S4 then
			T5_EN<='1';



			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='1';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="10"; --IR3-5
			RF_D3_mux<="11";--ALUout
		
		elsif Q=S5 then
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='1';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="01"; --IR6-8
			RF_D3_mux<="11";--ALUout

		elsif Q=S6 then
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S7 then
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";--T1
			ALU_B_Mux<="00";--T2
			if op_code="1000" then
				ALU_Op<="01";--sub
			elsif op_code="0101" or op_code="0111" then
				ALU_Op<="10";--add
			else
				ALU_Op<="00";--nand
			end if;
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
			if op_code="0111" then
			else
			z<=z_in;
			end if;
		elsif Q=S8 then
			T5_EN<='1';

			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='0';
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";--IR9-11SELS
			RF_D3_mux<="10";--IR0-8SELS
		elsif Q=S9 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='1';--RegB
			T2_Mux<='0';--IR0-5SE
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S10 then
			T5_EN<='1';
			MR<='1';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="10";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="00";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S11 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="10";
			ALU_A_Mux<="00";
			ALU_B_Mux<="00";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S12 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="11";
			ALU_A_Mux<="10";
			ALU_B_Mux<="01";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S13 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="11";
			ALU_A_Mux<="10";
			ALU_B_Mux<="11";
			ALU_Op<="01";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S14 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="11";
			ALU_A_Mux<="10";
			ALU_B_Mux<="11";
			ALU_Op<="01";
			RF_A3_Mux<="00";
			RF_D3_mux<="01";
		elsif Q=S15 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="11";
			ALU_A_Mux<="10";
			ALU_B_Mux<="10";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S16 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='1';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="11";
			ALU_A_Mux<="10";
			ALU_B_Mux<="01";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S17 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='1';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="10";
			ALU_A_Mux<="00";
			ALU_B_Mux<="10";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S18 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='1';
			T5_Mux<='0';
			MEM_Addr_Mux<="10";
			ALU_A_Mux<="10";
			ALU_B_Mux<="01";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S19 then
			T5_EN<='1';
			MR<='0';
			MW<='1';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='0';
			MEM_Addr_Mux<="10";
			ALU_A_Mux<="10";
			ALU_B_Mux<="01";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S20 then
			T5_EN<='1';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='0';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='1';
			T5_Mux<='0';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="01";
			ALU_B_Mux<="11";
			ALU_Op<="10";
			RF_A3_Mux<="11";
			RF_D3_mux<="00";
		elsif Q=S21 then
			T5_EN<='1';
			MR<='1';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='1';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='1';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="01";
			ALU_B_Mux<="11";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";									
		elsif Q=S22 then
			T5_EN<='1';
			MR<='1';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='1';
			RF_A1_Mux<='1';--
			T1_Mux<='1';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='1';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="01";
			ALU_B_Mux<="11";
			ALU_Op<="10";
			RF_A3_Mux<="11";
			RF_D3_mux<="00";	
		elsif Q=S23 then
			T5_EN<='0';
			MR<='0';
			MW<='0';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='1';--
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='1';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="01";
			ALU_B_Mux<="11";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";
		elsif Q=S24 then
			T5_EN<='1';
			MR<='0';
			MW<='1';
			IR_EN<='0';
			PC_EN<='0';
			PC_Mux<='0';
			RF_EN<='0';
			RF_A1_Mux<='1';--
			T1_Mux<='0';
			T2_Mux<='0';
			T4_Mux<='0';
			T5_Mux<='1';
			MEM_Addr_Mux<="00";
			ALU_A_Mux<="01";
			ALU_B_Mux<="11";
			ALU_Op<="10";
			RF_A3_Mux<="00";
			RF_D3_mux<="00";					
		end if;
	end process;

end architecture;