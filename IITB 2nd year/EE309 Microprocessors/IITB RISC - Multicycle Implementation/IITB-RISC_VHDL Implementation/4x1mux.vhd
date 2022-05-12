library IEEE;
use IEEE.STD_LOGIC_1164.all;
 
entity mux4 is
 generic(N: integer);
 port(
 
     A,B,C,D : in std_logic_vector (N-1 downto 0);
     S0,S1: in STD_LOGIC;
     Z: out std_logic_vector (N-1 downto 0)
  );
end entity;
 
architecture bhv of mux4 is
begin
process (A,B,C,D,S0,S1) is
begin
  if (S0 ='0' and S1 = '0') then
      Z <= A;
  elsif (S0 ='0' and S1 = '1') then
      Z <= B;
  elsif (S0 ='1' and S1 = '0') then
      Z <= C;
  else
      Z <= D;
  end if;
end process;
end architecture;
