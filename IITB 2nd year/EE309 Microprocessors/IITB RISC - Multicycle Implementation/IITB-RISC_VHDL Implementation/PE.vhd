library std;
library ieee;
use ieee.std_logic_1164.all;
--library work;
--use work.components_init.all;

entity PE is
  port (
	pe_in : in std_logic_vector(7 downto 0);
	pe_out1 : out std_logic_vector(2 downto 0);
	pe_out2 : out std_logic_vector(7 downto 0);
	z2: out std_logic
  ) ;
end entity ; -- PE

architecture PE_logic of PE is 

begin

process(pe_in)
variable pe_out_var : std_logic_vector(2 downto 0);
variable po : std_logic_vector(7 downto 0);
variable z2_var : std_logic;
begin
	z2_var:='0';
	if pe_in(0) = '1' then 
		pe_out_var := "000";
		po(7 downto 1):=pe_in(7 downto 1);
		po(0):='0';
	elsif pe_in(1) = '1' then
		pe_out_var := "001";
		po(7 downto 2):=pe_in(7 downto 2);
		po(1 downto 0):="00";
	elsif pe_in(2) = '1' then
		pe_out_var := "010";
		po(7 downto 3):=pe_in(7 downto 3);
		po(2 downto 0):="000";
	elsif pe_in(3) = '1' then
		pe_out_var := "011";
		po(7 downto 4):=pe_in(7 downto 4);
		po(3 downto 0):="0000";
	elsif pe_in(4) = '1' then
		pe_out_var := "100";
		po(7 downto 5):=pe_in(7 downto 5);
		po(4 downto 0):="00000";
	elsif pe_in(5) = '1' then
		pe_out_var := "101";
		po(7 downto 6):=pe_in(7 downto 6);
		po(5 downto 0):="000000";
	elsif pe_in(6) = '1' then
		pe_out_var := "110";
		po(7):=pe_in(7);
		po(6 downto 0):="0000000";
	elsif pe_in(7) = '1' then
		pe_out_var := "111";
		po:="00000000";
	end if;
	if po = "00000000" then
		z2_var:='1';
	end if;
	pe_out1 <= pe_out_var;
	pe_out2 <= po;
	z2<=z2_var;
end process;
end architecture ; -- PE