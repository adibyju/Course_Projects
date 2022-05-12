library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity Reg is
generic(N: integer);
port (	
	clk: in std_logic;
	inp: in std_logic_vector (N-1 downto 0);
	outp: out std_logic_vector (N-1 downto 0)
	);
end entity;

architecture Reg_logic of Reg is

signal reg: std_logic_vector(N-1 downto 0);

begin
	process(inp, clk) is
	begin
		if(rising_edge(clk)) then
			reg<=inp;
		elsif(falling_edge(clk)) then
			outp<=reg;
		end if;
	end process;
end architecture;