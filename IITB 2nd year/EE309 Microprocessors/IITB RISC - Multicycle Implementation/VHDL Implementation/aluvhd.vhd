library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.add.all;

entity alu is
	generic(word_length: integer := 16);
	port(
		input1, input2: in std_logic_vector(word_length-1 downto 0);
		output: out std_logic_vector(word_length-1 downto 0);
		cin: in std_logic;
		sel: in std_logic_vector(1 downto 0)
		CY, Z: out std_logic);
end entity;

architecture behave of alu is
	signal output_temp: std_logic_vector(word_length-1 downto 0);
	signal output_add: std_logic_vector(word_length-1 downto 0);
	signal output_sub: std_logic_vector(word_length-1 downto 0);
	signal C: std_logic_vector(word_length downto 1);
begin
	
	ADD0: adder
		generic map(word_length,4)
		port map(
			A => input1, B => input2,
			cin => cin, S => output_add, Cout => C);
			CY <= C(word_length);
	
	process(input1,input2,sel,output_add)
	begin
		if (sel = '00') then
			output_temp <= output_add;
		elsif (sel = '01')
			output_temp <= input1 nand input2;
		elseif (sel = '10') then
		   output_temp <= 
		
		end if;
	end process;
	
	Z <= '1' when (to_integer(unsigned(output_temp)) = 0) else '0';
	output <= output_temp;

		
end architecture;