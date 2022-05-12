library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity LS7 is
	port(
		inp: in std_logic_vector(15 downto 0);
		outp: out std_logic_vector(15 downto 0));
end entity;

architecture LS7_logic of LS7 is
begin
	
    outp(6 downto 0)<="0000000";
	outp(15 downto 7) <= inp(8 downto 0);
	
end architecture;