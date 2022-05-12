library IEEE;
use IEEE.STD_LOGIC_1164.all;
 
entity mux2 is
 generic (N: integer);
 port(
 
     A,B : in std_logic_vector (N-1 downto 0);
     S: in STD_LOGIC;
     Z: out std_logic_vector (N-1 downto 0)
  );
end entity;
 
architecture bhv of mux2 is
begin
process (A,B,S) is
begin
  if (S ='0') then
      Z <= A;
  else
      Z <= B;
  end if;
end process;
end architecture;
