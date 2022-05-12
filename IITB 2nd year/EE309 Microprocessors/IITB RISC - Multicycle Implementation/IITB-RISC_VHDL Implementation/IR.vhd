library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity IR is
port (	
	clk, rst, write_REG: in std_logic;
	inp: in std_logic_vector (15 downto 0);
	outp: out std_logic_vector (15 downto 0)
	);
end entity;

architecture IR_logic of IR is

signal reg: std_logic_vector(15 downto 0);

begin
	process(inp, clk) is
	begin
		if(rst='1') then
			reg<=(others=>'0');
		elsif(write_REG='1') then
			reg<=inp;
		end if;
		outp<=reg;
	end process;
end architecture;