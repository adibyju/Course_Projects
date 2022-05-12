library ieee;
use ieee.std_logic_1164.all;
use ieee.math_real.all;
use ieee.numeric_std.all;

entity RF is
port (	
	clk, rst, write_Reg, PC_EN: in std_logic;
	RA1, RA2, RA3: in std_logic_vector (2 downto 0);
	write_data ,R7_write_data: in std_logic_vector (15 downto 0);
	outputA, outputB, R7outp: out std_logic_vector (15 downto 0)
	);
end entity;

architecture RF_logic of RF is

type registerFile is array(0 to 7) of std_logic_vector(15 downto 0);
signal registers : registerFile;

begin
	process(clk) is
		begin
			if(rst='1') then
				registers(0)<="0000000000000001";
				registers(1)<="0000000000000010";
				registers(2)<="0000000000001000";
				registers(3)<="0000000000000100";
				registers(4)<="0000000000000101";
				registers(5)<="0000000000000110";
				registers(6)<="0000000000001011";
				registers(7)<="0000000000000000";

				-- for I in 0 to 7 loop
				-- 	registers(I)<=(others=>'0');
				-- end loop;
			else
				R7outp<=registers(7);
				if(write_Reg='1') then
					registers(to_integer(unsigned(RA3)))<=write_data;
				end if;
				if(PC_EN='1') then
					registers(7)<=R7_write_data;
				end if;
			end if;

	end process;

	process(RA1, RA2) is
		begin

				outputA<=registers(to_integer(unsigned(RA1)));
				outputB<=registers(to_integer(unsigned(RA2)));


	end process;

end architecture;