 library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;



entity register_file is
	generic(
		word_length: integer := 16;
		num_words: integer := 8);
		
	port(
		data_in, R7_in: in std_logic_vector(15 downto 0);
		data_out1, data_out2, R7_out: out std_logic_vector(15 downto 0);
		sel_in, sel_out1, sel_out2: in std_logic_vector(2 downto 0);
		clk, wr_ena,pc_ena, reset: in std_logic);
		
end entity;

architecture trial of register_file is
	type word_bus is array(6 downto 0) of std_logic_vector(15 downto 0);
	signal reg_out: word_bus;
	signal R7: std_logic_vector(15 downto 0);
	
begin
	
	in_decode: process(sel_in, wr_ena, pc_ena, data_in, R7_in)
	begin
	if(rising_edge(clk)) then
		if(wr_ena='1') then
					reg_out(to_integer(unsigned(sel_in)))<=data_in;
		end if;
			 if(pc_ena='1') then
					R7<=R7_in;
		end if;			
	end if; 
	end process;
	
	data_out1 <= reg_out(to_integer(unsigned(sel_out1)));
	data_out2 <= reg_out(to_integer(unsigned(sel_out2)));
	R7_out <= R7;
	
end architecture;