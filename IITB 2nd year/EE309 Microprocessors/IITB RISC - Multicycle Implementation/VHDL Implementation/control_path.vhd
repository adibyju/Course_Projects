library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;

entity control_path is
	port(
		reset, clk: in std_logic; 
		ir: in std_logic_vector(15 downto 0);
		op_code: in std_logic_vector(3 downto 0);
		condition: in std_logic_vector(1 downto 0);
		C: out std_logic_vector(27 downto 0);
		CY, Z, invalid_next: in std_logic);
end entity;

architecture fsm of control_path is
	type fsm_state is (s0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, s17, s18, s19,s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35);
	signal Q, nQ: fsm_state := s0;
begin

	clocked:
	process(clk, nQ)
	begin
		if (clk'event and clk = '1') then
			Q <= nQ;
		end if;
	end process;
	
				
	
	next_state:
	process(op_code, condition, CY, Z, reset, Q, ir)
	begin
		nQ <= Q;

			case Q is
			
			   when s0 =>
			   case op_code is
				  when "1001" => nQ <= s34;
				  when "1010" => nQ <= s34;
				  when others => nQ <= s1;
			   end case;
				when S1 =>
					case op_code is
						when "0000" => nQ <= S6;
						when "0001" | "0010"=>
							case condition is
								when "00" => nQ <= S2;
								when "10" =>
									if (CY = '1') then	nQ <= S2;
									else	nQ <= S0;
									end if;
								when "01" =>
									if (Z = '1') then	nQ <= S2;
									else nQ <= S0;
									end if;
								when others =>	nQ <= S2;
					end case;
				when S2 =>
				
				case op_code is
						when "1101" => 
						if (ir(0)='1') then
				    nQ<=s19;
					elsif (ir(1)='1') then
					 nQ<=s20;
					 elsif (ir(2)='1') then
					 nQ<=s21;
					 elsif (ir(3)='1') then
					 nQ<=s22;
					 elsif (ir(4)='1') then
					 nQ<=s23;
					 elsif (ir(5)='1') then
					 nQ<=s24;
					 elsif (ir(6)='1') then
					 nQ<=s25;
					 elsif (ir(7)='1') then
					 nQ<=s26;
					 else
					 nQ<=s0;
				    	end if;
						when "1100" =>	nQ <= S10;
						when "0000" => nQ <= s5;
						when "0111" => nQ <= s5;
						when "0101" => nQ <= s5;
						when "1011" => nQ <= s5;
						when others => nQ <= s3;
				end case;		
						
							
				when S3 =>	
				case op_code is
						when "1000" => 
						 if (Z='1') then nQ <= S29;
						 else nQ <= s0;

						 end if;
						 when others => nQ<= s4;
				end case;		 
						
				when S4 => 	
					nQ<=s0;
					
				when S5 =>	
				  case op_code is 
				      when "0111"=> nQ<=s7;
						when "0101"=> nQ<=s9;
						when "1011"=> nQ<=s35;
					end case;
					
				when S6 =>	nQ<=s0;
					
				when S7 =>
					nQ<=s8;
				when S8 => nQ <= S0;
					
				when S9 =>	nQ <= S0;
				when S10 => 
				    if (ir(0)='1') then
				    nQ<=s11;
					 elsif (ir(1)='1') then
					 nQ<=s12;
					 elsif (ir(2)='1') then
					 nQ<=s13;
					 elsif (ir(3)='1') then
					 nQ<=s14;
					 elsif (ir(4)='1') then
					 nQ<=s15;
					 elsif (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
			
				when S11 =>
					if (ir(1)='1') then
					 nQ<=s12;
					 elsif (ir(2)='1') then
					 nQ<=s13;
					 elsif (ir(3)='1') then
					 nQ<=s14;
					 elsif (ir(4)='1') then
					 nQ<=s15;
					 elsif (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when S12 =>	
				if (ir(2)='1') then
					 nQ<=s13;
					 elsif (ir(3)='1') then
					 nQ<=s14;
					 elsif (ir(4)='1') then
					 nQ<=s15;
					 elsif (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when S13 =>
					if (ir(3)='1') then
					 nQ<=s14;
					 elsif (ir(4)='1') then
					 nQ<=s15;
					 elsif (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when S14 =>	
					if (ir(4)='1') then
					 nQ<=s15;
					 elsif (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when S15 => 
					if (ir(5)='1') then
					 nQ<=s16;
					 elsif (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when S16 =>
					if (ir(6)='1') then
					 nQ<=s17;
					 elsif (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
				    	end if;
				when s17=>
		        		if (ir(7)='1') then
					 nQ<=s18;
					 else
					 nQ<=s0;
					 end if;
				when s18 =>
		          nQ<=s0;
				
		when s19=>nQ<=s27;
		
		when s20=>nQ<=s27;
		
		when s21=>nQ<=s27;
		
		when s22=>nQ<=s27;
		
		when s23=>nQ<=s27;
		
		when s24=>nQ<=s27;
		
		when s25=>nQ<=s27;
		
		when s26=>nQ<=s27;
		
		when s27=>nQ<=s28;
         		
		when others =>	nQ <= S0;
end case;

end process;		

	output:
	process(op_code, Q)
	begin
		T <= (others => '0');
	
	end process;	
end architecture;