library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity T5 is
port (	
	clk,WE: in std_logic;
	inp: in std_logic_vector (15 downto 0);
	outp: out std_logic_vector (15 downto 0)
	);
end entity;

architecture T5_logic of T5 is

signal reg: std_logic_vector(15 downto 0);

begin
	process(inp, clk) is
	begin
		if(rising_edge(clk)) then
			if WE='1' then
				reg<=inp;
			end if;
		elsif(falling_edge(clk)) then
            outp<=reg;
		end if;
	end process;
end architecture;