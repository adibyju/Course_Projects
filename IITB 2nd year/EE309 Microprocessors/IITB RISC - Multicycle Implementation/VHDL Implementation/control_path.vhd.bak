library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;

entity control_path is
	port(
		reset, clk: in std_logic; 
		op_code: in std_logic_vector(3 downto 0);
		condition: in std_logic_vector(1 downto 0);
		T: out std_logic_vector(24 downto 0);
		C, Z, invalid_next, eq, B: in std_logic);
end entity;

architecture fsm of control_path is
	type fsm_state is (S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16);
	signal Q, nQ: fsm_state;
begin

	clocked:
	process(clk, nQ)
	begin
		if (clk'event and clk = '1') then
			Q <= nQ;
		end if;
	end process;
	
	next_state:
	process(op_code, condition, C, Z, invalid_next, eq, B, reset, Q)
	begin
		nQ <= Q;
--		if (reset = '1') then
--			nQ <= S1;
--		else
			case Q is
				when S1 =>
					case op_code is
						when "0011" => nQ <= S6;
						when "1000" =>	nQ <= S15;
						when "1001" => nQ <= S16;
						when others =>	nQ <= S2;
					end case;
				when S2 =>
					case op_code is
						when "0000" | "0010"=>
							case condition is
								when "00" => nQ <= S3;
								when "10" =>
									if (C = '1') then	nQ <= S3;
									else	nQ <= S5;
									end if;
								when "01" =>
									if (Z = '1') then	nQ <= S3;
									else nQ <= S5;
									end if;
								when others =>	nQ <= S5;
							end case;
						when "0001" => nQ <= S7;
						when "0100" =>	nQ <= S7;
						when "0101" =>	nQ <= S7;
						when "0110" => nQ <= S10;
						when "0111" => nQ <= S12;
						when "1100" =>
							if (eq = '1') then nQ <= S14;
							else	nQ <= S5;
							end if;
						when "1001" =>	nQ <= S16;
						when others =>	nQ <= S1;
					end case;
				when S3 =>	nQ <= S4;
				when S4 => 	
					if(B = '0') then nQ <= S5;
					else nQ <= S1;
					end if;
				when S5 =>	nQ <= S1;
				when S6 =>	
					if(B = '0') then nQ <= S5;
					else nQ <= S1;
					end if;
				when S7 =>
					if (op_code(0) = '0') then	nQ <= S8;
					else	nQ <= S9;
					end if;
				when S8 => nQ <= S5;
					--if (valid = '1') then nQ <= S9;
					--else 	nQ <= S1;
					--end if;
				when S9 =>	nQ <= S1;
				when S10 => nQ <= S11;
				--	if (valid = '1') then
				--		if (op_code(0) = '0') then	nQ <= S9;
				--		else	nQ <= S12;
				--		end if;
				--	else 
				--		nQ <= S1;
				--	end if;
				when S11 =>
					if (invalid_next = '0') then
						if(B = '0') then nQ <= S5;
						else nQ <= S1;
						end if;
					else nQ <= S10;
					end if;
				when S12 =>	nQ <= S13;
				when S13 =>
					if (invalid_next = '0') then
						if(B = '0') then nQ <= S5;
						else nQ <= S1;
						end if;
					else nQ <= S12;
					end if;
				when S14 =>	
					if(B = '0') then nQ <= S5;
					else nQ <= S1;
					end if;
				when S15 => 
					if(B = '0') then nQ <= S5;
					else nQ <= S1;
					end if;
				when S16 =>
					if(B = '0') then nQ <= S5;
					else nQ <= S1;
					end if;
				when others =>	nQ <= S5;
			end case;
--		end if;
	end process;
	
	output:
	process(op_code, Q)
	begin
		T <= (others => '0');
	end process;
		
end architecture;