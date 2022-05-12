library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity LS1 is
	port(
		inp: in std_logic_vector(15 downto 0);
		outp: out std_logic_vector(15 downto 0));
end entity;

architecture LS1_logic of LS1 is
begin
	
    outp(0)<='0';
	outp(15 downto 1) <= inp(14 downto 0);
	
end architecture;